#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "Image/Image.hpp"
#include "HIKMTree/HIKMTree.hpp"
#include "Util/opts.hpp"
#include "Util/util.hpp"
#include "ivfile/src/ccInvertedFile.hpp"

namespace bfs = boost::filesystem;

typedef std::vector<std::string> str_vector;

void read_inlist_file(std::string const & file, str_vector & list)
{
	TRACE;

	if (!checkFile(file))
		throw std::runtime_error("List file is not exsist");

	bfs::path p(file);

	bfs::ifstream ifs;
	ifs.open(p);
	std::string s;
	while (ifs >> s)
		list.push_back(s);
	ifs.close();
}

void prepare(int argc, char* argv[], std::string& ofname, std::string& tree_infile, str_vector& word_infiles) 
{
	std::string inlist_file;

	bpo::options_description desc("");
	desc.add_options()
		("help,h", "Help message")
		("output,o", bpo::value(&ofname)->required(), "Output ivf file")
		("tree,t", bpo::value(&tree_infile)->required(), "HIKM Tree file")
		("list,l", bpo::value(&inlist_file), "File with the list of input sift files")
		("input,i", bpo::value(&word_infiles), "Words input files")
		;

	bpo::positional_options_description p;
	p.add("input", -1);

	bpo::variables_map vm;
	bpo::store(bpo::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		exit(0);
	}

	bpo::notify(vm);

	conflicting_options(vm, "input", "list");

	if (vm.count("input"))
	{
		word_infiles = vm["input"].as<str_vector>();
	}
	else
	{
		word_infiles.clear();
		read_inlist_file(inlist_file, word_infiles);
	}
}


void print_help(char const* pname)
{
	std::cout << pname << " ivf_outfile tree_infile word_infile [word_infile ...]" << std::endl;
}

int main(int argc, char* argv[]) try
{
	TRACE;

	std::string ofname;
	std::string tree_infile;
	str_vector word_infiles;

	prepare(argc, argv, ofname, tree_infile, word_infiles);

	if (!checkFile(tree_infile))
		throw std::runtime_error(tree_infile + " not found");

	docvec dv;

	for (auto it = word_infiles.cbegin(); it != word_infiles.end(); ++it)
	{
		auto inf = *it;
		if (!checkFile(inf))
			throw std::runtime_error(inf + " not found");
		
		Image img("");
		img.load(inf);

		dv.push_back(img.getWords());
	}

	HIKMTree tree(1,2,3);
	tree.load(tree_infile);

	ivFile file;
	file.fill(dv, tree.maxWord(), 0);
	file.computeStats();


	file.save(ofname);

	return 0;
}
catch (std::exception& e)
{
	std::cerr << "Error: " << e.what() << std::endl;
	return 10;
}
catch (...)
{
	std::cerr << "Something awfull" << std::endl;
	return 11;
}