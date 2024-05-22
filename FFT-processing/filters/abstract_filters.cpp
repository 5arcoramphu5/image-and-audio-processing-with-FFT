#include "abstract_filters.h"

#include "../utils/widgets_utils.h"

Filter::Filter(QString _displayName) : displayName(_displayName)
{}

ImageFilter::ImageFilter(QString _displayName) : Filter(_displayName)
{}

AudioFilter::AudioFilter(QString _displayName) : Filter(_displayName)
{}

OneParameterImageFilter::OneParameterImageFilter(int _initialValue, QString _displayName) : ImageFilter(_displayName), initialValue(_initialValue)
{}

double OneParameterImageFilter::getParameterValue() const
{
    int sliderValue = slider->value();
    return (double)sliderValue / 99;
}

void OneParameterImageFilter::insertParametersUI(QLayout &layout)
{
    slider = createSlider(initialValue);
    auto sliderLabel = createLabel("scale:");

    layout.addWidget(sliderLabel);
    layout.addWidget(slider);
}

OneParameterAudioFilter::OneParameterAudioFilter(int _initialValue, QString _displayName) : AudioFilter(_displayName), initialValue(_initialValue)
{}

double OneParameterAudioFilter::getParameterValue() const
{
    int sliderValue = slider->value();
    return (double)sliderValue / 99;
}

void OneParameterAudioFilter::insertParametersUI(QLayout &layout)
{
    slider = createSlider(initialValue);
    auto sliderLabel = createLabel("scale:");

    layout.addWidget(sliderLabel);
    layout.addWidget(slider);
}
