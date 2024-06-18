#include "WinWindow.hpp"

#include <QSettings>
#include <QApplication>
#include <QDir>
#include <QWindowStateChangeEvent>

#include <windows.h>



WinWindow::WinWindow(const QString & settingsPath,
                     const QString & windowIdentifier,
                     int defaultWidth, int defaultHeight,
                     QWidget * parent)
    : QWidget(parent)
    , _settingsPath(settingsPath)
    , _windowIdentifier(windowIdentifier)
{
    resize(defaultWidth, defaultHeight);
}



void WinWindow::setMaximizedSizeRestoring(bool enable) {
    _isMaximizedSizeRestoring = enable;
}



bool WinWindow::changeDownScreenMode(bool enableNormalazeFromMaximize,
                                     bool enableHiding)
{
    if (isFullScreen()) {
        changeFullscreenMode();
        return true;
    }
    if (enableNormalazeFromMaximize && isMaximized()) {
        showNormal();
        return true;
    }
    if (enableHiding) {
        showMinimized();
        return true;
    }
    return false;
}



void WinWindow::changeFullscreenMode() {
    if (isFullScreen()) {
        if (_isMaximizedBeforeFullScreen) {
            _isProblemGeometryEvent = true;
            _ignoreMaximizedEvent = true;
            showMaximized();
            _isProblemGeometryEvent = false;
        }
        else {
            showNormal();
        }

        if (_isKeepOnTopEnable) {
            _setKeepOnTopMode(true);
        }
    }
    else {
        _saveGeometry();

        if (_isKeepOnTopEnable) {
            _setKeepOnTopMode(false);
        }

        if (isMaximized()) {
            _isMaximizedBeforeFullScreen = true;
            QWidget::showFullScreen();
        }
        else {
            _isMaximizedBeforeFullScreen = false;

            _isProblemFullscreenChangeEvent = true;
            //setVisible(false);
            showMaximized();
            QWidget::showFullScreen();
            //setVisible(true);
            _isProblemFullscreenChangeEvent = false;
        }
    }
}

bool WinWindow::setFullscreenMode(bool enable) {
    if (isFullScreen() == enable) {
        return false;
    }
    changeFullscreenMode();
    return true;
}



void WinWindow::changeKeepOnTopMode() {
    _isKeepOnTopEnable = !_isKeepOnTopEnable;
    if (!isFullScreen()) {
        _setKeepOnTopMode(_isKeepOnTopEnable);
    }
}

bool WinWindow::setKeepOnTopMode(bool enable) {
    if (_isKeepOnTopEnable == enable) {
        return false;
    }
    changeKeepOnTopMode();
    return true;
}

void WinWindow::setVisible(bool visible) {
    if (_ignoreSettingVisible) return;

    if (visible && !isVisible() && !isMinimized()) {
        _loadGeometry();
    }

    QWidget::setVisible(visible);
}



void WinWindow::closeEvent(QCloseEvent * event) {
    if (!isFullScreen() && isVisible()) {
        _saveGeometry();
    }
    QWidget::closeEvent(event);
}



bool WinWindow::event(QEvent * event) {
    if (event->type() != QEvent::WindowStateChange) {
        return QWidget::event(event);
    }

    const QWindowStateChangeEvent * windowStateEvent =
        static_cast<QWindowStateChangeEvent *>(event);

    if (windowState() & Qt::WindowMinimized ||
        windowStateEvent->oldState() & Qt::WindowMinimized)
    {
        Q_EMIT minimizedState();
    }
    else {
        if (windowState() == Qt::WindowFullScreen  ||
            windowStateEvent->oldState() == Qt::WindowFullScreen)
        {
            Q_EMIT fullscreenState();
        }
        else if (windowState() == Qt::WindowMaximized  ||
                 windowStateEvent->oldState() == Qt::WindowMaximized)
        {
            if (_ignoreMaximizedEvent) {
                _ignoreMaximizedEvent = false;
            }
            else if (!_isProblemFullscreenChangeEvent) {
                Q_EMIT maximizedState();
            }
        }
    }

    return QWidget::event(event);
}



void WinWindow::_setKeepOnTopMode(bool enable) {
    const HWND state = enable ? HWND_TOPMOST : HWND_NOTOPMOST;
    SetWindowPos(HWND(winId()), state, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}



void WinWindow::_loadGeometry() {
    const QString settingsPath =
        QDir(QApplication::applicationDirPath()).filePath(_settingsPath);

    QSettings settings(settingsPath, QSettings::IniFormat);
    settings.beginGroup(_windowIdentifier);

    const QByteArray ba = QByteArray::fromBase64(
        settings.value("WindowPlacement").toByteArray()
    );

    settings.endGroup();

    if (!ba.isEmpty()) {
        const WINDOWPLACEMENT * placement =
            reinterpret_cast<const WINDOWPLACEMENT *>(ba.constData());

        if (placement->showCmd == SW_SHOWMAXIMIZED) {
            _ignoreSettingVisible = true;
            showMaximized();
            _ignoreSettingVisible = false;
            QWidget::setVisible(true);
        }

        SetWindowPlacement(HWND(winId()), placement);
    }
}

void WinWindow::_saveGeometry() {
    WINDOWPLACEMENT placement;
    placement.length = sizeof(WINDOWPLACEMENT);

    if (GetWindowPlacement(HWND(winId()), &placement)) {
        if (_isMaximizedSizeRestoring && isMaximized()) {
            placement.showCmd = SW_SHOWMAXIMIZED;
        }
        else {
            placement.showCmd = SW_SHOWNORMAL;
        }

        const char * const p = reinterpret_cast<const char * >(&placement);
        const QByteArray ba(p, sizeof(WINDOWPLACEMENT));

        const QString settingsPath =
            QDir(QApplication::applicationDirPath()).filePath(_settingsPath);

        QSettings settings(settingsPath, QSettings::IniFormat);
        settings.beginGroup(_windowIdentifier);
        settings.setValue("WindowPlacement", ba.toBase64());
        settings.endGroup();
    }
}
