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
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
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
    
    cout << "*" << endl << "* Hessian Type Mapping" << endl << "*" << endl;
    {
        // NULL
        // Object null1 = null;
        ValuePtr null1 = new Value();
        ValuePtr null2 = new Value(Value::TYPE_NULL);
        if (null1->isNull() && null2->isNull()) {
            cout << "null" << endl;
        }
     
        // BOOLEAN
        // Boolean bool1 = new Boolean(true);
        // Boolean bool2 = new Boolean(false);
        ValuePtr bool1 = new Value(true);
        ValuePtr bool2 = new Value(false);
        if (bool1->isBoolean() && bool1->getBoolean() && bool2->isBoolean() && !bool2->getBoolean()) {
            cout << "true && false" << endl;
        }
        
        // INT
        // Integer int1 = new Integer(1337);
        ValuePtr int1 = new Value(1337);
        if (int1->isInteger() && int1->getInteger() == 1337) {
            cout << "1337" << endl;
        }
        
        // LONG
        // Long long1 = new Long(2147483647);
        ValuePtr long1 = new Value((Poco::Int64)2147483648LL);
        ValuePtr long2 = new Value(2147483648LL, Value::TYPE_LONG);
        if (long1->isLong() && long1->getLong() == 2147483648LL && long2->isLong() && long2->getLong() == 2147483648LL) {
            cout << "2147483648" << endl;
        }
        
        // DOUBLE
        // Double double1 = new Double(3.1416);
        ValuePtr double1 = new Value(3.1416);
        if (double1->isDouble() && double1->getDouble() == 3.1416) {
            cout << "3.1416" << endl;
        }
        
        // DATE
        // Date date1 = new Date();
        Poco::Timestamp now;
        ValuePtr date1 = new Value(now.epochTime() * 1000, Value::TYPE_DATE);
        ValuePtr date2 = new Value(now);
        if (date1->isDate() && date1->getDateAsTimestamp().epochTime() == now.epochTime()
                && date2->isDate() && date2->getDateAsTimestamp().epochTime() == now.epochTime()) {
            cout << Poco::DateTimeFormatter::format(now, "%Y-%m-%d %H:%M:%S") << endl;
        }
        
        // STRING
        // String string1 = "Encodés en UTF-8";
        ValuePtr string1 = new Value("Encod\u00e9s en UTF-8");
        ValuePtr string2 = new Value(std::string("Encod\u00e9s en UTF-8"));
        ValuePtr string3 = new Value("Encod\u00e9s en UTF-8", Value::TYPE_STRING);
        ValuePtr string4 = new Value(std::string("Encod\u00e9s en UTF-8"), Value::TYPE_STRING);
        if (string1->isString() && string1->getString() == "Encod\u00e9s en UTF-8"
                && string2->isString() && string2->getString() == "Encod\u00e9s en UTF-8"
                && string3->isString() && string3->getString() == "Encod\u00e9s en UTF-8"
                && string4->isString() && string4->getString() == "Encod\u00e9s en UTF-8") {
            cout << "Encod\u00e9s en UTF-8" << endl;
        }
        
        // XML
        // String xml1 = "<body/>";
        ValuePtr xml1 = new Value("<body/>", Value::TYPE_XML);
        ValuePtr xml2 = new Value(std::string("<body/>"), Value::TYPE_XML);
        if (xml1->isXml() && xml1->getXml() == "<body/>"
                && xml2->isXml() && xml2->getXml() == "<body/>") {
            cout << "<body/>" << endl;
        }
        
        // BINARY
        // byte[] binary1 = "binary".getBytes();
        ValuePtr binary1 = new Value("binary", Value::TYPE_BINARY);
        ValuePtr binary2 = new Value(std::string("binary"), Value::TYPE_BINARY);
        if (binary1->isBinary() && binary1->getBinary() == "binary"
                && binary2->isBinary() && binary2->getBinary() == "binary") {
            cout << "binary" << endl;
        }
        
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
