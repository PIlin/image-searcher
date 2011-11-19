#pragma once

#include <istream>
#include <ostream>
#include <string>
#include <vector>


#include "Util/types.hpp"

class Image_pimpl;

class Image
{
public:
	
	Image(std::string fname);
	~Image();

	void open();
	void siftIt();

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

	std::vector<Word> & getWords();

private:

	Image_pimpl* pimpl;

	std::string mFname;

	std::vector<Word> mWords;

	SiftDescr* mpDescr;
	size_t mDescrCount;

	
};
