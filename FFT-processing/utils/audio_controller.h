#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <complex>

#include <QWidget>
#include <QBoxLayout>
#include <QSlider>
#include <QAudioDecoder>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QMediaPlayer>
#include <QBuffer>

#define FFT_SIZE 1024
#define BINS 32
#define MAX_QINT16 32767

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
    void setTimePassedSliderRef(QSlider* slider);

    void updateVisualisations(std::complex<double>* DFT, std::complex<double>* filteredDFT);
    void clearVisualisations();

    // FFT
    void calculateDFTs();

    void refreshAfterFiltering();

    std::complex<double>** DFTs;
    std::complex<double>** filteredDFTs;
    int numberOfDFTs;

private slots:
    void bufferReady();
    void audioDecodingFinished();

    void notifyOriginalAudio();

private:

    void reloadAudio();
    void stopAudio();

    bool audioReady;
    bool audioStarted;
    qint64 audioLength;

    int bytesPerSample;

    QSlider** originalVisualisationSliders;
    QSlider** filteredVisualisationSliders;
    QSlider* timePassedSlider;
    double maxAbsBinValue;

    QAudioDecoder* audioDecoder;
    QAudioFormat format;

    QAudioOutput* audioOutputOriginal;
    QAudioOutput* audioOutputFiltered;

    QByteArray byteArrOriginal;
    QByteArray byteArrFiltered;
    QBuffer *bufferOriginal;
    QBuffer *bufferFiltered;
};


#endif // AUDIO_UTILS_H
