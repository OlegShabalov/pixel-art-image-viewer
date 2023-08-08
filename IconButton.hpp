#ifndef ICONBUTTON_HPP
#define ICONBUTTON_HPP

#include <QObject>
#include <QIcon>
#include <QVariantAnimation>

#include "ConfigItem.hpp"



class IconButton : public QObject {
    Q_OBJECT

public:
    enum MouseMoveResult : char {
        hovered = 1,
        inArea = 2,
    };

public:
    IconButton(const QIcon & icon,
               const ConfigItem & config,
               QWidget * parent);

    void addSwitchedIcon(const QIcon & switchedIcon);
    void enableAutoRepeat();
    inline bool visible() const;
    void setVisible(bool visible);
    void setSwitched(bool switched);

    void paint(QPainter & painter);
    void setGeometry(int x, int y, int w, int h);

    char mouseMove(const QPoint & pos, int area);
    bool mousePressed(const QPoint & pos);
    bool mouseReleased(const QPoint & pos);

Q_SIGNALS:
    void clicked(bool switched);

private Q_SLOTS:
    void _animationFinished();
    void _animateScale(const QVariant & scale);
    void _autoRepeat();

private:
    void _setTargetIconScale(float scale);
    void _clickStart();
    void _clickEnd();
    void _setIsDown(bool isDown);

    inline QPointF _center() const;
    bool _inRadius(const QPoint & pos) const;
    void _update();

private:
    static constexpr float c_normalScale = 0.8;
    static constexpr float c_howerScale = 0.92;
    static constexpr float c_clickedScale = 0.6;

    int _x = 0;
    int _y = 0;
    int _w = 1;
    int _h = 1;

    float _scale = c_normalScale;
    float _targetScale = c_normalScale;

    float _radius = 1;
    float _iconSize = 1;

    bool _pressed = false;
    bool _switched = false;
    bool _visible = true;
    bool _mouseLeave = false;
    bool _isDown = false;

    QIcon _icon;
    QIcon _switchedIcon;

    const ConfigItem & _config;

    QVariantAnimation * _sizeAnimation;
    QTimer * _autoRepeatTimer = nullptr;
};



inline bool IconButton::visible() const {
    return _visible;
}

inline QPointF IconButton::_center() const {
    return QPointF(_x + _w / 2.0, _y + _h / 2.0);
}



#endif // ICONBUTTON_HPP
