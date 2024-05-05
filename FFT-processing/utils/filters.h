#ifndef FILTERS_H
#define FILTERS_H

#include "filter.h"
#include <QStringList>
#include <QString>
#include <QWidget>
#include <QLayout>


class Filters
{
    public:
        Filters();
        ~Filters();

        QStringList imageFilterNamesList() const;

        void insertParametersUI(int filterIndex, QLayout &layout) const;

    private:

        static const int N = 2;
        Filter* imageFilters[N] = {
            new OneParameterFilter(FilterType::Blur, 50),
            new OneParameterFilter(FilterType::EdgeDetection, 30)
        };

        QString displayName(FilterType type) const;
};

#endif // FILTERS_H
