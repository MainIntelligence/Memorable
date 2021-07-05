#pragma once
#include "XREmbeddor.h"

//namespace Memory {

//Package: At construction, creates or (if exists) loads the binary data at file into memory,
//         deals out objects that use package memory (objects inherit data from file if it was loaded)
//	   at destruction, writes all data (implicitly modified over the lifetime of dealt objects) back to binary file.
struct Package {
	const char * filename;
	//void * memory;
	//void * oldmemory = nullptr;
	XREmbeddor embeddor;
	unsigned int memlen = 0;
	
	//The constructors will create the package file if it does not yet exist.
	//Grab the amount of memory specified, open package, read it into memory.
	Package(const char * filename, unsigned int bytecapacity); // : filename{filename}, memory{malloc(bytecapacity)}, embeddor(memory, bytecapacity) {}
	//Open the package, read it into memory (types can be reloaded after construction), getting as much memory as is needed for package contents.
	Package(const char * filename);
	~Package();
};


//};

