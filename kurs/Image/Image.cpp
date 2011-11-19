#include <exception>
#include <fstream>

#include "Image.hpp"
#include "Image_pimpl.hpp"

#include "Util/util.hpp"

#include "Sift/Sift.hpp"

Image::Image(std::string fname):
	pimpl(new Image_pimpl),
	mFname(fname),
	mpDescr(nullptr),
	mDescrCount(0)
{
}

Image::~Image()
{
	forgetDescr();
}


void Image::open()
{
	TRACE;
	pimpl->open(mFname);
}

void Image::siftIt()
{
	TRACE;
	Sift sift(getWidth(), getHeight());
	sift.setData(pimpl->mImage.data());
	
	//Sift::Frame* frames = nullptr;
	int reserved = 0;

	forgetDescr();

	Sift::Frame* f = nullptr;
	mDescrCount = sift.run(f, mpDescr, reserved);
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
	saveDescr(of);
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
	return pimpl->mImage.width();
}

int Image::getHeight() const
{
	return pimpl->mImage.height();
}

SiftDescr const * Image::getDescr() const
{
	return mpDescr;
}

size_t Image::getDescrCount() const
{
	return mDescrCount;
}

std::vector<Word> & Image::getWords()
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
