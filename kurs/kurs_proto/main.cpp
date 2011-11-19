
#include <iostream>
#include <string>
#include <vector>

//#include <vl/hikmeans.h>

#include "Sift/Sift.hpp"
#include "HIKMTree/HIKMTree.hpp"
#include "ccInvertedFile.hpp"
#include "Image/Image.hpp"

#include "Util/util.hpp"



using std::cout;
using std::endl;
using std::vector;
using std::string;


vector <Image> images;
vector <SiftDescr> all_descr;

void initImages()
{
	TRACE;

	all_descr.clear();

	for (auto it = images.begin(); it != images.end(); ++it )
	{
		Image& i = *it;

		i.open();
		i.siftIt();

		SiftDescr const * descr = i.getDescr();
		size_t descrCount = i.getDescrCount();

		all_descr.reserve(all_descr.size() + descrCount * 128 * sizeof(SiftDescr));

		for (int j = 0; j < descrCount; ++j)
		{
			const auto b = descr + 128*j;
			const auto e = b + 128;
			all_descr.insert(all_descr.end(), b, e);
		}

	}
}


void computeWords(HIKMTree& tree)
{
	TRACE;

	for (auto it = images.begin(); it != images.end(); ++it)
	{
		Image& i = *it;

		tree.push(i);
		//i.computeWords(tree);
		i.forgetDescr();
	}
}

void prepIvFile(ivFile& file, uint nwords)
{
	TRACE;

	docvec dv;
	for (auto it = images.begin(); it != images.end(); ++it)
	{
		dv.push_back((*it).getWords());
	}

	file.fill(dv, nwords, 0);
	
	file.computeStats(ivFile::WEIGHT_NONE, ivFile::NORM_L1);
}


void makeQuery(ivFile& ivf, vector<int> const & q)
{
	TRACE;

	docvec query;

	for (auto it = q.begin(); it != q.end(); ++it)
	{
		query.push_back(images[(*it)].getWords());
	}

	ivNodeLists score;

	ivf.search(query, 
		ivFile::WEIGHT_NONE, ivFile::NORM_L1, ivFile::DIST_L1,
        false, (uint)5, score, false);
	
	cout << "Score" << endl;
	for (auto it = score.begin(); it != score.end(); ++it)
	{
		ivNodeList const& nl = *it;
		cout << "list" << endl;
		for (auto nit = nl.begin(); nit != nl.end(); ++nit)
		{
			ivNode const& n = *nit;
			cout << n.id + 1 << " " << n.val << endl;
		}
	}
}

int main()
{
	TRACE;
	char namebuf[256];
	for (int i = 1; i <= 86; ++i)
	{
		snprintf(namebuf, sizeof(namebuf), "d:/Downloads/caltech_101/kangaroo/image_%04d.jpg", i);
		images.push_back(Image(namebuf));
	}

	

	initImages();
	////

	int dims = 128;
	int clusters = 3;
	int leaves = 100;


	HIKMTree tree(dims, clusters, leaves);
	tree.train(all_descr);
	//tree.save("tree.cache");

	//HIKMTree tree("tree.cache");
	
	computeWords(tree);
	all_descr.clear();


	////

	ivFile ivf;
	prepIvFile(ivf, tree.maxWord());


	vector<int> q;
	q.push_back(0);
	q.push_back(1);
	q.push_back(2);
	q.push_back(3);

	makeQuery(ivf, q);

	return 0;
}
