#pragma once

#include <string>
#include <iostream>

#if defined(WIN32)
#define WINDOWS_LEAN_AND_MEAN
#include "Windows.h"
#else
#include <ctime>
#endif

struct Timer {
#if defined(WIN32)
	LARGE_INTEGER ticFreq ;
	LARGE_INTEGER ticMark ;
#else
	clock_t ticMark ;
#endif

	Timer();
	void tic();
	double toc();
};

struct Trace {
	Trace(char const* fname);
	~Trace();
	char const * fname;
	Timer timer;
};
#define TRACE Trace __trace(__FUNCTION__)

//////////////////////////////////////////////////////////////////////////

template<typename T>
void write(std::ostream & os, T const & t)
{
	os.write(reinterpret_cast<char const *>(&t), sizeof(t));
}
template<typename T>
void read(std::istream & is, T & t)
{
	is.read(reinterpret_cast<char*>(&t), sizeof(t));
}
#define WRITE(t) write(os, t)
#define READ(t)  read(is, t)

//////////////////////////////////////////////////////////////////////////

bool checkFile(std::string const & p);

//////////////////////////////////////////////////////////////////////////

#ifdef __GNUC__
#define nullptr NULL

//const                      // this is a const object...
//class {
//public:
//template<class T>          // convertible to any type
	//operator T*() const      // of null non-member
	//{ return 0; }            // pointer...
//template<class C, class T> // or any type of null
	//operator T C::*() const  // member pointer...
	//{ return 0; }
//private:
	//void operator&() const;    // whose address can't be taken
//} nullptr = {};              // and whose name is nullptr
#endif // __GNUC__
