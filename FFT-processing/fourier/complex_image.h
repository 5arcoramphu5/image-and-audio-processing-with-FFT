#ifndef COMPLEX_IMAGE_H
#define COMPLEX_IMAGE_H

#include "complex_color.h"

#include <QSize>
#include <QImage>
#include <complex>

typedef std::vector<COMPLEX_DOUBLE> COMPLEX_VECTOR;
typedef std::vector<COMPLEX_VECTOR> COMPLEX_MATRIX;
typedef double (*complexToDouble)(COMPLEX_DOUBLE);

class ComplexImage
{
    public:
        QSize size;
        COMPLEX_MATRIX redChannel;
        COMPLEX_MATRIX greenChannel;
        COMPLEX_MATRIX blueChannel;

        ComplexImage();
        ComplexImage(const QSize &size);
        ComplexImage(const QImage &image);

        QImage toImageFromAbs(bool normalized, double maxRed = 1, double maxGreen = 1, double maxBlue = 1) const;
        QImage toImageFromReal(bool normalized, double maxRed = 1, double maxGreen = 1, double maxBlue = 1) const;

        void setColor(int i, int j, const ComplexColor &color);
        ComplexColor getColor(int i, int j) const;

        void maxAbsValuesExcludingCenter(double &maxRed, double &maxGreen, double &maxBlue);

    private:
        QImage toImage(complexToDouble convert, bool normalized, double maxRed = 1, double maxGreen = 1, double maxBlue = 1) const;
        bool isInsideImage(int i, int j) const;
};
#endif // COMPLEX_IMAGE_H
