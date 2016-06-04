libkar
======

libkar is an extremely simple Qt based keyed archiver for usage by the KISS IDE suite of applications. This is a separate library because it is used as a data interchange format between applications in the suite.

Requirements
============

* Qt 5.0 or higher
* CMake 2.8.12

Building
========

## OS X and Linux

Run the follow shell commands in the terminal.  If the last command fails, you may need to elevate prileges.  In Debian/Ubuntu systems, replace `make install` with `sudo make install`.

```shell
cd libkar
mkdir build
cd build
cmake ..
make
make install
```

## Windows
1. Clone this repository into `<dir>\libkar`.
2. Configure it and generate the makefiles with cmake. Set the build directory to `<dir>\libkar\build`.
3. Open `<dir>\libkar\build\libkar.sln` with Visual Studio
4. Build the `INSTALL` project

The binaries/includes/libraries are installed into `<dir>\prefix`

Example Usage
=============

```cpp
Kiss::Kar *archive = Kiss::Kar::create();
archive->addFile("hello.txt", "Hello, World!\n");
archive->save("test.kar");
delete archive;
```

Authors
=======

* Braden McDorman

License
=======

libkar is released under the terms of the GPLv3 license. For more information, see the LICENSE file in the root of this project.
