#include <exception>
#include <fstream>
#include <iostream>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>

#include "Sift/Sift.hpp"
#include "HIKMTree/HIKMTree.hpp"
#include "ivfile/src/ccInvertedFile.hpp"
#include "Image/Image.hpp"
#include "Util/util.hpp"
#include "Util/opts.hpp"

namespace bfs = boost::filesystem;
namespace bpo = boost::program_options;

using std::string;



void calc_words(Image& img, HIKMTree& tree)
{
	TRACE;
	img.open();
	img.siftIt();
	tree.push(img);
}

void make_query(Image& img, HIKMTree& tree, ivFile& ivf, ivFile::Dist& dist, std::ostream& out)
{
	TRACE;

	docvec query;
	query.push_back(img.getWords());

	ivNodeLists score;

	ivf.search(query, 
		dist,
        false, (uint)5, score, false);
	
	for (auto it = score.begin(); it != score.end(); ++it)
	{
		ivNodeList const& nl = *it;
		for (auto nit = nl.begin(); nit != nl.end(); ++nit)
		{
			ivNode const& n = *nit;
			out << n.id << " " << n.val << '\n';
		}
	}
}

std::istream& operator>>(std::istream& is, ivFile::Dist& dist)
{
	std::string str;
	is >> str;
	if      (str == "l1")        dist = ivFile::DIST_L1;
	else if (str == "l2")        dist = ivFile::DIST_L2;
	else if (str == "ham")       dist = ivFile::DIST_HAM;
	else if (str == "kl")        dist = ivFile::DIST_KL;
	else if (str == "cos")       dist = ivFile::DIST_COS;
	else if (str == "jac")       dist = ivFile::DIST_JAC;
	else if (str == "hist-int")  dist = ivFile::DIST_HISTINT;
	else throw bpo::validation_error(bpo::validation_error::invalid_option_value, str);
	return is;
}

int main(int argc, char* argv[]) try
{
	TRACE;

	string ifname;
	string wfname;
	string tfname;
	string invfname;
	string ofname;
	ivFile::Dist dist = ivFile::DIST_L1;

	bpo::options_description desc("");
	desc.add_options()
		("help,h", "Help message")
		("image,i", bpo::value(&ifname), "Query image file")
		("words,w", bpo::value(&wfname), "Words file of query image")
		("tree,t", bpo::value(&tfname), "Tree file")
		("ivf,v", bpo::value(&invfname), "Inverted file")
		("output,o", bpo::value(&ofname), "Results output")
		;

	bpo::options_description optParams("Parameters");
	optParams.add_options()
		("dist,D", bpo::value(&dist), "Distance function in ivf")
		;

	desc.add(optParams);

	bpo::variables_map vm;
	bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
	bpo::notify(vm);

	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		return 0;
	}

	conflicting_options(vm, "image", "words");

	option_dependency(vm, "image", "tree");
	option_dependency(vm, "image", "ivf");
	option_dependency(vm, "image", "output");
	option_dependency(vm, "words", "tree");
	option_dependency(vm, "words", "ivf");
	option_dependency(vm, "words", "output");

//#define PR(a) std::cout << #a << " = " << a << endl;
//	PR(ifname);
//	PR(wfname);
//	PR(tfname);
//	PR(invfname);
//	PR(ofname);
//#undef PR

	if (!checkFile(tfname))
		throw std::runtime_error(tfname + " not found");
	if (!checkFile(invfname))
		throw std::runtime_error(invfname + " not found");

	HIKMTree tree(1,2,3);
	tree.load(tfname);

	ivFile ivf;
	ivf.load(invfname);

	Image img(ifname);

	if (vm.count("image"))
	{
		if (!checkFile(ifname))
			throw std::runtime_error(ifname + " not found");
		calc_words(img, tree);
	}
	else
	{
		if (!checkFile(wfname))
			throw std::runtime_error(wfname + " not found");
		img.load(wfname);
	}

	if (ofname == "--")
		make_query(img, tree, ivf, dist, cout);
	else
	{
		ofstream ofs;
		ofs.open(ofname);
		make_query(img, tree, ivf, dist, ofs);
		ofs.close();
	}


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