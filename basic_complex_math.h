#ifndef __COMPLEX_h__
#define __COMPLEX_h__

#include <math.h>

#define DATA_TYPE_FLOAT 
// #define DATA_TYPE_DOUBLE

#if defined(DATA_TYPE_FLOAT)
#define data_type float
#define internal_sqrt sqrtf
#define internal_atan2 atan2f
#elif defined(DATA_TYPE_DOUBLE)
#define data_type double
#define internal_sqrt sqrt
#define internal_atan2 atan2
#endif

typedef struct {
    data_type	re;
    data_type	im;
}complex;

inline complex MakeComplex(data_type inReal, data_type inImage = 0) { complex retVal; retVal.re = inReal; retVal.im = inImage; return retVal; }
inline data_type real(const complex c) { return c.re; }
inline data_type imag(const complex c) { return c.im; }
inline data_type complex_abs(complex c) { return internal_sqrt(c.re * c.re + c.im * c.im); }
inline data_type norm(complex c) { return (c.re * c.re + c.im * c.im); }
inline data_type arg(complex c) { return internal_atan2(c.im, c.re); }
inline complex complex_conj(complex c) { return MakeComplex(c.re, -c.im); }
inline complex polar2complex(data_type m, data_type a) { return MakeComplex(m * cos(a), m * sin(a)); }
inline complex complex_sqrt(complex c) { return polar2complex(internal_sqrt(complex_abs(c)), arg(c) * 0.5); }
inline complex complex_sqr(complex c) { return MakeComplex(c.re * c.re - c.im * c.im, 2.0 * c.re * c.im); }
inline complex operator+(complex c1, complex c2) { return MakeComplex(c1.re + c2.re, c1.im + c2.im); }
inline complex operator+(data_type d1, complex c2) { return MakeComplex(d1) + c2; }
inline complex operator+(complex c1, data_type d2) { return c1 + MakeComplex(d2); }
inline complex operator-(complex c1, complex c2) { return MakeComplex(c1.re - c2.re, c1.im - c2.im); }
inline complex operator-(complex c) { return MakeComplex(-c.re, -c.im); }
inline complex operator-(data_type d1, complex c2) { return MakeComplex(d1) - c2; }
inline complex operator-(complex c1, data_type d2) { return c1 - MakeComplex(d2); }
inline complex operator*(complex c1, complex c2) { return MakeComplex(c1.re * c2.re - c1.im * c2.im, c1.re * c2.im + c1.im * c2.re); }
inline complex operator*(complex c, data_type d) { return MakeComplex(c.re * d, c.im * d); }
inline complex operator*(data_type d, complex c) { return MakeComplex(c.re * d, c.im * d); }
inline complex operator/(complex c1, complex c2) {
    data_type nc2 = 1.0 / (c2.re * c2.re + c2.im * c2.im);
    return MakeComplex(nc2 * (c1.re * c2.re + c1.im * c2.im), nc2 * (c1.im * c2.re - c1.re * c2.im));
}

inline complex operator/(complex c, data_type d) { return MakeComplex(c.re / d, c.im / d); }
inline complex operator /(data_type d, complex c) {
    data_type nc = d / (c.re * c.re + c.im * c.im);
    return MakeComplex(nc * c.re, -nc * c.im);
}

inline bool	   operator ==(complex c1, complex c2) { return ((c1.im == c2.im) && (c1.re == c2.re)); }
inline bool	   operator !=(complex c1, complex c2) { return ((c1.im != c2.im) || (c1.re != c2.re)); }

#endif  // __COMPLEX_h__
