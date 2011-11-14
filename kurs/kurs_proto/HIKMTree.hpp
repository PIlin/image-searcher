#pragma once

#include <vl/hikmeans.h>
#include <vector>

#include <istream>
#include <ostream>


class HIKMTree
{
public:
	HIKMTree(int dims, int clusters, int leaves, VlIKMAlgorithms method = VL_IKM_ELKAN);
	HIKMTree(std::string const &fname);
	~HIKMTree(void);

	void train(std::vector<unsigned char> const & data);

	void push(std::vector<unsigned char> const & data, std::vector<unsigned int> & word);
	void push(std::vector<unsigned char> const & data, unsigned int & word);


	unsigned int maxWord() const;

	int Dims() const { return vl_hikm_get_ndims(mTree); }
	int Clusters() const { return vl_hikm_get_K(mTree); }
	int Leaves() const { return mLeaves; }
	int Depth() const { return vl_hikm_get_depth(mTree); }

	void save(std::string const & fname) const;
	void save(std::ostream& os) const;

	void load(std::string const & fname);
	void load(std::istream& is);

	friend std::ostream& operator<<(std::ostream& os, HIKMTree const& tree);
	friend std::ostream& operator<<(std::ostream& os, VlHIKMTree const& tree);
	friend std::ostream& operator<<(std::ostream& os, VlHIKMNode const& node);
	friend std::ostream& operator<<(std::ostream& os, VlIKMFilt const& filt);

	friend std::istream& operator>>(std::istream& is, HIKMTree & tree);
	friend std::istream& operator>>(std::istream& is, VlHIKMTree & tree);
	friend std::istream& operator>>(std::istream& is, VlHIKMNode & node);
	friend std::istream& operator>>(std::istream& is, VlIKMFilt & filt);

private:

	HIKMTree(HIKMTree const & reff);

	VlHIKMTree* mTree;

	int mLeaves;

	std::vector<unsigned int> mWordBuf;

};

