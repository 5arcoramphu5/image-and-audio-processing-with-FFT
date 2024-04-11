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
    ComplexColor(ComplexColor &other);

    ComplexColor operator+(const ComplexColor &other) const;
    ComplexColor operator*(const ComplexColor &other) const;
    ComplexColor operator*(const COMPLEX_DOUBLE x) const;
    ComplexColor operator/(const double x) const;

    ComplexColor& operator+=(const ComplexColor &other);
};

#endif // COMPLEX_COLOR_H
