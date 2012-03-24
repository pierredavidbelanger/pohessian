
AC_PREREQ([2.61])
AC_INIT([PoHessian], [1.0])

AM_INIT_AUTOMAKE([-Wall])

LT_PREREQ([2.2])
LT_INIT

AC_CONFIG_SRCDIR([include/PoHessian/PoHessian.h])

AC_LANG([C++])

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
            [LDFLAGS="-L$withval -lPocoFoundation -lPocoNet $LDFLAGS"],
            [LDFLAGS="-lPocoFoundation -lPocoNet $LDFLAGS"])

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
