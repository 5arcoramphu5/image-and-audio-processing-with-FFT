#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>

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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.xpm *.jpg)"));
    qDebug() << fileName;
}

