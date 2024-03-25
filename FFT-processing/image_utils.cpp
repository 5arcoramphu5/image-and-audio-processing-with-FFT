#include "image_utils.h"

#include <QFileDialog>

void showImage(QImage &image, QLabel &label)
{
    QImage scaledImage = image.scaled(label.size(), Qt::KeepAspectRatio);
    label.setPixmap(QPixmap::fromImage(scaledImage));
}

QImage openImageFromFileExplorer(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(parent, "Open File", "/home", "Images (*.png *.xpm *.jpg)");
    QImage image(fileName);
    return image;
}
