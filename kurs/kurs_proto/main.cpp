#define  XMD_H

#define cimg_use_jpeg
#include <Cimg.h>

#include <iostream>

#include "Sift.h"


using cimg_library::CImg;
using cimg_library::CImgDisplay;

using std::cout;
using std::endl;

typedef unsigned char uchar;

template<typename T>
CImg<T> toGrayscale(CImg<T>& img)
{
	return (img.get_channel(0) + img.get_channel(1) + img.get_channel(2))/3;
}

int main()
{
	CImg<uchar> img("1.jpg");

	CImg<> dest = toGrayscale(img);

	cout << "w: " << dest.width() << '\n';
	cout << "h: " << dest.height() << '\n';
	cout << "s: " << dest.size() << endl;


	Sift sift(dest.width(), dest.height());
	sift.setData(dest.data());

	Sift::Frame* frames = nullptr;
	Sift::DescrType* desc = nullptr;
	int reserved = 0;

	int n = sift.run(frames, desc, reserved);
	sift.print_info();

	cout << "reserved: " << reserved << '\n';
	cout << "n: " << n << endl;

	return 0;
}
