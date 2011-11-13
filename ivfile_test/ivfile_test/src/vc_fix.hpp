#ifndef VC_FIX_HPP__ 
#define VC_FIX_HPP__

#ifndef log2
double log2( double n )  
{  
	// log(n)/log(2) is log2.  
	return log( n ) / log( 2.0 );  
}
#endif

#ifndef round
double round(double d)
{
	return floor(d + 0.5);
}
#endif

#endif // VC_FIX_HPP__