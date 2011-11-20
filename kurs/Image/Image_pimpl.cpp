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
	mImage = toGrayscale(CIMG(fname.c_str()));
}