#pragma once
#include "Embeddor.h"

//Data needed to update pointers between runs.
struct XRunData {
	void * curr;
	void * old = nullptr;
};


template<class T> concept PointerType = std::is_pointer_v<T>;

//XREmbeddor: works with package to update pointers to point to the right location when starting up another run
//	Note: currently assumes pointers are at a consistent location from start of package memory. 
//	Could detect when pointers point outside of package memory and do something different, but meh.
struct XREmbeddor : public Embeddor {
	XRunData mem;
	XREmbeddor() {};
	XREmbeddor(XRunData pack, const unsigned int cap) : Embeddor(pack.curr, cap), mem{pack} {};
	XREmbeddor(XRunData pack, void * embedbegin, const unsigned int cap) : Embeddor(embedbegin, cap), mem{pack} {};
	
	template <class T>
	inline T* Embed() { return Embeddor::Embed<T>(); }
	
	template <class T>
	inline T* Embed(const unsigned int total) { return Embeddor::Embed<T>(total); }
	
	//Embed<T>() will give the memory for one T
	template <PointerType T>
	T* Embed();
	
	//Embed<T>(unsigned int total) gives the memory for the 'total' number of T objects.
	template <PointerType T>
	T* Embed(const unsigned int total);
	
	inline XREmbeddor MakeSubEmbeddor(const unsigned int bytes);
};

template <PointerType T>
T* XREmbeddor::Embed() {
	T* pDat = Embed<T>();
	if (mem.old != nullptr) {
		return reinterpret_cast<T*>(mem.curr + ( reinterpret_cast<size_t>(*pDat) - reinterpret_cast<size_t>(mem.old) ));
	}
	return pDat;
}

//Embed<T>(unsigned int total) gives the memory for the 'total' number of T objects.
template <PointerType T>
T* XREmbeddor::Embed(const unsigned int total) {
	T* pDat = Embed<T>(total);
	if (mem.old != nullptr) {
		return reinterpret_cast<T*>(mem.curr + ( reinterpret_cast<size_t>(*pDat) - reinterpret_cast<size_t>(mem.curr) ));
	}
	return pDat;
}

inline XREmbeddor XREmbeddor::MakeSubEmbeddor(const unsigned int bytes) {
	return XREmbeddor(mem, EmbedBytes(bytes), bytes); 
}

