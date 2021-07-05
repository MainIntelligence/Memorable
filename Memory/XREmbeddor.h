#pragma once
#include "Embeddor.h"

//Data needed to update pointers between runs.
struct XRunData {
	void * curr;
	void * old = nullptr;
};


template<class T> concept PointerType = std::is_pointer_v<T>;

//struct Memory::Package;
//namespace Memory {
//Could detect when pointers point outside of program memory, 
//Format Migration Files (needed when a type adds a member that also gets embedded, use to modify the package content so that loading works normally)
struct XREmbeddor : public Embeddor {
	//void * currdat;
	//int len;
	//using namespace Memory;
	XRunData mem;
	XREmbeddor() {};
	XREmbeddor(XRunData pack, const unsigned int cap) : Embeddor(pack.curr, cap), mem{pack} {};
	XREmbeddor(XRunData pack, void * embedbegin, const unsigned int cap) : Embeddor(embedbegin, cap), mem{pack} {};
	//Embeddor(void * memory, int bytes) : currdat{memory}, len{bytes} {};
	
	//use to be safe before embedding a bunch of data. (true if you can embed the given number of bytes using this Embeddor)
	//inline bool CanFit(const unsigned int bytes) const noexcept { return len >= bytes; }
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
	
	//Constructs an object of class T, and embeds it within memory.
	//template <typename T, typename ...Params>
	//Embedded<T> Construct(Params&& ...);
	
	//Embeddor MakeSubEmbeddor(const unsigned int bytes);
	
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

