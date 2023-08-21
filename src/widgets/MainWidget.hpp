#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
#include <QStringList>

#include "Image.hpp"
#include "ConfigItem.hpp"
#include "ImageLoader.hpp"
#include "Layout.hpp"
#include "Application.hpp"
#include "WinWindow.hpp"
#include "SettingsWidget.hpp"



class MainWidget : public WinWindow {
    Q_OBJECT

public:
    MainWidget(Application & application, int imageIndex);
    ~MainWidget();

protected:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
    void mouseDoubleClickEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void enterEvent(QEvent * event) override;
    void leaveEvent(QEvent * event) override;
    void wheelEvent(QWheelEvent * event) override;

    void paintEvent(QPaintEvent * event) override;

    void resizeEvent(QResizeEvent * event) override;
    void moveEvent(QMoveEvent * event) override;

private Q_SLOTS:
    void _loadNext();
    void _loadPrevious();

    void _setImage();

    void _changeFullScreen();
    void _browseCurrentFile();
    void _pinOnTop();
    void _setPaused(bool paused);
    void _jumpToNextFrame();
    void _jumpToPreviousFrame();

    void _openSettings();

private:
    void _setTitle(bool error = false);
    void _stopDragging();
    void _loadImage(const QString & filePath);

    void _createShortcuts();

private:
    Image * _current;
    ConfigItem _config;

    Application & _application;
    int _currentIndex = 0;

    bool _isDraggingImage = false;
    bool _draggingCursorEnable = false;
    bool _wasDragging = false;
    QPointF _imageDragOffset;

    double _accumulatedScroll = 0;
    ulong _lastTimestamp = 0;

    ImageLoader _imageLoader;

    Layout * _layout;

    SettingsWidget * _settings = nullptr;
};



#endif // MAINWIDGET_HPP
