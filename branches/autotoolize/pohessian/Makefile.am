#
# PoHessian
# Portable C++ Hessian Implementation
#
# Copyright (C) 2012  Pierre-David Belanger
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

pkginclude_HEADERS = include/PoHessian/Hessian1StreamReader.h \
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
    source/HessianType.cpp \
    source/conf.h
libpohessian_la_CPPFLAGS = -I$(top_srcdir)/include
libpohessian_la_LDFLAGS = -version-info 0:0:0
	
check_PROGRAMS = pohessiancheck

pohessiancheck_SOURCES = check/hessian_tests.cpp \
    check/hessian_tests.h \
    check/main.cpp
pohessiancheck_CPPFLAGS = -I$(top_srcdir)/include
pohessiancheck_LDADD = libpohessian.la

TESTS = pohessiancheck
