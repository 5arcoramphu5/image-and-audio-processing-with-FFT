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
    processWithFFT(processedImage);
    showImage(processedImage, *(ui->processedImage));
}

void MainWindow::on_uploadButton_clicked()
{
    image = openImageFromFileExplorer(this);
    ComplexImage dft = fft2D(image);
    imageDFT = dft.toImageFromAbs();
    processedDFT = imageDFT;
    // processedImage = image;
    processedImage = ifft2D(dft).toImageFromReal();

    showImage(image, *(ui->unprocessedImage));
    showImage(processedImage, *(ui->processedImage));
    showImage(imageDFT, *(ui->unprocessedImageDFT));
    showImage(processedDFT, *(ui->processedImageDFT));
}

void MainWindow::on_saveButton_clicked()
{
    saveImageToDistFromFileExplorer(processedImage, this);
}

