#include "filter.h"

#include <QDebug>


BlurFilter::BlurFilter() : OneParameterImageFilter(50, "Blur")
{}

void BlurFilter::performFiltering(ComplexImage &dft)
{
    double param = getParameterValue();

    // deleting highest frequencies (in the middle)
    int radiusX = round(dft.size.height() / 2 * (1 - param));
    int radiusY = round(dft.size.width() / 2 * (1 - param));
    int middleX = round(dft.size.height() / 2);
    int middleY = round(dft.size.width() / 2);
    ComplexColor zero(0, 0, 0);

    for(int i = 0; i < dft.size.height(); ++i)
        for(int j = 0; j < dft.size.width(); ++j)
        {
            double x = (double)(i - middleX) / radiusX;
            double y = (double)(j - middleY) / radiusY;
            if( x*x + y*y > 1)
                dft.setColor(i, j, zero);
        }
}

EdgeDetectionFilter::EdgeDetectionFilter() : OneParameterImageFilter(50, "Edge Detection")
{}

void EdgeDetectionFilter::performFiltering(ComplexImage &dft)
{
    double param = getParameterValue();

    // deleting highest frequencies (in the middle)
    int radiusX = round(dft.size.height() / 2 * param);
    int radiusY = round(dft.size.width() / 2 * param);
    int middleX = round(dft.size.height() / 2);
    int middleY = round(dft.size.width() / 2);
    ComplexColor zero(0, 0, 0);

    for(int i = 0; i < dft.size.height(); ++i)
        for(int j = 0; j < dft.size.width(); ++j)
        {
            double x = (double)(i - middleX) / radiusX;
            double y = (double)(j - middleY) / radiusY;
            if( x*x + y*y < 1)
                dft.setColor(i, j, zero);
        }
}

