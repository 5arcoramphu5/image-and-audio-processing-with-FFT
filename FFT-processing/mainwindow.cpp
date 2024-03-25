#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "image_utils.h"

#include <QFileDialog>
#include <QImage>
#include <QLabel>

#include<QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_applyButton_clicked()
{

}


void MainWindow::on_uploadButton_clicked()
{
    QImage image = openImageFromFileExplorer(this);
    showImage(image, *(ui->unprocessedImage));
}
