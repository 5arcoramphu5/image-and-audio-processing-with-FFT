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

QSlider** createAudioVisualisation(QBoxLayout &layout, const int N)
{
    QString stylesheet = QStringLiteral("QSlider::groove:vertical {"
                                        "background: white; }"
                                        "QSlider::add-page:vertical {"
                                        "background: rgb(2, 119, 189); }"
                                        "QSlider::sub-page:vertical {"
                                        "background: white; }");

    auto sliders = new QSlider*[N];

    double step = (double)100 / N;
    for(int i = 0; i < N; ++i)
    {
        sliders[i] = createSlider( i*step, Qt::Orientation::Vertical);
        sliders[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sliders[i]->setStyleSheet(stylesheet);
        sliders[i]->setEnabled(false);
        layout.addWidget(sliders[i]);
    }

    return sliders;
}

void deleteAllChildren(QObject &object)
{
    while ( QWidget* w = object.findChild<QWidget*>() )
        delete w;
}
