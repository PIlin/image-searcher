#include "Image_pimpl.hpp"


Image_pimpl::Image_pimpl(void) :
	mImage()
{
}


Image_pimpl::~Image_pimpl(void)
{
}


void Image_pimpl::open( std::string const & fname )
{
	CIMG tmp(fname.c_str());
	mImage = toGrayscale(tmp);
}
