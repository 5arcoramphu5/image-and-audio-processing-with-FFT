#include "complex_color.h"

ComplexColor::ComplexColor() : red(0), green(0), blue(0)
{}

ComplexColor::ComplexColor(COMPLEX_DOUBLE r, COMPLEX_DOUBLE g, COMPLEX_DOUBLE b) : red(r), green(g), blue(b)
{}

ComplexColor::ComplexColor(ComplexColor &other) : red(other.red), green(other.green), blue(other.blue)
{}

ComplexColor ComplexColor::operator+(const ComplexColor &other) const
{
    return ComplexColor(red + other.red, green + other.green, blue + other.blue);
}

ComplexColor ComplexColor::operator*(const ComplexColor &other) const
{
    return ComplexColor(red * other.red, green * other.green, blue * other.blue);
}

ComplexColor ComplexColor::operator*(const COMPLEX_DOUBLE x) const
{
    return ComplexColor(red * x, green * x, blue * x);
}

ComplexColor ComplexColor::operator/(const double x) const
{
    return ComplexColor(red / x, green / x, blue / x);
}

ComplexColor& ComplexColor::operator+=(const ComplexColor &other)
{
    red += other.red;
    green += other.green;
    blue += other.blue;
    return *this;
}

