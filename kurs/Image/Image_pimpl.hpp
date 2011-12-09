#pragma once

#include <string>

#include "cimg.hpp"

class Image_pimpl
{
public:
	Image_pimpl(void);
	~Image_pimpl(void);

	void open(std::string const & fname);
	typedef cimg_library::CImg<float> CIMG;

	CIMG toGrayscale(CIMG& img)
	{
		return (img.get_channel(0) + img.get_channel(1) + img.get_channel(2))/3;
	}



	CIMG mImage;
};

