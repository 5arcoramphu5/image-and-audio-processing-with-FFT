#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <QWidget>
#include <QBoxLayout>
#include <QSlider>

class AudioController
{
public:
    AudioController();
    ~AudioController();

    void openAudioFromFileExplorer(QWidget *parent);

    void saveAudioToDistFromFileExplorer(QWidget *parent);

    void addOriginalVisualisation(QBoxLayout &parent);

    void addFilteredVisualisation(QBoxLayout &parent);

private:
    const int N;
    QSlider** originalVisualisationSliders;
    QSlider** filteredVisualisationSliders;
};


#endif // AUDIO_UTILS_H
