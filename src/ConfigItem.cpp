#include "ConfigItem.hpp"

#include <QSettings>
#include <QApplication>
#include <QDir>



void Changes::set(bool value) {
    if (value != _value) {
        _value = value;
        Q_EMIT changed(value);
    }
}





ConfigItem::ConfigItem()
    : useCursorAsScaleCenter(true, "UseCursorAsScaleCenter", this) //
    , enableOneToOneScaling(true, "EnableOneToOneScaling", this) //
    , maxPixelSize(400, "MaxPixelSize", this) //
    , minImageSize(100, "MinImageSize", this) //

    , moveMarginInPixels(20, "MoveMarginInPixels", this) //

    , fixImageWhenWindowMaximized(false, "FixImageWhenWindowMaximized", this) //
    , fixImageWhenWindowFullScreen(true, "FixImageWhenWindowFullScreen", this) //
    , windowResizeStrategy(ScaleFragment, "WindowResizeStrategy", this) //

    , enableHiding(true, "EnableHiding", this) //
    , enableNormalazeFromMaximize(true, "EnableNormalazeFromMaximize", this) //

    , enableHidingHoveredButtons(false, "EnableHidingHoveredButtons", this) //
    , guiScale(0.8f, "GuiScale", this) //
    , enablePictureCount(true, "EnablePictureCount", this) //
    , buttonsBackgroundColor({25, 10, 15, 160}, "ButtonsBackgroundColor", this) //
    , windowBackgroundColor({240, 240, 240}, "WindowBackgroundColor", this) //

    , enableGesturesToScroll(true, "EnableGesturesToScroll", this)

    , keyNext                  {Qt::Key_Right, Qt::Key_D, Qt::Key_L}
    , keyPrevious              {Qt::Key_Left,  Qt::Key_A, Qt::Key_J}
    , keyScaleUp               {Qt::Key_Up, Qt::Key_W}
    , keyScaleDown             {Qt::Key_Down, Qt::Key_S}
    , keyBrowse                {Qt::CTRL | Qt::Key_E}
    , keyChangeFullScreenState {Qt::Key_F}
    , keyScreenStateDown       {Qt::Key_Escape}
    , keyPinOnTop              {Qt::Key_P}
    , keyChangePaused          {Qt::Key_Space, Qt::Key_K}
    , keyNextFrame             {Qt::Key_Period}
    , keyPreviousFrame         {Qt::Key_Comma}
    , keyClose                 {Qt::CTRL | Qt::Key_Q}
    , keySettings              {Qt::CTRL | Qt::Key_I}
{  }

ConfigItem::ConfigItem(const QString & settingsFile)
    : ConfigItem()
{
    loadData(settingsFile);
}



void ConfigItem::copyData(const ConfigItem & from) {
    for (int i=0; i<_fields.size(); ++i) {
        _fields.at(i)->copy(from._fields.at(i));
    }
}

void ConfigItem::loadData(const QString & settingsFile) {
    const QString settingsPath =
        QDir(QApplication::applicationDirPath()).filePath(settingsFile);

    QSettings settings(settingsPath, QSettings::IniFormat);
    settings.beginGroup("Settings");

    for (int i=0; i<_fields.size(); ++i) {
        _fields.at(i)->load(settings);
    }

    settings.endGroup();
}

void ConfigItem::saveData(const QString & settingsFile) {
    const QString settingsPath =
        QDir(QApplication::applicationDirPath()).filePath(settingsFile);

    QSettings settings(settingsPath, QSettings::IniFormat);
    settings.beginGroup("Settings");

    for (int i=0; i<_fields.size(); ++i) {
        _fields.at(i)->save(settings);
    }

    settings.endGroup();
}

void ConfigItem::_addField(AbstractConfigField * field) {
    _fields.append(field);
}

void ConfigItem::_change() {
    thereAreChanges.set(true);
}
