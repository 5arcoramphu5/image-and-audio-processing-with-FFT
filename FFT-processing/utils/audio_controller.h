#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <QWidget>
#include <QBoxLayout>
#include <QSlider>
#include <QAudioDecoder>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QMediaPlayer>
#include <QBuffer>

#define FFT_SIZE 1024
#define BINS 64

class AudioController: public QObject
{
public:
    AudioController();
    ~AudioController();

    // file operations
    void openAudioFromFileExplorer(QWidget *parent, QString &fileName);

    void saveAudioToDistFromFileExplorer(QWidget *parent);

    // audio player controls
    void play();

    void pause();

    void setVolumeOriginal(double volume);
    void setVolumeFiltered(double volume);

    // UI
    void addOriginalVisualisation(QBoxLayout &parent);

    void addFilteredVisualisation(QBoxLayout &parent);

private slots:
    void bufferReady();
    void audioFinished();

    void notifyOriginalAudio();

private:

    bool audioReady;
    bool audioStarted;

    int bytesPerSample;

    QSlider** originalVisualisationSliders;
    QSlider** filteredVisualisationSliders;

    QAudioDecoder* audioDecoder;
    QAudioFormat format;

    QAudioOutput* audioOutputOriginal;
    QAudioOutput* audioOutputFiltered;

    QByteArray byteArrOriginal;
    QBuffer *bufferOriginal;
    QBuffer *bufferFiltered;
};


#endif // AUDIO_UTILS_H
