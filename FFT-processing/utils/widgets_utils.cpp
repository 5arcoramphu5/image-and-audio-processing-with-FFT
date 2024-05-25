#include "widgets_utils.h"


QSlider* createSlider(int initialValue, Qt::Orientation orientation)
{
    QSlider* slider = new QSlider();
    slider->setOrientation(orientation);
    slider->setMaximum(99);
    slider->setMinimum(0);
    slider->setValue(initialValue);

    return slider;
}

QLabel* createLabel(QString text)
{
    QLabel* label = new QLabel();
    label->setText(text);
    return label;
}

void deleteAllChildren(QObject &object)
{
    while ( QWidget* w = object.findChild<QWidget*>() )
        delete w;
}
