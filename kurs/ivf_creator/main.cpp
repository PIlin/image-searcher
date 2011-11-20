#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "Image/Image.hpp"
#include "HIKMTree/HIKMTree.hpp"
#include "Util/util.hpp"
#include "ivfile/src/ccInvertedFile.hpp"

namespace bfs = boost::filesystem;


void print_help(char const* pname)
{
	std::cout << pname << " ivf_outfile tree_infile word_infile [word_infile ...]" << std::endl;
}

int main(int argc, char* argv[])
{
	TRACE;

	if (argc < 4)
	{
		print_help(argv[0]);
		return 1;
	}

	bfs::path ouf(argv[1]);

	docvec dv;

	for (int i = 3; i < argc; ++i)
	{
		bfs::path inf(argv[i]);

		if (!(bfs::exists(inf) && bfs::is_regular_file(inf)))
		{
			std::cerr << inf << " not found. Exiting" << std::endl;
			return 2;
		}

		Image img("");
		img.load(inf.string());

		dv.push_back(img.getWords());
	}

	HIKMTree tree(1,2,3);
	{
		bfs::path inf(argv[2]);
		if (!(bfs::exists(inf) && bfs::is_regular_file(inf)))
		{
			std::cerr << inf << " not found. Exiting" << std::endl;
			return 2;
		}
		tree.load(inf.string());
	}

	ivFile file;
	file.fill(dv, tree.maxWord(), 0);
	file.computeStats(ivFile::WEIGHT_NONE, ivFile::NORM_L1);

	file.save(ouf.string());

	return 0;
}