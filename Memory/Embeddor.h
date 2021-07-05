#pragma once
#include <new>
#include "EmbeddedType.h"
//Embeddor acts as a linearizer of memory hierarchies.
	//ie. Every embedding gets memory immediately after the last.
struct Embeddor {
	void * currdat;
	unsigned int len;
	
	Embeddor() {}
	Embeddor(void * memory, const unsigned int bytes) : currdat{memory}, len{bytes} {};
	//use to be safe before embedding a bunch of data. (true if you can embed the given number of bytes using this Embeddor)
	inline bool CanFit(const unsigned int bytes) const noexcept { return len >= bytes; }
	
	inline void *EmbedBytes(unsigned int len) noexcept;
	//Embed<T>() will give the memory for one T
	template <typename T>
	T* Embed();
	
	//Embed<T>(unsigned int total) gives the memory for the 'total' number of T objects.
	template <typename T>
	T* Embed(const int total);
	
	//Constructs an object of class T, and embeds it within memory.
	template <typename T, typename ...Params>
	Embedded<T> Construct(Params&& ...);
	
	inline Embeddor MakeSubEmbeddor(const unsigned int bytes) noexcept;
};

inline void* Embeddor::EmbedBytes(unsigned int totalBytes) noexcept {
	len -= totalBytes;
	char * pDat = reinterpret_cast<char*>(currdat);
	currdat = pDat + totalBytes;
	return pDat;
}

template <typename T>
T* Embeddor::Embed() {
	T* pDat = reinterpret_cast<T*>(EmbedBytes(sizeof(T)));
	return pDat;
}

template <typename T, typename ... Params>
Embedded<T> Embeddor::Construct(Params&& ... params) {
	return Embedded<T>(Embed<T>(), params ...);
}

//Embed<T>(unsigned int total) gives the memory for the 'total' number of T objects.
template <typename T>
T* Embeddor::Embed(const int total) {
	T* pDat = reinterpret_cast<T*>(EmbedBytes(total * sizeof(T)));
	return pDat;
}

inline Embeddor Embeddor::MakeSubEmbeddor(const unsigned int bytes) noexcept {
	void * data = Embed<char>(bytes);
	return Embeddor(data, bytes); 
}
	
