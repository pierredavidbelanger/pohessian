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

#include "pohessian/Hessian1StreamReader.h"

#include "conf.h"

#include <string>
#include <vector>
#include <istream>

#include <string.h>

#include "pohessian/HessianTypes.h"
#include "pohessian/HessianStreamReader.h"

#include "Poco/Types.h"
#include "Poco/Exception.h"

using Poco::UInt8;
using Poco::UInt16;
using Poco::Int32;
using Poco::Int64;
using Poco::Exception;

namespace PoHessian {

    static UInt16 readUInt16(std::istream& in) {
        UInt16 b2 = in.get() & 0xFF;
        UInt16 b1 = in.get() & 0xFF;
        UInt16 tmp = (b2 << 8) | b1;
        return tmp;
    }

    static Int32 readInt32(std::istream& in) {
        Int32 b4 = in.get() & 0xFF;
        Int32 b3 = in.get() & 0xFF;
        Int32 b2 = in.get() & 0xFF;
        Int32 b1 = in.get() & 0xFF;
        Int32 tmp = (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
        return tmp;
    }

    static Int64 readInt64(std::istream& in) {
        Int64 b8 = in.get() & 0xFF;
        Int64 b7 = in.get() & 0xFF;
        Int64 b6 = in.get() & 0xFF;
        Int64 b5 = in.get() & 0xFF;
        Int64 b4 = in.get() & 0xFF;
        Int64 b3 = in.get() & 0xFF;
        Int64 b2 = in.get() & 0xFF;
        Int64 b1 = in.get() & 0xFF;
        Int64 tmp = (b8 << 56) | (b7 << 48) | (b6 << 40) | (b5 << 32) | (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
        return tmp;
    }

    static void readUtf8String(std::istream& in, std::string& dest, UInt16 size) {
        for (UInt16 i = 0; i < size; i++) {
            UInt8 c = in.get();
            if (c <= 0x7F) {
                dest.push_back(c);
            } else if (c <= 0xDF) {
                dest.push_back(c);
                dest.push_back(in.get());
            } else if (c <= 0xEF) {
                dest.push_back(c);
                dest.push_back(in.get());
                dest.push_back(in.get());
            } else if (c <= 0xF4) {
                dest.push_back(c);
                dest.push_back(in.get());
                dest.push_back(in.get());
                dest.push_back(in.get());
            }
        }
    }

    static std::string readString(std::istream& in, char type) {
        if (in.get() != type)
            throw Exception("Expected String (TODO)");
        std::string value;
        UInt16 size = readUInt16(in);
        value.reserve(value.size() + size);
        readUtf8String(in, value, size);
        return value;
    }

    static std::string readString(std::istream& in, char initial_type, char type) {
        char tag;
        tag = in.get();
        if (tag != initial_type && tag != type)
            throw Exception("Expected String (TODO|TODO)");
        std::string value;
        while (tag == initial_type) {
            UInt16 size = readUInt16(in);
            value.reserve(value.size() + size);
            readUtf8String(in, value, size);
            tag = in.get();
        }
        if (tag != type)
            throw Exception(std::string("Expected tag ") + tag);
        UInt16 size = readUInt16(in);
        value.reserve(value.size() + size);
        readUtf8String(in, value, size);
        return value;
    }

    static ValuePtr readNull(std::istream& in) {
        if (in.get() != 'N')
            throw Exception("Expected Null (N)");
        return new Value;
    }

    static ValuePtr readBoolean(std::istream& in) {
        char tag = in.get();
        if (tag != 'T' && tag != 'F')
            throw Exception("Expected Boolean (T|F)");
        return new Value(tag == 'T');
    }

    static ValuePtr readInteger(std::istream& in) {
        if (in.get() != 'I')
            throw Exception("Expected Integer (I)");
        return new Value(readInt32(in));
    }

    static ValuePtr readLong(std::istream& in) {
        if (in.get() != 'L')
            throw Exception("Expected Long (L)");
        return new Value(readInt64(in));
    }

    static ValuePtr readDouble(std::istream& in) {
        if (in.get() != 'D')
            throw Exception("Expected Double (D)");
        Int64 src = readInt64(in);
        double tmp;
        memcpy(&tmp, &src, sizeof (Int64));
        return new Value(tmp);
    }

    static ValuePtr readDate(std::istream& in) {
        if (in.get() != 'd')
            throw Exception("Expected Date (d)");
        return new Value(readInt64(in), Value::TYPE_DATE);
    }

    static ValuePtr readString(std::istream& in) {
        return new Value(readString(in, 's', 'S'));
    }

    static ValuePtr readXml(std::istream& in) {
        return new Value(readString(in, 'x', 'X'), Value::TYPE_XML);
    }

    static ValuePtr readBinary(std::istream& in) {
        char tag;
        tag = in.get();
        if (tag != 'b' && tag != 'B')
            throw Exception("Expected Binary (b|B)");
        std::string value;
        while (tag == 'b') {
            UInt16 size = readUInt16(in);
            value.reserve(value.size() + size);
            //char tmp[size];
            //in.read(tmp, size);
            //value.append(tmp, size);
            for (UInt16 i = 0; i < size; i++)
                value.push_back(in.get());
            tag = in.get();
        }
        if (tag != 'B')
            throw Exception(std::string("Expected tag ") + tag);
        UInt16 size = readUInt16(in);
        value.reserve(value.size() + size);
        //char tmp[size];
        //in.read(tmp, size);
        //value.append(tmp, size);
        for (UInt16 i = 0; i < size; i++)
            value.push_back(in.get());
        return new Value(value, Value::TYPE_BINARY);
    }

    static ValuePtr readValue(std::istream& in, RefList& refs);

    static ValuePtr readList(std::istream& in, RefList& refs) {
        if (in.get() != 'V')
            throw Exception("Expected List (V)");
        ValuePtr value;
        if (in.peek() == 't') {
            std::string type = readString(in, 't');
            value = new Value(type, Value::TYPE_LIST);
        } else {
            value = new Value(Value::TYPE_LIST);
        }
        Int32 length = -1;
        if (in.peek() == 'l') {
            if (in.get() != 'l')
                throw Exception("Expected List length (l)");
            length = readInt32(in);
            value->reserve(length);
        }
        refs.push_back(value);
        while (in.peek() != 'z')
            value->add(readValue(in, refs));
        if (in.get() != 'z')
            throw Exception("Expected end List (z)");
        return value;
    }

    static ValuePtr readMap(std::istream& in, RefList& refs) {
        if (in.get() != 'M')
            throw Exception("Expected Map (M)");
        ValuePtr value;
        if (in.peek() == 't') {
            std::string type = readString(in, 't');
            value = new Value(type, Value::TYPE_MAP);
        } else {
            value = new Value(Value::TYPE_MAP);
        }
        refs.push_back(value);
        while (in.peek() != 'z') {
            ValuePtr k = readValue(in, refs);
            ValuePtr v = readValue(in, refs);
            value->put(k, v);
        }
        if (in.get() != 'z')
            throw Exception("Expected end Map (z)");
        return value;
    }

    static ValuePtr readRef(std::istream& in, RefList& refs) {
        if (in.get() != 'R')
            throw Exception("Expected Ref (R)");
        RefList::size_type idx = readInt32(in);
        if (idx >= refs.size())
            throw Exception("Unexpected Ref index out of bound: TODO");
        return ValuePtr(refs.at(idx), false);
    }

    static ValuePtr readRemote(std::istream& in) {
        if (in.get() != 'r')
            throw Exception("Expected Remote (r)");
        std::string type = readString(in, 't');
        std::string url = readString(in, 's', 'S');
        return new Value(type, url);
    }

    static ValuePtr readFault(std::istream& in, RefList& refs) {
        static const std::string fault_property_code("code");
        static const std::string fault_property_message("message");
        static const std::string fault_property_detail("detail");
        if (in.get() != 'f')
            throw Exception("Expected Fault (f)");
        std::string code;
        std::string message;
        ValuePtr detail;
        while (in.peek() != 'z') {
            std::string fault_property = readString(in, 's', 'S');
            if (fault_property == fault_property_code) {
                code = readString(in, 's', 'S');
            } else if (fault_property == fault_property_message) {
                message = readString(in, 's', 'S');
            } else if (fault_property == fault_property_detail) {
                detail = readValue(in, refs);
            }
        }
        if (in.get() != 'z')
            throw Exception("Expected end Fault (z)");
        return new Value(code, message, detail);
    }

    static ValuePtr readValue(std::istream& in, RefList& refs) {
        char tag = in.peek();
        switch (tag) {
            case 'N':
                return readNull(in);
            case 'T':
            case 'F':
                return readBoolean(in);
            case 'I':
                return readInteger(in);
            case 'L':
                return readLong(in);
            case 'D':
                return readDouble(in);
            case 'd':
                return readDate(in);
            case 's':
            case 'S':
                return readString(in);
            case 'x':
            case 'X':
                return readXml(in);
            case 'b':
            case 'B':
                return readBinary(in);
            case 'V':
                return readList(in, refs);
            case 'M':
                return readMap(in, refs);
            case 'R':
                return readRef(in, refs);
            case 'r':
                return readRemote(in);
            case 'f':
                return readFault(in, refs);
            default:
                throw Exception(std::string("Unexpected tag ") + tag);
        }
    }

    static HeaderPtr readHeader(std::istream& in, RefList& refs) {
        if (in.peek() != 'H')
            throw Exception("Expected Header (H)");
        std::string name = readString(in, 'H');
        ValuePtr value = readValue(in, refs);
        return new Header(name, value);
    }

    static CallPtr readCall(std::istream& in, RefList& refs) {
        if (in.get() != 'c')
            throw Exception("Expected Call (c)");
        if (in.get() != (char) 1)
            throw Exception("Expected Call major version (1)");
        if (in.get() != (char) 0)
            throw Exception("Expected Call minor version (0)");
        HeaderList headers;
        while (in.peek() == 'H')
            headers.push_back(readHeader(in, refs));
        std::string method = readString(in, 'm');
        ParameterList parameters;
        while (in.peek() != 'z')
            parameters.push_back(readValue(in, refs));
        if (in.get() != 'z')
            throw Exception("Expected end Call (z)");
        return new Call(method, headers, parameters);
    }

    static ReplyPtr readReply(std::istream& in, RefList& refs) {
        if (in.get() != 'r')
            throw Exception("Expected Reply (r)");
        if (in.get() != (char) 1)
            throw Exception("Expected Reply major version (1)");
        if (in.get() != (char) 0)
            throw Exception("Expected Reply minor version (0)");
        HeaderList headers;
        while (in.peek() == 'H')
            headers.push_back(readHeader(in, refs));
        ValuePtr value = readValue(in, refs);
        if (in.get() != 'z')
            throw Exception("Expected end Reply (z)");
        return new Reply(headers, value);
    }

    Hessian1StreamReader::Hessian1StreamReader(std::istream& in)
    : HessianStreamReader(in) {
    }

    ValuePtr Hessian1StreamReader::readValue() {
        return PoHessian::readValue(_in, _refs);
    }

    CallPtr Hessian1StreamReader::readCall() {
        return PoHessian::readCall(_in, _refs);
    }

    ReplyPtr Hessian1StreamReader::readReply() {
        return PoHessian::readReply(_in, _refs);
    }

}
