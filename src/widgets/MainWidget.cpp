#include "MainWidget.hpp"

#include <QPainter>
#include <QFileInfo>
#include <QDir>
#include <QCollator>
#include <QMouseEvent>
#include <QMovie>
#include <QProcess>

#include "Math.hpp"



MainWidget::MainWidget(Application & application, int imageIndex)
    : WinWindow("localconfig/winpos", "MainWidget")
    , _application(application)
    , _currentIndex(imageIndex)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);

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

    connect(&_imageLoader, &ImageLoader::finished,
            this, &MainWidget::_setImage);

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



void MainWidget::keyPressEvent(QKeyEvent * event) {
    if (_keyMatches(_config.keyNext, event)) {
        _loadNext();
        event->accept();
        return;
    }
    if (_keyMatches(_config.keyPrevious, event)) {
        _loadPrevious();
        event->accept();
        return;
    }

    if (_keyMatches(_config.keyScaleUp, event)) {
        _stopDragging();
        _current->scaleByWidgetCenter(1);
        update();
        event->accept();
        return;
    }
    if (_keyMatches(_config.keyScaleDown, event)) {
        _stopDragging();
        _current->scaleByWidgetCenter(-1);
        update();
        event->accept();
        return;
    }

    if (_keyMatches(_config.keyNextFrame, event)) {
        _jumpToNextFrame();
        event->accept();
        return;
    }
    if (_keyMatches(_config.keyPreviousFrame, event)) {
        _jumpToPreviousFrame();
        event->accept();
        return;
    }

    if (_keyMatches(_config.keyClose, event)) {
        close();
        event->accept();
        return;
    }

    if (!event->isAutoRepeat()) {
        if (_keyMatches(_config.keyChangeFullScreenState, event)) {
            _changeFullScreen();
            event->accept();
            return;
        }

        if (_keyMatches(_config.keyScreenStateDown, event)) {
            if (changeDownScreenMode(_config.enableNormalazeFromMaximize,
                                     _config.enableHiding))
            {
                _stopDragging();
                _layout->fullScreen->setSwitched(false);
            }
        }

        if (_keyMatches(_config.keyChangePaused, event)) {
            _setPaused(!_current->isPaused());
            event->accept();
            return;
        }

        if (_keyMatches(_config.keyPinOnTop, event)) {
            _pinOnTop();
            event->accept();
            return;
        }

        if (_keyMatches(_config.keyBrowse, event)) {
            _stopDragging();
            _browseCurrentFile();
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

bool MainWidget::event(QEvent * event) {
    if (event->type() == QEvent::WindowStateChange) {
        const QWindowStateChangeEvent * windowStateEvent =
            static_cast<QWindowStateChangeEvent *>(event);

        if (!((windowState() & Qt::WindowMinimized) ||
            (windowStateEvent->oldState() & Qt::WindowMinimized)))
        {
            if (_config.fixImageWhenWindowMaximized) {
                if (windowState() == Qt::WindowMaximized  ||
                    windowStateEvent->oldState() == Qt::WindowMaximized)
                {
                    _current->fixImage();
                    update();
                }
            }
            if (_config.fixImageWhenWindowFullScreen) {
                if (windowState() == Qt::WindowFullScreen  ||
                    windowStateEvent->oldState() == Qt::WindowFullScreen)
                {
                    _current->fixImage();
                    update();
                }
            }
        }
    }

    return QWidget::event(event);
}



bool MainWidget::_keyMatches(const QKeySequence & seq,
                             const QKeyEvent * event)
{
    for (char i=0; i<seq.count(); ++i) {
        if (seq[i] == (event->key() | static_cast<int>(event->modifiers()))) {
            return true;
        }
    }
    return false;
}

void MainWidget::_setTitle(bool error) {
    QString title = "(";

    if (_config.enablePictureCount) {
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
    _setTitle();
    setCursor(Qt::BusyCursor);

    _current->clear();
    _imageLoader.load(filePath);

    update();
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
