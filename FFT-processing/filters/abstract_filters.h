#ifndef ABSTRACT_FILTER_H
#define ABSTRACT_FILTER_H

#include "../fourier/complex_image.h"

#include <QLayout>
#include <QSlider>
#include <QWidget>

class Filter
{
public:
    Filter(QString _displayName);

    QString displayName;

    virtual void insertParametersUI(QLayout &layout) = 0;
};


// BY CONTENT TYPE

class ImageFilter : public Filter
{
public:
    ImageFilter(QString _displayName);

    virtual void performFiltering(ComplexImage &dft) = 0;
};

class AudioFilter : public Filter
{
public:
    AudioFilter(QString _displayName);

    // virtual void performFiltering() = 0;
};

// BY PARAMETERS TYPE

class OneParameterImageFilter : public ImageFilter
{

public:
    OneParameterImageFilter(int _initialValue, QString _displayName);
    double getParameterValue() const;

    void insertParametersUI(QLayout &layout);

private:
    int initialValue;
    QSlider* slider;
};

class OneParameterAudioFilter : public AudioFilter
{
public:
    OneParameterAudioFilter(int _initialValue, QString _displayName);
    double getParameterValue() const;

    void insertParametersUI(QLayout &layout);

private:
    int initialValue;
    QSlider* slider;
};

#endif // ABSTRACT_FILTER_H
