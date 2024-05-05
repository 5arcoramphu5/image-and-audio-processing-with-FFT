#include "filter.h"

#include "widgets_utils.h"

#include <QDebug>

Filter::Filter(FilterType _type) : type(_type)
{ }

void Filter::insertParametersUI(QLayout &layout)
{}


OneParameterFilter::OneParameterFilter(FilterType _type, int _initialValue) : Filter(_type), initialValue(_initialValue)
{}

double OneParameterFilter::getValue() const
{
    int sliderValue = slider->value();
    return (double)sliderValue / 99;
}

void OneParameterFilter::insertParametersUI(QLayout &layout)
{
    slider = createSlider(initialValue);
    auto sliderLabel = createLabel("scale:");

    layout.addWidget(sliderLabel);
    layout.addWidget(slider);
}
