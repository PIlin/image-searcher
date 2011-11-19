#include <exception>
#include <fstream>

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
	forgetDescr();
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
{
	std::ofstream of;
	of.open(fname.c_str(), std::ofstream::binary);
	save(of);
	of.close();
}


void Image::save(std::ostream& os)
{
	os << *this;
}

void Image::load(std::string const & fname)
{
	std::ifstream ifs;
	ifs.open(fname.c_str(), std::ifstream::binary);
	load(ifs);
	ifs.close();
}

void Image::load(std::istream& is)
{
	is >> *this;
}


void Image::saveDescr(std::string const & fname)
{
	std::ofstream of;
	of.open(fname.c_str(), std::ofstream::binary);
	save(of);
	of.close();
}


void Image::saveDescr(std::ostream& os)
{
	if (!mpDescr)
		throw std::runtime_error("No sift descriptors");

	WRITE(mDescrCount);
	os.write(reinterpret_cast<char const *>(mpDescr), sizeof(mpDescr[0]) * mDescrCount);
}

void Image::loadDescr(std::string const & fname)
{
	std::ifstream ifs;
	ifs.open(fname.c_str(), std::ifstream::binary);
	load(ifs);
	ifs.close();
}

void Image::loadDescr(std::istream& is)
{
	forgetDescr();
	READ(mDescrCount);
	mpDescr = static_cast<SiftDescr*>(malloc(sizeof(SiftDescr) * mDescrCount));
	is.read(reinterpret_cast<char*>(mpDescr), sizeof(mpDescr[0]) * mDescrCount);
}

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

std::ostream& operator<<(std::ostream& os, Image const & img)
{
	os << img.mFname;

	WRITE(img.mWords.size());
	os.write(reinterpret_cast<char const*>(&img.mWords.front()), 
		sizeof(img.mWords[0]) * img.mWords.size());

	return os;
}

std::istream& operator>>(std::istream& is, Image & img)
{
	is >> img.mFname;

	size_t s;
	READ(s);

	img.mWords.resize(s);
	is.read(reinterpret_cast<char*>(&img.mWords.front()), 
		sizeof(img.mWords[0]) * s);

	return is;
}
