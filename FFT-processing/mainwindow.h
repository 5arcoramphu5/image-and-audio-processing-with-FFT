#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include <QLabel>
#include "fourier/complex_image.h"
#include "filters/filters.h"

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

private:
    Ui::MainWindow *ui;
    QLabel* images[4];

    bool imageUploaded;
    QImage image;
    QImage processedImage;
    QImage imageDFT;
    QImage processedDFTImage;

    ComplexImage dft;

    QMovie loadingGif;

    Filters filters;

    void showImages();
    void loadImagesAndDFTs(QImage& image);
    void setScaledImages(bool scaled);
    void setImagesLoading();
    void performFFTProcessing();
    void initializeFiltersSelection();
};
#endif // MAINWINDOW_H
