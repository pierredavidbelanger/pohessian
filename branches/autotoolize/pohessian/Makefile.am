
include_HEADERS = include/PoHessian/Hessian1StreamReader.h \
    include/PoHessian/Hessian1StreamWriter.h \
    include/PoHessian/HessianClient.h \
    include/PoHessian/HessianStreamReader.h \
    include/PoHessian/HessianStreamWriter.h \
    include/PoHessian/HessianTypes.h \
    include/PoHessian/PoHessian.h

lib_LTLIBRARIES = libpohessian.la

libpohessian_la_SOURCES = source/Hessian1StreamReader.cpp \
    source/Hessian1StreamWriter.cpp \
    source/HessianClient.cpp \
    source/HessianStreamReader.cpp \
    source/HessianStreamWriter.cpp \
    source/HessianType.cpp
libpohessian_la_CPPFLAGS = -I$(top_srcdir)/include

check_PROGRAMS = pohessiancheck

pohessiancheck_SOURCES = test/hessian_tests.cpp \
    test/hessian_tests.h \
    test/main.cpp
pohessiancheck_CPPFLAGS = -I$(top_srcdir)/include
pohessiancheck_LDADD = libpohessian.la

TESTS = pohessiancheck
