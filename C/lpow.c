#include "lpow.h"
/* lpow calculates integer powers `n` of `x` (in a fast way) */
double /* `x^n`*/
lpow(double x,/* base */
     long n)/* exponent */{
  double y=1.0;
  size_t m=n>0?n:-n;
  while (m){
    if (m & 1) y*=x;
    m>>=1;
    x*=x;
  }
  return n>0?y:1.0/y;
}

/* same as `lpow()`, but with `short`*/
double /* `x^n`*/
spow(double x,/* base */
     short n)/* exponent */{
  double y=1.0;
  ushort m=n>0?n:-n;
  while (m){
    if (m & 1) y*=x;
    m>>=1;
    x*=x;
  }
  return n>0?y:1.0/y;
}

/* same as `lpow()` but with `int`*/
double /* `x^n`*/
ipow(double x,/* base */
     int n)/* exponent */{
  double y=1.0;
  uint m=n>0?n:-n;
  while (m){
    if (m & 1) y*=x;
    m>>=1;
    x*=x;
  }
  return n>0?y:1.0/y;
}
