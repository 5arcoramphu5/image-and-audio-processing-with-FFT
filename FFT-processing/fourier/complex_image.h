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

        QImage toImageFromAbs() const;
        QImage toImageFromReal() const;

        void setColor(int i, int j, const ComplexColor &color);
        ComplexColor getColor(int i, int j) const;

    private:
        QImage toImage(complexToDouble convert) const;
        bool isInsideImage(int i, int j) const;
};
#endif // COMPLEX_IMAGE_H
