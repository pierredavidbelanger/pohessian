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
#include <map>
#include <iostream>
#include <sstream>
#include <typeinfo>

#include <math.h>

#include "hessian_tests.h"

#include "PoHessian/HessianTypes.h"
#include "PoHessian/HessianClient.h"

#include "Poco/Types.h"
#include "Poco/Timespan.h"
#include "Poco/Exception.h"

using namespace PoHessian;

using Poco::Int32;
using Poco::Int64;
using Poco::Timestamp;
using Poco::Exception;

static void nullCall(HessianClient& client) {
    if (!client.call("nullCall")->isNull()) throw Exception("Should be NULL");
}

static void hello(HessianClient& client) {
    if (client.call("hello")->getString() != "Hello, World") throw Exception("Should be String 'Hello, World'");
}

static void subtract(HessianClient& client) {
    ParameterList parameters;
    parameters.push_back(new Value(50));
    parameters.push_back(new Value(3));
    if (client.call("subtract", parameters)->getInteger() != 47) throw Exception("Should be Integer 47");
}

static void echo(HessianClient& client) {
    HeaderList headers;
    headers.push_back(new Header("test", new Value(true)));
    Timestamp now;
    ParameterList parameters;
    parameters.push_back(new Value(now));
    if (client.call("echo", headers, parameters)->getDateAsLong() != now.epochMicroseconds() / 1000) throw Exception("Should be Date NOW");
}

static void fault(HessianClient& client) {
    static const char* s = "Should have thrown NullPointerException('sample exception')";
    try {
        client.call("fault");
        throw Exception(s);
    } catch (HessianException& e) {
        if (!e.getDetail()->isMap()) throw Exception(s);
        const ValuePtr& nullPointerException = e.getDetail();
        if (nullPointerException->getMapType() != "java.lang.NullPointerException") throw Exception(s);
        const ValuePtr& cause = nullPointerException->atKey(Value("cause"));
        if (cause != nullPointerException) throw Exception(s);
        const ValuePtr& detailMessage = nullPointerException->atKey(Value("detailMessage"));
        if (!detailMessage->isString()) throw Exception(s);
        if (detailMessage->getString() != "sample exception") throw Exception(s);
    }
}

static void replyNull(HessianClient& client) {
    if (!client.call("replyNull")->isNull()) throw Exception("Should be NULL");
}

static void replyTrue(HessianClient& client) {
    if (!client.call("replyTrue")->getBoolean()) throw Exception("Should be true");
}

static void replyFalse(HessianClient& client) {
    if (client.call("replyFalse")->getBoolean()) throw Exception("Should be false");
}

static void replyInt_0(HessianClient& client) {
    if (client.call("replyInt_0")->getInteger() != 0) throw Exception("Should be Integer 0");
}

static void replyInt_1(HessianClient& client) {
    if (client.call("replyInt_1")->getInteger() != 1) throw Exception("Should be Integer 1");
}

static void replyInt_47(HessianClient& client) {
    if (client.call("replyInt_47")->getInteger() != 47) throw Exception("Should be Integer 47");
}

static void replyInt_m16(HessianClient& client) {
    if (client.call("replyInt_m16")->getInteger() != -16) throw Exception("Should be Integer -16");
}

static void replyInt_0x30(HessianClient& client) {
    if (client.call("replyInt_0x30")->getInteger() != 0x30) throw Exception("Should be Integer 0x30");
}

static void replyInt_0x7ff(HessianClient& client) {
    if (client.call("replyInt_0x7ff")->getInteger() != 0x7ff) throw Exception("Should be Integer 0x7ff");
}

static void replyInt_m0x800(HessianClient& client) {
    if (client.call("replyInt_m0x800")->getInteger() != -0x800) throw Exception("Should be Integer -0x800");
}

static void replyInt_0x800(HessianClient& client) {
    if (client.call("replyInt_0x800")->getInteger() != 0x800) throw Exception("Should be Integer 0x800");
}

static void replyInt_0x3ffff(HessianClient& client) {
    if (client.call("replyInt_0x3ffff")->getInteger() != 0x3ffff) throw Exception("Should be Integer 0x3ffff");
}

static void replyInt_m0x801(HessianClient& client) {
    if (client.call("replyInt_m0x801")->getInteger() != -0x801) throw Exception("Should be Integer -0x801");
}

static void replyInt_m0x40000(HessianClient& client) {
    if (client.call("replyInt_m0x40000")->getInteger() != -0x40000) throw Exception("Should be Integer -0x40000");
}

static void replyInt_0x40000(HessianClient& client) {
    if (client.call("replyInt_0x40000")->getInteger() != 0x40000) throw Exception("Should be Integer 0x40000");
}

static void replyInt_0x7fffffff(HessianClient& client) {
    if (client.call("replyInt_0x7fffffff")->getInteger() != 0x7fffffff) throw Exception("Should be Integer 0x7fffffff");
}

static void replyInt_m0x40001(HessianClient& client) {
    if (client.call("replyInt_m0x40001")->getInteger() != -0x40001) throw Exception("Should be Integer -0x40001");
}

static void replyInt_m0x80000000(HessianClient& client) {
    if (client.call("replyInt_m0x80000000")->getInteger() != (Int32)-0x80000000) throw Exception("Should be Integer -0x80000000");
}

static void replyLong_0(HessianClient& client) {
    if (client.call("replyLong_0")->getLong() != 0LL) throw Exception("Should be Long 0");
}

static void replyLong_1(HessianClient& client) {
    if (client.call("replyLong_1")->getLong() != 1LL) throw Exception("Should be Long 1");
}

static void replyLong_15(HessianClient& client) {
    if (client.call("replyLong_15")->getLong() != 15LL) throw Exception("Should be Long 15");
}

static void replyLong_m8(HessianClient& client) {
    if (client.call("replyLong_m8")->getLong() != -8LL) throw Exception("Should be Long -8");
}

static void replyLong_0x10(HessianClient& client) {
    if (client.call("replyLong_0x10")->getLong() != 0x10LL) throw Exception("Should be Long 0x10");
}

static void replyLong_0x7ff(HessianClient& client) {
    if (client.call("replyLong_0x7ff")->getLong() != 0x7ffLL) throw Exception("Should be Long 0x7ff");
}

static void replyLong_m9(HessianClient& client) {
    if (client.call("replyLong_m9")->getLong() != -9LL) throw Exception("Should be Long -9");
}

static void replyLong_m0x800(HessianClient& client) {
    if (client.call("replyLong_m0x800")->getLong() != -0x800LL) throw Exception("Should be Long -0x800");
}

static void replyLong_0x800(HessianClient& client) {
    if (client.call("replyLong_0x800")->getLong() != 0x800LL) throw Exception("Should be Long 0x800");
}

static void replyLong_0x3ffff(HessianClient& client) {
    if (client.call("replyLong_0x3ffff")->getLong() != 0x3ffffLL) throw Exception("Should be Long 0x3ffff");
}

static void replyLong_m0x801(HessianClient& client) {
    if (client.call("replyLong_m0x801")->getLong() != -0x801LL) throw Exception("Should be Long -0x801");
}

static void replyLong_m0x40000(HessianClient& client) {
    if (client.call("replyLong_m0x40000")->getLong() != -0x40000LL) throw Exception("Should be Long -0x40000");
}

static void replyLong_0x40000(HessianClient& client) {
    if (client.call("replyLong_0x40000")->getLong() != 0x40000LL) throw Exception("Should be Long 0x40000");
}

static void replyLong_0x7fffffff(HessianClient& client) {
    if (client.call("replyLong_0x7fffffff")->getLong() != 0x7fffffffLL) throw Exception("Should be Long 0x7fffffff");
}

static void replyLong_m0x40001(HessianClient& client) {
    if (client.call("replyLong_m0x40001")->getLong() != -0x40001LL) throw Exception("Should be Long -0x40001");
}

static void replyLong_m0x80000000(HessianClient& client) {
    if (client.call("replyLong_m0x80000000")->getLong() != -0x80000000LL) throw Exception("Should be Long -0x80000000");
}

static void replyLong_0x80000000(HessianClient& client) {
    if (client.call("replyLong_0x80000000")->getLong() != 0x80000000LL) throw Exception("Should be Long 0x80000000");
}

static void replyLong_m0x80000001(HessianClient& client) {
    if (client.call("replyLong_m0x80000001")->getLong() != -0x80000001LL) throw Exception("Should be Long -0x80000001");
}

static void replyDouble_0_0(HessianClient& client) {
    if (client.call("replyDouble_0_0")->getDouble() != 0.0) throw Exception("Should be Double 0.0");
}

static void replyDouble_1_0(HessianClient& client) {
    if (client.call("replyDouble_1_0")->getDouble() != 1.0) throw Exception("Should be Double 1.0");
}

static void replyDouble_2_0(HessianClient& client) {
    if (client.call("replyDouble_2_0")->getDouble() != 2.0) throw Exception("Should be Double 2.0");
}

static void replyDouble_127_0(HessianClient& client) {
    if (client.call("replyDouble_127_0")->getDouble() != 127.0) throw Exception("Should be Double 127.0");
}

static void replyDouble_m128_0(HessianClient& client) {
    if (client.call("replyDouble_m128_0")->getDouble() != -128.0) throw Exception("Should be Double -128.0");
}

static void replyDouble_128_0(HessianClient& client) {
    if (client.call("replyDouble_128_0")->getDouble() != 128.0) throw Exception("Should be Double 128.0");
}

static void replyDouble_m129_0(HessianClient& client) {
    if (client.call("replyDouble_m129_0")->getDouble() != -129.0) throw Exception("Should be Double -129.0");
}

static void replyDouble_32767_0(HessianClient& client) {
    if (client.call("replyDouble_32767_0")->getDouble() != 32767.0) throw Exception("Should be Double 32767.0");
}

static void replyDouble_m32768_0(HessianClient& client) {
    if (client.call("replyDouble_m32768_0")->getDouble() != -32768.0) throw Exception("Should be Double -32768.0");
}

static void replyDouble_0_001(HessianClient& client) {
    if (client.call("replyDouble_0_001")->getDouble() != 0.001) throw Exception("Should be Double 0.001");
}

static void replyDouble_m0_001(HessianClient& client) {
    if (client.call("replyDouble_m0_001")->getDouble() != -0.001) throw Exception("Should be Double -0.001");
}

static void replyDouble_65_536(HessianClient& client) {
    if (client.call("replyDouble_65_536")->getDouble() != 65.536) throw Exception("Should be Double 65.536");
}

static void replyDouble_3_14159(HessianClient& client) {
    if (client.call("replyDouble_3_14159")->getDouble() != 3.14159) throw Exception("Should be Double 3.14159");
}

static void replyDate_0(HessianClient& client) {
    if (client.call("replyDate_0")->getDateAsLong() != 0L) throw Exception("Should be Date 0");
}

static void replyDate_1(HessianClient& client) {
    if (client.call("replyDate_1")->getDateAsLong() != 894621091000LL) throw Exception("Should be Date 894621091000");
}

static void replyDate_2(HessianClient& client) {
    if (client.call("replyDate_2")->getDateAsLong() != 894621091000LL - (894621091000LL % 60000LL)) throw Exception("Should be Date 894621091000L - (894621091000 % 60000)");
}

static void replyString_0(HessianClient& client) {
    if (client.call("replyString_0")->getString() != "") throw Exception("Should be String ''");
}

static void replyString_null(HessianClient& client) {
    if (!client.call("replyString_null")->isNull()) throw Exception("Should be NULL");
}

static void replyString_1(HessianClient& client) {
    if (client.call("replyString_1")->getString() != "0") throw Exception("Should be String '0'");
}

static void replyString_31(HessianClient& client) {
    if (client.call("replyString_31")->getString() != "0123456789012345678901234567890") throw Exception("Should be String '0123456789012345678901234567890'");
}

static void replyString_32(HessianClient& client) {
    if (client.call("replyString_32")->getString() != "01234567890123456789012345678901") throw Exception("Should be String '0123456789012345678901234567890'");
}

static void replyString_1023(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 16; i++) {
        sb << (i / 10) << (i % 10) << " 456789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    s.resize(1023);
    if (client.call("replyString_1023")->getString() != s) throw Exception("Should be String length 1023");
}

static void replyString_1024(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 16; i++) {
        sb << (i / 10) << (i % 10) << " 456789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    if (client.call("replyString_1024")->getString() != s) throw Exception("Should be String length 1024");
}

static void replyString_65536(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 64 * 16; i++) {
        sb << (i / 100) << (i / 10 % 10) << (i % 10) << " 56789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    s.resize(65536);
    if (client.call("replyString_65536")->getString() != s) throw Exception("Should be String length 65536");
}

static void replyBinary_0(HessianClient& client) {
    if (client.call("replyBinary_0")->getBinary() != "") throw Exception("Should be Binary ''");
}

static void replyBinary_null(HessianClient& client) {
    if (!client.call("replyBinary_null")->isNull()) throw Exception("Should be NULL");
}

static void replyBinary_1(HessianClient& client) {
    if (client.call("replyBinary_1")->getBinary() != "0") throw Exception("Should be Binary '0'");
}

static void replyBinary_15(HessianClient& client) {
    if (client.call("replyBinary_15")->getBinary() != "012345678901234") throw Exception("Should be Binary '012345678901234'");
}

static void replyBinary_16(HessianClient& client) {
    if (client.call("replyBinary_16")->getBinary() != "0123456789012345") throw Exception("Should be Binary '0123456789012345'");
}

static void replyBinary_1023(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 16; i++) {
        sb << (i / 10) << (i % 10) << " 456789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    s.resize(1023);
    if (client.call("replyBinary_1023")->getBinary() != s) throw Exception("Should be Binary length 1023");
}

static void replyBinary_1024(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 16; i++) {
        sb << (i / 10) << (i % 10) << " 456789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    if (client.call("replyBinary_1024")->getBinary() != s) throw Exception("Should be Binary length 1024");
}

static void replyBinary_65536(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 64 * 16; i++) {
        sb << (i / 100) << (i / 10 % 10) << (i % 10) << " 56789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    s.resize(65536);
    if (client.call("replyBinary_65536")->getBinary() != s) throw Exception("Should be Binary length 65536");
}

static void replyUntypedFixedList_0(HessianClient& client) {
    ValuePtr list = client.call("replyUntypedFixedList_0");
    if (list->getListType() != "") throw Exception("Should be List untyped");
    if (list->getListSize() != 0) throw Exception("Should be List length 0");
}

static void replyUntypedFixedList_1(HessianClient& client) {
    ValuePtr list = client.call("replyUntypedFixedList_1");
    if (list->getListType() != "") throw Exception("Should be List untyped");
    if (list->getListSize() != 1) throw Exception("Should be List length 1");
    if (list->atIndex(0)->getString() != "1") throw Exception("Should be List element 0 '1'");
}

static void replyUntypedFixedList_7(HessianClient& client) {
    ValuePtr list = client.call("replyUntypedFixedList_7");
    if (list->getListType() != "") throw Exception("Should be List untyped");
    if (list->getListSize() != 7) throw Exception("Should be List length 7");
    if (list->atIndex(0)->getString() != "1") throw Exception("Should be List element 0 '1'");
    if (list->atIndex(1)->getString() != "2") throw Exception("Should be List element 1 '2'");
    if (list->atIndex(2)->getString() != "3") throw Exception("Should be List element 2 '3'");
    if (list->atIndex(3)->getString() != "4") throw Exception("Should be List element 3 '4'");
    if (list->atIndex(4)->getString() != "5") throw Exception("Should be List element 4 '5'");
    if (list->atIndex(5)->getString() != "6") throw Exception("Should be List element 5 '6'");
    if (list->atIndex(6)->getString() != "7") throw Exception("Should be List element 6 '7'");
}

static void replyUntypedFixedList_8(HessianClient& client) {
    ValuePtr list = client.call("replyUntypedFixedList_8");
    if (list->getListType() != "") throw Exception("Should be List untyped");
    if (list->getListSize() != 8) throw Exception("Should be List length 8");
    if (list->atIndex(0)->getString() != "1") throw Exception("Should be List element 0 '1'");
    if (list->atIndex(1)->getString() != "2") throw Exception("Should be List element 1 '2'");
    if (list->atIndex(2)->getString() != "3") throw Exception("Should be List element 2 '3'");
    if (list->atIndex(3)->getString() != "4") throw Exception("Should be List element 3 '4'");
    if (list->atIndex(4)->getString() != "5") throw Exception("Should be List element 4 '5'");
    if (list->atIndex(5)->getString() != "6") throw Exception("Should be List element 5 '6'");
    if (list->atIndex(6)->getString() != "7") throw Exception("Should be List element 6 '7'");
    if (list->atIndex(7)->getString() != "8") throw Exception("Should be List element 7 '8'");
}

static void replyTypedFixedList_0(HessianClient& client) {
    ValuePtr list = client.call("replyTypedFixedList_0");
    if (list->getListType() != "[string") throw Exception("Should be List typed [string");
    if (list->getListSize() != 0) throw Exception("Should be List length 0");
}

static void replyTypedFixedList_1(HessianClient& client) {
    ValuePtr list = client.call("replyTypedFixedList_1");
    if (list->getListType() != "[string") throw Exception("Should be List typed [string");
    if (list->getListSize() != 1) throw Exception("Should be List length 1");
    if (list->atIndex(0)->getString() != "1") throw Exception("Should be List element 0 '1'");
}

static void replyTypedFixedList_7(HessianClient& client) {
    ValuePtr list = client.call("replyTypedFixedList_7");
    if (list->getListType() != "[string") throw Exception("Should be List typed [string");
    if (list->getListSize() != 7) throw Exception("Should be List length 7");
    if (list->atIndex(0)->getString() != "1") throw Exception("Should be List element 0 '1'");
    if (list->atIndex(1)->getString() != "2") throw Exception("Should be List element 1 '2'");
    if (list->atIndex(2)->getString() != "3") throw Exception("Should be List element 2 '3'");
    if (list->atIndex(3)->getString() != "4") throw Exception("Should be List element 3 '4'");
    if (list->atIndex(4)->getString() != "5") throw Exception("Should be List element 4 '5'");
    if (list->atIndex(5)->getString() != "6") throw Exception("Should be List element 5 '6'");
    if (list->atIndex(6)->getString() != "7") throw Exception("Should be List element 6 '7'");
}

static void replyTypedFixedList_8(HessianClient& client) {
    ValuePtr list = client.call("replyTypedFixedList_8");
    if (list->getListType() != "[string") throw Exception("Should be List typed [string");
    if (list->getListSize() != 8) throw Exception("Should be List length 8");
    if (list->atIndex(0)->getString() != "1") throw Exception("Should be List element 0 '1'");
    if (list->atIndex(1)->getString() != "2") throw Exception("Should be List element 1 '2'");
    if (list->atIndex(2)->getString() != "3") throw Exception("Should be List element 2 '3'");
    if (list->atIndex(3)->getString() != "4") throw Exception("Should be List element 3 '4'");
    if (list->atIndex(4)->getString() != "5") throw Exception("Should be List element 4 '5'");
    if (list->atIndex(5)->getString() != "6") throw Exception("Should be List element 5 '6'");
    if (list->atIndex(6)->getString() != "7") throw Exception("Should be List element 6 '7'");
    if (list->atIndex(7)->getString() != "8") throw Exception("Should be List element 7 '8'");
}

static void replyUntypedMap_0(HessianClient& client) {
    ValuePtr map = client.call("replyUntypedMap_0");
    if (map->getMapType() != "") throw Exception("Should be Map untyped");
    if (map->getMapSize() != 0) throw Exception("Should be Map length 0");
}

static void replyUntypedMap_1(HessianClient& client) {
    ValuePtr map = client.call("replyUntypedMap_1");
    if (map->getMapType() != "") throw Exception("Should be Map untyped");
    if (map->getMapSize() != 1) throw Exception("Should be Map length 1");
    if (map->atKey("a") == NULL || map->atKey("a")->getInteger() != 0) throw Exception("Should be Map element 'a' 0");
}

static void replyUntypedMap_2(HessianClient& client) {
    ValuePtr map = client.call("replyUntypedMap_2");
    if (map->getMapType() != "") throw Exception("Should be Map untyped");
    if (map->getMapSize() != 2) throw Exception("Should be Map length 2");
    if (map->atKey(Value(0)) == NULL || map->atKey(Value(0))->getString() != "a") throw Exception("Should be Map element 0 'a'");
    if (map->atKey(1) == NULL || map->atKey(1)->getString() != "b") throw Exception("Should be Map element 1 'b'");
}

static void replyUntypedMap_3(HessianClient& client) {
    ValuePtr map = client.call("replyUntypedMap_3");
    if (map->getMapType() != "") throw Exception("Should be Map untyped");
    if (map->getMapSize() != 1) throw Exception("Should be Map length 1");
    Value::Map::value_type entry = *map->getMap().begin();
    ValuePtr list = entry.first;
    ValuePtr zero = entry.second;
    if (list->atIndex(0)->getString() != "a") throw Exception("Should be Map element 0 first List element 0 'a'");
    if (zero->getInteger() != 0) throw Exception("Should be Map element 0 second 0");
}

static void replyTypedMap_0(HessianClient& client) {
    ValuePtr map = client.call("replyTypedMap_0");
    if (map->getMapType() != "java.util.Hashtable") throw Exception("Should be Map typed Hashtable");
    if (map->getMapSize() != 0) throw Exception("Should be Map length 0");
}

static void replyTypedMap_1(HessianClient& client) {
    ValuePtr map = client.call("replyTypedMap_1");
    if (map->getMapType() != "java.util.Hashtable") throw Exception("Should be Map typed Hashtable");
    if (map->getMapSize() != 1) throw Exception("Should be Map length 1");
    if (map->atKey("a") == NULL || map->atKey("a")->getInteger() != 0) throw Exception("Should be Map element 'a' 0");
}

static void replyTypedMap_2(HessianClient& client) {
    ValuePtr map = client.call("replyTypedMap_2");
    if (map->getMapType() != "java.util.Hashtable") throw Exception("Should be Map typed Hashtable");
    if (map->getMapSize() != 2) throw Exception("Should be Map length 2");
    if (map->atKey(Value(0)) == NULL || map->atKey(Value(0))->getString() != "a") throw Exception("Should be Map element 0 'a'");
    if (map->atKey(1) == NULL || map->atKey(1)->getString() != "b") throw Exception("Should be Map element 1 'b'");
}

static void replyTypedMap_3(HessianClient& client) {
    ValuePtr map = client.call("replyTypedMap_3");
    if (map->getMapType() != "java.util.Hashtable") throw Exception("Should be Map typed Hashtable");
    if (map->getMapSize() != 1) throw Exception("Should be Map length 1");
    Value::Map::value_type entry = *map->getMap().begin();
    ValuePtr list = entry.first;
    ValuePtr zero = entry.second;
    if (list->atIndex(0)->getString() != "a") throw Exception("Should be Map element 0 first List element 0 'a'");
    if (zero->getInteger() != 0) throw Exception("Should be Map element 0 second 0");
}

static void replyObject_0(HessianClient& client) {
    ValuePtr map = client.call("replyObject_0");
    if (map->getMapType() != "com.caucho.hessian.test.A0") throw Exception("Should be Map typed A0");
    if (map->getMapSize() != 0) throw Exception("Should be Map length 0");
}

static void replyObject_16(HessianClient& client) {
    ValuePtr list = client.call("replyObject_16");
    if (list->getListSize() != 17) throw Exception("Should be List length 17");
    if (list->atIndex(0)->getMapType() != "com.caucho.hessian.test.A0") throw Exception("Should be Map typed A0");
    if (list->atIndex(1)->getMapType() != "com.caucho.hessian.test.A1") throw Exception("Should be Map typed A1");
    if (list->atIndex(2)->getMapType() != "com.caucho.hessian.test.A2") throw Exception("Should be Map typed A2");
    if (list->atIndex(3)->getMapType() != "com.caucho.hessian.test.A3") throw Exception("Should be Map typed A3");
    if (list->atIndex(4)->getMapType() != "com.caucho.hessian.test.A4") throw Exception("Should be Map typed A4");
    if (list->atIndex(5)->getMapType() != "com.caucho.hessian.test.A5") throw Exception("Should be Map typed A5");
    if (list->atIndex(6)->getMapType() != "com.caucho.hessian.test.A6") throw Exception("Should be Map typed A6");
    if (list->atIndex(7)->getMapType() != "com.caucho.hessian.test.A7") throw Exception("Should be Map typed A7");
    if (list->atIndex(8)->getMapType() != "com.caucho.hessian.test.A8") throw Exception("Should be Map typed A8");
    if (list->atIndex(9)->getMapType() != "com.caucho.hessian.test.A9") throw Exception("Should be Map typed A9");
    if (list->atIndex(10)->getMapType() != "com.caucho.hessian.test.A10") throw Exception("Should be Map typed A10");
    if (list->atIndex(11)->getMapType() != "com.caucho.hessian.test.A11") throw Exception("Should be Map typed A11");
    if (list->atIndex(12)->getMapType() != "com.caucho.hessian.test.A12") throw Exception("Should be Map typed A12");
    if (list->atIndex(13)->getMapType() != "com.caucho.hessian.test.A13") throw Exception("Should be Map typed A13");
    if (list->atIndex(14)->getMapType() != "com.caucho.hessian.test.A14") throw Exception("Should be Map typed A14");
    if (list->atIndex(15)->getMapType() != "com.caucho.hessian.test.A15") throw Exception("Should be Map typed A15");
    if (list->atIndex(16)->getMapType() != "com.caucho.hessian.test.A16") throw Exception("Should be Map typed A16");
}

static void replyObject_1(HessianClient& client) {
    ValuePtr map = client.call("replyObject_1");
    if (map->getMapType() != "com.caucho.hessian.test.TestObject") throw Exception("Should be Map typed TestObject");
    if (map->getMapSize() != 1) throw Exception("Should be Map length 1");
    if (map->atKey("_value")->getInteger() != 0) throw Exception("Should be Map element _value 0");
}

static void replyObject_2(HessianClient& client) {
    ValuePtr list = client.call("replyObject_2");
    if (list->getListSize() != 2) throw Exception("Should be List length 2");
    ValuePtr map1 = list->atIndex(0);
    if (map1->getMapType() != "com.caucho.hessian.test.TestObject") throw Exception("Should be Map 1 typed TestObject");
    if (map1->getMapSize() != 1) throw Exception("Should be Map 1 length 1");
    if (map1->atKey("_value")->getInteger() != 0) throw Exception("Should be Map 1 element _value 0");
    ValuePtr map2 = list->atIndex(1);
    if (map2->getMapType() != "com.caucho.hessian.test.TestObject") throw Exception("Should be Map 2 typed TestObject");
    if (map2->getMapSize() != 1) throw Exception("Should be Map 2 length 1");
    if (map2->atKey("_value")->getInteger() != 1) throw Exception("Should be Map 2 element _value 1");
}

static void replyObject_2a(HessianClient& client) {
    ValuePtr list = client.call("replyObject_2a");
    if (list->getListSize() != 2) throw Exception("Should be List length 2");
    ValuePtr map1 = list->atIndex(0);
    if (map1->getMapType() != "com.caucho.hessian.test.TestObject") throw Exception("Should be Map 1 typed TestObject");
    if (map1->getMapSize() != 1) throw Exception("Should be Map 1 length 1");
    if (map1->atKey("_value")->getInteger() != 0) throw Exception("Should be Map 1 element _value 0");
    ValuePtr map2 = list->atIndex(1);
    if (map1 != map2) throw Exception("Should be Map 1 same pointer Map 2");
}

static void replyObject_2b(HessianClient& client) {
    ValuePtr list = client.call("replyObject_2b");
    if (list->getListSize() != 2) throw Exception("Should be List length 2");
    ValuePtr map1 = list->atIndex(0);
    if (map1->getMapType() != "com.caucho.hessian.test.TestObject") throw Exception("Should be Map 1 typed TestObject");
    if (map1->getMapSize() != 1) throw Exception("Should be Map 1 length 1");
    if (map1->atKey("_value")->getInteger() != 0) throw Exception("Should be Map 1 element _value 0");
    ValuePtr map2 = list->atIndex(1);
    if (map2->getMapType() != "com.caucho.hessian.test.TestObject") throw Exception("Should be Map 2 typed TestObject");
    if (map2->getMapSize() != 1) throw Exception("Should be Map 2 length 1");
    if (map2->atKey("_value")->getInteger() != 0) throw Exception("Should be Map 2 element _value 0");
}

static void replyObject_3(HessianClient& client) {
    ValuePtr map = client.call("replyObject_3");
    if (map->getMapType() != "com.caucho.hessian.test.TestCons") throw Exception("Should be Map typed TestCons");
    if (map->getMapSize() != 2) throw Exception("Should be Map length 2");
    if (map->atKey("_first")->getString() != "a") throw Exception("Should be Map element _first 'a'");
    if (map->atKey("_rest") != map) throw Exception("Should be Map element _rest same pointer Map");
}

static void arg(HessianClient& client, const std::string& method, ValuePtr parameter) {
    ParameterList parameters;
    parameters.push_back(parameter);
    ValuePtr ret = client.call(method, parameters);
    if (!ret->isBoolean() || !ret->getBoolean()) throw Exception(ret->getString());
}

static void argNull(HessianClient& client) {
    arg(client, "argNull", new Value());
}

static void argTrue(HessianClient& client) {
    arg(client, "argTrue", new Value(true));
}

static void argFalse(HessianClient& client) {
    arg(client, "argFalse", new Value(false));
}

static void argInt_0(HessianClient& client) {
    arg(client, "argInt_0", new Value(0));
}

static void argInt_1(HessianClient& client) {
    arg(client, "argInt_1", new Value(1));
}

static void argInt_47(HessianClient& client) {
    arg(client, "argInt_47", new Value(47));
}

static void argInt_m16(HessianClient& client) {
    arg(client, "argInt_m16", new Value(-16));
}

static void argInt_0x30(HessianClient& client) {
    arg(client, "argInt_0x30", new Value(0x30));
}

static void argInt_0x7ff(HessianClient& client) {
    arg(client, "argInt_0x7ff", new Value(0x7ff));
}

static void argInt_m17(HessianClient& client) {
    arg(client, "argInt_m17", new Value(-17));
}

static void argInt_m0x800(HessianClient& client) {
    arg(client, "argInt_m0x800", new Value(-0x800));
}

static void argInt_0x800(HessianClient& client) {
    arg(client, "argInt_0x800", new Value(0x800));
}

static void argInt_0x3ffff(HessianClient& client) {
    arg(client, "argInt_0x3ffff", new Value(0x3ffff));
}

static void argInt_m0x801(HessianClient& client) {
    arg(client, "argInt_m0x801", new Value(-0x801));
}

static void argInt_m0x40000(HessianClient& client) {
    arg(client, "argInt_m0x40000", new Value(-0x40000));
}

static void argInt_0x40000(HessianClient& client) {
    arg(client, "argInt_0x40000", new Value(0x40000));
}

static void argInt_0x7fffffff(HessianClient& client) {
    arg(client, "argInt_0x7fffffff", new Value(0x7fffffff));
}

static void argInt_m0x40001(HessianClient& client) {
    arg(client, "argInt_m0x40001", new Value(-0x40001));
}

static void argInt_m0x80000000(HessianClient& client) {
    arg(client, "argInt_m0x80000000", new Value((Int32) -0x80000000));
}

static void argLong_0(HessianClient& client) {
    arg(client, "argLong_0", new Value((Int64) 0LL));
}

static void argLong_1(HessianClient& client) {
    arg(client, "argLong_1", new Value((Int64) 1LL));
}

static void argLong_15(HessianClient& client) {
    arg(client, "argLong_15", new Value((Int64) 15LL));
}

static void argLong_m8(HessianClient& client) {
    arg(client, "argLong_m8", new Value((Int64) -8LL));
}

static void argLong_0x10(HessianClient& client) {
    arg(client, "argLong_0x10", new Value((Int64) 0x10LL));
}

static void argLong_0x7ff(HessianClient& client) {
    arg(client, "argLong_0x7ff", new Value((Int64) 0x7ffLL));
}

static void argLong_m9(HessianClient& client) {
    arg(client, "argLong_m9", new Value((Int64) -9LL));
}

static void argLong_m0x800(HessianClient& client) {
    arg(client, "argLong_m0x800", new Value((Int64) -0x800LL));
}

static void argLong_0x800(HessianClient& client) {
    arg(client, "argLong_0x800", new Value((Int64) 0x800LL));
}

static void argLong_0x3ffff(HessianClient& client) {
    arg(client, "argLong_0x3ffff", new Value((Int64) 0x3ffffLL));
}

static void argLong_m0x801(HessianClient& client) {
    arg(client, "argLong_m0x801", new Value((Int64) -0x801LL));
}

static void argLong_m0x40000(HessianClient& client) {
    arg(client, "argLong_m0x40000", new Value((Int64) -0x40000LL));
}

static void argLong_0x40000(HessianClient& client) {
    arg(client, "argLong_0x40000", new Value((Int64) 0x40000LL));
}

static void argLong_0x7fffffff(HessianClient& client) {
    arg(client, "argLong_0x7fffffff", new Value((Int64) 0x7fffffffLL));
}

static void argLong_m0x40001(HessianClient& client) {
    arg(client, "argLong_m0x40001", new Value((Int64) -0x40001LL));
}

static void argLong_m0x80000000(HessianClient& client) {
    arg(client, "argLong_m0x80000000", new Value((Int64) -0x80000000LL));
}

static void argLong_0x80000000(HessianClient& client) {
    arg(client, "argLong_0x80000000", new Value((Int64) 0x80000000LL));
}

static void argLong_m0x80000001(HessianClient& client) {
    arg(client, "argLong_m0x80000001", new Value((Int64) -0x80000001LL));
}

static void argDouble_0_0(HessianClient& client) {
    arg(client, "argDouble_0_0", new Value(0.0));
}

static void argDouble_1_0(HessianClient& client) {
    arg(client, "argDouble_1_0", new Value(1.0));
}

static void argDouble_2_0(HessianClient& client) {
    arg(client, "argDouble_2_0", new Value(2.0));
}

static void argDouble_127_0(HessianClient& client) {
    arg(client, "argDouble_127_0", new Value(127.0));
}

static void argDouble_m128_0(HessianClient& client) {
    arg(client, "argDouble_m128_0", new Value(-128.0));
}

static void argDouble_128_0(HessianClient& client) {
    arg(client, "argDouble_128_0", new Value(128.0));
}

static void argDouble_m129_0(HessianClient& client) {
    arg(client, "argDouble_m129_0", new Value(-129.0));
}

static void argDouble_32767_0(HessianClient& client) {
    arg(client, "argDouble_32767_0", new Value(32767.0));
}

static void argDouble_m32768_0(HessianClient& client) {
    arg(client, "argDouble_m32768_0", new Value(-32768.0));
}

static void argDouble_0_001(HessianClient& client) {
    arg(client, "argDouble_0_001", new Value(0.001));
}

static void argDouble_m0_001(HessianClient& client) {
    arg(client, "argDouble_m0_001", new Value(-0.001));
}

static void argDouble_65_536(HessianClient& client) {
    arg(client, "argDouble_65_536", new Value(65.536));
}

static void argDouble_3_14159(HessianClient& client) {
    arg(client, "argDouble_3_14159", new Value(3.14159));
}

static void argDate_0(HessianClient& client) {
    arg(client, "argDate_0", new Value((Int64) 0, Value::TYPE_DATE));
}

static void argDate_1(HessianClient& client) {
    arg(client, "argDate_1", new Value(894621091000LL, Value::TYPE_DATE));
}

static void argDate_2(HessianClient& client) {
    arg(client, "argDate_2", new Value(894621091000LL - (894621091000LL % 60000LL), Value::TYPE_DATE));
}

static void argString_0(HessianClient& client) {
    arg(client, "argString_0", new Value(""));
}

static void argString_1(HessianClient& client) {
    arg(client, "argString_1", new Value("0"));
}

static void argString_31(HessianClient& client) {
    arg(client, "argString_31", new Value("0123456789012345678901234567890"));
}

static void argString_32(HessianClient& client) {
    arg(client, "argString_32", new Value("01234567890123456789012345678901"));
}

static void argString_1023(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 16; i++) {
        sb << (i / 10) << (i % 10) << " 456789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    s.resize(1023);
    arg(client, "argString_1023", new Value(s));
}

static void argString_1024(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 16; i++) {
        sb << (i / 10) << (i % 10) << " 456789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    arg(client, "argString_1024", new Value(s));
}

static void argString_65536(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 64 * 16; i++) {
        sb << (i / 100) << (i / 10 % 10) << (i % 10) << " 56789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    s.resize(65536);
    arg(client, "argString_65536", new Value(s));
}

static void argBinary_0(HessianClient& client) {
    arg(client, "argBinary_0", new Value("", Value::TYPE_BINARY));
}

static void argBinary_1(HessianClient& client) {
    arg(client, "argBinary_1", new Value("0", Value::TYPE_BINARY));
}

static void argBinary_15(HessianClient& client) {
    arg(client, "argBinary_15", new Value("012345678901234", Value::TYPE_BINARY));
}

static void argBinary_16(HessianClient& client) {
    arg(client, "argBinary_16", new Value("0123456789012345", Value::TYPE_BINARY));
}

static void argBinary_1023(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 16; i++) {
        sb << (i / 10) << (i % 10) << " 456789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    s.resize(1023);
    arg(client, "argBinary_1023", new Value(s, Value::TYPE_BINARY));
}

static void argBinary_1024(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 16; i++) {
        sb << (i / 10) << (i % 10) << " 456789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    arg(client, "argBinary_1024", new Value(s, Value::TYPE_BINARY));
}

static void argBinary_65536(HessianClient& client) {
    std::ostringstream sb;
    for (int i = 0; i < 64 * 16; i++) {
        sb << (i / 100) << (i / 10 % 10) << (i % 10) << " 56789012345678901234567890123456789012345678901234567890123\n";
    }
    std::string s = sb.str();
    s.resize(65536);
    arg(client, "argBinary_65536", new Value(s, Value::TYPE_BINARY));
}

static void argUntypedFixedList_0(HessianClient& client) {
    arg(client, "argUntypedFixedList_0", new Value(Value::TYPE_LIST));
}

static void argUntypedFixedList_1(HessianClient& client) {
    ValuePtr list = new Value(Value::TYPE_LIST);
    list->add(new Value("1"));
    arg(client, "argUntypedFixedList_1", list);
}

static void argUntypedFixedList_7(HessianClient& client) {
    ValuePtr list = new Value(Value::TYPE_LIST);
    list->add(new Value("1"));
    list->add(new Value("2"));
    list->add(new Value("3"));
    list->add(new Value("4"));
    list->add(new Value("5"));
    list->add(new Value("6"));
    list->add(new Value("7"));
    arg(client, "argUntypedFixedList_7", list);
}

static void argUntypedFixedList_8(HessianClient& client) {
    ValuePtr list = new Value(Value::TYPE_LIST);
    list->add(new Value("1"));
    list->add(new Value("2"));
    list->add(new Value("3"));
    list->add(new Value("4"));
    list->add(new Value("5"));
    list->add(new Value("6"));
    list->add(new Value("7"));
    list->add(new Value("8"));
    arg(client, "argUntypedFixedList_8", list);
}

static void argTypedFixedList_0(HessianClient& client) {
    arg(client, "argTypedFixedList_0", new Value("[string", Value::TYPE_LIST));
}

static void argTypedFixedList_1(HessianClient& client) {
    ValuePtr list = new Value("[string", Value::TYPE_LIST);
    list->add(new Value("1"));
    arg(client, "argTypedFixedList_1", list);
}

static void argTypedFixedList_7(HessianClient& client) {
    ValuePtr list = new Value("[string", Value::TYPE_LIST);
    list->add(new Value("1"));
    list->add(new Value("2"));
    list->add(new Value("3"));
    list->add(new Value("4"));
    list->add(new Value("5"));
    list->add(new Value("6"));
    list->add(new Value("7"));
    arg(client, "argTypedFixedList_7", list);
}

static void argTypedFixedList_8(HessianClient& client) {
    ValuePtr list = new Value("[string", Value::TYPE_LIST);
    list->add(new Value("1"));
    list->add(new Value("2"));
    list->add(new Value("3"));
    list->add(new Value("4"));
    list->add(new Value("5"));
    list->add(new Value("6"));
    list->add(new Value("7"));
    list->add(new Value("8"));
    arg(client, "argTypedFixedList_8", list);
}

static void argUntypedMap_0(HessianClient& client) {
    arg(client, "argUntypedMap_0", new Value(Value::TYPE_MAP));
}

static void argUntypedMap_1(HessianClient& client) {
    ValuePtr map = new Value(Value::TYPE_MAP);
    map->put(new Value("a"), new Value((Int32) 0));
    arg(client, "argUntypedMap_1", map);
}

static void argUntypedMap_2(HessianClient& client) {
    ValuePtr map = new Value(Value::TYPE_MAP);
    map->put(new Value((Int32) 0), new Value("a"));
    map->put(new Value((Int32) 1), new Value("b"));
    arg(client, "argUntypedMap_2", map);
}

static void argUntypedMap_3(HessianClient& client) {
    ValuePtr map = new Value(Value::TYPE_MAP);
    ValuePtr list = new Value(Value::TYPE_LIST);
    list->add(new Value("a"));
    map->put(list, new Value((Int32) 0));
    arg(client, "argUntypedMap_3", map);
}

static void argTypedMap_0(HessianClient& client) {
    arg(client, "argTypedMap_0", new Value("java.util.Hashtable", Value::TYPE_MAP));
}

static void argTypedMap_1(HessianClient& client) {
    ValuePtr map = new Value("java.util.Hashtable", Value::TYPE_MAP);
    map->put(new Value("a"), new Value((Int32) 0));
    arg(client, "argTypedMap_1", map);
}

static void argTypedMap_2(HessianClient& client) {
    ValuePtr map = new Value("java.util.Hashtable", Value::TYPE_MAP);
    map->put(new Value((Int32) 0), new Value("a"));
    map->put(new Value((Int32) 1), new Value("b"));
    arg(client, "argTypedMap_2", map);
}

static void argTypedMap_3(HessianClient& client) {
    ValuePtr map = new Value("java.util.Hashtable", Value::TYPE_MAP);
    ValuePtr list = new Value(Value::TYPE_LIST);
    list->add(new Value("a"));
    map->put(list, new Value((Int32) 0));
    arg(client, "argTypedMap_3", map);
}

static void argObject_0(HessianClient& client) {
    arg(client, "argObject_0", new Value("com.caucho.hessian.test.A0", Value::TYPE_MAP));
}

static void argObject_16(HessianClient& client) {
    ValuePtr list = new Value(Value::TYPE_LIST);
    list->add(new Value("com.caucho.hessian.test.A0", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A1", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A2", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A3", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A4", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A5", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A6", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A7", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A8", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A9", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A10", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A11", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A12", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A13", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A14", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A15", Value::TYPE_MAP));
    list->add(new Value("com.caucho.hessian.test.A16", Value::TYPE_MAP));
    arg(client, "argObject_16", list);
}

static void argObject_1(HessianClient& client) {
    ValuePtr map = new Value("com.caucho.hessian.test.TestObject", Value::TYPE_MAP);
    map->put(new Value("_value"), new Value((Int32) 0));
    arg(client, "argObject_1", map);
}

static void argObject_2(HessianClient& client) {
    ValuePtr list = new Value(Value::TYPE_LIST);
    ValuePtr map1 = new Value("com.caucho.hessian.test.TestObject", Value::TYPE_MAP);
    map1->put(new Value("_value"), new Value((Int32) 0));
    list->add(map1);
    ValuePtr map2 = new Value("com.caucho.hessian.test.TestObject", Value::TYPE_MAP);
    map2->put(new Value("_value"), new Value((Int32) 1));
    list->add(map2);
    arg(client, "argObject_2", list);
}

static void argObject_2a(HessianClient& client) {
    ValuePtr list = new Value(Value::TYPE_LIST);
    ValuePtr map1 = new Value("com.caucho.hessian.test.TestObject", Value::TYPE_MAP);
    map1->put(new Value("_value"), new Value((Int32) 0));
    list->add(map1);
    list->add(map1);
    arg(client, "argObject_2a", list);
}

static void argObject_2b(HessianClient& client) {
    ValuePtr list = new Value(Value::TYPE_LIST);
    ValuePtr map1 = new Value("com.caucho.hessian.test.TestObject", Value::TYPE_MAP);
    map1->put(new Value("_value"), new Value((Int32) 0));
    list->add(map1);
    ValuePtr map2 = new Value("com.caucho.hessian.test.TestObject", Value::TYPE_MAP);
    map2->put(new Value("_value"), new Value((Int32) 0));
    list->add(map2);
    arg(client, "argObject_2b", list);
}

static void argObject_3(HessianClient& client) {
    ValuePtr map = new Value("com.caucho.hessian.test.TestCons", Value::TYPE_MAP);
    map->put(new Value("_first"), new Value("a"));
    map->put(new Value("_rest"), ValuePtr(map, false));
    arg(client, "argObject_3", map);
}

typedef void (*hessian_test_function)(HessianClient& client);
typedef std::pair<std::string, hessian_test_function> test_list_entry;
typedef std::vector<test_list_entry> test_list;
typedef test_list::const_iterator test_list_iterator;

static void execute(HessianClient& client, const test_list& tests) {
    for (test_list_iterator it = tests.begin(); it != tests.end(); it++) {
        test_list_entry test_entry = *it;
        std::string test_name = test_entry.first;
        hessian_test_function test_function = test_entry.second;
        std::cout << test_name << ": ";
        try {
            test_function(client);
            std::cout << "PASSED";
        } catch (HessianException& e) {
            std::cout << "FAILED: " << e.getMessage();
        } catch (Exception& e) {
            std::cout << "FAILED: " << e.displayText();
        } catch (std::exception& e) {
            std::cout << "FAILED: " << e.what();
        }
        std::cout << std::endl;
    }
}

void hessian_test(HessianClient& client) {
    test_list tests;
    tests.push_back(test_list_entry("nullCall", nullCall));
    tests.push_back(test_list_entry("hello", hello));
    tests.push_back(test_list_entry("subtract", subtract));
    tests.push_back(test_list_entry("echo", echo));
    tests.push_back(test_list_entry("fault", fault));
    execute(client, tests);
}

void hessian_test2(HessianClient& client) {
    test_list tests;
    tests.push_back(test_list_entry("replyNull", replyNull));
    tests.push_back(test_list_entry("replyTrue", replyTrue));
    tests.push_back(test_list_entry("replyFalse", replyFalse));
    tests.push_back(test_list_entry("replyInt_0", replyInt_0));
    tests.push_back(test_list_entry("replyInt_1", replyInt_1));
    tests.push_back(test_list_entry("replyInt_47", replyInt_47));
    tests.push_back(test_list_entry("replyInt_m16", replyInt_m16));
    tests.push_back(test_list_entry("replyInt_0x30", replyInt_0x30));
    tests.push_back(test_list_entry("replyInt_0x7ff", replyInt_0x7ff));
    tests.push_back(test_list_entry("replyInt_m0x800", replyInt_m0x800));
    tests.push_back(test_list_entry("replyInt_0x800", replyInt_0x800));
    tests.push_back(test_list_entry("replyInt_0x3ffff", replyInt_0x3ffff));
    tests.push_back(test_list_entry("replyInt_m0x801", replyInt_m0x801));
    tests.push_back(test_list_entry("replyInt_m0x40000", replyInt_m0x40000));
    tests.push_back(test_list_entry("replyInt_0x40000", replyInt_0x40000));
    tests.push_back(test_list_entry("replyInt_0x7fffffff", replyInt_0x7fffffff));
    tests.push_back(test_list_entry("replyInt_m0x40001", replyInt_m0x40001));
    tests.push_back(test_list_entry("replyInt_m0x80000000", replyInt_m0x80000000));
    tests.push_back(test_list_entry("replyLong_0", replyLong_0));
    tests.push_back(test_list_entry("replyLong_1", replyLong_1));
    tests.push_back(test_list_entry("replyLong_15", replyLong_15));
    tests.push_back(test_list_entry("replyLong_m8", replyLong_m8));
    tests.push_back(test_list_entry("replyLong_0x10", replyLong_0x10));
    tests.push_back(test_list_entry("replyLong_0x7ff", replyLong_0x7ff));
    tests.push_back(test_list_entry("replyLong_m9", replyLong_m9));
    tests.push_back(test_list_entry("replyLong_m0x800", replyLong_m0x800));
    tests.push_back(test_list_entry("replyLong_0x800", replyLong_0x800));
    tests.push_back(test_list_entry("replyLong_0x3ffff", replyLong_0x3ffff));
    tests.push_back(test_list_entry("replyLong_m0x801", replyLong_m0x801));
    tests.push_back(test_list_entry("replyLong_m0x40000", replyLong_m0x40000));
    tests.push_back(test_list_entry("replyLong_0x40000", replyLong_0x40000));
    tests.push_back(test_list_entry("replyLong_0x7fffffff", replyLong_0x7fffffff));
    tests.push_back(test_list_entry("replyLong_m0x40001", replyLong_m0x40001));
    tests.push_back(test_list_entry("replyLong_m0x80000000", replyLong_m0x80000000));
    tests.push_back(test_list_entry("replyLong_0x80000000", replyLong_0x80000000));
    tests.push_back(test_list_entry("replyLong_m0x80000001", replyLong_m0x80000001));
    tests.push_back(test_list_entry("replyDouble_0_0", replyDouble_0_0));
    tests.push_back(test_list_entry("replyDouble_1_0", replyDouble_1_0));
    tests.push_back(test_list_entry("replyDouble_2_0", replyDouble_2_0));
    tests.push_back(test_list_entry("replyDouble_127_0", replyDouble_127_0));
    tests.push_back(test_list_entry("replyDouble_m128_0", replyDouble_m128_0));
    tests.push_back(test_list_entry("replyDouble_128_0", replyDouble_128_0));
    tests.push_back(test_list_entry("replyDouble_m129_0", replyDouble_m129_0));
    tests.push_back(test_list_entry("replyDouble_32767_0", replyDouble_32767_0));
    tests.push_back(test_list_entry("replyDouble_m32768_0", replyDouble_m32768_0));
    tests.push_back(test_list_entry("replyDouble_0_001", replyDouble_0_001));
    tests.push_back(test_list_entry("replyDouble_m0_001", replyDouble_m0_001));
    tests.push_back(test_list_entry("replyDouble_65_536", replyDouble_65_536));
    tests.push_back(test_list_entry("replyDouble_3_14159", replyDouble_3_14159));
    tests.push_back(test_list_entry("replyDate_0", replyDate_0));
    tests.push_back(test_list_entry("replyDate_1", replyDate_1));
    tests.push_back(test_list_entry("replyDate_2", replyDate_2));
    tests.push_back(test_list_entry("replyString_0", replyString_0));
    tests.push_back(test_list_entry("replyString_null", replyString_null));
    tests.push_back(test_list_entry("replyString_1", replyString_1));
    tests.push_back(test_list_entry("replyString_31", replyString_31));
    tests.push_back(test_list_entry("replyString_32", replyString_32));
    tests.push_back(test_list_entry("replyString_1023", replyString_1023));
    tests.push_back(test_list_entry("replyString_1024", replyString_1024));
    tests.push_back(test_list_entry("replyString_65536", replyString_65536));
    tests.push_back(test_list_entry("replyBinary_0", replyBinary_0));
    tests.push_back(test_list_entry("replyBinary_null", replyBinary_null));
    tests.push_back(test_list_entry("replyBinary_1", replyBinary_1));
    tests.push_back(test_list_entry("replyBinary_15", replyBinary_15));
    tests.push_back(test_list_entry("replyBinary_16", replyBinary_16));
    tests.push_back(test_list_entry("replyBinary_1023", replyBinary_1023));
    tests.push_back(test_list_entry("replyBinary_1024", replyBinary_1024));
    tests.push_back(test_list_entry("replyBinary_65536", replyBinary_65536));
    tests.push_back(test_list_entry("replyUntypedFixedList_0", replyUntypedFixedList_0));
    tests.push_back(test_list_entry("replyUntypedFixedList_1", replyUntypedFixedList_1));
    tests.push_back(test_list_entry("replyUntypedFixedList_7", replyUntypedFixedList_7));
    tests.push_back(test_list_entry("replyUntypedFixedList_8", replyUntypedFixedList_8));
    tests.push_back(test_list_entry("replyTypedFixedList_0", replyTypedFixedList_0));
    tests.push_back(test_list_entry("replyTypedFixedList_1", replyTypedFixedList_1));
    tests.push_back(test_list_entry("replyTypedFixedList_7", replyTypedFixedList_7));
    tests.push_back(test_list_entry("replyTypedFixedList_8", replyTypedFixedList_8));
    tests.push_back(test_list_entry("replyUntypedMap_0", replyUntypedMap_0));
    tests.push_back(test_list_entry("replyUntypedMap_1", replyUntypedMap_1));
    tests.push_back(test_list_entry("replyUntypedMap_2", replyUntypedMap_2));
    tests.push_back(test_list_entry("replyUntypedMap_3", replyUntypedMap_3));
    tests.push_back(test_list_entry("replyTypedMap_0", replyTypedMap_0));
    tests.push_back(test_list_entry("replyTypedMap_1", replyTypedMap_1));
    tests.push_back(test_list_entry("replyTypedMap_2", replyTypedMap_2));
    tests.push_back(test_list_entry("replyTypedMap_3", replyTypedMap_3));
    tests.push_back(test_list_entry("replyObject_0", replyObject_0));
    tests.push_back(test_list_entry("replyObject_16", replyObject_16));
    tests.push_back(test_list_entry("replyObject_1", replyObject_1));
    tests.push_back(test_list_entry("replyObject_2", replyObject_2));
    tests.push_back(test_list_entry("replyObject_2a", replyObject_2a));
    tests.push_back(test_list_entry("replyObject_2b", replyObject_2b));
    tests.push_back(test_list_entry("replyObject_3", replyObject_3));
    tests.push_back(test_list_entry("argNull", argNull));
    tests.push_back(test_list_entry("argTrue", argTrue));
    tests.push_back(test_list_entry("argFalse", argFalse));
    tests.push_back(test_list_entry("argInt_0", argInt_0));
    tests.push_back(test_list_entry("argInt_1", argInt_1));
    tests.push_back(test_list_entry("argInt_47", argInt_47));
    tests.push_back(test_list_entry("argInt_m16", argInt_m16));
    tests.push_back(test_list_entry("argInt_0x30", argInt_0x30));
    tests.push_back(test_list_entry("argInt_0x7ff", argInt_0x7ff));
    tests.push_back(test_list_entry("argInt_m17", argInt_m17));
    tests.push_back(test_list_entry("argInt_m0x800", argInt_m0x800));
    tests.push_back(test_list_entry("argInt_0x800", argInt_0x800));
    tests.push_back(test_list_entry("argInt_0x3ffff", argInt_0x3ffff));
    tests.push_back(test_list_entry("argInt_m0x801", argInt_m0x801));
    tests.push_back(test_list_entry("argInt_m0x40000", argInt_m0x40000));
    tests.push_back(test_list_entry("argInt_0x40000", argInt_0x40000));
    tests.push_back(test_list_entry("argInt_0x7fffffff", argInt_0x7fffffff));
    tests.push_back(test_list_entry("argInt_m0x40001", argInt_m0x40001));
    tests.push_back(test_list_entry("argInt_m0x80000000", argInt_m0x80000000));
    tests.push_back(test_list_entry("argLong_0", argLong_0));
    tests.push_back(test_list_entry("argLong_1", argLong_1));
    tests.push_back(test_list_entry("argLong_15", argLong_15));
    tests.push_back(test_list_entry("argLong_m8", argLong_m8));
    tests.push_back(test_list_entry("argLong_0x10", argLong_0x10));
    tests.push_back(test_list_entry("argLong_0x7ff", argLong_0x7ff));
    tests.push_back(test_list_entry("argLong_m9", argLong_m9));
    tests.push_back(test_list_entry("argLong_m0x800", argLong_m0x800));
    tests.push_back(test_list_entry("argLong_0x800", argLong_0x800));
    tests.push_back(test_list_entry("argLong_0x3ffff", argLong_0x3ffff));
    tests.push_back(test_list_entry("argLong_m0x801", argLong_m0x801));
    tests.push_back(test_list_entry("argLong_m0x40000", argLong_m0x40000));
    tests.push_back(test_list_entry("argLong_0x40000", argLong_0x40000));
    tests.push_back(test_list_entry("argLong_0x7fffffff", argLong_0x7fffffff));
    tests.push_back(test_list_entry("argLong_m0x40001", argLong_m0x40001));
    tests.push_back(test_list_entry("argLong_m0x80000000", argLong_m0x80000000));
    tests.push_back(test_list_entry("argLong_0x80000000", argLong_0x80000000));
    tests.push_back(test_list_entry("argLong_m0x80000001", argLong_m0x80000001));
    tests.push_back(test_list_entry("argDouble_0_0", argDouble_0_0));
    tests.push_back(test_list_entry("argDouble_1_0", argDouble_1_0));
    tests.push_back(test_list_entry("argDouble_2_0", argDouble_2_0));
    tests.push_back(test_list_entry("argDouble_127_0", argDouble_127_0));
    tests.push_back(test_list_entry("argDouble_m128_0", argDouble_m128_0));
    tests.push_back(test_list_entry("argDouble_128_0", argDouble_128_0));
    tests.push_back(test_list_entry("argDouble_m129_0", argDouble_m129_0));
    tests.push_back(test_list_entry("argDouble_32767_0", argDouble_32767_0));
    tests.push_back(test_list_entry("argDouble_m32768_0", argDouble_m32768_0));
    tests.push_back(test_list_entry("argDouble_0_001", argDouble_0_001));
    tests.push_back(test_list_entry("argDouble_m0_001", argDouble_m0_001));
    tests.push_back(test_list_entry("argDouble_65_536", argDouble_65_536));
    tests.push_back(test_list_entry("argDouble_3_14159", argDouble_3_14159));
    tests.push_back(test_list_entry("argDate_0", argDate_0));
    tests.push_back(test_list_entry("argDate_1", argDate_1));
    tests.push_back(test_list_entry("argDate_2", argDate_2));
    tests.push_back(test_list_entry("argString_0", argString_0));
    tests.push_back(test_list_entry("argString_1", argString_1));
    tests.push_back(test_list_entry("argString_31", argString_31));
    tests.push_back(test_list_entry("argString_32", argString_32));
    tests.push_back(test_list_entry("argString_1023", argString_1023));
    tests.push_back(test_list_entry("argString_1024", argString_1024));
    tests.push_back(test_list_entry("argString_65536", argString_65536));
    tests.push_back(test_list_entry("argBinary_0", argBinary_0));
    tests.push_back(test_list_entry("argBinary_1", argBinary_1));
    tests.push_back(test_list_entry("argBinary_15", argBinary_15));
    tests.push_back(test_list_entry("argBinary_16", argBinary_16));
    tests.push_back(test_list_entry("argBinary_1023", argBinary_1023));
    tests.push_back(test_list_entry("argBinary_1024", argBinary_1024));
    tests.push_back(test_list_entry("argBinary_65536", argBinary_65536));
    tests.push_back(test_list_entry("argUntypedFixedList_0", argUntypedFixedList_0));
    tests.push_back(test_list_entry("argUntypedFixedList_1", argUntypedFixedList_1));
    tests.push_back(test_list_entry("argUntypedFixedList_7", argUntypedFixedList_7));
    tests.push_back(test_list_entry("argUntypedFixedList_8", argUntypedFixedList_8));
    tests.push_back(test_list_entry("argTypedFixedList_0", argTypedFixedList_0));
    tests.push_back(test_list_entry("argTypedFixedList_1", argTypedFixedList_1));
    tests.push_back(test_list_entry("argTypedFixedList_7", argTypedFixedList_7));
    tests.push_back(test_list_entry("argTypedFixedList_8", argTypedFixedList_8));
    tests.push_back(test_list_entry("argUntypedMap_0", argUntypedMap_0));
    tests.push_back(test_list_entry("argUntypedMap_1", argUntypedMap_1));
    tests.push_back(test_list_entry("argUntypedMap_2", argUntypedMap_2));
    tests.push_back(test_list_entry("argUntypedMap_3", argUntypedMap_3));
    tests.push_back(test_list_entry("argTypedMap_0", argTypedMap_0));
    tests.push_back(test_list_entry("argTypedMap_1", argTypedMap_1));
    tests.push_back(test_list_entry("argTypedMap_2", argTypedMap_2));
    tests.push_back(test_list_entry("argTypedMap_3", argTypedMap_3));
    tests.push_back(test_list_entry("argObject_0", argObject_0));
    tests.push_back(test_list_entry("argObject_16", argObject_16));
    tests.push_back(test_list_entry("argObject_1", argObject_1));
    tests.push_back(test_list_entry("argObject_2", argObject_2));
    tests.push_back(test_list_entry("argObject_2a", argObject_2a));
    tests.push_back(test_list_entry("argObject_2b", argObject_2b));
    tests.push_back(test_list_entry("argObject_3", argObject_3));
    execute(client, tests);
}
