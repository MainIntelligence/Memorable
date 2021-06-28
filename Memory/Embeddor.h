#pragma once

//Embeddor acts as a linearizer of memory hierarchies.
	//ie. suppose class A has a both data and data pointers.
	//	 The data pointed to by class A will be arranged immediately after A.
struct Embeddor {
	void * currdat;
	int len;
	
	Embeddor(void * memory, int bytes) : currdat{memory}, len{bytes} {};
	
	//use to be safe before embedding a bunch of data. (true if you can embed the given number of bytes using this Embeddor)
	inline bool CanFit(const unsigned int bytes) const noexcept { return len >= bytes; }
	
	//Embed<T>() will give the memory for one T
	template <typename T>
	T* Embed();
	//Embed<T>(unsigned int total) gives the memory for the 'total' number of T objects.
	template <typename T>
	T* Embed(const unsigned int total);
	
	//Embed<T>(&arg0, ... , &argn) will make the pointers point to linear, unique memory (arg0 < argn)
		//Not actually suggesting you use this in typical embedded software ( uneccessary code ).
		// 	Unless if getting rid of many member initializations for this one call might be advantageous.
	template <typename TArg1, typename ...TArgs>
	void Embed( TArg1 *& arg1, TArgs *& ... args);
	
	//Embeds memory for one T, and sets the value to the given initval using operator=
	template <typename T>
	inline T* EmbedInit(T initval);
};

template <typename T>
T* Embeddor::Embed() {
	len -= sizeof(T);
	T* pDat = reinterpret_cast<T*>(currdat);
	currdat = pDat + 1;
	return pDat;
}
//Embed<T>(unsigned int total) gives the memory for the 'total' number of T objects.
template <typename T>
T* Embeddor::Embed(const unsigned int total) {
	len -= total * sizeof(T);
	T* pDat = reinterpret_cast<T*>(currdat);
	currdat = pDat + 1;
	return pDat;
}

//Embed<T>(&arg0, ... , &argn) will make the pointers point to linear, unique memory (arg0 < argn)
	//Not actually suggesting you use this in typical embedded software ( uneccessary code ).
	// 	Unless if getting rid of many member initializations for this one call might be advantageous.
template <typename TArg1, typename ...TArgs>
void Embeddor::Embed( TArg1 *& arg1, TArgs *& ... args) {
	arg1 = Embed<TArg1>();
	if constexpr (sizeof...(args) != 0) {
		Embed(args...);
	}
}

template <typename T>
inline T* Embeddor::EmbedInit(T initval) {
	T* pDat = Embed<T>();
	*pDat = initval;
}

