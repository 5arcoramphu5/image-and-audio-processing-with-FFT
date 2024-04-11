#ifndef COMPLEX_COLOR_H
#define COMPLEX_COLOR_H

#include<complex>

typedef std::complex<double> COMPLEX_DOUBLE;

struct ComplexColor
{
    COMPLEX_DOUBLE red;
    COMPLEX_DOUBLE green;
    COMPLEX_DOUBLE blue;

    ComplexColor();
    ComplexColor(COMPLEX_DOUBLE r, COMPLEX_DOUBLE g, COMPLEX_DOUBLE b);

    ComplexColor operator+(const ComplexColor &other);
    ComplexColor operator*(const ComplexColor &other);
    ComplexColor operator*(const COMPLEX_DOUBLE x);
};

#endif // COMPLEX_COLOR_H
