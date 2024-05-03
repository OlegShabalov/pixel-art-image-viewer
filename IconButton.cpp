#include "IconButton.hpp"

#include <QWidget>
#include <QPainter>
#include <QTimer>

#include <cmath>



IconButton::IconButton(const QIcon & icon,
                       const ConfigItem & config,
                       QWidget * parent)
    : QObject(parent)
    , _icon(icon)
    , _config(config)
{
    _sizeAnimation = new QVariantAnimation(this);
    _sizeAnimation->setEasingCurve(QEasingCurve::OutBack);
    _sizeAnimation->setDuration(200);

    connect(_sizeAnimation, &QVariantAnimation::finished,
            this, &IconButton::_animationFinished);
    connect(_sizeAnimation, &QVariantAnimation::valueChanged,
            this, &IconButton::_animateScale);
}

void IconButton::addSwitchedIcon(const QIcon & switchedIcon) {
    _switchedIcon = switchedIcon;
}

void IconButton::enableAutoRepeat() {
    if (_autoRepeatTimer) return;

    _autoRepeatTimer = new QTimer(this);
    connect(_autoRepeatTimer, &QTimer::timeout,
            this, &IconButton::_autoRepeat);
}

void IconButton::setVisible(bool visible) {
    if (_visible == visible) return;
    _visible = visible;
    _update();

}

void IconButton::setSwitched(bool switched) {
    if (_switched == switched) return;
    _switched = switched;
    _update();
}



void IconButton::paint(QPainter & painter)  {
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(_config.buttonsBackgroundColor());

    const float backgroundRadius = _radius * (_scale / 3.0 + 0.65);
    painter.drawEllipse(_center(), backgroundRadius, backgroundRadius);

    const float iconSize = _iconSize * _scale;
    const QRect iconRect(_x + (_w - iconSize) / 2.0 + 0.5,
                         _y + (_h - iconSize) / 2.0 + 0.5,
                         iconSize, iconSize);

    if (_switched && !_switchedIcon.isNull()) {
        _switchedIcon.paint(&painter, iconRect);
    }
    else {
        _icon.paint(&painter, iconRect);
    }
}



void IconButton::setGeometry(int x, int y, int w, int h) {
    _x = x;
    _y = y;
    _w = w;
    _h = h;
    _radius = std::min(_w, _h) / 2.0;
    _iconSize = std::sqrt(_radius * _radius * 2);
}

char IconButton::mouseMove(const QPoint & pos, int area) {
    const QPointF t = _center() - pos;
    const float g = std::sqrt(t.x() * t.x() + t.y() * t.y());

    char result = 0;
    if (g <= area + _radius) result = inArea;

    if (_pressed) {
        if (g <= _radius) {
            _clickStart();
            _mouseLeave = false;

            _setIsDown(true);
            return result | hovered;
        }
        else {
            _setTargetIconScale(c_normalScale);

            _sizeAnimation->stop();
            _sizeAnimation->setStartValue(_scale);
            _sizeAnimation->setEndValue(_targetScale);
            _sizeAnimation->start();

            _mouseLeave = true;
            _setIsDown(false);
            return result;
        }
    }
    else {
        if (g <= _radius) {
            _setTargetIconScale(c_howerScale);
            return result | hovered;
        }
        else {
            _setTargetIconScale(c_normalScale);
            return result;
        }
    }
}

bool IconButton::mousePressed(const QPoint & pos) {
    if (_inRadius(pos)) {
        _pressed = true;
        _clickStart();
        _setIsDown(true);
        return true;
    }
    return false;
}

bool IconButton::mouseReleased(const QPoint & pos) {
    if (_pressed) {
        if (_inRadius(pos)) {
            _switched = ! _switched;
            Q_EMIT clicked(_switched);
        }
        _pressed = false;
        _setIsDown(false);
        _clickEnd();
        _update();
        return true;
    }
    return false;
}



void IconButton::_animationFinished() {
    if (_pressed) {
        if (!_mouseLeave) return;
        _mouseLeave = false;
    }

    if (std::abs(_targetScale - _scale) > 0.01) {
        _sizeAnimation->setStartValue(_scale);
        _sizeAnimation->setEndValue(_targetScale);
        _sizeAnimation->start();
    }
}

void IconButton::_animateScale(const QVariant & scale) {
    _scale = scale.toFloat();
    _update();
}

void IconButton::_autoRepeat() {
    if (_autoRepeatTimer->interval() > 20) {
        _autoRepeatTimer->setInterval(20);
    }
    _switched = ! _switched;
    Q_EMIT clicked(_switched);
}



void IconButton::_setTargetIconScale(float scale) {
    if (_targetScale == scale) return;
    _targetScale = scale;

    if (!_pressed) {
        if (_sizeAnimation->state() != QAbstractAnimation::Running) {
            _sizeAnimation->setStartValue(_scale);
            _sizeAnimation->setEndValue(_targetScale);
            _sizeAnimation->start();
        }
    }
}

void IconButton::_clickStart() {
    _sizeAnimation->stop();
    _sizeAnimation->setStartValue(_scale);
    _sizeAnimation->setEndValue(c_clickedScale);
    _sizeAnimation->start();
}

void IconButton::_clickEnd() {
    if (_sizeAnimation->state() != QAbstractAnimation::Running) {
        _sizeAnimation->setStartValue(_scale);
        _sizeAnimation->setEndValue(_targetScale);
        _sizeAnimation->start();
    }
}



void IconButton::_setIsDown(bool isDown) {
    if (_isDown == isDown) return;
    _isDown = isDown;

    if (_autoRepeatTimer) {
        if (_isDown) {
            _autoRepeatTimer->setInterval(500);
            _autoRepeatTimer->start();
        }
        else {
            _autoRepeatTimer->stop();
        }
    }
}



bool IconButton::_inRadius(const QPoint & pos) const {
    const QPointF t = _center() - pos;
    const float g = std::sqrt(t.x() * t.x() + t.y() * t.y());
    return (g <= _radius);
}

void IconButton::_update() {
    static_cast<QWidget *>(parent())->update();
}
