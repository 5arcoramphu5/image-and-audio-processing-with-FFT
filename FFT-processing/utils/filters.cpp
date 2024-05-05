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
        names << displayName(filter->type);
    }
    return names;
}

QString Filters::displayName(FilterType type) const
{
    switch(type)
    {
        case Blur:
            return "Blur";
        case EdgeDetection:
            return "Edge Detection";
        default:
            return "";
    }
}

void Filters::insertParametersUI(int filterIndex, QLayout &layout) const
{
    if(filterIndex < 0 || filterIndex >= N)
        return;

    imageFilters[filterIndex]->insertParametersUI(layout);
}
