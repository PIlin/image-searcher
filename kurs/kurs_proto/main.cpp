#define  XMD_H

#define cimg_use_jpeg
#include <Cimg.h>

#include <iostream>
#include <string>
#include <vector>

#include "Sift.h"
#include "HIKMTree.hpp"

#include <vl/hikmeans.h>


using cimg_library::CImg;
using cimg_library::CImgDisplay;

using std::cout;
using std::endl;
using std::vector;
using std::string;

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
	all_descr.clear();

	for (auto it = images.begin(); it != images.end(); ++it )
	{
		Image& i = *it;

		CImg<> dest = toGrayscale(CImg<uchar>(i.fname.c_str()));

		cout << i.fname << '\n';
		cout << "w: " << dest.width() << '\n';
		cout << "h: " << dest.height() << '\n';
		cout << "s: " << dest.size() << endl;

		Sift sift(dest.width(), dest.height());
		sift.setData(dest.data());
		Sift::Frame* frames = nullptr;
		Sift::DescrType* desc = nullptr;
		int reserved = 0;

		int n = sift.run(frames, desc, reserved);
		cout << "n: " << n << endl;

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


int main()
{
	images.push_back(Image("1.jpg"));

	initImages();
	////

	int dims = 128;
	int clusters = 3;
	int leaves = 100;


	HIKMTree tree(dims, clusters, leaves);

	tree.train(all_descr);
	
	computeWords(tree);


	return 0;
}
