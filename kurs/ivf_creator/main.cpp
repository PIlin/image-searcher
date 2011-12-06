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

std::istream& operator>>(std::istream& is, ivFile::Norm& norm)
{
	std::string str;
	is >> str;
	if      (str == "none")  norm = ivFile::NORM_NONE;
	else if (str == "l0")    norm = ivFile::NORM_L0;
	else if (str == "l1")    norm = ivFile::NORM_L1;
	else if (str == "l2")    norm = ivFile::NORM_L2; 
	else throw bpo::validation_error(bpo::validation_error::invalid_option_value, str);
	return is;
}

std::istream& operator>>(std::istream& is, ivFile::Weight& wt)
{
	std::string str;
	is >> str;
	if      (str == "none")   wt = ivFile::WEIGHT_NONE;
	else if (str == "bin")    wt = ivFile::WEIGHT_BIN;
	else if (str == "tf")     wt = ivFile::WEIGHT_TF;
	else if (str == "tfidf")  wt = ivFile::WEIGHT_TFIDF;
	else throw bpo::validation_error(bpo::validation_error::invalid_option_value, str);
	return is;
}

void prepare(int argc, char* argv[], std::string& ofname, std::string& tree_infile, str_vector& word_infiles, ivFile::Params& params) 
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

	bpo::options_description optParams("IVF parameters");
	optParams.add_options()
		("weight,W", bpo::value(&params.weight), "Weight: none, bin, tf, tfidf")
		("norm,N", bpo::value(&params.norm), "Norm: none, l0, l1, l2")
		;

	desc.add(optParams);

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
	ivFile::Params params;

	prepare(argc, argv, ofname, tree_infile, word_infiles, params);

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

	ivFile file(params);
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