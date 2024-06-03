#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils/image_utils.h"
#include "utils/audio_controller.h"
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
    audioController(),
    isImageUploaded(false),
    isAudioUploaded(false)
{
    ui->setupUi(this);
    loadingGif.setScaledSize(QSize(50, 50));

    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());

    audioController.addFilteredVisualisation(*ui->filteredAudioVisualisation);
    audioController.addOriginalVisualisation(*ui->originalAudioVisualisation);
    audioController.setTimePassedSliderRef(ui->audioTimeSlider);

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
            processAudioWithFFT();
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

QString trimFileNamePath(QString fileName)
{
    return fileName.split("/").last();
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
            QString fileName;
            audioController.openAudioFromFileExplorer(this, fileName);
            ui->fileName->setText(trimFileNamePath(fileName));
            ui->fileName->setStyleSheet("font-weight: bold;");
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
        audioController.saveAudioToDistFromFileExplorer(this);
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
    filters.performAudioFiltering();//audioController.DFTs, audioController.filteredDFTs, audioController.numberOfDFTs, FFT_SIZE);
    audioController.refreshAfterFiltering();
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

void MainWindow::on_playButton_toggled(bool play)
{
    if(play)
        audioController.play();
    else
        audioController.pause();
}

void MainWindow::on_originalAudioVolume_sliderMoved(int value)
{
    if(!ui->originalAudioMute->isChecked())
        audioController.setVolumeOriginal((double)value / 100);
}

void MainWindow::on_originalAudioMute_toggled(bool mute)
{
    if(mute)
        audioController.setVolumeOriginal(0);
    else
        audioController.setVolumeOriginal( (double)ui->originalAudioVolume->value() / 100);
}

void MainWindow::on_filteredAudioMute_toggled(bool mute)
{
    if(mute)
        audioController.setVolumeFiltered(0);
    else
        audioController.setVolumeFiltered( (double)ui->filteredAudioVolume->value() / 100);
}

void MainWindow::on_filteredAudioVolume_sliderMoved(int value)
{
    if(!ui->filteredAudioMute->isChecked())
        audioController.setVolumeFiltered((double)value / 100);
}

