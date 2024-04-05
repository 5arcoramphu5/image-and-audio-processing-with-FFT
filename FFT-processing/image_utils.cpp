#include "image_utils.h"
#include "utils/fourier.h"

#include <QFileDialog>

#include <iostream>

void showImage(QImage &image, QLabel &label)
{
    QImage scaledImage = image.scaled(label.size(), Qt::KeepAspectRatio);
    label.setPixmap(QPixmap::fromImage(scaledImage));
}

QImage openImageFromFileExplorer(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(parent, "Open File", "/home", "Images (*.png *.xpm *.jpg *.bmp)");
    return QImage(fileName);
}

void saveImageToDistFromFileExplorer(QImage &image, QWidget *parent)
{
    QString path = QFileDialog::getSaveFileName(parent, "Choose location");
    bool result = image.save(path + ".png");
    if(!result)
        std::cerr << "error saving file" << std::endl;
}

QImage processWithFFT(QImage &image)
{
    return fft2D(image);
    // for(int i = 0; i < image.size().width(); ++i)
    //     for(int j = 0; j < image.size().height(); ++j)
    //     {
    //         QRgb color = image.pixel(i, j);
    //         QColor newColor(qRed(color), 255, 255, qAlpha(color));
    //         image.setPixelColor(i, j, newColor);
    //     }
}
