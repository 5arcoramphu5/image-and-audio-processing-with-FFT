#ifndef WIDGETS_UTILS_H
#define WIDGETS_UTILS_H

#include <QSlider>
#include <QObject>
#include <QLabel>
#include <QBoxLayout>

QSlider* createSlider(int initialValue, Qt::Orientation orientation);

QLabel* createLabel(QString text);

QSlider** createAudioVisualisation(QBoxLayout &layout, const int N);

void deleteAllChildren(QObject &object);

#endif // WIDGETS_UTILS_H
