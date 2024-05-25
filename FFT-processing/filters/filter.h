#ifndef FILTER_H
#define FILTER_H

#include "abstract_filters.h"

// IMAGE FILTERS
class BlurFilter : public OneParameterImageFilter
{
public:
    BlurFilter();
    void performFiltering(ComplexImage &dft);
};

class EdgeDetectionFilter : public OneParameterImageFilter
{
public:
    EdgeDetectionFilter();
    void performFiltering(ComplexImage &dft);
};

// AUDIO FILTERS

class EqualizerFilter: public MultipleHorizontalSlidersAudioFilter
{
public:
    EqualizerFilter(const int N);

};

#endif // FILTER_H
