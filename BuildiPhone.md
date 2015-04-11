Follow the Poco instructions to build for iPhone and iPhoneSimulator. It should be something like this, in the Poco source directory:

```
$ ./configure --config=iPhone
$ make
$ ./configure --config=iPhoneSimulator
$ make
```

Follow the GettingStarted page until you have unzipped PoHessian.

Open `build-ios.sh` to adjust paths for your system:

`PLATFORMS` should point to the `Developer/Platforms` directory.
Before Xcode 4 this directory was: `/Developer/Platforms`
Since Xcode 4 this directory is: `/Applications/Xcode.app/Contents/Developer/Platforms`

`IOS_VERSION` should be the version of the installed iOS SDK.
As of this writing the latest iOS SDK was 5.1.

`POCO` should point to the source directory of Poco (built previously).

Then execute `build-ios.sh`.

```
$ ./build-ios.sh
```

It will product a FAT library that can be statically linked into iOS applications to be used into the iOS simulator (i386) and all the iOS devices (armv6 and armv7).

```
$ file build/ios/libpohessian.a 
build/ios/libpohessian.a: Mach-O universal binary with 3 architectures
build/ios/libpohessian.a (for architecture i386):       current ar archive random library
build/ios/libpohessian.a (for architecture armv6):      current ar archive random library
build/ios/libpohessian.a (for architecture armv7):      current ar archive random library
```