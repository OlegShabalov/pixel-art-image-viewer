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

    , enableHiding(true) //
    , enableNormalazeFromMaximize(true) //

    , enableHidingHoveredButtons(false) //
    , guiScale(0.8) //
    , enablePictureCount(true) //
    , buttonsBackgroundColor({25, 10, 15, 160})

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
    if (moveMarginInPixels != value) {
        moveMarginInPixels = value;
        Q_EMIT moveMarginChanged();
    }
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
void ConfigItem::setEnableHiding(bool enable) {
    enableHiding = enable;
}
void ConfigItem::setEnableNormalazeFromMaximize(bool enable) {
    enableNormalazeFromMaximize = enable;
}
void ConfigItem::setEnablePictureCount(bool enable) {
    if (enablePictureCount != enable) {
        enablePictureCount = enable;
        Q_EMIT pictureCountingChanged();
    }
}
void ConfigItem::setEnableHidingHoveredButtons(bool enable) {
    enableHidingHoveredButtons = enable;
}
void ConfigItem::setGuiScale(float value) {
    if (guiScale != value) {
        guiScale = value;
        Q_EMIT guiScaleChanged();
    }
}
void ConfigItem::setButtonsBackgroundColor(const QColor & color) {
    if (buttonsBackgroundColor != color) {
        buttonsBackgroundColor = color;
        Q_EMIT buttonsBackgroundColorChanged();
    }
}
