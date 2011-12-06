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

void prepare(int argc, char* argv[], std::string& ofname, str_vector& sift_infiles, HIKMTree::Params& hikmParams) 
{
	std::string inlist_file;

	bpo::options_description desc("");
	desc.add_options()
		("help,h", "Help message")
		("output,o", bpo::value(&ofname)->required(), "Output tree file")
		("list,l", bpo::value(&inlist_file), "File with the list of input sift files")
		("input,i", bpo::value(&sift_infiles), "Sift descriptors input files")
		;

	bpo::options_description optParams("Tree parameters");
	optParams.add_options()
		("clustres,C", bpo::value(&hikmParams.clusters)->default_value(hikmParams.clusters), "Count of clusters on each level")
		("leaves,L", bpo::value(&hikmParams.leaves)->default_value(hikmParams.leaves), "Maximum number of leaves")
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
		sift_infiles = vm["input"].as<str_vector>();
	}
	else
	{
		sift_infiles.clear();
		read_inlist_file(inlist_file, sift_infiles);
	}
}

void readSiftInFilese( str_vector &sift_infiles, std::vector<SiftDescr> &all_descr ) 
{
	TRACE;

	for (auto it = sift_infiles.begin(); it != sift_infiles.end(); ++it)
	{
		std::string const & inf = *it;
		if (!checkFile(inf))
			throw std::runtime_error(inf + " not found");

		Image img("");
		img.loadDescr(inf);

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
}
int main(int argc, char* argv[]) try
{
	TRACE;

	std::string ofname;
	str_vector sift_infiles;
	HIKMTree::Params hikmParams;

	prepare(argc, argv, ofname, sift_infiles, hikmParams);
	
	bfs::path ouf(ofname);

	std::vector <SiftDescr> all_descr;

	readSiftInFilese(sift_infiles, all_descr);


	HIKMTree tree(hikmParams);
	tree.train(all_descr);
	tree.save(ouf.string());

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