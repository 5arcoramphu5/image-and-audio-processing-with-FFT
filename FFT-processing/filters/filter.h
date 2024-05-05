#ifndef FILTER_H
#define FILTER_H

#include "abstract_filters.h"


class BlurFilter : public OneParameterImageFilter
{
public:
    BlurFilter();
    void performFiltering(ComplexImage &dft);
};

class SharpenFilter : public OneParameterImageFilter
{
public:
    SharpenFilter();
    void performFiltering(ComplexImage &dft);

};

class EdgeDetectionFilter : public OneParameterImageFilter
{
public:
    EdgeDetectionFilter();
    void performFiltering(ComplexImage &dft);
};

#endif // FILTER_H
