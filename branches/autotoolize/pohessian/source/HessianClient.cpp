//
// PoHessian
// Portable C++ Hessian Implementation
//
// Copyright (C) 2012  Pierre-David Belanger
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Contributor(s): Pierre-David Belanger <pierredavidbelanger@gmail.com>
//

#include "pohessian/HessianClient.h"

#include "conf.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <typeinfo>

#include "pohessian/HessianTypes.h"
#include "pohessian/Hessian1StreamReader.h"
#include "pohessian/Hessian1StreamWriter.h"

#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"

using Poco::Exception;
using Poco::StreamCopier;
using Poco::URI;

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPResponse;
using Poco::Net::Socket;
using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
using Poco::Net::SocketStream;
using Poco::Net::SocketInputStream;
using Poco::Net::SocketOutputStream;

using Poco::icompare;

namespace PoHessian {

    static void throwHessianExceptionIfFault(const ValuePtr& value) {
        if (!value || !value->isFault()) return;
        throw HessianException(value->getFaultCode(), value->getFaultMessage(), value->getFaultDetail());
    }

    static ReplyPtr callHessian1Http(const URI& uri, const CallPtr& call) {
        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest request(HTTPRequest::HTTP_POST, uri.getPathEtc(), HTTPMessage::HTTP_1_1);
        if (false) {
            request.setChunkedTransferEncoding(true);
            std::ostream& request_out = session.sendRequest(request);
            Hessian1StreamWriter hessian_writer(request_out);
            hessian_writer.writeCall(call);
            request_out.flush();
        } else {
            std::stringstream memory_stream;
            Hessian1StreamWriter hessian_writer(memory_stream);
            hessian_writer.writeCall(call);
            request.setContentLength(memory_stream.str().length());
            std::ostream& request_out = session.sendRequest(request);
            StreamCopier::copyStream(memory_stream, request_out);
            request_out.flush();
        }
        HTTPResponse response;
        std::istream& response_in = session.receiveResponse(response);
        if (response.getStatus() != HTTPResponse::HTTP_OK) throw Exception(std::string("HTTP error: ") + response.getReason());
        Hessian1StreamReader hessian_reader(response_in);
        ReplyPtr reply = hessian_reader.readReply();
        return reply;
    }

    static ReplyPtr callHessian1Tcp(const URI& uri, const CallPtr& call) {
        SocketAddress address(uri.getHost(), uri.getPort());
        StreamSocket socket(address);
        SocketOutputStream out(socket);
        Hessian1StreamWriter hessian_writer(out);
        hessian_writer.writeCall(call);
        out.flush();
        SocketInputStream in(socket);
        Hessian1StreamReader hessian_reader(in);
        ReplyPtr reply = hessian_reader.readReply();
        return reply;
    }

    HessianClient::HessianClient(const HessianVersion version, const URI& uri)
    : _version(version),
    _uri(uri) {
    }

    ValuePtr HessianClient::call(const std::string& method) {
        const HeaderList headers;
        const ParameterList parameters;
        return call(method, headers, parameters);
    }

    ValuePtr HessianClient::call(const std::string& method, const HeaderList& headers) {
        const ParameterList parameters;
        return call(method, headers, parameters);
    }

    ValuePtr HessianClient::call(const std::string& method, const ParameterList& parameters) {
        const HeaderList headers;
        return call(method, headers, parameters);
    }

    ValuePtr HessianClient::call(const std::string& method, const HeaderList& headers, const ParameterList& parameters) {
        CallPtr call = new Call(method, headers, parameters);
        ReplyPtr reply = this->call(call);
        PoHessian::throwHessianExceptionIfFault(reply->getValue());
        return reply->getValue();
    }

    ReplyPtr HessianClient::call(const CallPtr& call) {
        if (icompare(_uri.getScheme(), "HTTP") == 0) {
            return PoHessian::callHessian1Http(_uri, call);
        } else if (icompare(_uri.getScheme(), "TCP") == 0) {
            return PoHessian::callHessian1Tcp(_uri, call);
        } else {
            throw Exception("Invalid scheme: " + _uri.getScheme());
        }
    }

}
