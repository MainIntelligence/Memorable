#pragma once
#include "XREmbeddor.h"

//namespace Memory {

//Package: uses .pkg file that defines the partitions and data in raw memory.
//	(specifically, it gives information about linear chunks of memory used (how many there are, their sizes), and the actual data in that memory.)
// stores the pointer to the filename given to constructor locally, you decide if you want to use that (like, if it will still be in a valid memory location later on).

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

