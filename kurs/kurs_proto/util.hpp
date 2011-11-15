#pragma once

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
