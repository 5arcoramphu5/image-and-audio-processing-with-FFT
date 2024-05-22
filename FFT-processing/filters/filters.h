#ifndef FILTERS_H
#define FILTERS_H

#include "filter.h"
#include <QStringList>
#include <QString>
#include <QWidget>
#include <QLayout>

#include "../utils/mode.h"

class Filters
{
    public:
        Filters();
        ~Filters();

        QStringList filterNamesList() const;

        void insertParametersUI(QLayout &layout) const;

        void performImageFiltering(ComplexImage &dft) const;
        void performAudioFiltering() const;

        void setIndex(int index);

        void setMode(Mode mode);

    private:

        static const int IMAGE_N = 2;
        ImageFilter* imageFilters[IMAGE_N] = {
            new BlurFilter(),
            new EdgeDetectionFilter()
        };

        static const int AUDIO_N = 2;
        AudioFilter* audioFilters[AUDIO_N] = {
            new OneParameterAudioFilter(0, "Audio filter 1"),
            new OneParameterAudioFilter(50, "Audio filter 2")
        };

        bool indexValid(int index);

        int currentIndex;
        Mode mode;
};

#endif // FILTERS_H
