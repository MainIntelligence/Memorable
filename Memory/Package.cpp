
#include <stdio.h>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include "Package.h"
#include "XREmbeddor.h"
/*
	const char * filename;
	Embeddor embeddor;
	void * memory;
*/

//#define READ_CHUNK_SIZE 65536
void LoadPackageFromSize(int file, void * memory, unsigned int memlen, const char *filename) {
	ssize_t nread = read(file, memory, memlen);
	if (nread <= 0) {
		fprintf(stderr, "Error reading package file: %s\n", filename);
	}else {
	
	if (nread < memlen) {
		fprintf(stdout, "detected bad length specified (overdocumented data) in package : %s\n", filename);
	}
	 //check screwups at the other end.
	int excess = 0;
	if (read(file, &excess, 4) != 0) {
		fprintf(stderr, "detected bad length specified (undocumented data) in package : %s\n", filename);
	}
	
	}
}
#include <errno.h>
//The constructors will create the package file if it does not yet exist.
//Grab the amount of memory specified, open package, read it into memory.
Package::Package(const char * filename, unsigned int bytecapacity) : filename{filename}, embeddor(XRunData{calloc(1, bytecapacity)}, bytecapacity) {
	int file = open(filename, O_RDONLY | O_CREAT | S_IRWXU);
	if (file < 0) {
		if (errno == 17) { //file already exists
			errno = 0;
			file = open(filename, O_RDWR);
			fprintf(stdout, "Found package: %s\n", filename);
		}
		if (errno != 0) {
			fprintf(stderr, "Error creating package file:\n%d\n", errno);
			throw (std::runtime_error("Error creating package file"));
		}
	}
	
	ssize_t nread = read(file, &memlen, sizeof(unsigned int));
	if (nread != sizeof(unsigned int)) { //must be creating the package on first run
		fprintf(stdout, "Created Package File %s!\n", filename);
		close(file);
		return;
	}
	if (bytecapacity < memlen) {
		fprintf(stderr, "Error: Bad byte capacity (%u) specified, package memory has %u bytes, fixing & continuing with larger byte capacity...\n", bytecapacity, memlen);
		free(embeddor.mem.curr); 
		bytecapacity = memlen; 
		embeddor.mem.curr = calloc(1, bytecapacity);
		embeddor = XREmbeddor(embeddor.mem, bytecapacity);
	}
	if (memlen < bytecapacity) {
		fprintf(stdout, "Excess memory allocated for package %s: %ld\n", filename, bytecapacity - nread);
	}
	nread = read(file, &embeddor.mem.old, sizeof(void*));
	LoadPackageFromSize(file, embeddor.mem.curr, memlen, filename);
	
	/*do {  nread = read(file, memtmp, READ_CHUNK_SIZE);
		memtmp += READ_CHUNK_SIZE;
	} while (nread == READ_CHUNK_SIZE && memtmp < LastReadLoc);
	*/
	close(file);
}
//Open the package, read it into memory (types can be reloaded after construction), getting as much memory as is needed for package contents.
Package::Package(const char * filename) : filename{filename} {//, memory{malloc(bytecapacity)}, embeddor(package) {
	int file = open(filename, O_RDONLY | O_CREAT | S_IRWXU);
	if (file < 0) {
		if (errno == 17) { //file already exists
			errno = 0;
			file = open(filename, O_RDWR | S_IRWXU);
			fprintf(stdout, "Found package: %s\n", filename);
		}
		if (errno != 0) {
			fprintf(stderr, "Error creating package file:\n%d\n", errno);
			throw (std::runtime_error("Error creating package file"));
		}
	}
	read(file, &memlen, 4);
	void * currmem = calloc(1, memlen);
	void * oldmem = 0;
	read(file, &oldmem, sizeof(void*));
	
	embeddor = XREmbeddor(XRunData{currmem, oldmem}, memlen);
	
	LoadPackageFromSize(file, embeddor.mem.curr, memlen, filename);
	
	/*do {  nread = read(file, memtmp, READ_CHUNK_SIZE);
		memtmp += READ_CHUNK_SIZE;
	} while (nread == READ_CHUNK_SIZE && memtmp < LastReadLoc);
	*/
	close(file);
}

void CheckFullWrite(int file, void * mem, unsigned int bytes) {
	ssize_t rv = write(file, mem, bytes);
	if (rv != bytes) {
		if (rv == -1) {
			fprintf(stderr, "Error on write\n");
			return;
		}
		fprintf(stderr, "Failed to write full data\n");
	}
}

Package::~Package() {
	int file = open(filename, O_WRONLY | O_APPEND | O_TRUNC);
	if (file <= 0) {
		fprintf(stderr, "Error creating package file for write\n");
	}
	fprintf(stdout, "%p %p\n", embeddor.currdat, embeddor.mem.curr);
	unsigned int len = reinterpret_cast<size_t>(embeddor.currdat) - reinterpret_cast<size_t>(embeddor.mem.curr);
	fprintf(stdout, "Writing memory of length %u\n", len);
	CheckFullWrite(file, &len, sizeof(unsigned int));
	CheckFullWrite(file, &embeddor.mem.curr, sizeof(void*));
	CheckFullWrite(file, embeddor.mem.curr, len);
	close(file);
}
