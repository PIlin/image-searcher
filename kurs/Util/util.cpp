#include <iostream>

#include <boost/filesystem.hpp>

#include "util.hpp"

Timer::Timer()
{
#if defined(WIN32)
	QueryPerformanceFrequency(&ticFreq) ;
	ticMark.QuadPart = 0;
#else
	ticMark = 0;
#endif
}

void Timer::tic()
{
#if defined(WIN32)
	QueryPerformanceCounter (&ticMark) ;
#else
	ticMark = clock() ;
#endif
}

double Timer::toc()
{
	#if defined(WIN32)
		LARGE_INTEGER tocMark ;
		QueryPerformanceCounter(&tocMark) ;
		return (double)(tocMark.QuadPart - ticMark.QuadPart) / ticFreq.QuadPart;
#else
		return (double)(clock() - ticMark) / CLOCKS_PER_SEC ;
#endif
}

Trace::Trace(char const* fname) : 
	fname(fname) 
{ 
	std::cerr << fname
//		<< std::endl; 
		<< '\n';
	timer.tic(); 
}

Trace::~Trace() 
{ 
	std::cerr << fname << " done " << timer.toc() 
//		<< std::endl; 
		<< '\n';
}

bool checkFile(std::string const & p)
{
	namespace bfs = boost::filesystem;
	bfs::path pp(p);
	return bfs::exists(pp) && bfs::is_regular_file(pp);
}