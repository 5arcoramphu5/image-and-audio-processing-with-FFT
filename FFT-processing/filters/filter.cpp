#include "filter.h"

#include <QDebug>


BlurFilter::BlurFilter() : OneParameterImageFilter(50, "Blur")
{}

void BlurFilter::performFiltering(ComplexImage &dft)
{
    double param = getParameterValue();

    // deleting highest frequencies (in the middle)
    int radiusX = round(dft.size.height() / 2 * param);
    int radiusY = round(dft.size.width() / 2 * param);
    int middleX = round(dft.size.height() / 2);
    int middleY = round(dft.size.width() / 2);
    ComplexColor zero(0, 0, 0);

    for(int i = 0; i < radiusX; ++i)
        for(int j = 0; j < dft.size.width(); ++j)
        {
            dft.setColor(middleX + i, j, zero);
            dft.setColor(middleX - i, j, zero);
        }

    for(int i = 0; i < dft.size.height(); ++i)
        for(int j = 0; j < radiusY; ++j)
        {
            dft.setColor(i, middleY + j, zero);
            dft.setColor(i, middleY - j, zero);
        }
}

SharpenFilter::SharpenFilter() : OneParameterImageFilter(50, "Sharpen")
{}

void SharpenFilter::performFiltering(ComplexImage &dft)
{

}

EdgeDetectionFilter::EdgeDetectionFilter() : OneParameterImageFilter(50, "Edge Detection")
{}

void EdgeDetectionFilter::performFiltering(ComplexImage &dft)
{

}

