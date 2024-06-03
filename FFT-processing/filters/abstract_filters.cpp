#include "abstract_filters.h"

#include "../utils/widgets_utils.h"
#include <QHBoxLayout>
#include <QDebug>

Filter::Filter(QString _displayName) : displayName(_displayName)
{}

ImageFilter::ImageFilter(QString _displayName) : Filter(_displayName)
{}

AudioFilter::AudioFilter(QString _displayName) : Filter(_displayName)
{}

OneParameterImageFilter::OneParameterImageFilter(int _initialValue, QString _displayName) : ImageFilter(_displayName), initialValue(_initialValue)
{}

OneParameterImageFilter::~OneParameterImageFilter()
{
    delete slider;
}

double OneParameterImageFilter::getParameterValue() const
{
    int sliderValue = slider->value();
    return (double)sliderValue / 99;
}

void OneParameterImageFilter::insertParametersUI(QBoxLayout &layout)
{
    slider = createSlider(initialValue, Qt::Orientation::Horizontal);
    auto sliderLabel = createLabel("scale:");

    layout.addWidget(sliderLabel);
    layout.addWidget(slider);
}

OneParameterAudioFilter::OneParameterAudioFilter(int _initialValue, QString _displayName) : AudioFilter(_displayName), initialValue(_initialValue)
{}

OneParameterAudioFilter::~OneParameterAudioFilter()
{
    delete slider;
}

double OneParameterAudioFilter::getParameterValue() const
{
    int sliderValue = slider->value();
    return (double)sliderValue / 99;
}

void OneParameterAudioFilter::insertParametersUI(QBoxLayout &layout)
{
    slider = createSlider(initialValue, Qt::Orientation::Horizontal);
    auto sliderLabel = createLabel("scale:");

    layout.addWidget(sliderLabel);
    layout.addWidget(slider);
}

MultipleHorizontalSlidersAudioFilter::MultipleHorizontalSlidersAudioFilter(const int _N, const int _initialValue, const QString _displayName)
    : AudioFilter(_displayName), N(_N), initialValue(_initialValue)
{
    sliders = new QSlider*[N];
}

MultipleHorizontalSlidersAudioFilter::~MultipleHorizontalSlidersAudioFilter()
{
    delete[] sliders;
}

double MultipleHorizontalSlidersAudioFilter::getValueAtIndex(const int i) const
{
    int sliderValue = sliders[i]->value();
    return (double)sliderValue / 50; // values from 0 to 2
}

void MultipleHorizontalSlidersAudioFilter::insertParametersUI(QBoxLayout &layout)
{
    auto hLayout = new QHBoxLayout();
    layout.addLayout(hLayout);

    for (int i = 0; i < N; ++i)
    {
        sliders[i] = createSlider(initialValue, Qt::Orientation::Vertical);
        hLayout->addWidget(sliders[i]);
    }
}
