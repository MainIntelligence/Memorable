#pragma once
#include <new>
//Embedded type manages a pointer to a value that is embedded within previously allocated memory.
//	(ie. the lifetime of the value at pv is the same as the lifetime of the Embedded<T> object).
//TODO: might also be nice to have something that stores an offset from a large memory area (to reference locale)
template <typename T>
struct Embedded {
	T* pv;
	operator T& () { return *pv; }
	
	template <typename ...Args>
	Embedded(void * memory, Args&& ... args);
	~Embedded();
};

template <typename T>
template <typename ...Args>
Embedded<T>::Embedded(void * memory, Args&& ... args) {
	pv = new (memory) T(args...);
}

template <typename T>
Embedded<T>::~Embedded() {
	pv->~T();
}

