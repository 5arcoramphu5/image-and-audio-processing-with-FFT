#include "audio_controller.h"
#include "widgets_utils.h"
#include "../fourier/fourier.h"

#include <array>
#include <complex>
#include <cmath>

#include <QFileDialog>
#include <QDebug>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QtEndian>
#include <iostream>

AudioController::AudioController(): audioReady(false), audioStarted(false), maxAbsBinValue(0)
{}

AudioController::~AudioController()
{
    delete[] originalVisualisationSliders;
    delete[] filteredVisualisationSliders;

    for(int i = 0; i < numberOfDFTs; ++i)
    {
        delete[] DFTs[i];
        delete[] filteredDFTs[i];
    }

    delete audioDecoder;
}

void AudioController::openAudioFromFileExplorer(QWidget *parent, QString &fileName)
{
    // finding file
    fileName = QFileDialog::getOpenFileName(parent, "Open Audio File", QString(), "Audio (*.wav *.mp3)");

    // creating file format
    format = QAudioFormat(QAudioDeviceInfo::defaultOutputDevice().preferredFormat());
    format.setCodec(QStringLiteral("audio/pcm"));
    format.setSampleType(QAudioFormat::SignedInt);
    if (QSysInfo::ByteOrder == QSysInfo::BigEndian)
      format.setByteOrder(QAudioFormat::BigEndian);
    else
      format.setByteOrder(QAudioFormat::LittleEndian);

    bytesPerSample = format.sampleSize() / 8;

    // setting up QAudioDecoder
    audioDecoder = new QAudioDecoder(parent);

    QAudioFormat decodeFormat(format);
    decodeFormat.setSampleType(QAudioFormat::Float);

    audioDecoder->setAudioFormat(decodeFormat);
    audioDecoder->setSourceFilename(fileName);
    audioDecoder->start();

    QObject::connect(audioDecoder, &QAudioDecoder::bufferReady, this, &AudioController::bufferReady);
    QObject::connect(audioDecoder, &QAudioDecoder::finished, this, &AudioController::audioDecodingFinished);

    // setting up QAudioOutputs
    audioOutputOriginal = new QAudioOutput(format, this);
    audioOutputFiltered = new QAudioOutput(format, this);

    audioOutputOriginal->setNotifyInterval(format.durationForBytes(FFT_SIZE * bytesPerSample) / 1000);
    QObject::connect(audioOutputOriginal, &QAudioOutput::notify, this, &AudioController::notifyOriginalAudio);
}

void AudioController::calculateDFTs()
{
    numberOfDFTs = floor((float)byteArrOriginal.length() / (bytesPerSample*FFT_SIZE));
    DFTs = new COMPLEX_DOUBLE*[numberOfDFTs];
    filteredDFTs = new COMPLEX_DOUBLE*[numberOfDFTs];

    for(int k = 0; k < numberOfDFTs; ++k)
    {
        // calculating the DFT of a portion of samples
        DFTs[k] = new COMPLEX_DOUBLE[FFT_SIZE];
        filteredDFTs[k] = new COMPLEX_DOUBLE[FFT_SIZE];
        int* frame = new int[FFT_SIZE];

        for(int i = 0; i < FFT_SIZE; ++i)
        {
            int pos = (k*FFT_SIZE + i) * bytesPerSample;
            QByteArray arr;
            for(int j = 0; j < bytesPerSample; ++j)
                arr.append(byteArrOriginal[pos+j]);

            frame[i] = QSysInfo::ByteOrder == QSysInfo::BigEndian ? qFromBigEndian<qint16>(arr.data()) : qFromLittleEndian<qint16>(arr.data());
        }

        fft1D(frame, DFTs[k], FFT_SIZE);

        delete[] frame;

        //copying values to filteredDFTs
        for(int i = 0; i < FFT_SIZE; ++i)
            filteredDFTs[k][i] = DFTs[k][i];

        // calculating max abs sum in visualisation
        int halfFFT = FFT_SIZE / 2;
        int freqsForBin = halfFFT / BINS;

        for(int i = 0; i < BINS; ++i)
        {
            double value = 0;
            for(int x = 0; x < freqsForBin; ++x)
                value += std::abs(DFTs[k][halfFFT + freqsForBin*i + x]);

            if(value > maxAbsBinValue)
                maxAbsBinValue = value;
        }
    }
}

void AudioController::bufferReady()
{
    auto audioBuffer = audioDecoder->read();
    byteArrOriginal.append(audioBuffer.constData<char>(), audioBuffer.byteCount());
}


void AudioController::audioDecodingFinished()
{
    audioReady = true;
    audioLength = format.durationForBytes(byteArrOriginal.length());
    calculateDFTs();

    byteArrFiltered = QByteArray(byteArrOriginal);
    reloadAudio();

    qDebug() << "audio ready";
}

void AudioController::reloadAudio()
{
    if(audioStarted)
        stopAudio();

    bufferOriginal = new QBuffer(&byteArrOriginal);
    bufferOriginal->open(QIODevice::ReadOnly);

    bufferFiltered = new QBuffer(&byteArrFiltered);
    bufferFiltered->open(QIODevice::ReadOnly);
}

void AudioController::stopAudio()
{
    audioOutputOriginal->stop();
    audioOutputFiltered->stop();
    timePassedSlider->setValue(0);
    audioStarted = false;
}

void AudioController::notifyOriginalAudio()
{
    int timePassed = audioOutputOriginal->processedUSecs();
    timePassedSlider->setValue(std::round( (double)timePassed / audioLength * 100 ));

    int currentPos = format.bytesForDuration(timePassed);

    // stop audio after buffer ended
    if(currentPos >= byteArrOriginal.length())
    {
        audioOutputOriginal->stop();
        audioOutputFiltered->stop();
        audioStarted = false;
        clearVisualisations();
        return;
    }

    int i = floor((double)currentPos / (FFT_SIZE*bytesPerSample));

    if(i < numberOfDFTs)
    {
        updateVisualisations(DFTs[i], filteredDFTs[i]);
    }
}

void AudioController::updateVisualisations(std::complex<double>* DFT, std::complex<double>* filteredDFT)
{
    // update frequency bars (they show the second half of the FFT in reverse order)
    int halfFFT = FFT_SIZE / 2;
    int freqsForBin = halfFFT / BINS;

    for(int i = 0; i < BINS; ++i)
    {
        double originalValue = 0;
        double filteredValue = 0;
        for(int x = 0; x < freqsForBin; ++x)
        {
            originalValue += std::abs(DFT[halfFFT + freqsForBin*i + x]) / (maxAbsBinValue/500);
            filteredValue += std::abs(filteredDFT[halfFFT + freqsForBin*i + x]) / (maxAbsBinValue/500);
        }

        originalVisualisationSliders[BINS - i - 1]->setValue(std::round(originalValue));
        filteredVisualisationSliders[BINS - i - 1]->setValue(std::round(filteredValue));
    }
}

void AudioController::clearVisualisations()
{
    for(int i = 0; i < BINS; ++i)
    {
        originalVisualisationSliders[i]->setValue(0);
        filteredVisualisationSliders[i]->setValue(0);
    }
}

void AudioController::play()
{
    if(!audioReady)
        return;

    if(audioStarted)
    {
        audioOutputOriginal->resume();
        audioOutputFiltered->resume();
    }
    else
    {
        audioOutputOriginal->start(bufferOriginal);
        audioOutputFiltered->start(bufferFiltered);
        audioStarted = true;
    }
}

void AudioController::pause()
{
    if(!audioReady)
        return;

    audioOutputOriginal->suspend();
    audioOutputFiltered->suspend();
}

void AudioController::setVolumeOriginal(double volume)
{
    if(!audioReady)
        return;

    audioOutputOriginal->setVolume(volume);
}

void AudioController::setVolumeFiltered(double volume)
{
    if(!audioReady)
        return;

    audioOutputFiltered->setVolume(volume);
}

void AudioController::saveAudioToDistFromFileExplorer(QWidget *parent)
{}


void AudioController::addFilteredVisualisation(QBoxLayout &parent)
{
    filteredVisualisationSliders = createAudioVisualisation(parent, BINS);
}

void AudioController:: addOriginalVisualisation(QBoxLayout &parent)
{
    originalVisualisationSliders = createAudioVisualisation(parent, BINS);
}

void AudioController::setTimePassedSliderRef(QSlider* slider)
{
    timePassedSlider = slider;
}

void AudioController::refreshAfterFiltering()
{
    // operacje na DFTs
    // debug: ucięcie środka transformaty (wysokich częstotliwości)
    double scale = 0.9;
    int cutOffValue = FFT_SIZE/2 * scale;
    int center = FFT_SIZE/2;
    for(int k = 0; k < numberOfDFTs; ++k)
    {
        for(int i = 0; i < FFT_SIZE; ++i)
            filteredDFTs[k][i] = DFTs[k][i];

        // modyfikacja
        for(int i = center - cutOffValue; i < center + cutOffValue; ++i)
        {
            filteredDFTs[k][i] = 0;
        }
    }

    // wykonanie ifft i zapisanie w byteArrFiltered
    byteArrFiltered.clear();

    for(int i = 0; i < numberOfDFTs; ++i)
    {
        qDebug() << i << " / " << numberOfDFTs;

        COMPLEX_DOUBLE iDFT[FFT_SIZE];
        ifft1D(filteredDFTs[i], iDFT, FFT_SIZE);

        for(int j = 0; j < FFT_SIZE; ++j)
        {
            int value = std::round(iDFT[j].real());

            if(value > MAX_QINT16)
                value = MAX_QINT16;
            else if (value < -MAX_QINT16)
                value = -MAX_QINT16;

            qint16 qint16Value = value;

            char buffer[2];
            if(QSysInfo::ByteOrder == QSysInfo::BigEndian)
                qToBigEndian(qint16Value, buffer);
            else
                qToLittleEndian(qint16Value, buffer);

            byteArrFiltered.append(buffer, 2);
        }
    }

    // przeładowanie kontrolera dźwięku
    reloadAudio();

    qDebug() << "applied";
}
