#include "ConfigItem.hpp"



ConfigItem::ConfigItem()
    : useCursorAsScaleCenter(true)
    , windowResizeStrategy(ScaleFragment)
    , enableGesturesToScroll(true)
    , moveMarginInPixels(20)
    , maxPixelSize(400)
    , minImageSize(100)
    , enableOneToOneScaling(true)

    , fixImageWhenWindowMaximized(false)
    , fixImageWhenWindowFullScreen(true)

    , enableHiding(true)
    , enableNormalazeFromMaximize(true)

    , enableHidingHoveredButtons(false)
    , buttonsBackgroundColor({25, 10, 15, 160})
    , guiScale(0.8)

    , enablePictureCount(true)

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
{  }
