#include <exception>
#include <fstream>
#include <iostream>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <Image/Image.hpp>
#include "HIKMTree/HIKMTree.hpp"
#include "Util/util.hpp"

namespace bfs = boost::filesystem;


void print_help(char const* pname)
{
	std::cout << pname << " tree_infile sift_infile words_outfile" << std::endl;
}

int main(int argc, char* argv[])
{
	TRACE;

	if (argc != 4)
	{
		print_help(argv[0]);
		return 1;
	}

	bfs::path tree_i(argv[1]);
	bfs::path sift_i(argv[2]);
	bfs::path word_o(argv[3]);

	if (!(bfs::exists(tree_i) && bfs::is_regular_file(tree_i)))
	{
		std::cerr << tree_i << " not found. Exiting" << std::endl;
		return 2;
	}

	if (!(bfs::exists(sift_i) && bfs::is_regular_file(sift_i)))
	{
		std::cerr << sift_i << " not found. Exiting" << std::endl;
		return 2;
	}

	try 
	{
		Image i("");
		i.loadDescr(sift_i.string());

		HIKMTree tree(1,2,3);
		tree.load(tree_i.string());

		tree.push(i);

		i.save(word_o.string());
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 3;
	}
	catch (...)
	{
		std::cerr << "Something awful" << std::endl;
		return 4;
	}

	return 0;
}