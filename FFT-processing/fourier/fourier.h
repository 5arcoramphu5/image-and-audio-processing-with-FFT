#ifndef FOURIER_H
#define FOURIER_H

#include "complex_image.h"
#include <QImage>
#include <complex>


ComplexImage fft2D(const ComplexImage &image);
ComplexImage fft2D(const QImage &image);

ComplexImage ifft2D(const ComplexImage &image);

ComplexImage fftShift(const ComplexImage &image);

void fft1D(COMPLEX_DOUBLE* series, COMPLEX_DOUBLE* DFT, int len);
void fft1D(int* series, COMPLEX_DOUBLE* DFT, int len);

void ifft1D(COMPLEX_DOUBLE* DFT, COMPLEX_DOUBLE* series, int len);

#endif // FOURIER_H
