#pragma once

#include <vl/hikmeans.h>
#include <vector>


class HIKMTree
{
public:
	HIKMTree(int dims, int clusters, int leaves, VlIKMAlgorithms method = VL_IKM_ELKAN);
	~HIKMTree(void);

	void train(std::vector<unsigned char> const & data);

	void push(std::vector<unsigned char> const & data, std::vector<unsigned int> & word);
	void push(std::vector<unsigned char> const & data, unsigned int & word);


	unsigned int maxWord() const;

	int Dims() const { return vl_hikm_get_ndims(mTree); }
	int Clusters() const { return vl_hikm_get_K(mTree); }
	int Leaves() const { return mLeaves; }
	int Depth() const { return vl_hikm_get_depth(mTree); }
private:

	VlHIKMTree* mTree;

	int mLeaves;

	std::vector<unsigned int> mWordBuf;

};

