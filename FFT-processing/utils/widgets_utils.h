#ifndef WIDGETS_UTILS_H
#define WIDGETS_UTILS_H

#include <QSlider>
#include <QObject>
#include <QLabel>

QSlider* createSlider(int initialValue, Qt::Orientation orientation);

QLabel* createLabel(QString text);

void deleteAllChildren(QObject &object);

#endif // WIDGETS_UTILS_H
