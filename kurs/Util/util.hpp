#pragma once

#include <string>

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
