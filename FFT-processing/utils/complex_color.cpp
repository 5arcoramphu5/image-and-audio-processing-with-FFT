#include "complex_color.h"

ComplexColor::ComplexColor() : red(0), green(0), blue(0)
{
}

ComplexColor::ComplexColor(COMPLEX_DOUBLE r, COMPLEX_DOUBLE g, COMPLEX_DOUBLE b) : red(r), green(g), blue(b)
{
}

ComplexColor ComplexColor::operator+(const ComplexColor &other)
{
    return ComplexColor(red + other.red, green + other.green, blue + other.blue);
}

ComplexColor ComplexColor::operator*(const ComplexColor &other)
{
    return ComplexColor(red * other.red, green * other.green, blue * other.blue);
}

ComplexColor ComplexColor::operator*(const COMPLEX_DOUBLE x)
{
    return ComplexColor(red * x, green * x, blue * x);
}
