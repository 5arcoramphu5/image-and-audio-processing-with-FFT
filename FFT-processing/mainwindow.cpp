#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils/image_utils.h"
#include "utils/fourier.h"

#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QColor>

#include<QDebug>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_applyButton_clicked()
{
    processWithFFT(dft, processedDFT, processedImage);
    showImages();
}

void MainWindow::on_uploadButton_clicked()
{
    image = openImageFromFileExplorer(this);
    dft = fft2D(image);
    imageDFT = dft.toImageFromAbs();
    processedDFT = imageDFT;
    processedImage = image;

    showImages();
}

void MainWindow::showImages()
{
    qDebug() <<"images start";
    showImage(image, *(ui->unprocessedImage));
    showImage(imageDFT, *(ui->unprocessedImageDFT));
    showImage(processedImage, *(ui->processedImage));
    showImage(processedDFT, *(ui->processedImageDFT));
    qDebug() <<"images end";
}

void MainWindow::on_saveButton_clicked()
{
    saveImageToDistFromFileExplorer(processedImage, this);
}

