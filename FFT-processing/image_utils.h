#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <QImage>
#include <QLabel>

void showImage(QImage &image, QLabel &label);

QImage openImageFromFileExplorer(QWidget *parent);

#endif // IMAGE_UTILS_H
