#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <QWidget>


class AudioController
{
public:
    AudioController();

    void openAudioFromFileExplorer(QWidget *parent);

    void saveAudioToDistFromFileExplorer(QWidget *parent);
};


#endif // AUDIO_UTILS_H
