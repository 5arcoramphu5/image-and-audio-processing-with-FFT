#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include <QLabel>
#include "fourier/complex_image.h"
#include "filters/filters.h"
#include "utils/mode.h"
#include "utils/audio_controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_applyButton_clicked();

    void on_uploadButton_clicked();

    void on_saveButton_clicked();

    void on_filterSelectionComboBox_currentIndexChanged(int index);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;

    QMovie loadingGif;

    Filters filters;

    Mode mode;

    void updateFiltersSelection();

    //  __________________
    // |___ IMAGES TAB ___|

    QLabel* images[4];

    bool isImageUploaded;
    QImage image;
    QImage processedImage;
    QImage imageDFT;
    QImage processedDFTImage;

    ComplexImage dft;

    // normalizing DFT
    double maxRed, maxGreen, maxBlue;

    void showImages();
    void loadImagesAndDFTs(QImage& image);
    void setImagesLoading();
    void processImageWithFFT();

    //  _________________
    // |___ AUDIO TAB ___|

    bool isAudioUploaded;
    AudioController audioController;

    void processAudioWithFFT();
};
#endif // MAINWINDOW_H
