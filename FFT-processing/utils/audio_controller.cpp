#include "audio_controller.h"
#include "widgets_utils.h"
#include "../fourier/fourier.h"

#include <array>
#include <complex>
#include <QFileDialog>
#include <QDebug>
#include <QAudioDeviceInfo>
#include <QBuffer>

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
    QObject::connect(audioDecoder, &QAudioDecoder::finished, this, &AudioController::audioFinished);

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

void AudioController::audioFinished()
{
    audioReady = true;

    bufferOriginal = new QBuffer(&byteArrOriginal);
    bufferOriginal->open(QIODevice::ReadOnly);

    bufferFiltered = new QBuffer(&byteArrOriginal);
    bufferFiltered->open(QIODevice::ReadOnly);
}

void AudioController::notifyOriginalAudio()
{
    // find current frame
    int currentPos = format.bytesForDuration(audioOutputOriginal->processedUSecs());
    int frame[FFT_SIZE];

    int j = 0;
    for(int i = currentPos - FFT_SIZE / 2 + 1; i <= currentPos + FFT_SIZE / 2; ++i)
    {
        frame[j] = (int)byteArrOriginal.at(i * bytesPerSample);
        j++;
    }

    // calculate FFT of the frame
    std::complex<double> fftResult[FFT_SIZE];
    fft1D(frame, fftResult, FFT_SIZE);

    // update frequency bars
    int freqsForBin = FFT_SIZE / BINS;
    for(int i = 0; i < BINS; ++i)
    {
        double value = 0;
        for(int x = 0; x < freqsForBin; ++x)
            value += std::abs(fftResult[freqsForBin*i + x]) / 100;

        originalVisualisationSliders[i]->setValue(std::round(value));
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
