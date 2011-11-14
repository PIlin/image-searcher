#include <exception>

#include "HIKMTree.hpp"

#include <fstream>


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

HIKMTree::HIKMTree(std::string const &fname) :
	mTree(nullptr),
	mLeaves(0)
{
	load(fname);
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

void HIKMTree::save(std::string const & fname) const
{
	std::ofstream of;
	of.open(fname.c_str(), std::ofstream::binary);
	save(of);
	of.close();
}

void HIKMTree::save(std::ostream& os) const
{	
	os << *this;
}


void HIKMTree::load(std::string const & fname)
{
	std::ifstream ifs;
	ifs.open(fname.c_str(), std::ifstream::binary);
	load(ifs);
	ifs.close();
}

void HIKMTree::load(std::istream& is)
{
	is >> *this;
}

//////////////////////////////////////////////////////////////////////////

template<typename T>
void write(std::ostream & os, T const & t)
{
	os.write(reinterpret_cast<char const *>(&t), sizeof(t));
}
template<typename T>
void read(std::istream & is, T & t)
{
	is.read(reinterpret_cast<char*>(&t), sizeof(t));
}
#define WRITE(t) write(os, t)
#define READ(t)  read(is, t)

std::ostream& operator<<(std::ostream& os, HIKMTree const & tree)
{
	WRITE(tree.mLeaves);
	if (!tree.mTree)
		throw std::runtime_error("mTree in HIKMTree is nullptr. Cannot save it");
	os << *tree.mTree;
	return os;
}

std::istream& operator>>(std::istream& is, HIKMTree & tree)
{
	READ(tree.mLeaves);

	vl_hikm_delete(tree.mTree);
	tree.mTree = vl_hikm_new(0);

	is >> *tree.mTree;
	return is;
}


std::ostream& operator<<(std::ostream& os, VlHIKMTree const& tree)
{
	WRITE(tree.M);
	WRITE(tree.K);
	WRITE(tree.max_niters);
	WRITE(tree.method);
	WRITE(tree.verb);
	WRITE(tree.depth);
	if (!tree.root)
		throw std::runtime_error("root in VlHIKMTree is nullptr. Cannot save it");
	os << *tree.root;
	return os;
}

std::istream& operator>>(std::istream& is, VlHIKMTree & tree)
{
	READ(tree.M);
	READ(tree.K);
	READ(tree.max_niters);
	READ(tree.method);
	READ(tree.verb);
	READ(tree.depth);

	tree.root = static_cast<VlHIKMNode*>(vl_malloc(sizeof(*tree.root)));
	is >> *tree.root;
	return is;
}

std::ostream& operator<<(std::ostream& os, VlHIKMNode const& node)
{
	if (!node.filter)
		throw std::runtime_error("filter in VlHIKMNode is nullptr. Cannot save it");
	
	os << *node.filter;
	bool chld =  (node.children != nullptr);
	WRITE(chld);
	if (chld)
	{
		for (int k = 0; k < node.filter->K; ++k)
		{
			if (!node.children[k])
				throw std::runtime_error("children[k] in VlHIKMNode is nullptr. Cannot save it");
			os << *node.children[k];
		}
	}
	
	return os;
}

std::istream& operator>>(std::istream& is, VlHIKMNode & node)
{
	node.filter = vl_ikm_new(0);
	is >> *node.filter;

	bool chld = false;
	READ(chld);
	if (chld)
	{
		node.children = static_cast<VlHIKMNode**>(vl_malloc(sizeof(*node.children) * node.filter->K));
		for (int k = 0; k < node.filter->K; ++k)
		{
			node.children[k] = static_cast<VlHIKMNode*>(vl_malloc(sizeof(*node.children[k])));
			is >> *node.children[k];
		}
	}
	else
	{
		node.children = nullptr;
	}
	return is;
}

std::ostream& operator<<(std::ostream& os, VlIKMFilt const& filt)
{
	WRITE(filt.M);
	WRITE(filt.K);
	WRITE(filt.method);
	WRITE(filt.max_niters);
	WRITE(filt.verb);
	if (!filt.centers)
		throw std::runtime_error("centers in VlIKMFilt is nullptr. Cannot save it");

	os.write(reinterpret_cast<char const *>(filt.centers), 
		sizeof(filt.centers[0]) * filt.M * filt.K);

	if (filt.method == VL_IKM_ELKAN)
	{
		if (!filt.inter_dist)
			throw std::runtime_error("inter_dist in VlIKMFilt is nullptr. Cannot save it");

		os.write(reinterpret_cast<char const *>(filt.inter_dist), 
			sizeof(filt.inter_dist[0]) * filt.K * filt.K);
	}

	return os;
}

std::istream& operator>>(std::istream& is, VlIKMFilt & filt)
{
	READ(filt.M);
	READ(filt.K);
	READ(filt.method);
	READ(filt.max_niters);
	READ(filt.verb);

	size_t sctr = sizeof(filt.centers[0]) * filt.M * filt.K;
	filt.centers = static_cast<vl_ikm_acc*>(vl_malloc(sctr));
	is.read(reinterpret_cast<char*>(filt.centers), sctr);

	if (filt.method == VL_IKM_ELKAN)
	{
		size_t sidst = sizeof(filt.inter_dist[0]) * filt.K * filt.K;
		filt.inter_dist = static_cast<vl_ikm_acc*>(vl_malloc(sidst));
		is.read(reinterpret_cast<char*>(filt.inter_dist), sidst);
	}
	else
	{
		filt.inter_dist = nullptr;
	}

	return is;
}