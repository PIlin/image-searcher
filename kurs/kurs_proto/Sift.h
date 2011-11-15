#pragma once


#include <vl/sift.h>

#include "types.hpp"

class Sift
{
public:

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

	int run(Frame*& frames, SiftDescr*& descr, int& reserved,
		bool getFrames = false, bool getDescrs = true);

	void print_info();

private:

	int computeOctave(bool first);

private:
	VlSiftFilt* mSiftFlt;
	vl_sift_pix const* mData;

};



