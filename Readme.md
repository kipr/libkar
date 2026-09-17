# libkar

libkar is an extremely simple Qt based keyed archiver for usage by the KISS IDE suite of applications. This is a separate library because it is used as a data interchange format between applications in the suite.

# Requirements

* Qt 6
* CMake 3.5

# Building

## Cross-compile to the Wombat (Raspberry Pi 3b+)

Local build, tested on Debian 13:

```sh
sudo dpkg --add-architecture arm64
sudo apt update
sudo apt install make cmake gcc-aarch64-linux-gnu g++-aarch64-linux-gnu qt6-base-dev:arm64
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=toolchain/aarch64-linux-gnu.cmake .
cmake --build build -j "$(nproc)"
```

Build with Docker:

```sh
docker build -t libkar-builder .
docker run --rm --mount type=bind,source=.,destination=/src/ libkar-builder sh -c 'cmake -B/src/build -DCMAKE_TOOLCHAIN_FILE=/src/toolchain/aarch64-linux-gnu.cmake /src && cmake --build /src/build -j "$(nproc)" && cd /src/build && cpack'
```

Runtime dependencies on Pi:

```sh
$ readelf -d /usr/local/lib/libkar.so | grep NEEDED
 0x0000000000000001 (NEEDED)             Shared library: [libQt6Core.so.6]
 0x0000000000000001 (NEEDED)             Shared library: [libstdc++.so.6]
 0x0000000000000001 (NEEDED)             Shared library: [libgcc_s.so.1]
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
```

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

# Example Usage


```cpp
Kiss::Kar *archive = Kiss::Kar::create();
archive->addFile("hello.txt", "Hello, World!\n");
archive->save("test.kar");
delete archive;
```

# Authors

* Braden McDorman

# License

libkar is released under the terms of the GPLv3 license. For more information, see the LICENSE file in the root of this project.
