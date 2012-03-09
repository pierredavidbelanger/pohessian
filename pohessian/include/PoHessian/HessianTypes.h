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

#ifndef PoHessian_HessianTypes_INCLUDED
#define PoHessian_HessianTypes_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "PoHessian/PoHessian.h"

#include "Poco/SharedPtr.h"
#include "Poco/Types.h"
#include "Poco/Timestamp.h"

namespace PoHessian {

    template <class C, class RC = Poco::ReferenceCounter>
    class Ptr {
    public:

        Ptr() : _strong(false), _rc(NULL), _ptr(NULL) {
        }

        Ptr(C* ptr) : _strong(true), _rc(new RC), _ptr(ptr) {
        }

        Ptr(const Ptr& ptr) : _strong(ptr._strong), _rc(ptr._rc), _ptr(ptr._ptr) {
            if (_strong)
                _rc->duplicate();
        }

        Ptr(const Ptr& ptr, bool strong) : _strong(strong), _rc(ptr._rc), _ptr(ptr._ptr) {
            if (_strong)
                _rc->duplicate();
        }

        ~Ptr() {
            if (_strong)
                if (_rc->release() == 0) {
                    delete _ptr;
                    delete _rc;
                }
        }

        C* deref() const {
            if (!_ptr)
                throw Poco::NullPointerException();
            return _ptr;
        }

        void swap(Ptr& ptr) {
            std::swap(_strong, ptr._strong);
            std::swap(_rc, ptr._rc);
            std::swap(_ptr, ptr._ptr);
        }

        Ptr& operator=(const Ptr& ptr) {
            Ptr tmp(ptr);
            swap(tmp);
            return *this;
        }

        C* operator->() {
            return deref();
        }

        const C* operator->() const {
            return deref();
        }

        C& operator*() {
            return *deref();
        }

        const C& operator*() const {
            return *deref();
        }

        bool operator!() const {
            return _ptr == NULL;
        }

        bool operator==(const Ptr& ptr) const {
            return _ptr == ptr._ptr;
        }

        bool operator!=(const Ptr& ptr) const {
            return _ptr != ptr._ptr;
        }

        bool operator<(const Ptr& ptr) const {
            return _ptr < ptr._ptr;
        }

    protected:
        bool _strong;
        RC* _rc;
        C* _ptr;
    };

    template <typename PtrType>
    struct PoHessian_API PtrLess : public std::binary_function<const PtrType, const PtrType, bool> {

        bool operator()(const PtrType o1, const PtrType o2) const {
            return *o1 < *o2;
        }

    };

    class Value;

    typedef Ptr<Value> ValuePtr;

    class PoHessian_API Value {
    public:

        typedef std::vector<ValuePtr> List;
        typedef std::map<ValuePtr, ValuePtr, PtrLess<ValuePtr> > Map;

        enum Type {
            TYPE_NULL,
            TYPE_BOOLEAN,
            TYPE_INTEGER,
            TYPE_LONG,
            TYPE_DOUBLE,
            TYPE_DATE,
            TYPE_STRING,
            TYPE_XML,
            TYPE_BINARY,
            TYPE_LIST,
            TYPE_MAP,
            TYPE_REMOTE,
            TYPE_FAULT
        };

        Value(const Value& value);
        Value(const Type type = TYPE_NULL);
        Value(const bool boolean);
        Value(const Poco::Int32 integer);
        Value(const double value);
        Value(const Poco::Int64 value, const Type type = TYPE_LONG);
        Value(const Poco::Timestamp dateAsTimestamp);
        Value(const char* value, const Type type = TYPE_STRING);
        Value(const std::string& value, const Type type = TYPE_STRING);
        Value(const List& list, const char* listType = "");
        Value(const List& list, const std::string& listType = "");
        Value(const Map& map, const char* mapType = "");
        Value(const Map& map, const std::string& mapType = "");
        Value(const char* remoteType, const char* remoteUrl);
        Value(const std::string& remoteType, const std::string& remoteUrl);
        Value(const char* faultCode, const char* faultMessage, const ValuePtr& faultDetail);
        Value(const std::string& faultCode, const std::string& faultMessage, const ValuePtr& faultDetail);

        Type getType() const;

        bool isNull() const;
        bool isBoolean() const;
        bool isInteger() const;
        bool isLong() const;
        bool isDouble() const;
        bool isDate() const;
        bool isString() const;
        bool isXml() const;
        bool isBinary() const;
        bool isList() const;
        bool isMap() const;
        bool isRemote() const;
        bool isFault() const;

        bool getBoolean() const;
        Poco::Int32 getInteger() const;
        Poco::Int64 getLong() const;
        double getDouble() const;
        Poco::Int64 getDateAsLong() const;
        Poco::Timestamp getDateAsTimestamp() const;
        const std::string& getString() const;
        const std::string& getXml() const;
        const std::string& getBinary() const;
        const std::string& getListType() const;
        const List& getList() const;
        List::size_type getListSize() const;
        const std::string& getMapType() const;
        const Map& getMap() const;
        Map::size_type getMapSize() const;
        const std::string& getRemoteType() const;
        const std::string& getRemoteUrl() const;
        const std::string& getFaultCode() const;
        const std::string& getFaultMessage() const;
        const ValuePtr& getFaultDetail() const;

        void reserve(const List::size_type n);
        void add(const List::value_type& value);
        const List::value_type& atIndex(const List::size_type n) const;

        std::pair<Map::iterator, bool> put(const Map::key_type& key, const Map::mapped_type& value);
        const Map::mapped_type& atKey(const Map::key_type& key) const;
        const Map::mapped_type& atKey(const Value& key) const;

        bool operator<(const Value& value) const;

        friend std::ostream& operator<<(std::ostream& out, const Value* value);

    private:
        Type _type;
        bool _bool;
        Poco::Int64 _integer;
        double _double;
        std::string _string;
        std::string _string2;
        List _list;
        Map _map;
        ValuePtr _value;
    };

    typedef std::vector<ValuePtr> RefList;

    class PoHessian_API Header {
    public:

        Header(const std::string& name, const ValuePtr& value);

        const std::string& getName() const;
        const ValuePtr& getValue() const;

    private:
        std::string _name;
        ValuePtr _value;
    };

    typedef Ptr<Header> HeaderPtr;

    typedef std::vector<HeaderPtr> HeaderList;

    typedef std::vector<ValuePtr> ParameterList;

    class PoHessian_API Call {
    public:

        Call(const std::string& method);
        Call(const std::string& method, const HeaderList& headers);
        Call(const std::string& method, const ParameterList& parameters);
        Call(const std::string& method, const HeaderList& headers, const ParameterList& parameters);

        const std::string& getMethod() const;
        const HeaderList& getHeaders() const;
        const ParameterList& getParameters() const;

    private:
        std::string _method;
        HeaderList _headers;
        ParameterList _parameters;
    };

    typedef Ptr<Call> CallPtr;

    class PoHessian_API Reply {
    public:

        Reply(const ValuePtr& value);
        Reply(const HeaderList& headers, const ValuePtr& value);

        const HeaderList& getHeaders() const;
        const ValuePtr& getValue() const;

    private:
        HeaderList _headers;
        ValuePtr _value;
    };

    typedef Ptr<Reply> ReplyPtr;

    class PoHessian_API HessianException : public std::exception {
    public:

        HessianException(const std::string& code, const std::string& message, const ValuePtr& detail);

        virtual ~HessianException() throw ();

        virtual const char* what() const throw ();
        const std::string& getCode() const;
        const std::string& getMessage() const;
        const ValuePtr& getDetail() const;

    private:
        std::string _code;
        std::string _message;
        ValuePtr _detail;
    };

}

#endif
