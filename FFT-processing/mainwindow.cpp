#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils/image_utils.h"
#include "utils/fourier.h"

#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QColor>

#include <QDebug>
#include <QMovie>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), loadingGif("/home/sarcoramphus/Documents/kod/image-and-audio-processing-with-FFT/FFT-processing/graphics/loading2.gif")
{
    ui->setupUi(this);
    loadingGif.setScaledSize(QSize(100, 100));
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
    setImagesLoading();

    auto watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished,
        [watcher, this] ()
    {
        watcher->deleteLater();

        setScaledImages(false);
        showImages();
    });

    watcher->setFuture(QtConcurrent::run(this, &MainWindow::loadImagesAndDFTs, image));
}

void MainWindow::loadImagesAndDFTs(QImage& image)
{
    dft = fft2D(image);
    imageDFT = dft.toImageFromAbs();
    processedDFT = imageDFT;
    processedImage = image;
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

void MainWindow::setImagesLoading()
{
    ui->unprocessedImage->setMovie(&loadingGif);
    ui->unprocessedImageDFT->setMovie(&loadingGif);
    ui->processedImage->setMovie(&loadingGif);
    ui->processedImageDFT->setMovie(&loadingGif);
    ui->unprocessedImage->show();
    ui->unprocessedImageDFT->show();
    ui->processedImage->show();
    ui->processedImageDFT->show();
    loadingGif.start();
}

void MainWindow::on_saveButton_clicked()
{
    saveImageToDistFromFileExplorer(processedImage, this);
}

