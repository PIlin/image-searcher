#pragma once

#include <string>

#include "cimg.hpp"

class Image_pimpl
{
public:
	Image_pimpl(void);
	~Image_pimpl(void);

	void open(std::string fname);

	template<typename T>
	T toGrayscale(T& img)
	{
		return (img.get_channel(0) + img.get_channel(1) + img.get_channel(2))/3;
	}

	typedef cimg_library::CImg<float> CIMG;

	CIMG mImage;
};

