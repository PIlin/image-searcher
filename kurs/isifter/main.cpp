#include <exception>
#include <fstream>
#include <iostream>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <Image/Image.hpp>
#include "Util/util.hpp"

namespace bfs = boost::filesystem;


void print_help(char const* pname)
{
	std::cout << pname << " infile outfile" << std::endl;
}

int main(int argc, char* argv[])
{
	TRACE;

	//DebugBreak();

	if (argc != 3)
	{
		print_help(argv[0]);
		return 1;
	}

	bfs::path inf(argv[1]);
	bfs::path ouf(argv[2]);

	if (!(bfs::exists(inf) && bfs::is_regular_file(inf)))
	{
		std::cerr << inf << " not found. Exiting" << std::endl;
		return 2;
	}

	try 
	{
		Image i(inf.string());
		i.open();
		i.siftIt();
		i.saveDescr(ouf.string());
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 3;
	}

	return 0;
}