libkar
======

libkar is an extremely simple Qt based keyed archiver for usage by the KISS IDE suite of applications. This is a separate library because it is used as a data interchange format between applications in the suite.

Requirements
============

* Qt 5.0 or higher
* CMake 2.8.12

Building (OS X and Linux)
=========================

	cd libkar
	mkdir build
	cd build
	cmake ..
	make
	make install

Example Usage
=============

	Kiss::Kar *archive = Kiss::Kar::create();
	archive->addFile("hello.txt", "Hello, World!\n");
	archive->save("test.kar");
	delete archive;
	
Authors
=======

* Braden McDorman

License
=======

libkar is released under the terms of the GPLv3 license. For more information, see the LICENSE file in the root of this project.
