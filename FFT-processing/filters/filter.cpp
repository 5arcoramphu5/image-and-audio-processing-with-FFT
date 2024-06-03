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

EqualizerFilter::EqualizerFilter(const int N): MultipleHorizontalSlidersAudioFilter(N, 50, "Equalizer")
{}

void EqualizerFilter::performFiltering(COMPLEX_DOUBLE** DFTs, COMPLEX_DOUBLE** filteredDFTs, int numberOfDFTs, int FFTsize)
{
    // wartości sliderów odpowiadają częstotliwościa w lewej połowie transformaty (prawa symetrycznie zmodyfikowana)
    int step = FFTsize / 2 / N; // ilość częstotliwości przypisanych do jednego slidera po jednej stronie

    double* values = new double[N];
    for(int i = 0; i < N; ++i)
    {
        values[i] = getValueAtIndex(i);
        qDebug() << values[i];
    }

    for(int k = 0; k < numberOfDFTs; ++k)
    {
        for(int i = 0; i < FFTsize; ++i)
            filteredDFTs[k][i] = DFTs[k][i];

        for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < step; ++j)
            {
                int pos = i*step + j;
                filteredDFTs[k][pos] *= values[i];
                filteredDFTs[k][FFTsize-pos-1] *= values[i];
            }
        }
    }

    delete[] values;
}

