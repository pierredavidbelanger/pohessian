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

#include "pohessian/HessianTypes.h"

#include <iostream>
#include <typeinfo>

#include "Poco/Types.h"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"

using Poco::Int32;
using Poco::Int64;
using Poco::Timestamp;
using Poco::Exception;

namespace PoHessian {

    /////////////////
    // Value

    Value::Value(const Value& value)
    : _type(value._type),
	_bool(value._bool),
    _integer(value._integer),
    _double(value._double),
    _string(value._string),
    _string2(value._string2),
    _list(value._list),
    _map(value._map),
    _value(value._value) {
    }

    Value::Value(const Type type)
    : _type(type) {
        if (type != TYPE_NULL
                && type != TYPE_LIST
                && type != TYPE_MAP)
            throw Exception("Must be a NULL, LIST or MAP");
    }

    Value::Value(const bool boolean)
    : _type(Value::TYPE_BOOLEAN),
    _bool(boolean) {
    }

    Value::Value(const Int32 integer)
    : _type(Value::TYPE_INTEGER),
    _integer(integer) {
    }

    Value::Value(const double value)
    : _type(Value::TYPE_DOUBLE),
    _double(value) {
    }

    Value::Value(const Int64 value, const Type type)
    : _type(type),
    _integer(value) {
        if (type != TYPE_LONG
                && type != TYPE_DATE)
            throw Exception("Must be a LONG or DATE");
    }

    Value::Value(const Timestamp dateAsTimestamp)
    : _type(Value::TYPE_DATE),
    _integer(dateAsTimestamp.epochMicroseconds() / 1000) {
    }

    Value::Value(const char* value, const Type type)
    : _type(type),
    _string(value) {
        if (type != TYPE_STRING
                && type != TYPE_XML
                && type != TYPE_BINARY
                && type != TYPE_LIST
                && type != TYPE_MAP)
            throw Exception("Must be a STRING, XML, BINARY, LIST or MAP");
    }

    Value::Value(const std::string& value, const Type type)
    : _type(type),
    _string(value) {
        if (type != TYPE_STRING
                && type != TYPE_XML
                && type != TYPE_BINARY
                && type != TYPE_LIST
                && type != TYPE_MAP)
            throw Exception("Must be a STRING, XML, BINARY, LIST or MAP");
    }

    Value::Value(const List& list, const char* listType)
    : _type(Value::TYPE_LIST),
    _string(listType),
    _list(list) {
    }

    Value::Value(const List& list, const std::string& listType)
    : _type(Value::TYPE_LIST),
    _string(listType),
    _list(list) {
    }

    Value::Value(const Map& map, const char* mapType)
    : _type(Value::TYPE_MAP),
    _string(mapType),
    _map(map) {
    }

    Value::Value(const Map& map, const std::string& mapType)
    : _type(Value::TYPE_MAP),
    _string(mapType),
    _map(map) {
    }

    Value::Value(const char* remoteType, const char* remoteUrl)
    : _type(Value::TYPE_REMOTE),
    _string(remoteType),
    _string2(remoteUrl) {
    }

    Value::Value(const std::string& remoteType, const std::string& remoteUrl)
    : _type(Value::TYPE_REMOTE),
    _string(remoteType),
    _string2(remoteUrl) {
    }

    Value::Value(const char* faultCode, const char* faultMessage, const ValuePtr& faultDetail)
    : _type(Value::TYPE_FAULT),
    _string(faultCode),
    _string2(faultMessage),
    _value(faultDetail) {
    }

    Value::Value(const std::string& faultCode, const std::string& faultMessage, const ValuePtr& faultDetail)
    : _type(Value::TYPE_FAULT),
    _string(faultCode),
    _string2(faultMessage),
    _value(faultDetail) {
    }

    Value::Type Value::getType() const {
        return _type;
    }

    bool Value::isNull() const {
        return _type == Value::TYPE_NULL;
    }

    bool Value::isBoolean() const {
        return _type == Value::TYPE_BOOLEAN;
    }

    bool Value::isInteger() const {
        return _type == Value::TYPE_INTEGER;
    }

    bool Value::isLong() const {
        return _type == Value::TYPE_LONG;
    }

    bool Value::isDouble() const {
        return _type == Value::TYPE_DOUBLE;
    }

    bool Value::isDate() const {
        return _type == Value::TYPE_DATE;
    }

    bool Value::isString() const {
        return _type == Value::TYPE_STRING;
    }

    bool Value::isXml() const {
        return _type == Value::TYPE_XML;
    }

    bool Value::isBinary() const {
        return _type == Value::TYPE_BINARY;
    }

    bool Value::isList() const {
        return _type == Value::TYPE_LIST;
    }

    bool Value::isMap() const {
        return _type == Value::TYPE_MAP;
    }

    bool Value::isRemote() const {
        return _type == Value::TYPE_REMOTE;
    }

    bool Value::isFault() const {
        return _type == Value::TYPE_FAULT;
    }

    bool Value::getBoolean() const {
        if (_type != TYPE_BOOLEAN)
            throw Exception("Must be a BOOLEAN");
        return _bool;
    }

    Int32 Value::getInteger() const {
        if (_type != TYPE_INTEGER)
            throw Exception("Must be a INTEGER");
        return (Int32) _integer;
    }

    Int64 Value::getLong() const {
        if (_type != TYPE_LONG)
            throw Exception("Must be a LONG");
        return _integer;
    }

    double Value::getDouble() const {
        if (_type != TYPE_DOUBLE)
            throw Exception("Must be a DOUBLE");
        return _double;
    }

    Int64 Value::getDateAsLong() const {
        if (_type != TYPE_DATE)
            throw Exception("Must be a DATE");
        return _integer;
    }

    Timestamp Value::getDateAsTimestamp() const {
        if (_type != TYPE_DATE)
            throw Exception("Must be a DATE");
        return Timestamp(_integer * 1000);
    }

    const std::string& Value::getString() const {
        if (_type != TYPE_STRING)
            throw Exception("Must be a STRING");
        return _string;
    }

    const std::string& Value::getXml() const {
        if (_type != TYPE_XML)
            throw Exception("Must be a XML");
        return _string;
    }

    const std::string& Value::getBinary() const {
        if (_type != TYPE_BINARY)
            throw Exception("Must be a BINARY");
        return _string;
    }

    const std::string& Value::getListType() const {
        if (_type != TYPE_LIST)
            throw Exception("Must be a LIST");
        return _string;
    }

    const Value::List& Value::getList() const {
        if (_type != TYPE_LIST)
            throw Exception("Must be a LIST");
        return _list;
    }

    Value::List::size_type Value::getListSize() const {
        if (_type != TYPE_LIST)
            throw Exception("Must be a LIST");
        return _list.size();
    }

    const std::string& Value::getMapType() const {
        if (_type != TYPE_MAP)
            throw Exception("Must be a MAP");
        return _string;
    }

    const Value::Map& Value::getMap() const {
        if (_type != TYPE_MAP)
            throw Exception("Must be a MAP");
        return _map;
    }

    Value::Map::size_type Value::getMapSize() const {
        if (_type != TYPE_MAP)
            throw Exception("Must be a MAP");
        return _map.size();
    }

    const std::string& Value::getRemoteType() const {
        if (_type != TYPE_REMOTE)
            throw Exception("Must be a REMOTE");
        return _string;
    }

    const std::string& Value::getRemoteUrl() const {
        if (_type != TYPE_REMOTE)
            throw Exception("Must be a REMOTE");
        return _string2;
    }

    const std::string& Value::getFaultCode() const {
        if (_type != TYPE_FAULT)
            throw Exception("Must be a FAULT");
        return _string;
    }

    const std::string& Value::getFaultMessage() const {
        if (_type != TYPE_FAULT)
            throw Exception("Must be a FAULT");
        return _string2;
    }

    const ValuePtr& Value::getFaultDetail() const {
        if (_type != TYPE_FAULT)
            throw Exception("Must be a FAULT");
        return _value;
    }

    void Value::reserve(const List::size_type n) {
        if (_type != TYPE_LIST)
            throw Exception("Must be a LIST");
        _list.reserve(n);
    }

    void Value::add(const Value::List::value_type& value) {
        if (_type != TYPE_LIST)
            throw Exception("Must be a LIST");
        _list.push_back(value);
    }

    const Value::List::value_type& Value::atIndex(const Value::List::size_type n) const {
        if (_type != TYPE_LIST)
            throw Exception("Must be a LIST");
        return _list.at(n);
    }

    std::pair<Value::Map::iterator, bool> Value::put(const Value::Map::key_type& key, const Value::Map::mapped_type& value) {
        if (_type != TYPE_MAP)
            throw Exception("Must be a MAP");
        return _map.insert(Value::Map::value_type(key, value));
    }

    const Value::Map::mapped_type& Value::atKey(const Value::Map::key_type& key) const {
        if (_type != TYPE_MAP)
            throw Exception("Must be a MAP");
        return _map.find(key)->second;
    }

    const Value::Map::mapped_type& Value::atKey(const Value& key) const {
        if (_type != TYPE_MAP)
            throw Exception("Must be a MAP");
        ValuePtr ptr = new Value(key);
        return _map.find(ptr)->second;
    }

    bool Value::operator<(const Value& value) const {
        // TODO: good enough for now
        if (_type == value._type) {
            switch (_type) {
                case TYPE_NULL:
                    return false;
                case TYPE_BOOLEAN:
                    return _bool < value._bool;
                case TYPE_INTEGER:
                case TYPE_LONG:
                case TYPE_DATE:
                    return _integer < value._integer;
                case TYPE_DOUBLE:
                    return _double < value._double;
                case TYPE_STRING:
                case TYPE_XML:
                case TYPE_BINARY:
                    return _string < value._string;
                case TYPE_LIST:
                    return _list < value._list;
                case TYPE_MAP:
                    return _map < value._map;
                case TYPE_REMOTE:
                    return _string + _string2 < value._string + value._string2;
                case TYPE_FAULT:
                    return _string + _string2 < value._string + value._string2;
            }
        }
        return _type < value._type;
    }

    std::ostream& operator<<(std::ostream& out, const Value* value) {
        // TODO: good enough for now
        switch (value->_type) {
            case Value::TYPE_NULL:
                out << "NULL";
                break;
            case Value::TYPE_BOOLEAN:
                out << value->_bool;
                break;
            case Value::TYPE_INTEGER:
            case Value::TYPE_LONG:
            case Value::TYPE_DATE:
                out << value->_integer;
                break;
            case Value::TYPE_DOUBLE:
                out << value->_double;
                break;
            case Value::TYPE_STRING:
            case Value::TYPE_XML:
                out << value->_string;
                break;
            case Value::TYPE_BINARY:
                out << "binary(" << value->_string.size() << ")";
                break;
            case Value::TYPE_LIST:
                out << "list(" << value->_string << ")";
                break;
            case Value::TYPE_MAP:
                out << "map(" << value->_string << ")";
                break;
            case Value::TYPE_REMOTE:
                out << "remote(" << value->_string << ", " << value->_string2 << ")";
                break;
            case Value::TYPE_FAULT:
                out << "fault(" << value->_string << ", " << value->_string2 << ")";
                break;
        }
        return out;
    }

    /////////////////
    // Header

    Header::Header(const std::string& name, const ValuePtr& value)
    : _name(name),
    _value(value) {
    }

    const std::string& Header::getName() const {
        return _name;
    }

    const ValuePtr& Header::getValue() const {
        return _value;
    }

    /////////////////
    // Call

    Call::Call(const std::string& method)
    : _method(method) {
    }

    Call::Call(const std::string& method, const HeaderList& headers)
    : _method(method),
    _headers(headers) {
    }

    Call::Call(const std::string& method, const ParameterList& parameters)
    : _method(method),
    _parameters(parameters) {
    }

    Call::Call(const std::string& method, const HeaderList& headers, const ParameterList& parameters)
    : _method(method),
    _headers(headers),
    _parameters(parameters) {
    }

    const std::string& Call::getMethod() const {
        return _method;
    }

    const HeaderList& Call::getHeaders() const {
        return _headers;
    }

    const ParameterList& Call::getParameters() const {
        return _parameters;
    }

    /////////////////
    // Reply

    Reply::Reply(const ValuePtr& value)
    : _value(value) {
    }

    Reply::Reply(const HeaderList& headers, const ValuePtr& value)
    : _headers(headers),
    _value(value) {
    }

    const HeaderList& Reply::getHeaders() const {
        return _headers;
    }

    const ValuePtr& Reply::getValue() const {
        return _value;
    }

    /////////////////
    // Reply

    HessianException::HessianException(const std::string& code, const std::string& message, const ValuePtr& detail)
    : _code(code),
    _message(message),
    _detail(detail) {
    }

    HessianException::~HessianException() throw () {
    }

    const char* HessianException::what() const throw () {
        return (_code + _message).c_str();
    }

    const std::string& HessianException::getCode() const {
        return _code;
    }

    const std::string& HessianException::getMessage() const {
        return _message;
    }

    const ValuePtr& HessianException::getDetail() const {
        return _detail;
    }

}
