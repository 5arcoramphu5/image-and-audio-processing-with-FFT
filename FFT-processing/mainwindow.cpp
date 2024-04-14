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

    setScaledImages(false);
    showImages();
}

void MainWindow::showImages()
{
    showImage(image, *(ui->unprocessedImage));
    showImage(imageDFT, *(ui->unprocessedImageDFT));
    showImage(processedImage, *(ui->processedImage));
    showImage(processedDFT, *(ui->processedImageDFT));
}

void MainWindow::setScaledImages(bool scaled)
{
    ui->unprocessedImage->setScaledContents(scaled);
    ui->unprocessedImageDFT->setScaledContents(scaled);
    ui->processedImage->setScaledContents(scaled);
    ui->processedImageDFT->setScaledContents(scaled);
}

void MainWindow::on_saveButton_clicked()
{
    saveImageToDistFromFileExplorer(processedImage, this);
}

