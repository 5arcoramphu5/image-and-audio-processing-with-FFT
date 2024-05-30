#include "audio_controller.h"
#include "widgets_utils.h"
#include "../fourier/fourier.h"

#include <array>
#include <complex>
#include <QFileDialog>
#include <QDebug>
#include <QAudioDeviceInfo>
#include <QBuffer>
#include <QtEndian>
#include <iostream>

AudioController::AudioController(): audioReady(false), audioStarted(false)
{}

AudioController::~AudioController()
{
    delete[] originalVisualisationSliders;
    delete[] filteredVisualisationSliders;

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

void AudioController::bufferReady()
{
    auto audioBuffer = audioDecoder->read();
    byteArrOriginal.append(audioBuffer.constData<char>(), audioBuffer.byteCount());
}


void AudioController::audioDecodingFinished()
{
    audioReady = true;
    audioLength = format.durationForBytes(byteArrOriginal.length());

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
    if(currentPos + FFT_SIZE/2*bytesPerSample >= byteArrOriginal.length())
    {
        audioOutputOriginal->stop();
        audioOutputFiltered->stop();
        audioStarted = false;
        clearVisualisations();
        return;
    }

    // find current frame
    int frame[FFT_SIZE];
    int j = 0;

    for(int offset = -FFT_SIZE / 2; offset < FFT_SIZE / 2; ++offset)
    {
        int pos = currentPos + offset*bytesPerSample;

        QByteArray arr;
        for(int i = 0; i < bytesPerSample; ++i)
            arr.append(byteArrOriginal[pos+i]);

        frame[j] = QSysInfo::ByteOrder == QSysInfo::BigEndian ? qFromBigEndian<qint16>(arr.data()) : qFromLittleEndian<qint16>(arr.data());
        j++;
    }

    // calculate FFT of the frame
    std::complex<double> fftResult[FFT_SIZE];
    fft1D(frame, fftResult, FFT_SIZE);

    updateVisualisations(fftResult);
}

void AudioController::updateVisualisations(std::complex<double>* DFT)
{
    // update frequency bars (they show the second half of the FFT)
    int freqsForBin = FFT_SIZE / BINS;

    for(int i = 0; i < BINS; ++i)
    {
        double value = 0;
        for(int x = 0; x < freqsForBin; ++x)
            value += std::abs(DFT[freqsForBin*i + x]) / 1000;

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
