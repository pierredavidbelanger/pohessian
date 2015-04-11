## Dependencies ##

PoHessian depends on the modules Foundation and Net from [Poco](http://pocoproject.org).

Using Debian, for example, you can get it using:
```
$ sudo apt-get install libpoco-dev
```

If you need to build it from source, here is its [download](http://pocoproject.org/download/index.html) page. Read the [Poco README](http://poco.svn.sourceforge.net/viewvc/poco/poco/branches/poco-1.4.3/README).

## Get the code ##

Download the [latest source package](http://code.google.com/p/pohessian/downloads/list).

## Unzip ##

```
$ tar -xzf pohessian-x.y.tar.gz
$ cd pohessian-x.y
```

## Compile ##

```
$ ./configure
$ make
$ make check #optional
```

This will create the libpohessian library (here on Darwin):

```
$ ls -p1 .libs/libpohessian.*
.libs/libpohessian.0.0.0.dylib
.libs/libpohessian.0.dylib
.libs/libpohessian.a
.libs/libpohessian.dylib
.libs/libpohessian.la
.libs/libpohessian.lai
```

## Install ##

```
$ sudo make install
```

On Linux you probably will need to do a `ldconfig`:

```
$ sudo ldconfig
```

## Use ##

Here is a simple example code that call the method [java.lang.String replyString\_32()](http://javadoc4.caucho.com/com/caucho/hessian/test/TestHessian2.html#replyString_32%28%29) on the URL http://hessian.caucho.com/test/test2.

_main.cpp_
```
#include <iostream>
#include "Poco/URI.h"
#include "pohessian/HessianTypes.h"
#include "pohessian/HessianClient.h"

using namespace std;
using namespace Poco;
using namespace PoHessian;

int main()
{
    URI uri("http://hessian.caucho.com/test/test2");
    HessianClient client(HessianClient::HESSIAN_VERSION_1, uri);
    cout << client.call("replyString_32")->getString() << endl;
    return 0;
}
```

Compile the example:

```
$ g++ main.cpp -lpohessian -lPocoFoundation -o replyString_32
```

Execute the example:

```
$ ./replyString_32
01234567890123456789012345678901
```

## Uninstall ##

```
$ sudo make uninstall
```