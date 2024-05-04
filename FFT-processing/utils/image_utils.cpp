#include "image_utils.h"
#include "fourier.h"
#include "complex_image.h"

#include <QFileDialog>

#include <QDebug>

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
        qDebug() << "error saving file";
}

void blur(ComplexImage &dft, double param)
{
    // deleting smallest frequencies (in the middle)
    int radiusX = round(dft.size.height() / 2 * param);
    int radiusY = round(dft.size.width() / 2 * param);
    int middleX = round(dft.size.height() / 2);
    int middleY = round(dft.size.width() / 2);
    ComplexColor zero(0, 0, 0);

    for(int i = 0; i < radiusX; ++i)
        for(int j = 0; j < dft.size.width(); ++j)
        {
            dft.setColor(middleX + i, j, zero);
            dft.setColor(middleX - i, j, zero);
        }

    for(int i = 0; i < dft.size.height(); ++i)
        for(int j = 0; j < radiusY; ++j)
        {
            dft.setColor(i, middleY + j, zero);
            dft.setColor(i, middleY - j, zero);
        }
}

void processWithFFT(ComplexImage &dft, QImage &processedDFTImage, QImage &processedImage)
{
    ComplexImage processedDFT(dft);
    blur(processedDFT, 0.7);

    ComplexImage ifft = ifft2D(processedDFT);
    processedDFTImage = processedDFT.toImageFromAbs();
    processedImage = ifft.toImageFromReal();
}

void displayAsLoading(QLabel* label, QMovie &movie)
{
    label->setMovie(&movie);
    label->show();
}
