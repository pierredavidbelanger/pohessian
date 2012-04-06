define(PoHessian_COPYRIGHT,
[[
PoHessian
Portable C++ Hessian Implementation

Copyright (C) 2012  Pierre-David Belanger

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
]])

AC_PREREQ([2.61])
AC_INIT([PoHessian], [1.1], [pierredavidbelanger@gmail.com])

AM_INIT_AUTOMAKE([-Wall])

LT_PREREQ([2.2])
LT_INIT([win32-dll])

AC_COPYRIGHT(PoHessian_COPYRIGHT)

AH_TOP(/*PoHessian_COPYRIGHT*/)

AC_CONFIG_SRCDIR([include/pohessian/PoHessian.h])

AC_LANG([C++])

AC_ARG_WITH([sysroot],
            [AC_HELP_STRING([--with-sysroot=DIR],
                            [Compiler --sysroot option])],
            [CPPFLAGS="--sysroot $withval $CPPFLAGS"])

AC_ARG_WITH([poco_foundation_include],
            [AC_HELP_STRING([--with-poco-foundation-include=DIR],
                            [Poco Foundation include directory])],
            [CPPFLAGS="-I$withval $CPPFLAGS"])

AC_ARG_WITH([poco_net_include],
            [AC_HELP_STRING([--with-poco-net-include=DIR],
                            [Poco Net include directory])],
            [CPPFLAGS="-I$withval $CPPFLAGS"])

AC_ARG_WITH([poco_lib],
            [AC_HELP_STRING([--with-poco-lib=DIR],
                            [Poco lib directory])],
            [LDFLAGS="-L$withval $LDFLAGS"; LIBS="-lPocoFoundation -lPocoNet $LIBS"],
            [LIBS="-lPocoFoundation -lPocoNet $LIBS"])

AC_ARG_WITH([arch],
            [AC_HELP_STRING([--with-arch=ARCH],
                            [Compiler -arch option])],
            [CPPFLAGS="-arch $withval $CPPFLAGS"; CXXFLAGS="-arch $withval $CXXFLAGS"])

AC_PROG_CXX
AC_PROG_INSTALL

AC_CHECK_HEADERS([istream])
AC_CHECK_HEADERS([ostream])
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([vector])
AC_CHECK_HEADERS([map])
AC_CHECK_HEADERS([algorithm])

AC_CHECK_HEADERS([string.h])

AC_CHECK_HEADERS([Poco/Exception.h])
AC_CHECK_HEADERS([Poco/SharedPtr.h])
AC_CHECK_HEADERS([Poco/String.h])
AC_CHECK_HEADERS([Poco/StreamCopier.h])
AC_CHECK_HEADERS([Poco/Timestamp.h])
AC_CHECK_HEADERS([Poco/Types.h])
AC_CHECK_HEADERS([Poco/URI.h])
AC_CHECK_HEADERS([Poco/Net/HTTPClientSession.h])
AC_CHECK_HEADERS([Poco/Net/HTTPRequest.h])
AC_CHECK_HEADERS([Poco/Net/HTTPMessage.h])
AC_CHECK_HEADERS([Poco/Net/HTTPResponse.h])
AC_CHECK_HEADERS([Poco/Net/Socket.h])
AC_CHECK_HEADERS([Poco/Net/SocketAddress.h])
AC_CHECK_HEADERS([Poco/Net/StreamSocket.h])
AC_CHECK_HEADERS([Poco/Net/SocketStream.h])

AC_HEADER_STDBOOL
AC_C_CONST

AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
