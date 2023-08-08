#ifndef WINWINDOW_HPP
#define WINWINDOW_HPP

#include <QWidget>



class WinWindow : public QWidget {
    Q_OBJECT

public:
    WinWindow(const QString & settingsPath,
              const QString & windowIdentifier = "",
              int defaultWidth = 600, int defaultHeight = 400,
              QWidget * parent = nullptr);

    void setMaximizedSizeRestoring(bool enable);
    bool changeDownScreenMode(bool enableNormalazeFromMaximize = true,
                              bool enableHiding = false);

    inline bool isKeepOnTopEnable() const;
    inline bool isProblemGeometryEvent() const;

public Q_SLOTS:
    void changeFullscreenMode();
    bool setFullscreenMode(bool enable);

    void changeKeepOnTopMode();
    bool setKeepOnTopMode(bool enable);

protected:
    void closeEvent(QCloseEvent * event) override;

private:
    void _setKeepOnTopMode(bool enable);
    void _loadGeometry();
    void _saveGeometry();

private:
    void showFullScreen() = delete;

private:
    const QString _settingsPath;
    const QString _windowIdentifier;
    bool _isMaximizedSizeRestoring = true;
    bool _isMaximizedBeforeFullScreen = false;
    bool _isKeepOnTopEnable = false;
    bool _isProblemGeometryEvent = false;
};





inline bool WinWindow::isKeepOnTopEnable() const {
    return _isKeepOnTopEnable;
}

inline bool WinWindow::isProblemGeometryEvent() const {
    return _isProblemGeometryEvent;
}



#endif // WINWINDOW_HPP
