#include "audio_controller.h"
#include "widgets_utils.h"

#include <QFileDialog>
#include <QDebug>
#include <QAudioDeviceInfo>
#include <QBuffer>

AudioController::AudioController(): N(32), audioReady(false), audioStarted(false)
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
    targetFormat = QAudioFormat(QAudioDeviceInfo::defaultOutputDevice().preferredFormat());
    targetFormat.setCodec(QStringLiteral("audio/pcm"));
    targetFormat.setSampleType(QAudioFormat::SignedInt);
    if (QSysInfo::ByteOrder == QSysInfo::BigEndian)
      targetFormat.setByteOrder(QAudioFormat::BigEndian);
    else
      targetFormat.setByteOrder(QAudioFormat::LittleEndian);

    // setting up QAudioDecoder
    audioDecoder = new QAudioDecoder(parent);

    QAudioFormat decodeFormat(targetFormat);
    decodeFormat.setSampleType(QAudioFormat::Float);

    audioDecoder->setAudioFormat(decodeFormat);
    audioDecoder->setSourceFilename(fileName);
    audioDecoder->start();

    QObject::connect(audioDecoder, &QAudioDecoder::bufferReady, this, &AudioController::bufferReady);
    QObject::connect(audioDecoder, &QAudioDecoder::finished, this, &AudioController::audioFinished);

    // setting up QAudioOutputs
    audioOutputOriginal = new QAudioOutput(targetFormat, this);
    audioOutputFiltered = new QAudioOutput(targetFormat, this);
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

    bufferFiltered = new QBuffer(bufferOriginal);
    bufferFiltered->open(QIODevice::ReadOnly);
}

void AudioController::play()
{
    if(!audioReady)
        return;

    if(audioStarted)
    {
        audioOutputOriginal->resume();
        // audioOutputFiltered->resume();
    }
    else
    {
        audioOutputOriginal->start(bufferOriginal);
        // audioOutputFiltered->start(bufferFiltered);
    }
}

void AudioController::pause()
{
    if(!audioReady)
        return;

    audioOutputOriginal->suspend();
    // audioOutputFiltered->suspend();
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
    filteredVisualisationSliders = createAudioVisualisation(parent, N);
}

void AudioController:: addOriginalVisualisation(QBoxLayout &parent)
{
    originalVisualisationSliders = createAudioVisualisation(parent, N);
}
