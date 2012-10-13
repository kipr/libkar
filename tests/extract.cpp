#include "kar.hpp"

using namespace Kiss;

int main(int argc, char *argv[])
{
	KarPtr archive = Kar::create();
	archive->addFile("test.txt", "Hello, World!");
	archive->addFile("directory/test1.txt", "Test 1!");
	archive->addFile("directory/test2.txt", "Test 2!");
	archive->extract("/Users/beta/Desktop/extract_test");
	return 0;
}