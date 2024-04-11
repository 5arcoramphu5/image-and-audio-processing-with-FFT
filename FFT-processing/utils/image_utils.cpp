#include "image_utils.h"
#include "fourier.h"
#include "complex_image.h"

#include <QFileDialog>

#include <iostream>

void showImage(QImage &image, QLabel &label)
{
    QImage scaledImage = image.scaled(label.size(), Qt::KeepAspectRatio);
    label.setPixmap(QPixmap::fromImage(scaledImage));
}

QImage openImageFromFileExplorer(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(parent, "Open File", QString(), "Images (*.png *.xpm *.jpg *.bmp)");
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
    ComplexImage complexImage(image);
    return complexImage.toImageFromAbs();
}
