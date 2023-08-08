#ifndef CONFIGITEM_HPP
#define CONFIGITEM_HPP

#include <QColor>
#include <QKeySequence>



class ConfigItem {
public:
    enum WindowResizeStrategy : char {
        NoStrategy,
        FocusOnCenter,
        ScaleFragment,
    };

public:
    ConfigItem();

public:
    bool useCursorAsScaleCenter;
    char windowResizeStrategy;

    bool enableGesturesToScroll;

    int moveMarginInPixels;

    int maxPixelSize;
    int minImageSize;
    bool enableOneToOneScaling;

    bool fixImageWhenWindowMaximized;
    bool fixImageWhenWindowFullScreen;

    bool enableHiding;
    bool enableNormalazeFromMaximize;

    bool enableHidingHoveredButtons;

    /*
    bool useDecreasingBestSmooth = true;
    bool useDecreasingSmooth = true;
    bool useIncreasingSmooth = false;
    */

    QColor buttonsBackgroundColor;

    float guiScale;

    bool enablePictureCount;



    QKeySequence keyNext;
    QKeySequence keyPrevious;
    QKeySequence keyScaleUp;
    QKeySequence keyScaleDown;
    QKeySequence keyBrowse;
    QKeySequence keyChangeFullScreenState;
    QKeySequence keyScreenStateDown;
    QKeySequence keyPinOnTop;
    QKeySequence keyChangePaused;
    QKeySequence keyNextFrame;
    QKeySequence keyPreviousFrame;
    QKeySequence keyClose;
};



#endif // CONFIGITEM_HPP
