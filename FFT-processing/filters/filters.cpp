#include "filters.h"

Filters::Filters()
{ }

Filters::~Filters()
{
    for(int i = 0; i < N; ++i)
        delete imageFilters[i];
}


QStringList Filters::imageFilterNamesList() const
{
    QStringList names;
    for (Filter *filter : imageFilters) {
        names << filter->displayName;
    }
    return names;
}

bool Filters::indexValid(int index)
{
    return index >= 0 && index < N;
}

void Filters::setIndex(int index)
{
    if(indexValid(index))
        currentIndex = index;
}

void Filters::insertParametersUI(QLayout &layout) const
{
    imageFilters[currentIndex]->insertParametersUI(layout);
}

void Filters::performFiltering(ComplexImage &dft) const
{
    imageFilters[currentIndex]->performFiltering(dft);
}
