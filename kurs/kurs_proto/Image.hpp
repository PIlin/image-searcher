#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include "cimg.hpp"

#include "types.hpp"
#include "HIKMTree.hpp"

class Image
{
public:
	
	Image(std::string fname);
	~Image();

	void open();
	void siftIt();
	void computeWords(HIKMTree const & tree);

	void forgetDescr();

	friend std::ostream& operator<<(std::ostream& os, Image const & img);
	friend std::istream& operator>>(std::istream& os, Image & img);


	void save(std::string const & fname);
	void save(std::ostream& os);

	void load(std::string const & fname);
	void load(std::istream& is);
	

	void saveDescr(std::string const & fname);
	void saveDescr(std::ostream& os);

	void loadDescr(std::string const & fname);
	void loadDescr(std::istream& is);

	int getWidth() const;
	int getHeight() const;

	SiftDescr const* getDescr() const;
	size_t getDescrCount() const;

	std::vector<Word> const & getWords() const;

private:

	template<typename T>
	T toGrayscale(T& img)
	{
		return (img.get_channel(0) + img.get_channel(1) + img.get_channel(2))/3;
	}

	typedef cimg_library::CImg<float> CIMG;

	std::string mFname;

	std::vector<Word> mWords;

	SiftDescr* mpDescr;
	size_t mDescrCount;

	CIMG mImage;
};
