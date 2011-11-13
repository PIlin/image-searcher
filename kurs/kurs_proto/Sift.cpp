#include <exception>

#include "Sift.h"


//////////////////////////////////////////////////////////////////////////

VL_INLINE void transpose_descriptor(vl_sift_pix* dst, vl_sift_pix* src);

template <typename T>
VL_INLINE T convertDescriptor(vl_sift_pix x);

template <>
VL_INLINE float convertDescriptor(vl_sift_pix x)
{
	return 512.0F * x;
}

template <>
VL_INLINE unsigned char convertDescriptor(vl_sift_pix x)
{
	float fx = 512.0F * x ;
	return (unsigned char)((fx < 255.0F) ? fx : 255.0F);
}

//////////////////////////////////////////////////////////////////////////


Sift::Sift(int width, int height, 
	int noct/* = -1*/, int nlev/* = 3*/, int o_min/* = 0*/) :
mSiftFlt(nullptr),
mData(nullptr)
{
	mSiftFlt = vl_sift_new(width, height, noct, nlev, o_min);
	if (!mSiftFlt)
		throw std::bad_alloc("vl_sift_new returned nullptr");
}


Sift::~Sift(void)
{
	if (mSiftFlt)
		vl_sift_delete(mSiftFlt);
}

void Sift::setData(vl_sift_pix const* data)
{
	mData = data;
}

int Sift::run(Frame*& frames, DescrType*& descr, int& reserved)
{
	bool first = true;
	
	VlSiftKeypoint const* keys = nullptr;
	int nkeys = 0;

	//double*    frames = nullptr;
	//DescrType* descr  = nullptr;
	//int reserved = 0;
	int nframes = 0;

	while (true)
	{
		if (computeOctave(first))
			break;
		first = false;

		vl_sift_detect(mSiftFlt);

		keys =  vl_sift_get_keypoints(mSiftFlt);
		nkeys = vl_sift_get_nkeypoints(mSiftFlt);

		for (int i = 0; i < nkeys; ++i)
		{
			double                angles [4] ;
			int                   nangles ;
			VlSiftKeypoint        ik ;
			VlSiftKeypoint const *k ;

			/* Obtain keypoint orientations ........................... */

			k = keys + i ;
			nangles = vl_sift_calc_keypoint_orientations(mSiftFlt, angles, k) ;


			/* For each orientation ................................... */
			for (int q = 0 ; q < nangles ; ++q) {
				vl_sift_pix  buf [128] ;
				vl_sift_pix rbuf [128] ;

				/* compute descriptor (if necessary) */
				if (true) {
					vl_sift_calc_keypoint_descriptor(mSiftFlt, buf, k, angles[q]);
					transpose_descriptor(rbuf, buf);
				}

				/* make enough room for all these keypoints and more */
				if (reserved < nframes + 1) {
					reserved += 2 * nkeys ;
					frames = static_cast<Frame*>(
						realloc(frames, sizeof(Frame) * reserved));
					descr  = static_cast<DescrType*>(
						realloc(descr,  128 * sizeof(DescrType) * reserved));
				}

				/* Save back with MATLAB conventions. Notice tha the input
				* image was the transpose of the actual image. */
				//frames [4 * nframes + 0] = k -> y + 1 ;
				//frames [4 * nframes + 1] = k -> x + 1 ;
				//frames [4 * nframes + 2] = k -> sigma ;
				//frames [4 * nframes + 3] = VL_PI / 2 - angles [q] ;

				frames[nframes].x     = k->x;
				frames[nframes].y     = k->y ;
				frames[nframes].sigma = k->sigma ;
				frames[nframes].angle = angles[q];

				for (int j = 0; j < 128; ++j)
				{
					descr[128 * nframes + j] = convertDescriptor<DescrType>(rbuf[j]);
				}

				++nframes;
			} /* next orientation */
		} /* next keypoint */
	} // next octave

	return nframes;
}

void Sift::print_info()
{
	printf ("sift: filter settings:\n") ;
	printf ("sift:   octaves      (O)     = %d\n",
		vl_sift_get_noctaves     (mSiftFlt)) ;
	printf ("sift:   levels       (S)     = %d\n",
		vl_sift_get_nlevels      (mSiftFlt)) ;
	printf ("sift:   first octave (o_min) = %d\n",
		vl_sift_get_octave_first (mSiftFlt)) ;
	printf ("sift:   edge thresh           = %g\n",
		vl_sift_get_edge_thresh  (mSiftFlt)) ;
	printf ("sift:   peak thresh           = %g\n",
		vl_sift_get_peak_thresh  (mSiftFlt)) ;
	printf ("sift:   magnif                = %g\n",
		vl_sift_get_magnif       (mSiftFlt)) ;
}



int Sift::computeOctave(bool first)
{
	if (first)
		return vl_sift_process_first_octave(mSiftFlt, mData);
	else
		return vl_sift_process_next_octave(mSiftFlt);
}


//////////////////////////////////////////////////////////////////////////

VL_INLINE void
	transpose_descriptor (vl_sift_pix* dst, vl_sift_pix* src)
{
	int const BO = 8 ;  /* number of orientation bins */
	int const BP = 4 ;  /* number of spatial bins     */
	int i, j, t ;

	for (j = 0 ; j < BP ; ++j) {
		int jp = BP - 1 - j ;
		for (i = 0 ; i < BP ; ++i) {
			int o  = BO * i + BP*BO * j  ;
			int op = BO * i + BP*BO * jp ;
			dst [op] = src[o] ;
			for (t = 1 ; t < BO ; ++t)
				dst [BO - t + op] = src [t + o] ;
		}
	}
}


