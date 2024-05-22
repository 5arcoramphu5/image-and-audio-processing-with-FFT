#include "filters.h"

#include <QDebug>

Filters::Filters()
{ }

Filters::~Filters()
{
    for(int i = 0; i < IMAGE_N; ++i)
        delete imageFilters[i];

    for(int i = 0; i < AUDIO_N; ++i)
        delete audioFilters[i];
}


QStringList Filters::filterNamesList() const
{
    QStringList names;
    if(mode == Mode::IMAGE)
    {
        for (Filter *filter : imageFilters)
            names << filter->displayName;
    } else
    {
        for (Filter *filter : audioFilters)
            names << filter->displayName;
    }

    return names;
}

bool Filters::indexValid(int index)
{
    return index >= 0 && index < (mode == Mode::IMAGE ? IMAGE_N : AUDIO_N);
}

void Filters::setIndex(int index)
{
    if(indexValid(index))
        currentIndex = index;
}

void Filters::setMode(Mode _mode)
{
    mode = _mode;
}

void Filters::insertParametersUI(QLayout &layout) const
{
    if(mode == Mode::IMAGE)
        imageFilters[currentIndex]->insertParametersUI(layout);
    else
        audioFilters[currentIndex]->insertParametersUI(layout);
}

void Filters::performImageFiltering(ComplexImage &dft) const
{
    if(mode != Mode::IMAGE)
    {
        qDebug() << "not in image mode";
        return;
    }

    imageFilters[currentIndex]->performFiltering(dft);
}


void Filters::performAudioFiltering() const
{
    qDebug() << "audio filtering...";
}
