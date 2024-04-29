#include "ColorButton.hpp"

#include <QPainter>



ColorButton::ColorButton(const QColor & color, QWidget * parent)
    : QPushButton(parent)
    , _color(color)
{  }



void ColorButton::setColor(const QColor & color) {
    if (_color != color) {
        _color = color;
        update();
    }
}



void ColorButton::paintEvent(QPaintEvent * event) {
    QPushButton::paintEvent(event);

    QPainter p(this);

    const int margin = qMin(width(), height()) / 5;

    p.setBrush(_color);
    p.setPen(Qt::NoPen);
    p.drawRect(margin, margin, width()-margin-margin, height()-margin-margin);
}
