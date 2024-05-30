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
        delete[] DFTs[i];

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

    qDebug() << "number of DFTs: " << numberOfDFTs;

    for(int k = 0; k < numberOfDFTs; ++k)
    {
        // calculating the DFT of a portion of samples
        DFTs[k] = new COMPLEX_DOUBLE[FFT_SIZE];
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

    qDebug() << "done calculating DFTs";
    qDebug() << "max value: " << maxAbsBinValue;
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

    bufferOriginal = new QBuffer(&byteArrOriginal);
    bufferOriginal->open(QIODevice::ReadOnly);

    bufferFiltered = new QBuffer(&byteArrOriginal);
    bufferFiltered->open(QIODevice::ReadOnly);
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
        updateVisualisations(DFTs[i]);
    }
}

void AudioController::updateVisualisations(std::complex<double>* DFT)
{
    // update frequency bars (they show the second half of the FFT)
    int halfFFT = FFT_SIZE / 2;
    int freqsForBin = halfFFT / BINS;

    for(int i = 0; i < BINS; ++i)
    {
        double value = 0;
        for(int x = 0; x < freqsForBin; ++x)
            value += std::abs(DFT[halfFFT + freqsForBin*i + x]) / (maxAbsBinValue/500);

        originalVisualisationSliders[i]->setValue(std::round(value));
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
