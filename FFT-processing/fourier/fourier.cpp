#include "fourier.h"

#include <cmath>
#include <vector>
#include <QDebug>

typedef std::complex<double> COMPLEX_DOUBLE;

const int FFT_MIN_SIZE = 16;
const COMPLEX_DOUBLE I(0, 1);

ComplexImage dft2D(const ComplexImage &image);

ComplexImage fft2D(const QImage &image)
{
    ComplexImage complexImage(image);
    return fft2D(complexImage);
}

COMPLEX_DOUBLE omega(int n)
{
    return exp(2 * M_PI * I / (COMPLEX_DOUBLE)n);
}

// fast Fourier transform in 2D - vector-radix algorithm
ComplexImage fft2D(const ComplexImage &image)
{
    // TODO: handling sizes not being powers of 2

    if(image.size.width() < FFT_MIN_SIZE && image.size.height() < FFT_MIN_SIZE)
        return dft2D(image);

    if(image.size.width() == 1 || image.size.height() == 1)
        return dft2D(image);

    // dividing image into 4 subimages based on parity of indexes
    ComplexImage subimages[2][2];
    ComplexImage partDFT[2][2];

    for(int i = 0; i<2; ++i)
        for(int j = 0; j<2; ++j)
        {
            subimages[i][j] = ComplexImage(image.size/2);
            for(int x = 0; x < image.size.height()/2; ++x)
                for(int y = 0; y < image.size.width()/2; ++y)
                    subimages[i][j].setColor(x, y, image.getColor(2*x + i, 2*y + j));

            partDFT[i][j] = fft2D(subimages[i][j]);
        }

    COMPLEX_DOUBLE omegaWidth = omega(image.size.width());
    COMPLEX_DOUBLE omegaHeight = omega(image.size.height());
    COMPLEX_DOUBLE omega2 = omega(2);

    // using 4 parts to calculate DFT
    ComplexImage DFT(image.size);
    for(int v1 = 0; v1 < 2; ++v1)
        for(int v2 = 0; v2 < 2; ++v2)
        {
            for(int u1 = 0; u1 < image.size.height()/2; ++u1)
                for(int u2 = 0; u2 < image.size.width()/2; ++u2)
                {
                    int x = u1 + v1*(image.size.height()/2);
                    int y = u2 + v2*(image.size.width()/2);

                    ComplexColor color;
                    color += partDFT[0][0].getColor(u1, u2);
                    color += partDFT[1][0].getColor(u1, u2) * pow(omegaHeight, -u1) * pow(omega2, -v1);
                    color += partDFT[0][1].getColor(u1, u2) * pow(omegaWidth, -u2) * pow(omega2, -v2);
                    color += partDFT[1][1].getColor(u1, u2) * pow(omegaHeight, -u1) * pow(omega2, -v1) * pow(omegaWidth, -u2) * pow(omega2, -v2);

                    DFT.setColor(x, y, color);
                }
        }

    return DFT;
}

// discrete Fourier transform calculated from the definition
ComplexImage dft2D(const ComplexImage &image)
{
    ComplexImage DFT(image.size);
    COMPLEX_DOUBLE omegaWidth = omega(image.size.width());
    COMPLEX_DOUBLE omegaHeight = omega(image.size.height());

    for(int k1 = 0; k1 < image.size.height(); ++k1)
        for(int k2 = 0; k2 < image.size.width(); ++k2)
        {
            ComplexColor sum;
            for(int n1 = 0; n1 < image.size.height(); ++n1)
                for(int n2 = 0; n2 < image.size.width(); ++n2)
                    sum = sum + image.getColor(n1, n2) * pow(omegaHeight, -k1*n1) * pow(omegaWidth, -k2*n2);

            DFT.setColor(k1, k2, sum);
        }

    return DFT;
}

COMPLEX_DOUBLE swapComplex(const COMPLEX_DOUBLE &x)
{
    return COMPLEX_DOUBLE(x.imag(), x.real());
}

ComplexColor swapComplexColor(const ComplexColor &color)
{
    return ComplexColor(swapComplex(color.red), swapComplex(color.green), swapComplex(color.blue));
}

ComplexImage swapComplexImage(const ComplexImage &image)
{
    ComplexImage swappedImage(image.size);

    for(int x = 0; x < image.size.height(); ++x)
        for(int y = 0; y < image.size.width(); ++y)
            swappedImage.setColor(x, y, swapComplexColor(image.getColor(x, y)));

    return swappedImage;
}

ComplexImage ifft2D(const ComplexImage &image)
{
    ComplexImage dft = swapComplexImage(image);
    dft = fft2D(dft);
    dft = swapComplexImage(dft);

    for(int x = 0; x < image.size.height(); ++x)
        for(int y = 0; y < image.size.width(); ++y)
            dft.setColor(x, y, dft.getColor(x, y) / (image.size.height() * image.size.width()));

    return dft;
}

// moving lower frequency components to the center
ComplexImage fftShift(const ComplexImage &image)
{
    ComplexImage shiftedImage(image);

    int halfHeight = image.size.height() / 2;
    int halfWidth = image.size.width() / 2;

    for(int i = 0; i < halfHeight; ++i)
        for(int j = 0; j < halfWidth; ++j)
        {
            // swapping upper left and lower right quarter
            shiftedImage.setColor(i, j, image.getColor(i + halfHeight, j + halfWidth));
            shiftedImage.setColor(i + halfHeight, j + halfWidth, image.getColor(i, j));

            // swapping upper right and lowej left quarter
            shiftedImage.setColor(i, j + halfWidth, image.getColor(i + halfHeight, j));
            shiftedImage.setColor(i + halfHeight, j, image.getColor(i, j + halfWidth));
        }

    return shiftedImage;
}

void fft1D(COMPLEX_DOUBLE* series, COMPLEX_DOUBLE* DFT, int len)
{
    if(len == 1)
    {
        DFT[0] = series[0];
        return;
    }

    COMPLEX_DOUBLE* even = new COMPLEX_DOUBLE[len/2];
    COMPLEX_DOUBLE* odd = new COMPLEX_DOUBLE[len/2];
    COMPLEX_DOUBLE* evenDFT = new COMPLEX_DOUBLE[len/2];
    COMPLEX_DOUBLE* oddDFT = new COMPLEX_DOUBLE[len/2];

    for(int i = 0; i < len/2; ++i)
    {
        even[i] = series[2*i];
        odd[i] = series[2*i+1];
    }

    fft1D(even, evenDFT, len/2);
    fft1D(odd, oddDFT, len/2);

    for(int i = 0; i < len/2; ++i)
    {
        COMPLEX_DOUBLE multiplier = exp(-2 * i * M_PI * I / (COMPLEX_DOUBLE)len);
        DFT[i] = evenDFT[i] + multiplier * oddDFT[i];
        DFT[i + len/2] = evenDFT[i] - multiplier * oddDFT[i];
    }

    delete[] even, odd, evenDFT, oddDFT;
}

void fft1D(int* in, COMPLEX_DOUBLE* out, int len)
{
    COMPLEX_DOUBLE* newIn = new COMPLEX_DOUBLE[len];
    for(int i = 0; i < len; ++i)
        newIn[i] = COMPLEX_DOUBLE(in[i], 0);

    fft1D(newIn, out, len);

    delete[] newIn;
}

void ifft1D(COMPLEX_DOUBLE* DFT, COMPLEX_DOUBLE* series, int len)
{
    for(int i = 0; i < len; ++i)
        series[i] = swapComplex(DFT[i]);

    COMPLEX_DOUBLE* ifft = new COMPLEX_DOUBLE[len];
    fft1D(series, ifft, len);

    for(int i = 0; i < len; ++i)
    {
        series[i] = swapComplex(ifft[i]) / (COMPLEX_DOUBLE)len;
    }

    delete[] ifft;
}
