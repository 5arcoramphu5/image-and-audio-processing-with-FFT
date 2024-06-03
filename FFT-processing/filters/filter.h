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

    void performFiltering(COMPLEX_DOUBLE** DFTs, COMPLEX_DOUBLE** filteredDFTs, int numberOfDFTs, int FFTsize);
};

#endif // FILTER_H
