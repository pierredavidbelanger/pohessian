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

#include <string>
#include <vector>
#include <ostream>

#include <string.h>

#include "PoHessian/Hessian1StreamWriter.h"

#include "PoHessian/HessianTypes.h"
#include "PoHessian/HessianStreamWriter.h"

#include "Poco/Types.h"
#include "Poco/Exception.h"

using Poco::UInt8;
using Poco::UInt16;
using Poco::Int32;
using Poco::Int64;
using Poco::Exception;

namespace PoHessian {

    static const Poco::UInt16 uint16_max_size = 0xFFFF;
    
    static void writeUInt16(std::ostream& out, UInt16 value) {
        UInt16 tmp = value;
        out << (char) ((tmp >> 8) & 0xFF);
        out << (char) (tmp & 0xFF);
    }

    static void writeInt32(std::ostream& out, Int32 value) {
        Int32 tmp = value;
        out << (char) ((tmp >> 24) & 0xFF);
        out << (char) ((tmp >> 16) & 0xFF);
        out << (char) ((tmp >> 8) & 0xFF);
        out << (char) (tmp & 0xFF);
    }

    static void writeInt64(std::ostream& out, Int64 value) {
        Int64 tmp = value;
        out << (char) ((tmp >> 56) & 0xFF);
        out << (char) ((tmp >> 48) & 0xFF);
        out << (char) ((tmp >> 40) & 0xFF);
        out << (char) ((tmp >> 32) & 0xFF);
        out << (char) ((tmp >> 24) & 0xFF);
        out << (char) ((tmp >> 16) & 0xFF);
        out << (char) ((tmp >> 8) & 0xFF);
        out << (char) (tmp & 0xFF);
    }

    static RefList::size_type vectorIndexOf(const RefList& refs, const ValuePtr& ref) {
        RefList::size_type i = 0;
        for (RefList::const_iterator it = refs.begin(); it != refs.end(); it++, i++)
            if (*it == ref)
                return i;
        return -1;
    }

    static std::string::size_type utf8StringLength(const std::string& value) {
        std::string::size_type length = 0;
        for (std::string::size_type i = 0; i < value.length(); i++) {
            UInt8 c = value[i];
            if (c <= 0x7F) {
            } else if (c <= 0xDF) {
                i += 1;
            } else if (c <= 0xEF) {
                i += 2;
            } else if (c <= 0xF4) {
                i += 3;
            }
            length++;
        }
        return length;
    }

    static void writeUtf8Substr(std::ostream& out, const std::string& value, std::string::size_type pos, std::string::size_type length) {
        for (std::string::size_type i = 0, count = 0; i < value.length() && count < length; i++) {
            UInt8 c = value[i];
            if (i < pos) {
                if (c <= 0x7F) {
                } else if (c <= 0xDF) {
                    i += 1;
                } else if (c <= 0xEF) {
                    i += 2;
                } else if (c <= 0xF4) {
                    i += 3;
                }
            } else {
                if (c <= 0x7F) {
                    out << c;
                } else if (c <= 0xDF) {
                    out << c;
                    out << value[++i];
                } else if (c <= 0xEF) {
                    out << c;
                    out << value[++i];
                    out << value[++i];
                } else if (c <= 0xF4) {
                    out << c;
                    out << value[++i];
                    out << value[++i];
                    out << value[++i];
                }
                count++;
            }
        }
    }

    static void writeString(std::ostream& out, char type, const std::string& value) {
        out << type;
        std::string::size_type length = utf8StringLength(value);
        if (length > uint16_max_size) {
            writeUInt16(out, uint16_max_size);
            writeUtf8Substr(out, value, 0, uint16_max_size);
        } else {
            writeUInt16(out, length);
            out << value;
        }
    }

    static void writeString(std::ostream& out, char initial_type, char type, const std::string& value) {
        std::string::size_type count = utf8StringLength(value);
        std::string::size_type pos = 0;
        while (count > uint16_max_size) {
            out << initial_type;
            writeUInt16(out, uint16_max_size);
            writeUtf8Substr(out, value, pos, uint16_max_size);
            count -= uint16_max_size;
            pos += uint16_max_size;
        }
        out << type;
        writeUInt16(out, count);
        if (pos == 0) {
            out << value;
        } else {
            writeUtf8Substr(out, value, pos, count);
        }
    }

    static void writeNull(std::ostream& out) {
        out << 'N';
    }

    static void writeBoolean(std::ostream& out, const ValuePtr& value) {
        if (value->getBoolean())
            out << 'T';
        else
            out << 'F';
    }

    static void writeInteger(std::ostream& out, const ValuePtr& value) {
        out << 'I';
        writeInt32(out, value->getInteger());
    }

    static void writeLong(std::ostream& out, const ValuePtr& value) {
        out << 'L';
        writeInt64(out, value->getLong());
    }

    static void writeDouble(std::ostream& out, const ValuePtr& value) {
        double src = value->getDouble();
        Int64 tmp;
        memcpy(&tmp, &src, sizeof (Int64));
        out << 'D';
        writeInt64(out, tmp);
    }

    static void writeDate(std::ostream& out, const ValuePtr& value) {
        out << 'd';
        writeInt64(out, value->getDateAsLong());
    }

    static void writeString(std::ostream& out, const ValuePtr& value) {
        writeString(out, 's', 'S', value->getString());
    }

    static void writeXml(std::ostream& out, const ValuePtr& value) {
        writeString(out, 'x', 'X', value->getXml());
    }

    static void writeBinary(std::ostream& out, const ValuePtr& value) {
        const std::string& tmp = value->getBinary();
        std::string::size_type count = tmp.length();
        std::string::size_type pos = 0;
        while (count > uint16_max_size) {
            out << 'b';
            writeUInt16(out, uint16_max_size);
            out << tmp.substr(pos, uint16_max_size);
            count -= uint16_max_size;
            pos += uint16_max_size;
        }
        out << 'B';
        writeUInt16(out, count);
        if (pos == 0)
            out << tmp;
        else
            out << tmp.substr(pos, count);
    }

    static void writeValue(std::ostream& out, RefList& refs, const ValuePtr& value);

    static void writeList(std::ostream& out, RefList& refs, const ValuePtr& value) {
        out << 'V';
        if (!value->getListType().empty()) {
            writeString(out, 't', value->getListType());
        }
        out << 'l';
        writeInt32(out, value->getListSize());
        refs.push_back(value);
        const Value::List& list = value->getList();
        for (Value::List::const_iterator it = list.begin(); it != list.end(); it++)
            writeValue(out, refs, *it);
        out << 'z';
    }

    static void writeMap(std::ostream& out, RefList& refs, const ValuePtr& value) {
        out << 'M';
        if (!value->getMapType().empty())
            writeString(out, 't', value->getMapType());
        refs.push_back(value);
        const Value::Map& map = value->getMap();
        for (Value::Map::const_iterator it = map.begin(); it != map.end(); it++) {
            Value::Map::value_type entry = *it;
            writeValue(out, refs, entry.first);
            writeValue(out, refs, entry.second);
        }
        out << 'z';
    }

    static void writeRef(std::ostream& out, Int32 idx) {
        out << 'R';
        writeInt32(out, idx);
    }

    static void writeRemote(std::ostream& out, const ValuePtr& value) {
        out << 'r';
        writeString(out, 't', value->getRemoteType());
        writeString(out, 's', 'S', value->getRemoteUrl());
    }

    static void writeFault(std::ostream& out, RefList& refs, const ValuePtr& value) {
        static const std::string fault_property_code("code");
        static const std::string fault_property_message("message");
        static const std::string fault_property_detail("detail");
        out << 'f';
        writeString(out, 's', 'S', fault_property_code);
        writeString(out, 's', 'S', value->getFaultCode());
        writeString(out, 's', 'S', fault_property_message);
        writeString(out, 's', 'S', value->getFaultMessage());
        writeString(out, 's', 'S', fault_property_detail);
        writeValue(out, refs, value->getFaultDetail());
        out << 'z';
    }

    static void writeValue(std::ostream& out, RefList& refs, const ValuePtr& value) {
        if (!value || value->isNull()) {
            writeNull(out);
            return;
        }
        switch (value->getType()) {
            case Value::TYPE_BOOLEAN:
                writeBoolean(out, value);
                break;
            case Value::TYPE_INTEGER:
                writeInteger(out, value);
                break;
            case Value::TYPE_LONG:
                writeLong(out, value);
                break;
            case Value::TYPE_DOUBLE:
                writeDouble(out, value);
                break;
            case Value::TYPE_DATE:
                writeDate(out, value);
                break;
            case Value::TYPE_STRING:
                writeString(out, value);
                break;
            case Value::TYPE_XML:
                writeXml(out, value);
                break;
            case Value::TYPE_BINARY:
                writeBinary(out, value);
                break;
            case Value::TYPE_LIST:
            {
                Int32 idx = vectorIndexOf(refs, value);
                if (idx != -1)
                    writeRef(out, idx);
                else
                    writeList(out, refs, value);
                break;
            }
            case Value::TYPE_MAP:
            {
                Int32 idx = vectorIndexOf(refs, value);
                if (idx != -1)
                    writeRef(out, idx);
                else
                    writeMap(out, refs, value);
                break;
            }
            case Value::TYPE_REMOTE:
                writeRemote(out, value);
                break;
            case Value::TYPE_FAULT:
                writeFault(out, refs, value);
                break;
            default:
                throw Exception("Unknow type");

        }
    }

    static void writeHeader(std::ostream& out, RefList& refs, const HeaderPtr& header) {
        writeString(out, 'H', header->getName());
        writeValue(out, refs, header->getValue());
    }

    static void writeCall(std::ostream& out, RefList& refs, const CallPtr& call) {
        out << 'c' << (char) 1 << (char) 0;
        const HeaderList& headers = call->getHeaders();
        for (HeaderList::const_iterator it = headers.begin(); it != headers.end(); it++)
            writeHeader(out, refs, *it);
        writeString(out, 'm', call->getMethod());
        const ParameterList& parameters = call->getParameters();
        for (ParameterList::const_iterator it = parameters.begin(); it != parameters.end(); it++)
            writeValue(out, refs, *it);
        out << 'z';
    }

    static void writeReply(std::ostream& out, RefList& refs, const ReplyPtr& reply) {
        out << 'r' << (char) 1 << (char) 0;
        const HeaderList& headers = reply->getHeaders();
        for (HeaderList::const_iterator it = headers.begin(); it != headers.end(); it++)
            writeHeader(out, refs, *it);
        writeValue(out, refs, reply->getValue());
        out << 'z';
    }

    Hessian1StreamWriter::Hessian1StreamWriter(std::ostream& out)
    : HessianStreamWriter(out) {
    }

    void Hessian1StreamWriter::writeValue(const ValuePtr& value) {
        PoHessian::writeValue(_out, _refs, value);
    }

    void Hessian1StreamWriter::writeCall(const CallPtr& call) {
        PoHessian::writeCall(_out, _refs, call);
    }

    void Hessian1StreamWriter::writeReply(const ReplyPtr& reply) {
        PoHessian::writeReply(_out, _refs, reply);
    }

}
