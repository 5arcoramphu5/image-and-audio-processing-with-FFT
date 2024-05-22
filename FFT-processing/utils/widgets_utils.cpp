#include "widgets_utils.h"


QSlider* createSlider(int initialValue)
{
    QSlider* slider = new QSlider();
    slider->setOrientation(Qt::Orientation::Horizontal);
    slider->setMaximum(99);
    slider->setMinimum(0);
    slider->setValue(initialValue);
    // slider->setMaximumHeight(20);

    return slider;
}

QLabel* createLabel(QString text)
{
    QLabel* label = new QLabel();
    label->setText(text);
    // label->setMaximumHeight(20);
    return label;
}

void deleteAllChildren(QObject &object)
{
    while ( QWidget* w = object.findChild<QWidget*>() )
        delete w;
}
