#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <QObject>
#include <QVector>
#include <QTimer>
#include <QVariantAnimation>

#include "IconButton.hpp"



class Layout : public QObject {
    Q_OBJECT

public:
    Layout(QWidget * parent, const ConfigItem & config);

    void paint(QPainter & painter);

    void widgetResize(const QSize & windowSize, const QPoint & mousePos);
    void widgetMove(const QPoint & mousePos);

    bool mousePressed(const QPoint & pos);
    bool mouseReleased(const QPoint & pos);

    void mouseMove(const QPoint & pos);
    void mouseEnter(const QPoint & pos);
    void mouseLeave();

    void wasClicked();
    void wasInteraction();

    void enableGifMode(bool enable, const QPoint & pos);

private:
    void _updateGeometry() const;

    int _getNormalButtonSize(int minWidgetSide) const;
    int _getBigButtonSize(int minWidgetSide) const;

    void _updateHovering(const QPoint & pos);

    void _update();
    void _restartHideTimer();

    void _show();

private Q_SLOTS:
    void _hide();

    void _animateOpacity(const QVariant & opacity);

public:
    IconButton * next;
    IconButton * previous;
    IconButton * fullScreen;
    IconButton * settings;
    IconButton * browse;
    IconButton * pin;

    IconButton * playPause;
    IconButton * nextFrame;
    IconButton * previousFrame;

private:
    const ConfigItem & _config;
    QVector<IconButton *> _widgets;
    QVector<IconButton *> _topLine;
    IconButton * _pressedItem = nullptr;

    float _opacity = 0;
    int _buttonArea = 1;
    bool _visible = false;
    bool _haveHoveredItem = false;
    bool _inButtonArea = false;
    QTimer * _hideTimer;
    QVariantAnimation * _opacityAnimation;
    QSize _widgetSize;
};



#endif // LAYOUT_HPP
