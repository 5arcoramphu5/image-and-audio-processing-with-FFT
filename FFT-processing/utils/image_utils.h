#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include "complex_image.h"

#include <QImage>
#include <QLabel>

void showImage(QImage &image, QLabel &label);

QImage openImageFromFileExplorer(QWidget *parent);

void saveImageToDistFromFileExplorer(QImage &image, QWidget *parent);

void processWithFFT(ComplexImage &dft, QImage &processedDFT, QImage &processedImage);

void displayAsLoading(QLabel* label, QMovie &movie);

#endif // IMAGE_UTILS_H
