#include "complex_image.h"
#include "iostream"

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

int toPixelValue(COMPLEX_DOUBLE x)
{
    double doubleValue = abs(x);
    // to interval [0, 255]:
    // doubleValue = atan(doubleValue) * 2 /  M_PI;
    // doubleValue *= 255;

    int pixelValue = round(doubleValue);

    if(pixelValue > 255)
        return 255;
    if(pixelValue < 0)
        return 0;

    return pixelValue;
}

QImage ComplexImage::toImageFromAbs() const
{
    QImage image(size, QImage::Format_RGB32);

    for(int i = 0; i < size.width(); ++i)
        for(int j = 0; j < size.height(); ++j)
        {
            int r = toPixelValue(redChannel[i][j]);
            int g = toPixelValue(greenChannel[i][j]);
            int b = toPixelValue(blueChannel[i][j]);
            image.setPixelColor(i, j, QColor(r, g, b));
        }

    return image;
}

QImage ComplexImage::toImageFromReal() const
{
    QImage image(size, QImage::Format_RGB32);

    for(int i = 0; i < size.width(); ++i)
        for(int j = 0; j < size.height(); ++j)
        {
            int r = toPixelValue(real(redChannel[i][j]));
            int g = toPixelValue(real(greenChannel[i][j]));
            int b = toPixelValue(real(blueChannel[i][j]));
            image.setPixelColor(i, j, QColor(r, g, b));
        }

    return image;
}


bool ComplexImage::isInsideImage(int i, int j) const
{
    return i >= 0 && j >= 0 && i < size.width() && j < size.height();
}

ComplexColor ComplexImage::getColor(int i, int j) const
{
    if(!isInsideImage(i, j))
    {
        std::cout<< "indexes "<<i<<", "<<j<< " are outside of image bounds"<<std::endl;
        return ComplexColor();
    }

    return ComplexColor(redChannel[i][j], greenChannel[i][j], blueChannel[i][j]);
}

void ComplexImage::setColor(int i, int j, const ComplexColor &color)
{
    if(!isInsideImage(i, j))
    {
        std::cout<< "indexes "<<i<<", "<<j<< " are outside of image bounds"<<std::endl;
        return;
    }

    redChannel[i][j] = color.red;
    greenChannel[i][j] = color.green;
    blueChannel[i][j] = color.blue;
}

