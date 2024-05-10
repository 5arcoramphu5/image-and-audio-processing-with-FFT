#include "complex_image.h"

#include <QDebug>

typedef std::vector<COMPLEX_DOUBLE> COMPLEX_VECTOR;

ComplexImage::ComplexImage()
    : size(0, 0), redChannel(0, COMPLEX_VECTOR(0, 0)), greenChannel(0, COMPLEX_VECTOR(0, 0)), blueChannel(0, COMPLEX_VECTOR(0, 0))
{}

ComplexImage::ComplexImage(const QSize &_size)
    : size(_size),
      redChannel(size.width(), COMPLEX_VECTOR(size.height(), 0)),
      greenChannel(size.width(), COMPLEX_VECTOR(size.height(), 0)),
      blueChannel(size.width(), COMPLEX_VECTOR(size.height(), 0))
{
    for(int i = 0; i < _size.height(); ++i)
    {
        redChannel[i] = COMPLEX_VECTOR(_size.width(), 0);
        greenChannel[i] = COMPLEX_VECTOR(_size.width(), 0);
        blueChannel[i] = COMPLEX_VECTOR(_size.width(), 0);
    }
}

ComplexImage::ComplexImage(const QImage &image)
    : size(image.size()),
      redChannel(size.width(), COMPLEX_VECTOR(size.height(), 0)),
      greenChannel(size.width(), COMPLEX_VECTOR(size.height(), 0)),
      blueChannel(size.width(), COMPLEX_VECTOR(size.height(), 0))
{
    for(int i = 0; i < size.width(); ++i)
    {
        for(int j = 0; j < size.height(); ++j)
        {
            QColor color = image.pixelColor(i, j);
            redChannel[i][j] = color.red();
            greenChannel[i][j] = color.green();
            blueChannel[i][j] = color.blue();
        }
    }
}

int toPixelValue(double x)
{
    int pixelValue = round(x);

    if(pixelValue > 255)
        return 255;
    if(pixelValue < 0)
        return 0;

    return pixelValue;
}

double maxAbsValuesInMatrixExcludingCenter(const COMPLEX_MATRIX matrix, const QSize &size)
{
    int middleX = size.height()/2;
    int middleY = size.width()/2;
    double radius = (double)std::max(size.height(), size.width()) / 4;

    double max = std::abs(matrix[0][0]);
    for(int i = 0; i < size.width(); ++i)
        for(int j = 0; j < size.height(); ++j)
        {
            if( (i-middleX)*(i - middleY) + (j-middleY)*(j-middleY) < radius)
                continue;

            double dValue = std::abs(matrix[i][j]);
            if(dValue > max)
                max = dValue;
        }
    return max;
}

void ComplexImage::maxAbsValuesExcludingCenter(double &maxRed, double &maxGreen, double &maxBlue)
{
    maxRed = maxAbsValuesInMatrixExcludingCenter(redChannel, size);
    maxGreen = maxAbsValuesInMatrixExcludingCenter(greenChannel, size);
    maxBlue = maxAbsValuesInMatrixExcludingCenter(blueChannel, size);
}

double normalize(const double x, const double max)
{
    // to interval [0, 255], (max value -> 255)
    return x * 255 / max;
}

QImage ComplexImage::toImage(complexToDouble convert, bool normalized, double maxRed, double maxGreen, double maxBlue) const
{
    QImage image(size, QImage::Format_RGB32);
    for(int i = 0; i < size.width(); ++i)
        for(int j = 0; j < size.height(); ++j)
        {
            double rD = convert(redChannel[i][j]);
            double gD = convert(greenChannel[i][j]);
            double bD = convert(blueChannel[i][j]);

            if(normalized)
            {
                rD = normalize(rD, maxRed);
                gD = normalize(gD, maxGreen);
                bD = normalize(bD, maxBlue);
            }

            int r = toPixelValue(rD);
            int g = toPixelValue(gD);
            int b = toPixelValue(bD);
            image.setPixelColor(i, j, QColor(r, g, b));
        }

    return image;
}

QImage ComplexImage::toImageFromAbs(bool normalized, double maxRed, double maxGreen, double maxBlue) const
{
    return toImage([](COMPLEX_DOUBLE x){ return std::abs(x); }, normalized, maxRed, maxGreen, maxBlue);
}

QImage ComplexImage::toImageFromReal(bool normalized, double maxRed, double maxGreen, double maxBlue) const
{
    return toImage([](COMPLEX_DOUBLE x){ return std::real(x); }, normalized, maxRed, maxGreen, maxBlue);
}

bool ComplexImage::isInsideImage(int i, int j) const
{
    return i >= 0 && j >= 0 && i < size.width() && j < size.height();
}

ComplexColor ComplexImage::getColor(int i, int j) const
{
    if(!isInsideImage(i, j))
    {
        qDebug()<< "indexes "<<i<<", "<<j<< " are outside of image bounds";
        return ComplexColor();
    }

    return ComplexColor(redChannel[i][j], greenChannel[i][j], blueChannel[i][j]);
}

void ComplexImage::setColor(int i, int j, const ComplexColor &color)
{
    if(!isInsideImage(i, j))
    {
        qDebug()<< "indexes "<<i<<", "<<j<< " are outside of image bounds";
        return;
    }

    redChannel[i][j] = color.red;
    greenChannel[i][j] = color.green;
    blueChannel[i][j] = color.blue;
}

