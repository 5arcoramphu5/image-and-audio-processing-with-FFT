#ifndef FOURIER_H
#define FOURIER_H

#include "complex_image.h"
#include <QImage>
#include <complex>


ComplexImage fft2D(const ComplexImage &image);
ComplexImage fft2D(const QImage &image);

ComplexImage ifft2D(const ComplexImage &image);

#endif // FOURIER_H
