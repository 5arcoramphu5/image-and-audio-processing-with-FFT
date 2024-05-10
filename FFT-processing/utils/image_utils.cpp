#include "image_utils.h"
#include "../fourier/fourier.h"
#include "../fourier/complex_image.h"

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

void displayAsLoading(QLabel* label, QMovie &movie)
{
    label->setPixmap(QPixmap());
    label->setMovie(&movie);
    label->show();
}
