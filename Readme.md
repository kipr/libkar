libkar
======

libkar is an extremely simple Qt based keyed archiver for usage by the KISS IDE suite of applications. This is a separate
library because it is used as a data interchange format between applications in the suite.

Compilation and Installation
============================

	cd libkar
	mkdir build
	cd build
	cmake ..
	make && make install

Example Usage
=============

	Kiss::Kar *archive = Kiss::Kar::create();
	archive->addFile("hello.txt", "Hello, World!\n");
	archive->save("test.kar");
	delete archive;