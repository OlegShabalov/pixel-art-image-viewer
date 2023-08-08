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
    bool useCursorAsScaleCenter = true;
    char windowResizeStrategy = ScaleFragment;

    bool enableGesturesToScroll = true;

    int moveMarginInPixels = 20;

    int maxPixelSize = 400;
    int minImageSize = 100;
    bool enableOneToOneScaling = true;

    bool fixImageWhenWindowMaximized = true;
    bool fixImageWhenWindowFullScreen = false;

    bool enableHiding = true;
    bool enableNormalazeFromMaximize = true;

    bool enableHidingHoveredButtons = false;

    /*
    bool useDecreasingBestSmooth = true;
    bool useDecreasingSmooth = true;
    bool useIncreasingSmooth = false;
    */

    QColor buttonsBackgroundColor {25, 10, 15, 160};

    float guiScale = 0.8;

    bool enablePictureCount = true;



    QKeySequence keyNext      {Qt::Key_Right, Qt::Key_D, Qt::Key_L};
    QKeySequence keyPrevious  {Qt::Key_Left,  Qt::Key_A, Qt::Key_J};
    QKeySequence keyScaleUp   {Qt::Key_Up, Qt::Key_W};
    QKeySequence keyScaleDown {Qt::Key_Down, Qt::Key_S};
    QKeySequence keyBrowse    {Qt::CTRL + Qt::Key_E};
    QKeySequence keyChangeFullScreenState {Qt::Key_F};
    QKeySequence keyScreenStateDown       {Qt::Key_Escape};
    QKeySequence keyPinOnTop  {Qt::Key_P};
    QKeySequence keyChangePaused  {Qt::Key_Space, Qt::Key_K};
    QKeySequence keyNextFrame     {Qt::Key_Period};
    QKeySequence keyPreviousFrame {Qt::Key_Comma};
    QKeySequence keyClose {Qt::CTRL + Qt::Key_Q};
};



#endif // CONFIGITEM_HPP
