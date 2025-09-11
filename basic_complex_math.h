#ifndef __COMPLEX_h__
#define __COMPLEX_h__

#include <math.h>

#if 1
typedef struct {
    double	re;
    double	im;
}complex;

inline complex MakeComplex(double inReal, double inImage=0) {complex retVal; retVal.re = inReal; retVal.im = inImage; return retVal;}
inline double real(const complex c)	{return c.re;}
inline double imag(const complex c)	{return c.im;}
	
	// we cannot use plain "abs" because it clashes with the macro abs
inline double complex_abs(complex c)	{return sqrt(c.re*c.re + c.im*c.im);}
inline double norm(complex c)			{return (c.re*c.re + c.im*c.im);}
inline double arg(complex c)			{return atan2(c.im, c.re);}
inline complex complex_conj(complex c)			{return MakeComplex( c.re, -c.im);}

inline complex	polar2complex(double m, double a)	{return MakeComplex(m*cos(a), m*sin(a));}
inline complex	complex_sqrt(complex c)				{return polar2complex(sqrt(complex_abs(c)),arg(c)*0.5);}
	
inline complex	complex_sqr(complex c)				{return MakeComplex(c.re*c.re-c.im*c.im, 2.0*c.re*c.im);}

inline complex	operator+(complex c1, complex c2) { return MakeComplex(c1.re + c2.re, c1.im + c2.im);}
inline complex	operator+(double d1, complex c2) { return MakeComplex(d1) + c2;}
inline complex	operator+(complex c1, double d2) { return c1 + MakeComplex(d2);}


inline complex	operator-(complex c1, complex c2) { return MakeComplex(c1.re - c2.re, c1.im - c2.im);}
inline complex	operator-(complex c) { return MakeComplex(- c.re, -c.im);}
inline complex	operator-(double d1, complex c2) {return MakeComplex(d1) - c2;}
inline complex	operator-(complex c1, double d2) {return c1 - MakeComplex(d2);}

inline complex	operator*(complex c1, complex c2) {return MakeComplex(c1.re*c2.re-c1.im*c2.im, c1.re*c2.im+c1.im*c2.re);}
inline complex	operator*(complex c, double d) {return MakeComplex(c.re*d, c.im*d);}
inline complex	operator*(double d, complex c) {return MakeComplex(c.re*d, c.im*d);}

inline complex	operator/(complex c1, complex c2) {double nc2=1.0/(c2.re*c2.re + c2.im*c2.im);
										      return MakeComplex(nc2*(c1.re*c2.re+c1.im*c2.im), nc2*(c1.im*c2.re-c1.re*c2.im));}										      
inline complex	operator/(complex c, double d) {return MakeComplex(c.re/d, c.im/d);}
inline complex	operator /(double d, complex c) {double nc=d/(c.re*c.re + c.im*c.im);
                                           return MakeComplex(nc*c.re, -nc*c.im);}

inline bool		operator ==(complex c1, complex c2) {return ((c1.im==c2.im) && (c1.re==c2.re));}
inline bool		operator !=(complex c1, complex c2) {return ((c1.im!=c2.im) || (c1.re!=c2.re));}

#endif
#endif  // __COMPLEX_h__
