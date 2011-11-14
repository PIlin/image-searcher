#define  XMD_H

#define cimg_use_jpeg
#include <Cimg.h>

#include <iostream>
#include <string>
#include <vector>

#include "Sift.h"
#include "HIKMTree.hpp"
#include "ccInvertedFile.hpp"

#include <vl/hikmeans.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;

struct Timer {
#if defined(WIN32)
	LARGE_INTEGER ticFreq ;
	LARGE_INTEGER ticMark ;
#else
	clock_t ticMark ;
#endif

	Timer()
	{
#if defined(WIN32)
		QueryPerformanceFrequency(&ticFreq) ;
		ticMark.QuadPart = 0;
#else
		ticMark = 0;
#endif
	}

	void tic()
	{
#if defined(WIN32)
		QueryPerformanceCounter (&ticMark) ;
#else
		ticMark = clock() ;
#endif
	}

	double toc()
	{
#if defined(WIN32)
		LARGE_INTEGER tocMark ;
		QueryPerformanceCounter(&tocMark) ;
		return (double)(tocMark.QuadPart - ticMark.QuadPart) / ticFreq.QuadPart;
#else
		return (double)(clock() - ticMark) / CLOCKS_PER_SEC ;
#endif
	}
};

struct Trace {
	Trace(char const* fname) : fname(fname) { cout << fname << endl; timer.tic(); }
	~Trace() { cout << fname << " done " << timer.toc() << endl; }
	char const * fname;
	Timer timer;
};
#define TRACE Trace __trace(__FUNCTION__)

using cimg_library::CImg;
using cimg_library::CImgDisplay;



typedef unsigned char uchar;
typedef unsigned int  uint;

template<typename T>
CImg<T> toGrayscale(CImg<T>& img)
{
	return (img.get_channel(0) + img.get_channel(1) + img.get_channel(2))/3;
}

struct Image {
	string fname;
	vector<vector<uchar> > descr;

	//vector<vector<uint> > words;
	vector<uint> words;

	Image(string fname) : 
	fname(fname) 
	{
	}
};

vector <Image> images;
vector <uchar> all_descr;

void initImages()
{
	TRACE;

	all_descr.clear();

	for (auto it = images.begin(); it != images.end(); ++it )
	{
		Image& i = *it;

		CImg<> dest = toGrayscale(CImg<uchar>(i.fname.c_str()));

		//cout << i.fname << '\n';
		//cout << "w: " << dest.width() << '\n';
		//cout << "h: " << dest.height() << '\n';
		//cout << "s: " << dest.size() << endl;

		Sift sift(dest.width(), dest.height());
		sift.setData(dest.data());
		Sift::Frame* frames = nullptr;
		Sift::DescrType* desc = nullptr;
		int reserved = 0;

		int n = sift.run(frames, desc, reserved);
		//cout << "n: " << n << endl;

		i.descr.clear();
		i.descr.reserve(n);

		all_descr.reserve(all_descr.size() + n*128);

		for (int j = 0; j < n; ++j)
		{
			const auto b = desc + 128 * j;
			const auto e = b + 128;
			i.descr.push_back(vector<uchar>(b, e));
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

		i.words.clear();
		i.words.resize(i.descr.size());
		size_t j = 0;
		for (auto dit = i.descr.begin(); dit != i.descr.end(); ++dit)
		{
			auto& d = *dit;
			tree.push(d, i.words[j]);
			++j;
		}
	}
	
}

void prepIvFile(ivFile& file, uint nwords)
{
	TRACE;

	docvec dv;
	for (auto it = images.begin(); it != images.end(); ++it)
	{
		dv.push_back((*it).words);
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
		query.push_back(images[(*it)].words);
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
