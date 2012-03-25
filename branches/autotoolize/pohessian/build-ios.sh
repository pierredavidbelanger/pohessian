#!/bin/sh

PLATFORMS=/Applications/Xcode.app/Contents/Developer/Platforms
IOS_VERSION=5.1
POCO=/Users/pierredavidbelanger/Dev/lib/poco-1.4.3p1-all

rm -rf build/ios

mkdir -p build/ios/iPhoneSimulator/i386
cd build/ios/iPhoneSimulator/i386
../../../../configure \
CXX=clang++ \
--disable-shared \
--with-sysroot=$PLATFORMS/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator$IOS_VERSION.sdk \
--with-arch=i386 \
--with-poco-foundation-include=$POCO/Foundation/include \
--with-poco-net-include=$POCO/Net/include \
--with-poco-lib=$POCO/lib/iPhoneSimulator/i686
make
cd ../../../../

mkdir -p build/ios/iPhoneOS/armv6
cd build/ios/iPhoneOS/armv6
../../../../configure \
CXX=$PLATFORMS/iPhoneOS.platform/Developer/usr/bin/clang++ \
--disable-shared \
--with-sysroot=$PLATFORMS/iPhoneOS.platform/Developer/SDKs/iPhoneOS$IOS_VERSION.sdk \
--with-arch=armv6 \
--with-poco-foundation-include=$POCO/Foundation/include \
--with-poco-net-include=$POCO/Net/include \
--with-poco-lib=$POCO/lib/iPhoneOS/armv6
make
cd ../../../../

mkdir -p build/ios/iPhoneOS/armv7
cd build/ios/iPhoneOS/armv7
../../../../configure \
CXX=$PLATFORMS/iPhoneOS.platform/Developer/usr/bin/clang++ \
--disable-shared \
--with-sysroot=$PLATFORMS/iPhoneOS.platform/Developer/SDKs/iPhoneOS$IOS_VERSION.sdk \
--with-arch=armv7 \
--with-poco-foundation-include=$POCO/Foundation/include \
--with-poco-net-include=$POCO/Net/include \
--with-poco-lib=$POCO/lib/iPhoneOS/armv6
make
cd ../../../../

mkdir -p build/ios
cd build/ios
lipo \
-arch i386 iPhoneSimulator/i386/.libs/libpohessian.a \
-arch armv6 iPhoneOS/armv6/.libs/libpohessian.a \
-arch armv7 iPhoneOS/armv7/.libs/libpohessian.a \
-create \
-output libpohessian.a
ranlib libpohessian.a
otool -L libpohessian.a
file libpohessian.a
cd ../../
