#include "ConfigItem.hpp"



ConfigItem::ConfigItem()
    : useCursorAsScaleCenter(true) //
    , maxPixelSize(400) //
    , minImageSize(100) //
    , enableOneToOneScaling(true) //

    , moveMarginInPixels(20) //

    , fixImageWhenWindowMaximized(false) //
    , fixImageWhenWindowFullScreen(true) //
    , windowResizeStrategy(ScaleFragment) //

    , enableHiding(true)
    , enableNormalazeFromMaximize(true)

    , enableHidingHoveredButtons(false)
    , buttonsBackgroundColor({25, 10, 15, 160})
    , guiScale(0.8)

    , enablePictureCount(true)
    , enableGesturesToScroll(true)

    , keyNext                  {Qt::Key_Right, Qt::Key_D, Qt::Key_L}
    , keyPrevious              {Qt::Key_Left,  Qt::Key_A, Qt::Key_J}
    , keyScaleUp               {Qt::Key_Up, Qt::Key_W}
    , keyScaleDown             {Qt::Key_Down, Qt::Key_S}
    , keyBrowse                {Qt::CTRL + Qt::Key_E}
    , keyChangeFullScreenState {Qt::Key_F}
    , keyScreenStateDown       {Qt::Key_Escape}
    , keyPinOnTop              {Qt::Key_P}
    , keyChangePaused          {Qt::Key_Space, Qt::Key_K}
    , keyNextFrame             {Qt::Key_Period}
    , keyPreviousFrame         {Qt::Key_Comma}
    , keyClose                 {Qt::CTRL + Qt::Key_Q}
    , keySettings              {Qt::CTRL + Qt::Key_I}
{  }



void ConfigItem::setUseCursorAsScaleCenter(bool value) {
    useCursorAsScaleCenter = value;
}
void ConfigItem::setMaxPixelSize(int value) {
    if (maxPixelSize != value) {
        maxPixelSize = value;
        Q_EMIT minMaxImageSizeChanged();
    }
}
void ConfigItem::setMinImageSize(int value) {
    if (minImageSize != value) {
        minImageSize = value;
        Q_EMIT minMaxImageSizeChanged();
    }
}
void ConfigItem::setEnableOneToOneScaling(bool enable) {
    if (enableOneToOneScaling != enable) {
        enableOneToOneScaling = enable;
        Q_EMIT minMaxImageSizeChanged();
    }
}
void ConfigItem::setMoveMarginInPixels(int value) {
    moveMarginInPixels = value;
}
void ConfigItem::setFixImageWhenWindowMaximized(bool enable) {
    fixImageWhenWindowMaximized = enable;
}
void ConfigItem::setFixImageWhenWindowFullScreen(bool enable) {
    fixImageWhenWindowFullScreen = enable;
}
void ConfigItem::setWindowResizeStrategy(char strategy) {
    windowResizeStrategy = strategy;
}
