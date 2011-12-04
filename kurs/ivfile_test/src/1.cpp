#include <vector>

#include "ccInvertedFile.hpp"

docvec docs;

#define ARR_SIZE(a) sizeof(a)/sizeof(a[0])

int main()
{
	const wordtype max_word = 5;
	//wordtype al[] = {1,2,3,4,1,3,4};
	//wordtype be[] = {2,2,2,1};
	//wordtype ga[] = {3,4,5,4,5,1};
	wordtype al[] = {1, 1, 1, 1, 2, 3};
	wordtype be[] = {3, 4, 5};

	wordtype q1[] = {1};
	wordtype q2[] = {1, 2};
	wordtype q3[] = {1, 2, 3};
	wordtype q4[] = {1, 1};
	wordtype q5[] = {1, 1, 1};
	wordtype q6[] = {1, 1, 1, 1};
	wordtype q7[] = {1, 3, 5};

#define ADD_DOC(l, d) l.push_back(wordvec(d, d + ARR_SIZE(d)))
	ADD_DOC(docs, al);
	ADD_DOC(docs, be);
	ADD_DOC(docs, q7);
	//ADD_DOC(docs, ga);

	ivFile ivf;	
	ivf.fill(docs, max_word, 0);
	ivf.computeStats(ivFile::WEIGHT_NONE, ivFile::NORM_L1);

	docvec query;
	ADD_DOC(query, al);
	ADD_DOC(query, be);
	ADD_DOC(query, q7);
	

	ivNodeLists score;
	ivf.search(query, 
		ivFile::WEIGHT_NONE, ivFile::NORM_L1, ivFile::DIST_L2,
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
}

//int main()
//{
//	int n = 0;
//	freopen("in.txt", "r", stdin);
//	
//	if (scanf("%d", &n) == EOF)
//		return 1;
//
//	int nwords = 0;
//
//	docs.resize(n);
//	for (int i = 0; i < n; ++i)
//	{
//		int wrds;
//		scanf("%d", &wrds);
//		
//		docs[i].resize(wrds);
//
//		for (int j = 0; j < wrds; ++j)
//		{
//			int w;
//			scanf("%d", &w);
//
//			if (w == 0)
//			{
//				printf("Worng word 0\n");
//			}
//
//			docs[i][j] = w;
//
//			nwords = std::max(w, nwords);
//		}
//	}
//
//	printf("read done\n");
//	printf("nwords = %d\n", nwords);
//
//	ivFile ivf;	
//
//	ivf.fill(docs, nwords, 0);
//
//	ivf.computeStats(ivFile::WEIGHT_NONE, ivFile::NORM_L1);
//
//	//ivf.display();
//
//	docvec query;
//	query.push_back(docs[2]);
//	query.push_back(docs[3]);
//
//	ivNodeLists score;
//
//	ivf.search(query, 
//		ivFile::WEIGHT_NONE, ivFile::NORM_L1, ivFile::DIST_L1,
//        false, (uint)5, score, false);
//	
//	cout << "Score" << endl;
//	for (ivNodeLists::const_iterator it = score.begin(); it != score.end(); ++it)
//	{
//		ivNodeList const& nl = *it;
//		cout << "list" << endl;
//		for (ivNodeList::const_iterator it = nl.begin(); it != nl.end(); ++it)
//		{
//			ivNode const& n = *it;
//			cout << n.id + 1 << " " << n.val << endl;
//		}
//	}
//
//	cout << "done" << endl;
//	
//	return 0;
//}
