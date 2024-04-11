#include "complex_image.h"
#include "iostream"

ComplexImage::ComplexImage(QSize &_size)
    : size(_size),
      redChannel(size.width(), std::vector<std::complex<double>>(size.height(), 0)),
      greenChannel(size.width(), std::vector<std::complex<double>>(size.height(), 0)),
      blueChannel(size.width(), std::vector<std::complex<double>>(size.height(), 0))
{
    for(int i = 0; i < _size.height(); ++i)
    {
        redChannel[i] = std::vector<std::complex<double>>(_size.width(), 0);
        greenChannel[i] = std::vector<std::complex<double>>(_size.width(), 0);
        blueChannel[i] = std::vector<std::complex<double>>(_size.width(), 0);
    }
}

ComplexImage::ComplexImage(QImage &image)
    : size(image.size()),
      redChannel(size.width(), std::vector<std::complex<double>>(size.height(), 0)),
      greenChannel(size.width(), std::vector<std::complex<double>>(size.height(), 0)),
      blueChannel(size.width(), std::vector<std::complex<double>>(size.height(), 0))
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
    int intX = round(x);
    if(intX > 255)
        return 255;
    if(intX < 0)
        return 0;

    return intX;
}

QImage ComplexImage::toImageFromAbs()
{
    QImage image(size, QImage::Format_RGB32);

    for(int i = 0; i < size.width(); ++i)
        for(int j = 0; j < size.height(); ++j)
        {
            int r = toPixelValue(abs(redChannel[i][j]));
            int g = toPixelValue(abs(greenChannel[i][j]));
            int b = toPixelValue(abs(blueChannel[i][j]));
            image.setPixelColor(i, j, QColor(r, g, b));
        }

    return image;
}

QImage ComplexImage::toImageFromReal()
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


bool ComplexImage::isInsideImage(int i, int j)
{
    return i >= 0 && j >= 0 && i < size.width() && j < size.height();
}

ComplexColor ComplexImage::getColor(int i, int j)
{
    if(!isInsideImage(i, j))
    {
        std::cout<< "indexes "<<i<<", "<<j<< " are outside of image bounds"<<std::endl;
        return ComplexColor();
    }

    return ComplexColor(redChannel[i][j], greenChannel[i][j], blueChannel[i][j]);
}

void ComplexImage::setColor(int i, int j, ComplexColor color)
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

