#pragma once


#include <vl/sift.h>

//template <typename DT = unsigned char>
class Sift
{
public:
	//typedef DT DescrType;
	typedef unsigned char DescrType;

	struct Frame 
	{
		double x;
		double y;
		double sigma;
		double angle;
	};


public:
	Sift(int width, int height, int noct = -1, int nlev = 3, int o_min = 0);
	~Sift(void);

	void setData(vl_sift_pix const* data);

	int run(Frame*& frames, DescrType*& descr, int& reserved);

private:

	int computeOctave(bool first);

private:
	VlSiftFilt* mSiftFlt;
	vl_sift_pix const* mData;

};



