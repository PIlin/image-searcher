#include "Image.hpp"

#include "util.hpp"

#include "Sift.h"

Image::Image(std::string fname):
	mFname(fname),
	mpDescr(nullptr),
	mDescrCount(0),
	mImage()
{
}

Image::~Image()
{
}


void Image::open()
{
	TRACE;
	mImage = toGrayscale(CIMG(mFname.c_str()));
}

void Image::siftIt()
{
	TRACE;
	Sift sift(getWidth(), getHeight());
	sift.setData(mImage.data());
	
	//Sift::Frame* frames = nullptr;
	int reserved = 0;

	forgetDescr();

	Sift::Frame* f = nullptr;
	mDescrCount = sift.run(f, mpDescr, reserved);
}

void Image::computeWords(HIKMTree const & tree)
{
	TRACE;

	mWords.clear();
	mWords.resize(mDescrCount);
	for (size_t i = 0; i < mDescrCount; ++i)
	{
		tree.push(&mpDescr[i * 128], mWords[i]);
	}
}

void Image::forgetDescr()
{
	free(mpDescr);
	mpDescr = nullptr;
	mDescrCount = 0;
}

void Image::save(std::string const & fname)
{}
void Image::save(std::ostream& os)
{}

void Image::load(std::string const & fname)
{}
void Image::load(std::istream& os)
{}

int Image::getWidth() const 
{
	return mImage.width();
}

int Image::getHeight() const
{
	return mImage.height();
}

SiftDescr const * Image::getDescr() const
{
	return mpDescr;
}

size_t Image::getDescrCount() const
{
	return mDescrCount;
}

std::vector<Word> const & Image::getWords() const
{
	return mWords;
}

//////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, Image const & img);
std::istream& operator>>(std::istream& os, Image & img);
