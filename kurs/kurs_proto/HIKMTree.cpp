#include <exception>

#include "HIKMTree.hpp"


HIKMTree::HIKMTree(int dims, int clusters, int leaves, VlIKMAlgorithms method):
	mTree(nullptr),
	mLeaves(leaves)
{
	mTree = vl_hikm_new(method);
	if (!mTree)
		throw std::bad_alloc("vl_hikm_new returned nullptr");

	int depth = VL_MAX(1, ceil (log ((double)leaves * 1.0) / log((double)clusters)));

	vl_hikm_init(mTree, dims, clusters, depth);
}

HIKMTree::~HIKMTree(void)
{
	vl_hikm_delete(mTree);
}

void HIKMTree::train(std::vector<unsigned char> const & data)
{
	vl_hikm_train(mTree, &data.front(), data.size() / Dims());
}

void HIKMTree::push(std::vector<unsigned char> const & data, std::vector<unsigned int> & word)
{
	word.resize(Depth() * 1);
	vl_hikm_push(mTree, &word.front(), &data.front(), 1);
}

void HIKMTree::push(std::vector<unsigned char> const & data, unsigned int & word)
{
	push(data, mWordBuf);

	word = 0;
	unsigned int p = 1;
	unsigned int k = Clusters();
	for (auto it = mWordBuf.begin(); it != mWordBuf.end(); ++it)
	{
		word += (*it) * p;
		p *= k;
	}
}

unsigned int HIKMTree::maxWord() const
{
	return (unsigned int)pow((double)Clusters(), Depth()) - 1;
}