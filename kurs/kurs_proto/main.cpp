#define  XMD_H

#define cimg_use_jpeg
#include <Cimg.h>


using cimg_library::CImg;
using cimg_library::CImgDisplay;

typedef unsigned char uchar;

int main()
{
	CImg<uchar> img("1.jpg");
	CImgDisplay d(img);

	while(1);
	return 0;
}
