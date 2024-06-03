#ifndef ABSTRACT_FILTER_H
#define ABSTRACT_FILTER_H

#include "../fourier/complex_image.h"

#include <QBoxLayout>
#include <QSlider>
#include <QWidget>

class Filter
{
public:
    Filter(QString _displayName);

    QString displayName;

    virtual void insertParametersUI(QBoxLayout &layout) = 0;
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

    virtual void performFiltering(COMPLEX_DOUBLE** DFTs, COMPLEX_DOUBLE** filteredDFTs, int numberOfDFTs, int FFTsize) = 0;
};

// BY PARAMETERS TYPE

class OneParameterImageFilter : public ImageFilter
{

public:
    OneParameterImageFilter(int _initialValue, QString _displayName);
    ~OneParameterImageFilter();

    double getParameterValue() const;

    void insertParametersUI(QBoxLayout &layout);

private:
    int initialValue;
    QSlider* slider;
};

class OneParameterAudioFilter : public AudioFilter
{
public:
    OneParameterAudioFilter(int _initialValue, QString _displayName);
    ~OneParameterAudioFilter();

    double getParameterValue() const;

    void insertParametersUI(QBoxLayout &layout);

private:
    int initialValue;
    QSlider* slider;
};

class MultipleHorizontalSlidersAudioFilter: public AudioFilter
{
public:
    MultipleHorizontalSlidersAudioFilter(const int N, const int initialValue, const QString _displayName);
    ~MultipleHorizontalSlidersAudioFilter();

    double getValueAtIndex(const int i) const;

    void insertParametersUI(QBoxLayout &layout);

protected:
    const int N;

private:
    const int initialValue;
    QSlider** sliders;
};

#endif // ABSTRACT_FILTER_H
