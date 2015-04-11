

## PoHessian ##

A portable C++ [Hessian](http://hessian.caucho.com) implementation.

## Features ##

  * [Portable](http://code.google.com/p/pohessian/#Portability) (thanks to [Poco](http://pocoproject.org))
  * Compatible with Hessian 4.0.7 ([hessian-4.0.7.jar](http://caucho.com/download/hessian-4.0.7.jar))
  * Supports [Hessian client protocol version 1](http://hessian.caucho.com/doc/hessian-1.0-spec.xtp) (including headers)
  * Supports all [Hessian types](HessianTypeMapping.md) (including circular ref and remote object)
  * Automatic memory management (using smart pointer)
  * Use high level HTTP or TCP HessianClient or low level HessianStream

## Get the code ##

Download the [latest source package](http://code.google.com/p/pohessian/downloads/list).

## Getting started ##

See the GettingStarted page to learn how to begin. And after, have a look at the HessianTypeMapping page.

## Issues ##

For any kind of bug or request (for enhancement, documentation) or general question that can benefit someone else, please [create an issue](http://code.google.com/p/pohessian/issues/entry?template=Default%20-%20non-members). For everything else [contact me](mailto:pierredavidbelanger@gmail.com).

## License ##

PoHessian is released under the [GNU GPL v3](http://www.gnu.org/licenses/gpl.html). So you are free to use/modify/redistribute this code, as long as it is for an open source project. This way I [give the free software a step ahead](http://www.gnu.org/philosophy/why-not-lgpl.html).

If you plan to use this code in a commercial application, please [let me know](mailto:pierredavidbelanger@gmail.com).

## Portability ##

PoHessian is known to compile and run for:
  * Darwin x86\_64 and i386 (since 1.0) (see GettingStarted)
  * Linux i386 and ppc (since 1.0) (see GettingStarted)
  * iOS armv6 and armv7 devices and i386 simulator (since 1.0) (see BuildiPhone)
  * Window under MinGW (since 1.1) (see GettingStarted)

But since [Poco](http://pocoproject.org) is used, PoHessian should, without problem, compile and run on every [platforms](http://pocoproject.org/features.html) that Poco supports.

## Roadmap ##

  * create private repository for initial development **DONE**
  * initial hessian 1 stream impl (reader/writer) **DONE**
  * initial hessian client impl (http/tcp) **DONE**
  * complete caucho test suite client **DONE**
  * cleanup code **DONE**
  * public release on code.google.com **DONE**
  * autotoolize **DONE**
  * _MILESTONE 1.0_ **DONE** (updated 2012-03-25)
  * support hessian 2 stream
  * _MILESTONE 2.0_
  * impl server
  * _MILESTONE 3.0_
  * support https/tcps
  * support compression
  * _MILESTONE 4.0_
  * examples (iOS, WinCE, Linux, OSX, Win32)