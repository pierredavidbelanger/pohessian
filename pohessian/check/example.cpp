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

#include <iostream>

#include "Poco/URI.h"
#include "pohessian/HessianTypes.h"
#include "pohessian/HessianClient.h"

using namespace std;
using namespace Poco;
using namespace PoHessian;

int main(int argc, char* argv[]) {
    
    cout << "*" << endl << "* GettingStarted" << endl << "*" << endl;
    {
        URI uri("http://hessian.caucho.com/test/test2");
        HessianClient client(HessianClient::HESSIAN_VERSION_1, uri);
        cout << client.call("replyString_32")->getString() << endl;
    }
    
    cout << "*" << endl << "* Value" << endl << "*" << endl;
    {
        HessianClient client(HessianClient::HESSIAN_VERSION_1, URI("http://hessian-test.appspot.com/basic"));
    }
    
    cout << "*" << endl << "* HessianClient" << endl << "*" << endl;
    {
        HessianClient client(HessianClient::HESSIAN_VERSION_1, URI("http://hessian-test.appspot.com/basic"));
        
        {
            cout << "* call with no return, no parameter, no header (void nullCall()) " << endl;
            client.call("nullCall");
        }
        
        {
            cout << "* call with with header (void nullCall()) " << endl;
            HeaderList headers;
            headers.push_back(new Header("customHeaderName", new Value("customHeaderValue")));
            client.call("nullCall", headers);
        }

        {
            cout << "* call with one parameter (Object echo(Object value)) " << endl;
            ParameterList parameters;
            parameters.push_back(new Value(1337));
            cout << client.call("echo", parameters)->getInteger() << endl;
        }

        {
            cout << "* call with two parameter (int subtract(int a, int b)) " << endl;
            ParameterList parameters;
            parameters.push_back(new Value(2000));
            parameters.push_back(new Value(663));
            cout << client.call("subtract", parameters)->getInteger() << endl;
        }

        {
            cout << "* call with one parameter and header (Object echo(Object value)) " << endl;
            HeaderList headers;
            headers.push_back(new Header("customHeaderName", new Value("customHeaderValue")));
            ParameterList parameters;
            parameters.push_back(new Value("HelloWorld"));
            cout << client.call("echo", headers, parameters)->getString() << endl;
        }

        {
            cout << "* call low level (Object echo(Object value)) " << endl;
            HeaderList headers;
            headers.push_back(new Header("customHeaderName", new Value("customHeaderValue")));
            ParameterList parameters;
            parameters.push_back(new Value("HelloWorld"));
            CallPtr call = new Call("echo", headers, parameters);
            ReplyPtr reply = client.call(call);
            cout << reply->getValue()->getString() << endl;
        }
    }
    
    return 0;
}
