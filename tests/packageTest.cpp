
//#include <Memorable/Embeddor.h>
#include <Memorable/Package.h>
#include <cstdio>
#include <string>
#include <string.h>
void Print1(const char * name) {
	fprintf(stdout, "Howdy %s\n", name);
}

//Types of elements could be in package:
//	1. raw binary data, no dependencies, just data.
//	2. Pointers
//		- Pointers to things in the same package (easy given the packages old start point with this)
//		- Pointers to things in another package (easy given we have access to the other package)
//		- Pointers to things not in a package (nearly impossible to determine meaning from raw pointer data,
//			 must be loaded in at runtime, likely to be used as a consistent location for dependent pointers to find data.)
//This demonstrates basic "package" memory utility.
	//Here, we can run the program once and populate package memory with values, 
	//the next time we run we will get the same values. (pointers should be good too! ..with some conditions)
int main(int argc, char * argv[]) {
	std::string pkgfile = "/var/local/MI/testpack.bin";
	Package testpkg (pkgfile.c_str(), 64 * 1000);
	XREmbeddor& embeddor = testpkg.embeddor;
	
	bool firstrun = (testpkg.embeddor.mem.old == nullptr);
	//void (**pfunc)(const char *) = embeddor.Embed<void(*)(const char *)>(); //NOW Why tha heck don't this work
	//*pfunc = Print1;
	int * pnamelen = embeddor.Embed<int>();
	char * name;
	if (firstrun) {
		fprintf(stdout, "Henlo, wat is ur name? :\n");
		name = reinterpret_cast<char*>(embeddor.currdat);
		int n = 0;
		do {
			n = scanf("%s", name);
		} while (n != 1);
		*pnamelen = strlen(name);
		embeddor.Embed<char>(*pnamelen + 1);
		fprintf(stdout, "Nice to meet you %s! Come again!\n", name);
	} else {
		name = embeddor.Embed<char>(*pnamelen + 1);//should be at location of name.
		fprintf(stdout, "Welcome back %s, I've missed you <3\n", name);
	}
	
	//*(name + *pnamelen) = '\0';
	/*
	if (firstrun) {
		fprintf(stdout, "Set Name to 'Fren'\n");
		char * pc = name;
		*(pc++) = 'f';
		*(pc++) = 'r';
		*(pc++) = 'e';
		*(pc++) = 'n';
		*(pc++) = '\0';
	}
	//(*pfunc)(name);*/
	return 0;
}
