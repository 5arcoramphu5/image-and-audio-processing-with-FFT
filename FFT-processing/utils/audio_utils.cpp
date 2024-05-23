#include "audio_utils.h"

#include <QAudioDecoder>
#include <QFileDialog>
#include <QAudioBuffer>

void openAudioFromFileExplorer(QWidget *parent)
{
    QString fileName = QFileDialog::getOpenFileName(parent, "Open Audio File", QString(), "Audio (*.mp3)");

    auto audioDecoder = new QAudioDecoder(parent);

    // connect(pAudioDecoder, SIGNAL(bufferReady()), this, SLOT(AudioBufferReady()));

    // connect(pAudioDecoder, SIGNAL(finished()), this, SLOT(AudioFinished()));

    audioDecoder->setSourceFilename(fileName);
    audioDecoder->start();

    //pAudioDecoder->read();
}

void saveAudioToDistFromFileExplorer(QWidget *parent)
{}
