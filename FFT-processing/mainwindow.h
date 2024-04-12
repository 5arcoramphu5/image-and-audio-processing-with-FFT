#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "utils/complex_image.h"

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

private:
    Ui::MainWindow *ui;

    QImage image;
    QImage processedImage;

    ComplexImage dft;

    QImage imageDFT;
    QImage processedDFT;

    void showImages();
};
#endif // MAINWINDOW_H
