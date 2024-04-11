#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <QImage>
#include <QLabel>

void showImage(QImage &image, QLabel &label);

QImage openImageFromFileExplorer(QWidget *parent);

void saveImageToDistFromFileExplorer(QImage &image, QWidget *parent);

QImage processWithFFT(QImage &image);

#endif // IMAGE_UTILS_H
