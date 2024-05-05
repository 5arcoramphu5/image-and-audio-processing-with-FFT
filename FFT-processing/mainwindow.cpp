#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils/image_utils.h"
#include "fourier/fourier.h"
#include "utils/widgets_utils.h"

#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QColor>

#include <QDebug>
#include <QMovie>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    loadingGif(":/graphics/loading.gif"),
    imageUploaded(false)
{
    ui->setupUi(this);

    images[0] = ui->unprocessedImage;
    images[1] = ui->processedImage;
    images[2] = ui->unprocessedImageDFT;
    images[3] = ui->processedImageDFT;

    loadingGif.setScaledSize(QSize(50, 50));

    initializeFiltersSelection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeFiltersSelection()
{
    ui->filterSelectionComboBox->addItems(filters.imageFilterNamesList());
}

void MainWindow::on_applyButton_clicked()
{
    if(!imageUploaded)
        return;

    displayAsLoading(ui->processedImageDFT, loadingGif);
    displayAsLoading(ui->processedImage, loadingGif);
    loadingGif.start();

    auto watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished,
        [watcher, this] ()
    {
        watcher->deleteLater();
        showImages();
    });

    watcher->setFuture(QtConcurrent::run(this, &MainWindow::performFFTProcessing));
}

void MainWindow::on_uploadButton_clicked()
{
    image = openImageFromFileExplorer(this);

    setScaledImages(false);
    setImagesLoading();

    auto watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished,
        [watcher, this] ()
    {
        watcher->deleteLater();
        showImages();
        imageUploaded = true;
    });

    watcher->setFuture(QtConcurrent::run(this, &MainWindow::loadImagesAndDFTs, image));
}

void MainWindow::loadImagesAndDFTs(QImage& image)
{
    dft = fft2D(image);
    imageDFT = dft.toImageFromAbs();
    processedDFTImage = imageDFT;
    processedImage = image;
}

void MainWindow::showImages()
{
    showImage(image, *(ui->unprocessedImage));
    showImage(imageDFT, *(ui->unprocessedImageDFT));
    showImage(processedImage, *(ui->processedImage));
    showImage(processedDFTImage, *(ui->processedImageDFT));
}

void MainWindow::setScaledImages(bool scaled)
{
    for (QLabel* label : images)
        label->setScaledContents(scaled);
}

void MainWindow::setImagesLoading()
{
    for (QLabel* label : images)
        displayAsLoading(label, loadingGif);

    loadingGif.start();
}

void MainWindow::on_saveButton_clicked()
{
    saveImageToDistFromFileExplorer(processedImage, this);
}

void MainWindow::performFFTProcessing()
{
    ComplexImage processedDFT(dft);

    filters.performFiltering(processedDFT);

    ComplexImage ifft = ifft2D(processedDFT);
    processedDFTImage = processedDFT.toImageFromAbs();
    processedImage = ifft.toImageFromReal();
}

void MainWindow::on_filterSelectionComboBox_currentIndexChanged(int index)
{
    deleteAllChildren(*ui->parameters_group);
    filters.setIndex(index);
    filters.insertParametersUI(*ui->parameters_layout);
}
