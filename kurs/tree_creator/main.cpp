#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "Image/Image.hpp"
#include "HIKMTree/HIKMTree.hpp"
#include "Util/util.hpp"

namespace bfs = boost::filesystem;


void print_help(char const* pname)
{
	std::cout << pname << " tree_outfile sift_infile [sift_infile ...]" << std::endl;
}

int main(int argc, char* argv[])
{
	TRACE;

	if (argc < 3)
	{
		print_help(argv[0]);
		return 1;
	}

	bfs::path ouf(argv[1]);

	std::vector <SiftDescr> all_descr;

	for (int i = 2; i < argc; ++i)
	{
		bfs::path inf(argv[i]);

		if (!(bfs::exists(inf) && bfs::is_regular_file(inf)))
		{
			std::cerr << inf << " not found. Exiting" << std::endl;
			return 2;
		}

		Image img("");
		img.loadDescr(inf.string());


		SiftDescr const * descr = img.getDescr();
		size_t descrCount = img.getDescrCount();

		all_descr.reserve(all_descr.size() + descrCount * 128 * sizeof(SiftDescr));

		for (int j = 0; j < descrCount; ++j)
		{
			const auto b = descr + 128*j;
			const auto e = b + 128;
			all_descr.insert(all_descr.end(), b, e);
		}
	}

	int dims = 128;
	int clusters = 3;
	int leaves = 100;

	HIKMTree tree(dims, clusters, leaves);
	tree.train(all_descr);

	tree.save(ouf.string());

	return 0;
}