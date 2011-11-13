#include <vector>

#include "ccInvertedFile.hpp"

docvec docs;

int main()
{
	int n = 0;
	freopen("in.txt", "r", stdin);
	
	if (scanf("%d", &n) == EOF)
		return 1;

	int nwords = 0;

	docs.resize(n);
	for (int i = 0; i < n; ++i)
	{
		int wrds;
		scanf("%d", &wrds);
		
		docs[i].resize(wrds);

		for (int j = 0; j < wrds; ++j)
		{
			int w;
			scanf("%d", &w);

			if (w == 0)
			{
				printf("Worng word 0\n");
			}

			docs[i][j] = w;

			nwords = std::max(w, nwords);
		}
	}

	printf("read done\n");
	printf("nwords = %d\n", nwords);

	ivFile ivf;	

	ivf.add(docs, nwords, 0);

	ivf.computeStats(ivFile::WEIGHT_NONE, ivFile::NORM_L1);

	//ivf.display();

	docvec query;
	query.push_back(docs[2]);
	query.push_back(docs[3]);

	ivNodeLists score;

	ivf.search(query, 
		ivFile::WEIGHT_NONE, ivFile::NORM_L1, ivFile::DIST_L1,
        false, (uint)5, score, false);
	
	cout << "Score" << endl;
	for (ivNodeLists::const_iterator it = score.begin(); it != score.end(); ++it)
	{
		ivNodeList const& nl = *it;
		cout << "list" << endl;
		for (ivNodeList::const_iterator it = nl.begin(); it != nl.end(); ++it)
		{
			ivNode const& n = *it;
			cout << n.id + 1 << " " << n.val << endl;
		}
	}

	cout << "done" << endl;
	
	return 0;
}
