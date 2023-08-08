#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <QString>
#include <QPointF>
#include <QSize>

#include "ConfigItem.hpp"

class QWidget;
class QPixmap;
class QMovie;
class QPainter;



class Image {
public:
    Image(const ConfigItem & config);
    ~Image();

    void clear();

    void setImage(QPixmap * image);
    void setMovie(QMovie * movie);

    void updateGeometry(const QSize & widgetSize);
    void moveTo(const QPointF & pos);
    void scale(double by, const QPoint & mousePosition);
    void scaleByWidgetCenter(double by);

    void changeFixing(const QPoint & mousePosition);
    void fixImage();

    void setPaused(bool paused);
    bool isPaused() const;
    void jumpToNextFrame();
    void jumpToPreviousFrame();

    void paint(QPainter & painter);

    inline QPointF pos() const;
    inline bool loaded() const;

private:
    void _paint(QPainter & painter, const QPixmap & image);

    bool _changeScaleValue(double by);

    void _proportionalResize(const QSize & size);
    void _scaleContent(const QPointF & scaleCenter);
    void _scaleOneTyOne(const QPoint & mousePosition);

    void _correctPosition();
    void _calculateMinMaxScale();

    inline QPointF _scaleCenter(const QPoint & mousePosition) const;
    inline QPointF _widgetCenter() const;

    inline double _scale() const;
    inline void _setScale(double value);

    inline const QSize & _widgetSize() const;
    inline void _setWidgetSize(const QSize & value);

private:
    QPixmap * _image = nullptr;
    QMovie  * _movie = nullptr;

    double _x = 0, _y = 0, _w = 1, _h = 1;
    double _scalePrivate = 1;
    double _scaleBackup = 1;

    int _contentWidth = 1, _contentHeight = 1;
    double _minScale = 1, _maxScale = 1;

    QSize _widgetSizePrivate = {1, 1};
    QSize _widgetSizeBackup = {1, 1};

    bool _fixed = true;

    const ConfigItem & _config;
};



inline QPointF Image::pos() const {
    return QPointF(_x, _y);
}
inline bool Image::loaded() const {
    return (_image || _movie);
}



inline QPointF Image::_scaleCenter(const QPoint & mousePosition) const {
    return _config.useCursorAsScaleCenter ? mousePosition : _widgetCenter();
}
inline QPointF Image::_widgetCenter() const {
    return QPointF(_widgetSize().width() / 2.0, _widgetSize().height() / 2.0);
}



inline double Image::_scale() const {
    return _scalePrivate;
}
inline void Image::_setScale(double value) {
    _scaleBackup = _scalePrivate = value;
    _widgetSizeBackup = _widgetSizePrivate;
}

inline const QSize & Image::_widgetSize() const {
    return _widgetSizePrivate;
}
inline void Image::_setWidgetSize(const QSize & value) {
    _widgetSizeBackup = _widgetSizePrivate = value;
}



#endif // IMAGE_HPP
