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

        void insertParametersUI(QLayout &layout) const;

        void performFiltering(ComplexImage &dft) const;

        void setIndex(int index);

    private:

        static const int N = 3;
        ImageFilter* imageFilters[N] = {
            new BlurFilter(),
            new SharpenFilter(),
            new EdgeDetectionFilter()
        };

        bool indexValid(int index);
        int currentIndex;
};

#endif // FILTERS_H
