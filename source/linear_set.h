#ifndef LINEAR_SET_H
#define LINEAR_SET_H


#include <complex.h>

#ifndef LINEAR_SET_VALUE_TYPE
#define LINEAR_SET_VALUE_TYPE double
#endif /* ifndef VALUE_TYPE */

typedef LINEAR_SET_VALUE_TYPE complex Complex;


Complex* generateLinearSet(int width, int height, LINEAR_SET_VALUE_TYPE magnification, const Complex* center);
void printLinearSet(int width, int height, Complex* set);
void deleteLinearSet(Complex* set);


#endif /* ifndef LINEAR_SET_H */
