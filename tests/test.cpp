//#include <vector>
//#include <MemConfig.h>
#include <cstdlib>
#include <stdio.h>
#include <Memorable/Embeddor.h>
//#include <Test/Comparisons.h>

/*struct A {
	static MemConfig Mem;
	static std::vector<long unsigned int> vec;
	static void * Add(const long unsigned int val) {
		vec.push_back(val);
		return &vec[val];
	}
	static void FreeV(void * item) { 
		unsigned int i = (long unsigned int)(reinterpret_cast<long unsigned int*>(item)) - (long unsigned int)(&vec[0]);
		vec.erase(vec.begin() + i);
	 }
	static void Init() { Mem.SetADPair( Add, FreeV ); }
};
MemConfig A::Mem;
std::vector<long unsigned int> A::vec;
*/
struct A {
	int v [3];
	A() {
		fprintf(stdout, "\033[32mA Constructor Called\033[0m\n");
	}
	~A() {
		fprintf(stdout, "\033[32mA Destructor Called\033[0m\n");
	}
};

int main() {
	void * memory = malloc( 1000*sizeof(int) );
	unsigned int cap = 1000*sizeof(int);
	Embeddor embeddor (memory, cap);
	Embedded<A> a = embeddor.Construct<A>();

	/*
	std::string A = "a";
	std::string B = "b";
	Compare::Tester<std::string> tester;
	tester.Test(A, B, Compare::Assertion::Less);
	
	A::Init();
	A::Mem.alloc(1);
	if (A::vec[0] != 1 || A::vec.size() != 1) { return 1; }
	std::cout << A::vec[0] << " " << A::vec.size() << std::endl;
	A::Mem.dealloc(&A::vec[0]);
	if (A::vec.size() != 0) { std::cout << "FAILED " << A::vec.size() << std::endl; return 1; }
	*/
	return 0;
};
