#ifndef CONFIGITEM_HPP
#define CONFIGITEM_HPP

#include <QObject>
#include <QColor>
#include <QKeySequence>



class ConfigItem : public QObject {
    Q_OBJECT

public:
    enum WindowResizeStrategy : char {
        NoStrategy,
        FocusOnCenter,
        ScaleFragment,
    };

public:
    ConfigItem();

public Q_SLOTS:
    void setUseCursorAsScaleCenter(bool value);
    void setMaxPixelSize(int value);
    void setMinImageSize(int value);
    void setEnableOneToOneScaling(bool enable);
    void setMoveMarginInPixels(int value);
    void setFixImageWhenWindowMaximized(bool enable);
    void setFixImageWhenWindowFullScreen(bool enable);
    void setWindowResizeStrategy(char strategy);
    void setEnableHiding(bool enable);
    void setEnableNormalazeFromMaximize(bool enable);
    void setEnablePictureCount(bool enable);
    void setEnableHidingHoveredButtons(bool enable);
    void setGuiScale(float value);
    void setButtonsBackgroundColor(const QColor & color);
    void setWindowBackgroundColor(const QColor & color);

Q_SIGNALS:
    void minMaxImageSizeChanged();
    void moveMarginChanged();
    void pictureCountingChanged();
    void guiScaleChanged();
    void buttonsBackgroundColorChanged();
    void windowBackgroundColorChanged();

public:
    bool useCursorAsScaleCenter;
    int maxPixelSize;
    int minImageSize;
    bool enableOneToOneScaling;

    int moveMarginInPixels;

    bool fixImageWhenWindowMaximized;
    bool fixImageWhenWindowFullScreen;
    char windowResizeStrategy;

    bool enableHiding;
    bool enableNormalazeFromMaximize;

    /*
    bool useDecreasingBestSmooth = true;
    bool useDecreasingSmooth = true;
    bool useIncreasingSmooth = false;
    */

    bool enableHidingHoveredButtons;
    float guiScale;
    bool enablePictureCount;
    QColor buttonsBackgroundColor;
    QColor windowBackgroundColor;

    bool enableGesturesToScroll;

    QKeySequence keyNext[3];
    QKeySequence keyPrevious[3];
    QKeySequence keyScaleUp[2];
    QKeySequence keyScaleDown[2];
    QKeySequence keyBrowse;
    QKeySequence keyChangeFullScreenState;
    QKeySequence keyScreenStateDown;
    QKeySequence keyPinOnTop;
    QKeySequence keyChangePaused[2];
    QKeySequence keyNextFrame;
    QKeySequence keyPreviousFrame;
    QKeySequence keyClose;
    QKeySequence keySettings;
};



#endif // CONFIGITEM_HPP
