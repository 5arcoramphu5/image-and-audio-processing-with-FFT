#ifndef WIDGETS_UTILS_H
#define WIDGETS_UTILS_H

#include <QSlider>
#include <QObject>
#include <QLabel>

QSlider* createSlider(int initialValue);

void deleteAllChildren(QObject &object);

QLabel* createLabel(QString text);

#endif // WIDGETS_UTILS_H
