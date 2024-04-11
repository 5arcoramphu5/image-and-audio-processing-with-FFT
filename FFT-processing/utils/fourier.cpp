#include "fourier.h"

#include <cmath>

typedef std::complex<double> COMPLEX_DOUBLE;

const int FFT_MIN_SIZE = 16;
const COMPLEX_DOUBLE I(0, 1);

ComplexImage fft2D(QImage &image)
{
    ComplexImage complexImage(image);
    return fft2D(complexImage);
}

// fast Fourier transform in 2D - vector-radix algorithm
ComplexImage fft2D(ComplexImage &image)
{
    // TODO: handling sizes not being powers of 2

    // if(image.size.width() < FFT_MIN_SIZE && image.size.height() < FFT_MIN_SIZE)
    //     return dft2D(image);

    // // dividing image into 4 subimages based on parity of indexes
    // ComplexImage subimages[2][2];
    // ComplexImage partDFT[2][2];

    // for(int i = 0; i<2; ++i)
    //     for(int j = 0; j<2; ++j)
    //     {
    //         subimages[i][j] = ComplexImage(size/2);
    //         for(int x = 0; x < size.height()/2; ++x)
    //             for(int y = 0; y < size.width()/2; ++y)
    //                 subimages[i][j].setColor(x, y, image.getColor(2*x + i, 2*y + j));

    //         partDFT[i][j] = fft2D(subimages[i][j]);
    //     }

    // COMPLEX_DOUBLE omegaWidth = omega(image.size.width());
    // COMPLEX_DOUBLE omegaHeigth = omega(image.size.heigth());
    // COMPLEX_DOUBLE omega2 = omega(2);

    // // using 4 parts to calculate DFT
    ComplexImage DFT(image.size);
    // for(int v1 = 0; v1 < 2; ++v1)
    //     for(int v2 = 0; v2 < 2; ++v2)
    //     {
    //         for(int u1 = 0; u1 < size.height()/2; ++u1)
    //             for(int u2 = 0; u2 < size.width()/2; ++u2)
    //             {
    //                 int x = u1 + v1*(size.height()/2);
    //                 int y = u2 + v2*(size.width()/2);

    //                 ComplexColor color(r, g, b);
    //                 DFT.setColor(x, y, color);
    //             }
    //     }

    for(int x = 0; x < image.size.height(); ++x)
        for(int y = 0; y < image.size.width(); ++y)
            DFT.setColor(x, y, image.getColor(x, y) * ComplexColor(2, 1, 1) );

    return DFT;
}

COMPLEX_DOUBLE omega(int n)
{
    return exp(2 * M_PI * I / (COMPLEX_DOUBLE)n);
}

// discrete Fourier transform calculated from the definition
ComplexImage dft2D(ComplexImage &image)
{
    return image;
}
