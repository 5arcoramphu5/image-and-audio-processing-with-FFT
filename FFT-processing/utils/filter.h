#ifndef FILTER_H
#define FILTER_H

#include <QLayout>
#include <QSlider>
#include <QWidget>

enum FilterType { Blur, EdgeDetection };

class Filter
{
public:
    Filter(FilterType _type);

    FilterType type;

    virtual void insertParametersUI(QLayout &layout);
};

class OneParameterFilter : public Filter
{

public:
    OneParameterFilter(FilterType _type, int _initialValue);
    double getValue() const;

    void insertParametersUI(QLayout &layout);

private:
    int initialValue;
    QSlider* slider;
};

#endif // FILTER_H
