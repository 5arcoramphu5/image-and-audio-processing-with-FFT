#ifndef COMPLEX_IMAGE_H
#define COMPLEX_IMAGE_H

#include "complex_color.h"

#include <QSize>
#include <QImage>
#include <complex>

typedef std::vector<std::vector<COMPLEX_DOUBLE>> COMPLEX_MATRIX;

class ComplexImage
{
    public:
        QSize size;
        COMPLEX_MATRIX redChannel;
        COMPLEX_MATRIX greenChannel;
        COMPLEX_MATRIX blueChannel;

        ComplexImage(QSize &size);
        ComplexImage(QImage &image);

        QImage toImageFromAbs();
        QImage toImageFromReal();

        void setColor(int i, int j, ComplexColor color);
        ComplexColor getColor(int i, int j);

    private:
        bool isInsideImage(int i, int j);
};
#endif // COMPLEX_IMAGE_H
