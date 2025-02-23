#ifndef FILTERS_H
#define FILTERS_H

#include "filter.h"
#include <QStringList>
#include <QString>
#include <QWidget>
#include <QBoxLayout>

#include "../utils/mode.h"

class Filters
{
    public:
        Filters();
        ~Filters();

        QStringList filterNamesList() const;

        void insertParametersUI(QBoxLayout &layout) const;

        void performImageFiltering(ComplexImage &dft) const;
        void performAudioFiltering(COMPLEX_DOUBLE** DFTs, COMPLEX_DOUBLE** filteredDFTs, int numberOfDFTs, int FFTsize) const;

        void setIndex(int index);

        void setMode(Mode mode);

    private:

        static const int IMAGE_N = 2;
        ImageFilter* imageFilters[IMAGE_N] = {
            new BlurFilter(),
            new EdgeDetectionFilter()
        };

        static const int AUDIO_N = 1;
        AudioFilter* audioFilters[AUDIO_N] = {
            new EqualizerFilter(16)
        };

        bool indexValid(int index);

        int currentIndex;
        Mode mode;
};

#endif // FILTERS_H
