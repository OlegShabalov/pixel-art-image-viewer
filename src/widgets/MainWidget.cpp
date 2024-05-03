#include "MainWidget.hpp"

#include <QPainter>
#include <QFileInfo>
#include <QDir>
#include <QMouseEvent>
#include <QMovie>
#include <QProcess>
#include <QShortcut>
#include <QPalette>

#include "src/Math.hpp"



MainWidget::MainWidget(Application & application, int imageIndex)
    : WinWindow("localconfig/winpos", "MainWidget")
    , _config("localconfig/settings")
    , _application(application)
    , _currentIndex(imageIndex)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);

    {
        QPalette p;
        p.setColor(QPalette::Window, _config.windowBackgroundColor());
        setAutoFillBackground(true);
        setPalette(p);
    }
    connect(&_config.windowBackgroundColor, &ColorConfigField::changed,
            this, [this]()
    {
        QPalette p = palette();
        p.setColor(QPalette::Window, _config.windowBackgroundColor());
        setPalette(p);
    });

    _layout = new Layout(this, _config);
    connect(_layout->next, &IconButton::clicked,
            this, &MainWidget::_loadNext);
    connect(_layout->previous, &IconButton::clicked,
            this, &MainWidget::_loadPrevious);
    connect(_layout->fullScreen, &IconButton::clicked,
            this, &MainWidget::_changeFullScreen);
    connect(_layout->browse, &IconButton::clicked,
            this, &MainWidget::_browseCurrentFile);
    connect(_layout->pin, &IconButton::clicked,
            this, &MainWidget::_pinOnTop);
    connect(_layout->playPause, &IconButton::clicked,
            this, &MainWidget::_setPaused);
    connect(_layout->nextFrame, &IconButton::clicked,
            this, &MainWidget::_jumpToNextFrame);
    connect(_layout->previousFrame, &IconButton::clicked,
            this, &MainWidget::_jumpToPreviousFrame);
    connect(_layout->settings, &IconButton::clicked,
            this, &MainWidget::_openSettings);

    connect(&_imageLoader, &ImageLoader::finished,
            this, &MainWidget::_setImage);

    connect(this, &WinWindow::maximizedState, this, [this](){
        if (_config.fixImageWhenWindowMaximized) {
            _current->fixImage();
            update();
        }
    });
    connect(this, &WinWindow::fullscreenState, this, [this](){
        if (_config.fixImageWhenWindowFullScreen) {
            _current->fixImage();
            update();
        }
    });



    auto minImageSizeChangedFun = [this](){
        _current->correctScaleFromMin();
        update();
    };
    connect(&_config.minImageSize, &IntConfigField::changed,
            this, minImageSizeChangedFun);
    connect(&_config.enableOneToOneScaling, &BoolConfigField::changed,
            this, minImageSizeChangedFun);
    connect(&_config.maxPixelSize, &IntConfigField::changed, this, [this](){
        _current->correctScaleFromMax();
        update();
    });

    auto moveMarginChanged = [this](){
        _current->correctPosition();
        update();
     };
    connect(&_config.moveMarginInPixels, &IntConfigField::changed,
            this, moveMarginChanged);
    connect(&_config.enablePictureCount, &BoolConfigField::changed, this,
            [this]()
    {
        _setTitle(_loadingError);
    });

    _createShortcuts();

    _current = new Image(_config);

    _loadImage(_application.imagePathName(_currentIndex));
}

MainWidget::~MainWidget() {
    delete _current;
}



void MainWidget::mousePressEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {
        if (_layout->mousePressed(event->pos())) {
            event->accept();
            return;
        }

        if (_current->loaded() && !_isDraggingImage) {
            _isDraggingImage = true;
            _wasDragging = false;
            _imageDragOffset = _current->pos() - event->screenPos();
            event->accept();
            return;
        }
    }

    event->ignore();
}

void MainWidget::mouseReleaseEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {
        if (_layout->mouseReleased(event->pos())) {
            event->accept();
            return;
        }

        if (_isDraggingImage) {
            _stopDragging();

            if (!_wasDragging) {
                _layout->wasClicked();
            }

            event->accept();
            return;
        }
    }

    event->ignore();
}

void MainWidget::mouseDoubleClickEvent(QMouseEvent * event) {
    if (event->button() == Qt::LeftButton) {
        if (_layout->mousePressed(event->pos())) {
            event->accept();
            return;
        }

        _current->changeFixing(mapFromGlobal(QCursor::pos()));
        _layout->wasInteraction();
        update();
        event->accept();
        return;
    }

    event->ignore();
}

void MainWidget::mouseMoveEvent(QMouseEvent * event) {
    if (_isDraggingImage) {
        if (!_draggingCursorEnable) {
            setCursor(Qt::ClosedHandCursor);
            _draggingCursorEnable = true;
        }
        _wasDragging = true;
        _current->moveTo(event->screenPos() + _imageDragOffset);
        _layout->wasInteraction();
        update();
        event->accept();
        return;
    }
    else {
        _layout->mouseMove(event->pos());
    }

    event->ignore();
}

void MainWidget::enterEvent(QEvent * event) {
    _layout->mouseEnter(mapFromGlobal(QCursor::pos()));
    QWidget::enterEvent(event);
}

void MainWidget::leaveEvent(QEvent * event) {
    _layout->mouseLeave();
    QWidget::leaveEvent(event);
}



void MainWidget::wheelEvent(QWheelEvent * event) {
    if (_current->loaded()) {
        const QPoint value = event->angleDelta();

        if (value.x() == 0) {
            _current->scale(value.y() / 120.0, mapFromGlobal(QCursor::pos()));
            _layout->wasInteraction();
            update();
            event->accept();
            return;
        }
    }

    if (_config.enableGesturesToScroll && !_application.imageListEmpty()) {
        const QPoint value = event->angleDelta();

        if (value.y() == 0) {
            if (event->timestamp() - _lastTimestamp > 50) {
                _accumulatedScroll = 0;
            }
            _lastTimestamp = event->timestamp();

            const double d = restrictAbs((value.x() / 120.0) * 0.35, 0.065);

            if ((d > 0 && _accumulatedScroll < 0) ||
                (d < 0 && _accumulatedScroll > 0))
            {
                _accumulatedScroll = 0;
            }

            _accumulatedScroll += d;

            if (_accumulatedScroll <= -1) {
                _accumulatedScroll = 0;
                _loadNext();
            }
            if (_accumulatedScroll >= 1) {
                _accumulatedScroll = 0;
                _loadPrevious();
            }

            event->accept();
            return;
        }
    }

    event->ignore();
}



void MainWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    _current->paint(painter);

    _layout->paint(painter);
}



void MainWidget::resizeEvent(QResizeEvent *) {
    _current->updateGeometry(size());

    if (!isProblemGeometryEvent()) {
        _layout->widgetResize(size(), mapFromGlobal(QCursor::pos()));
    }
}

void MainWidget::moveEvent(QMoveEvent *) {
    if (!isProblemGeometryEvent()) {
        _layout->widgetMove(mapFromGlobal(QCursor::pos()));
    }
}



void MainWidget::_loadNext() {
    if (!_application.imageListEmpty()) {
        _stopDragging();
        _currentIndex = _application.nextIndex(_currentIndex);
        _loadImage(_application.imagePathName(_currentIndex));
    }
}

void MainWidget::_loadPrevious() {
    if (!_application.imageListEmpty()) {
        _stopDragging();
        _currentIndex = _application.previousIndex(_currentIndex);
        _loadImage(_application.imagePathName(_currentIndex));
    }
}

void MainWidget::_setImage() {
    QPixmap * image = nullptr;
    QMovie * movie = nullptr;

    _imageLoader.getResult(image, movie);

    unsetCursor();

    if (image) {
        _current->setImage(image);
        _layout->enableGifMode(false, mapFromGlobal(QCursor::pos()));
        update();
    }
    else if (movie) {
        connect(movie, &QMovie::frameChanged, this, [this](){this->update();});
        movie->setCacheMode(QMovie::CacheAll);
        movie->start();

        _current->setMovie(movie);
        _layout->playPause->setSwitched(false);
        _layout->enableGifMode(true, mapFromGlobal(QCursor::pos()));
        update();
    }
    else {
        _setTitle(true);
        _layout->enableGifMode(false, mapFromGlobal(QCursor::pos()));
    }
}

void MainWidget::_changeFullScreen() {
    changeFullscreenMode();
    _stopDragging();
    _layout->fullScreen->setSwitched(isFullScreen());
}

void MainWidget::_browseCurrentFile() {
    _stopDragging();

    const QString currentFilePath = _application.imagePathName(_currentIndex);

    const QFileInfo file(currentFilePath);
    if (!file.exists()) return;

    QStringList args;
    args << "/select," << QDir::toNativeSeparators(currentFilePath);

    QProcess * process = new QProcess(this);
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)),
            process, SLOT(deleteLater()));
    process->start("explorer.exe", args);
}

void MainWidget::_pinOnTop() {
    changeKeepOnTopMode();
    _layout->pin->setSwitched(isKeepOnTopEnable());
}

void MainWidget::_setPaused(bool paused) {
    _current->setPaused(paused);
    _layout->playPause->setSwitched(paused);
}

void MainWidget::_jumpToNextFrame() {
    _current->jumpToNextFrame();
    _setPaused(_current->isPaused());
}

void MainWidget::_jumpToPreviousFrame() {
    _current->jumpToPreviousFrame();
    _setPaused(_current->isPaused());
}

void MainWidget::_openSettings() {
    if (!_settings) {
        _settings = new SettingsWidget(this, _config);
    }
    _settings->show();
    _settings->activateWindow();
}



void MainWidget::_setTitle(bool error) {
    _loadingError = error;
    QString title;

    if (_config.enablePictureCount) {
        title += '(';
        if (_application.imageListSize() == 0) {
            title += '0';
        }
        else {
            title += QString::number(_currentIndex + 1);
        }
        title += '/' + QString::number(_application.imageListSize()) + ") ";
    }
    if (error) {
        title += "Error: ";
    }

    setWindowTitle(title + _application.imageName(_currentIndex));
}

void MainWidget::_stopDragging() {
    if (_isDraggingImage) {
        _isDraggingImage = false;
        if (_draggingCursorEnable) {
            unsetCursor();
            _draggingCursorEnable = false;
        }
    }
}

void MainWidget::_loadImage(const QString & filePath) {
    _setTitle(false);
    setCursor(Qt::BusyCursor);

    _current->clear();
    _imageLoader.load(filePath);

    update();
}



void MainWidget::_createShortcuts() {
    new QShortcut(_config.keyNext[0], this, this, &MainWidget::_loadNext);
    new QShortcut(_config.keyNext[1], this, this, &MainWidget::_loadNext);
    new QShortcut(_config.keyNext[2], this, this, &MainWidget::_loadNext);

    new QShortcut(_config.keyPrevious[0], this,
                  this, &MainWidget::_loadPrevious);
    new QShortcut(_config.keyPrevious[1], this,
                  this, &MainWidget::_loadPrevious);
    new QShortcut(_config.keyPrevious[2], this,
                  this, &MainWidget::_loadPrevious);

    auto funScaleUp = [this](){
        _stopDragging();
        _current->scaleByWidgetCenter(1);
        update();
    };
    new QShortcut(_config.keyScaleUp[0], this, this, funScaleUp);
    new QShortcut(_config.keyScaleUp[1], this, this, funScaleUp);

    auto funScaleDown = [this](){
        _stopDragging();
        _current->scaleByWidgetCenter(-1);
        update();
    };
    new QShortcut(_config.keyScaleDown[0], this, this, funScaleDown);
    new QShortcut(_config.keyScaleDown[1], this, this, funScaleDown);

    new QShortcut(_config.keyNextFrame, this,
                  this, &MainWidget::_jumpToNextFrame);
    new QShortcut(_config.keyPreviousFrame, this,
                  this, &MainWidget::_jumpToPreviousFrame);

    new QShortcut(_config.keyClose, this, this, &MainWidget::close);

    QShortcut * s;

    s = new QShortcut(_config.keyChangeFullScreenState, this,
                      this, &MainWidget::_changeFullScreen);
    s->setAutoRepeat(false);

    s = new QShortcut(_config.keyScreenStateDown, this, this, [this](){
        if (changeDownScreenMode(_config.enableNormalazeFromMaximize,
                                 _config.enableHiding))
        {
            _stopDragging();
            _layout->fullScreen->setSwitched(false);
        }
    });
    s->setAutoRepeat(false);

    auto funPaused = [this](){
        _setPaused(!_current->isPaused());
    };
    s = new QShortcut(_config.keyChangePaused[0], this, this, funPaused);
    s->setAutoRepeat(false);
    s = new QShortcut(_config.keyChangePaused[1], this, this, funPaused);
    s->setAutoRepeat(false);

    s = new QShortcut(_config.keyPinOnTop, this, this, &MainWidget::_pinOnTop);
    s->setAutoRepeat(false);

    s = new QShortcut(_config.keyBrowse, this,
                      this, &MainWidget::_browseCurrentFile);
    s->setAutoRepeat(false);

    s = new QShortcut(_config.keySettings, this,
                      this, &MainWidget::_openSettings);
    s->setAutoRepeat(false);
}
