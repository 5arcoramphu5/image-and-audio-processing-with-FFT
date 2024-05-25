#include "audio_controller.h"
#include "widgets_utils.h"

#include <QAudioDecoder>
#include <QFileDialog>
#include <QAudioBuffer>

AudioController::AudioController(): N(32) {}

AudioController::~AudioController()
{
    delete[] originalVisualisationSliders;
    delete[] filteredVisualisationSliders;
}

void AudioController::openAudioFromFileExplorer(QWidget *parent)
{
    // QString fileName = QFileDialog::getOpenFileName(parent, "Open Audio File", QString(), "Audio (*.mp3)");

    // auto audioDecoder = new QAudioDecoder(parent);

    // // connect(pAudioDecoder, SIGNAL(bufferReady()), this, SLOT(AudioBufferReady()));

    // // connect(pAudioDecoder, SIGNAL(finished()), this, SLOT(AudioFinished()));

    // audioDecoder->setSourceFilename(fileName);
    // audioDecoder->start();

    // //pAudioDecoder->read();
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
