#ifndef FOURIER_H
#define FOURIER_H

#include "complex_image.h"
#include <QImage>
#include <complex>

ComplexImage fft2D(ComplexImage &image);

ComplexImage fft2D(QImage &image);

#endif // FOURIER_H
