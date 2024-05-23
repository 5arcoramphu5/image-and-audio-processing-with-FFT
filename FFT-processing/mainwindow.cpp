#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils/image_utils.h"
#include "utils/audio_utils.h"
#include "fourier/fourier.h"
#include "utils/widgets_utils.h"
#include "utils/mode.h"

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
    isImageUploaded(false),
    isAudioUploaded(false)
{
    ui->setupUi(this);
    loadingGif.setScaledSize(QSize(50, 50));

    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());

    images[0] = ui->unprocessedImage;
    images[1] = ui->processedImage;
    images[2] = ui->unprocessedImageDFT;
    images[3] = ui->processedImageDFT;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFiltersSelection()
{
    ui->filterSelectionComboBox->clear();
    ui->filterSelectionComboBox->addItems(filters.filterNamesList());
}

void MainWindow::on_applyButton_clicked()
{
    switch(mode)
    {
        case Mode::IMAGE:
        {
            if(!isImageUploaded)
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

            watcher->setFuture(QtConcurrent::run(this, &MainWindow::processImageWithFFT));
        }
        break;

        case Mode::AUDIO:
        {
            qDebug() << "process audio";
            // auto watcher = new QFutureWatcher<void>(this);
            // connect(watcher, &QFutureWatcher<void>::finished,
            //     [watcher, this] ()
            // {
            //     watcher->deleteLater();
            //     // TODO
            // });

            // watcher->setFuture(QtConcurrent::run(this, &MainWindow::processAudioWithFFT));
        }
        break;
    }
}

void MainWindow::on_uploadButton_clicked()
{
    switch(mode)
    {
        case Mode::IMAGE:
        {
            image = openImageFromFileExplorer(this);

            for (QLabel* label : images)
                label->setScaledContents(false);

            setImagesLoading();

            auto watcher = new QFutureWatcher<void>(this);
            connect(watcher, &QFutureWatcher<void>::finished,
                [watcher, this] ()
            {
                watcher->deleteLater();
                showImages();
                isImageUploaded = true;
            });

            watcher->setFuture(QtConcurrent::run(this, &MainWindow::loadImagesAndDFTs, image));
        }
        break;

        case Mode::AUDIO:
        {
            openAudioFromFileExplorer(this);
            // TODO
        }
        break;
    }
}

void MainWindow::loadImagesAndDFTs(QImage& image)
{
    dft = fft2D(image);
    ComplexImage shiftedDFT = fftShift(dft);
    shiftedDFT.maxAbsValuesExcludingCenter(maxRed, maxGreen, maxBlue);

    imageDFT = shiftedDFT.toImageFromAbs(true, maxRed, maxGreen, maxBlue);
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

void MainWindow::setImagesLoading()
{
    for (QLabel* label : images)
        displayAsLoading(label, loadingGif);

    loadingGif.start();
}

void MainWindow::on_saveButton_clicked()
{
    if(mode == Mode::IMAGE)
        saveImageToDistFromFileExplorer(processedImage, this);
    else
        saveAudioToDistFromFileExplorer(this);
}

void MainWindow::processImageWithFFT()
{
    ComplexImage processedDFT = fftShift(dft);
    filters.performImageFiltering(processedDFT);
    processedDFT = fftShift(processedDFT);

    processedDFTImage = fftShift(processedDFT).toImageFromAbs(true, maxRed, maxGreen, maxBlue);
    ComplexImage ifft = ifft2D(processedDFT);
    processedImage = ifft.toImageFromReal(false);
}

void MainWindow::processAudioWithFFT()
{
    // TODO
}

void MainWindow::on_filterSelectionComboBox_currentIndexChanged(int index)
{
    deleteAllChildren(*ui->parameters_group);
    filters.setIndex(index);
    filters.insertParametersUI(*ui->parameters_layout);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    mode = index == 0 ? Mode::IMAGE : Mode::AUDIO;
    filters.setMode(mode);
    updateFiltersSelection();
}

