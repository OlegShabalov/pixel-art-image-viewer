#include "Image.hpp"

#include <QPixmap>
#include <QMovie>
#include <QPainter>
#include <QWidget>
#include <QFileInfo>

#include "Math.hpp"



Image::Image(const ConfigItem & config)
    : _config(config)
{}

Image::~Image() {
    clear();
}

void Image::clear() {
    if (_image) {
        delete _image;
        _image = nullptr;
    }
    if (_movie) {
        delete _movie;
        _movie = nullptr;
    }
}

void Image::setImage(QPixmap * image) {
    clear();
    if (image) {
        _image = image;

        _fixed = true;
        _contentWidth = _image->width();
        _contentHeight = _image->height();
        _calculateMinMaxScale();
        _proportionalResize(_widgetSize());
    }
}

void Image::setMovie(QMovie * movie) {
    clear();
    if (movie) {
        _movie = movie;

        _fixed = true;
        _contentWidth = _movie->frameRect().width();
        _contentHeight = _movie->frameRect().height();
        _calculateMinMaxScale();
        _proportionalResize(_widgetSize());
    }
}



void Image::updateGeometry(const QSize & widgetSize) {
    if (_fixed) {
        _setWidgetSize(widgetSize);
        _proportionalResize(_widgetSize());
    }
    else {
        if (_config.windowResizeStrategy == ConfigItem::FocusOnCenter) {
            const QSizeF offset = QSizeF(widgetSize - _widgetSize()) / 2.0;
            _x += offset.width();
            _y += offset.height();
            _setWidgetSize(widgetSize);
            _correctPosition();
        }
        else if (_config.windowResizeStrategy == ConfigItem::ScaleFragment) {
            _x += (widgetSize.width()  - _widgetSize().width())  / 2.0;
            _y += (widgetSize.height() - _widgetSize().height()) / 2.0;

            const int minSize = minI(_widgetSizeBackup.width(),
                                     _widgetSizeBackup.height());
            const double minBorder = minSize / _scaleBackup * _minScale;

            const int maxSize = maxI(_contentWidth, _contentHeight);
            const double maxBorder = maxSize * _maxScale;

            const double sizeNow = restrict(minI(widgetSize.width(),
                                                 widgetSize.height()),
                                            minBorder, maxBorder);
            const double sizeOld = restrict(minI(_widgetSize().width(),
                                                 _widgetSize().height()),
                                            minBorder, maxBorder);

            const double k = sizeNow / sizeOld;
            const double newScale = restrict(_scale()*k, _minScale, _maxScale);

            _widgetSizePrivate = widgetSize;

            if (newScale != _scalePrivate) {
                _scalePrivate = newScale;
                _scaleContent(_widgetCenter());
            }
            _correctPosition();
        }
        else {
            _setWidgetSize(widgetSize);
            _correctPosition();
        }
    }
}

void Image::moveTo(const QPointF & pos) {
    _fixed = false;

    _x = pos.x();
    _y = pos.y();
    _correctPosition();
}

void Image::scale(double by, const QPoint & mousePosition) {
    if (_changeScaleValue(by)) {
        _fixed = false;
        _scaleContent(_scaleCenter(mousePosition));
    }
}

void Image::scaleByWidgetCenter(double by) {
    if (_changeScaleValue(by)) {
        _fixed = false;
        _scaleContent(_widgetCenter());
    }
}



void Image::changeFixing(const QPoint & mousePosition) {
    if (_fixed) {
        _fixed = false;
        _scaleOneTyOne(mousePosition);
    }
    else {
        _fixed = true;
        _proportionalResize(_widgetSize());
    }
}

void Image::fixImage() {
    _fixed = true;
    _proportionalResize(_widgetSize());
}

void Image::calculateMinMaxScale() {
    _calculateMinMaxScale();
}



void Image::setPaused(bool paused) {
    if (_movie) _movie->setPaused(paused);
}

bool Image::isPaused() const {
    if (_movie && _movie->state() == QMovie::Running) return false;
    return true;
}

void Image::jumpToNextFrame() {
    if (_movie) {
        _movie->setPaused(true);
        _movie->jumpToNextFrame();
    }
}

void Image::jumpToPreviousFrame() {
    if (_movie) {
        _movie->setPaused(true);
        int frame = _movie->currentFrameNumber() - 1;
        if (frame < 0) frame = _movie->frameCount() - 1;
        _movie->jumpToFrame(frame);
    }
}



void Image::paint(QPainter & painter) {
    if (_image) {
        _paint(painter, *_image);
    }
    else if (_movie) {
        _paint(painter, _movie->currentPixmap());
    }
}

void Image::_paint(QPainter & painter, const QPixmap & image) {
    const int widgetW = _widgetSize().width();
    const int widgetH = _widgetSize().height();

    const double vl = maxD(_x, 0.0) - _x;
    const double vt = maxD(_y, 0.0) - _y;
    const double vr = minD(_x + _w, widgetW) - _x;
    const double vb = minD(_y + _h, widgetH) - _y;

    if (vl >= vr || vt >= vb) return;

    const double k = _scale();

    const int sl = static_cast<int>(vl / k);
    const int st = static_cast<int>(vt / k);
    const int sr = static_cast<int>(vr / k + 1.0);
    const int sb = static_cast<int>(vb / k + 1.0);

    const double rl = sl * k;
    const double rt = st * k;
    const double rr = sr * k;
    const double rb = sb * k;

    painter.drawPixmap(QRectF(_x+rl, _y+rt, rr-rl, rb-rt), image,
                       QRectF(sl, st, sr-sl, sb-st));
}



bool Image::_changeScaleValue(double by) {
    const double unpowScale = powD(_scale(), 1.0/16.0) + (by/70.0);
    const double powScale = powD(unpowScale, 16);
    const double newScale = restrict(powScale, _minScale, _maxScale);

    if (newScale == _scale()) return false;
    _setScale(newScale);
    return true;
}

void Image::_proportionalResize(const QSize & size) {
    const double kw = size.width()  / static_cast<double>(_contentWidth);
    const double kh = size.height() / static_cast<double>(_contentHeight);

    _setScale(restrict(minD(kh, kw), _minScale, _maxScale));

    _w = _contentWidth  * _scale();
    _h = _contentHeight * _scale();

    _x = (size.width()  - _w) / 2.0;
    _y = (size.height() - _h) / 2.0;
}

void Image::_scaleContent(const QPointF & scaleCenter) {
    const double scaleCenterX = restrict(scaleCenter.x(), _x, _x + _w);
    const double scaleCenterY = restrict(scaleCenter.y(), _y, _y + _h);

    const double kx = (scaleCenterX - _x) / _w;
    const double ky = (scaleCenterY - _y) / _h;

    _w = _contentWidth  * _scale();
    _h = _contentHeight * _scale();

    _x = scaleCenterX - (kx * _w);
    _y = scaleCenterY - (ky * _h);
}

void Image::_scaleOneTyOne(const QPoint & mousePosition) {
    _setScale(1);
    _scaleContent(_scaleCenter(mousePosition));
}

void Image::_correctPosition() {
    const double margin = _config.moveMarginInPixels;
    const double marginX = minD(_widgetSize().width()  / 2.0, margin);
    const double marginY = minD(_widgetSize().height() / 2.0, margin);

    _x = restrict(_x, marginX - _w, _widgetSize().width()  - marginX);
    _y = restrict(_y, marginY - _h, _widgetSize().height() - marginY);
}



void Image::_calculateMinMaxScale() {
    _minScale = _config.minImageSize / maxD(_contentHeight, _contentWidth);
    if (_config.enableOneToOneScaling) {
        if (_minScale > 1) _minScale = 1;
    }

    _maxScale = _config.maxPixelSize;
}
