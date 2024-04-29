#include "Layout.hpp"

#include <QWidget>
#include <QPainter>

#include "Math.hpp"



Layout::Layout(QWidget * parent, const ConfigItem & config)
    : QObject(parent)
    , _config(config)
{
    _hideTimer = new QTimer(this);
    _hideTimer->setSingleShot(true);
    connect(_hideTimer, &QTimer::timeout, this, &Layout::_hide);

    _opacityAnimation = new QVariantAnimation(this);
    _opacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
    connect(_opacityAnimation, &QVariantAnimation::valueChanged,
            this, &Layout::_animateOpacity);



    next = new IconButton(QIcon(":/icon/next"), _config, parent);
    next->enableAutoRepeat();
    _widgets.push_back(next);

    previous = new IconButton(QIcon(":/icon/previous"), _config, parent);
    previous->enableAutoRepeat();
    _widgets.push_back(previous);

    fullScreen = new IconButton(QIcon(":/icon/fullScreen"), _config, parent);
    fullScreen->addSwitchedIcon(QIcon(":/icon/normalSize"));
    _widgets.push_back(fullScreen);

    settings = new IconButton(QIcon(":/icon/dots"), _config, parent);
    _widgets.push_back(settings);
    _topLine.push_back(settings);

    browse = new IconButton(QIcon(":/icon/folder"), _config, parent);
    _widgets.push_back(browse);
    _topLine.push_back(browse);

    pin = new IconButton(QIcon(":/icon/pin"), _config, parent);
    pin->addSwitchedIcon(QIcon(":/icon/unpin"));
    _widgets.push_back(pin);
    _topLine.push_back(pin);

    playPause = new IconButton(QIcon(":/icon/pause"), _config, parent);
    playPause->addSwitchedIcon(QIcon(":/icon/play"));
    _widgets.push_back(playPause);

    nextFrame = new IconButton(QIcon(":/icon/next"), _config, parent);
    nextFrame->enableAutoRepeat();
    _widgets.push_back(nextFrame);

    previousFrame = new IconButton(QIcon(":/icon/previous"), _config, parent);
    previousFrame->enableAutoRepeat();
    _widgets.push_back(previousFrame);



    connect(&config, &ConfigItem::guiScaleChanged, this, [this]() {
        _updateGeometry();
        _show();
        _update();
    });
    connect(&config, &ConfigItem::buttonsBackgroundColorChanged, this,[this](){
        _show();
        _update();
    });
}



void Layout::paint(QPainter & painter) {
    if (_opacity > 0.01) {
        painter.setOpacity(_opacity);

        for (int i=0; i<_widgets.size(); ++i) {
            if (_widgets.at(i)->visible()) {
                _widgets.at(i)->paint(painter);
            }
        }
    }
}



void Layout::widgetResize(const QSize & windowSize, const QPoint & mousePos) {
    _widgetSize = windowSize;
    _buttonArea = minI(windowSize.width(), windowSize.height()) * 0.12;

    _updateGeometry();

    const bool wasHoveredItem = _haveHoveredItem;
    _updateHovering(mousePos);
    if (wasHoveredItem && !_haveHoveredItem) _restartHideTimer();
}

void Layout::widgetMove(const QPoint & mousePos) {
    const bool wasHoveredItem = _haveHoveredItem;
    _updateHovering(mousePos);
    if (wasHoveredItem && !_haveHoveredItem) _restartHideTimer();
}



bool Layout::mousePressed(const QPoint & pos) {
    for (int i=0; i<_widgets.size(); ++i) {
        if (_widgets.at(i)->visible()) {
            if (_widgets.at(i)->mousePressed(pos)) {
                _pressedItem = _widgets.at(i);

                _show();

                return true;
            }
        }
    }
    return false;
}

bool Layout::mouseReleased(const QPoint & pos) {
    if (_pressedItem) {
        _pressedItem->mouseReleased(pos);
        _pressedItem = nullptr;

        _updateHovering(pos);

        if (_haveHoveredItem) _show();
        else _restartHideTimer();

        return true;
    }

    _updateHovering(pos);
    if (_haveHoveredItem) _show();

    return false;
}



void Layout::mouseMove(const QPoint & pos) {
    const bool wasInArea = _inButtonArea;
    _updateHovering(pos);
    if (!_inButtonArea) {
        if (wasInArea) _restartHideTimer();
    }
    else if (!_pressedItem) _show();
}

void Layout::mouseEnter(const QPoint & pos) {
    _updateHovering(pos);
}

void Layout::mouseLeave() {
    _updateHovering(QPoint(-9000, -9000));
    _restartHideTimer();
}



void Layout::wasClicked() {
    if (_opacityAnimation->state() == QVariantAnimation::Running) return;
    if (_visible) _hide();
    else _show();
}

void Layout::wasInteraction() {
    _hide();
}



void Layout::enableGifMode(bool enable, const QPoint & pos) {
    if (playPause->visible() == enable) return;

    playPause->setVisible(enable);
    nextFrame->setVisible(enable);
    previousFrame->setVisible(enable);

    _updateGeometry();

    const bool wasHoveredItem = _haveHoveredItem;
    _updateHovering(pos);
    if (wasHoveredItem && !_haveHoveredItem) _restartHideTimer();
}



void Layout::_updateGeometry() const {
    const int margin = 20;
    const int spacing = 5;
    const int minSide = minI(_widgetSize.width(), _widgetSize.height());



    const int scrollSize = _getBigButtonSize(minSide);

    const int yCenter = (_widgetSize.height() - scrollSize) / 2.0;

    int x = margin;
    if (_widgetSize.width() < 2 * margin + 2 * scrollSize + spacing) {
        x = (_widgetSize.width() - 2 * scrollSize - spacing) / 2.0;
    }
    previous->setGeometry(x, yCenter, scrollSize, scrollSize);

    x = _widgetSize.width() - margin - scrollSize;
    if (_widgetSize.width() < 2 * margin + 2 * scrollSize + spacing) {
        x = (_widgetSize.width() + spacing) / 2.0;
    }
    next->setGeometry(x, yCenter, scrollSize, scrollSize);



    const int size = _getNormalButtonSize(minSide);

    int buttonsWidth;
    for (int i=0, n=0; true; ++i) {
        if (i == _topLine.size()) {
            buttonsWidth = size * n + spacing * (n-1);
            break;
        }
        if (_topLine.at(i)->visible()) ++n;
    }

    const int yTop = minI(margin, yCenter - spacing - size);

    x = maxI(_widgetSize.width() - size - margin,
             (_widgetSize.width() + buttonsWidth) / 2.0 - size);

    for (int i=0; i<_topLine.size(); ++i) {
        if (_topLine.at(i)->visible()) {
            _topLine.at(i)->setGeometry(x, yTop, size, size);
            x -= size + spacing;
        }
    }



    const int yBottom = maxI(_widgetSize.height() - size - margin,
                             yCenter + scrollSize + spacing);

    if (playPause->visible()) {
        const int center = (_widgetSize.width() - size) / 2.0;
        playPause->setGeometry(center, yBottom, size, size);

        previousFrame->setGeometry(center-spacing-size, yBottom, size, size);

        nextFrame->setGeometry(center+spacing+size, yBottom, size, size);

        x = maxI(_widgetSize.width() - size - margin,
                 center + 2 *(spacing + size));
        fullScreen->setGeometry(x, yBottom, size, size);
    }
    else {
        x = _widgetSize.width() - size - margin;
        fullScreen->setGeometry(x, yBottom, size, size);
    }
}



int Layout::_getNormalButtonSize(int minWidgetSide) const {
    return _config.guiScale * restrict(minWidgetSide * 0.05 + 15, 30, 100);
}

int Layout::_getBigButtonSize(int minWidgetSide) const {
    return _config.guiScale * restrict(minWidgetSide * 0.12, 40, 120);
}



void Layout::_updateHovering(const QPoint & pos) {
    _haveHoveredItem = false;
    _inButtonArea = false;

    if (_pressedItem) {
        _pressedItem->mouseMove(pos, _buttonArea);
    }
    else {
        char mouseMoveState;
        for (int i=0; i<_widgets.size(); ++i) {
            if (_widgets.at(i)->visible()) {
                mouseMoveState = _widgets.at(i)->mouseMove(pos, _buttonArea);

                if (mouseMoveState & IconButton::hovered) {
                    _haveHoveredItem = true;
                }
                if (mouseMoveState & IconButton::inArea) {
                    _inButtonArea = true;
                }
            }
        }
    }
}



inline void Layout::_update() {
    static_cast<QWidget *>(parent())->update();
}

inline void Layout::_restartHideTimer() {
    if (_visible) {
        _hideTimer->start(600);
    }
}



void Layout::_show() {
    _hideTimer->start(2500);

    if (_visible) return;

    _visible = true;

    _opacityAnimation->stop();
    _opacityAnimation->setStartValue(_opacity);
    _opacityAnimation->setEndValue(1.0f);
    _opacityAnimation->setDuration(100);
    _opacityAnimation->start();
}

void Layout::_hide() {
    if (!_visible) return;
    if (_pressedItem) return;
    if (_haveHoveredItem && !_config.enableHidingHoveredButtons) return;

    _visible = false;

    _opacityAnimation->stop();
    _opacityAnimation->setStartValue(_opacity);
    _opacityAnimation->setEndValue(0.0f);
    _opacityAnimation->setDuration(400);
    _opacityAnimation->start();
}



void Layout::_animateOpacity(const QVariant & opacity) {
    _opacity = opacity.toFloat();
    _update();
}
