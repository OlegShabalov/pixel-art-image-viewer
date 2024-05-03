#ifndef CONFIGITEM_HPP
#define CONFIGITEM_HPP

#include <QColor>
#include <QColor>
#include <QKeySequence>
#include <QVector>

#include "ConfigField.hpp"



class Changes : public QObject {
    Q_OBJECT
public:
    inline operator bool () const { return _value; }

public Q_SLOTS:
    void set(bool value);

Q_SIGNALS:
    void changed(bool value);

private:
    bool _value = false;
};





class ConfigItem {
public:
    enum WindowResizeStrategy : char {
        NoStrategy,
        FocusOnCenter,
        ScaleFragment,
    };

public:
    ConfigItem();
    ConfigItem(const QString & settingsFile);

    void copyData(const ConfigItem & from);
    void loadData(const QString & settingsFile);
    void saveData(const QString & settingsFile);

private:
    friend class AbstractConfigField;
    void _addField(AbstractConfigField * field);
    void _change();
    QList<AbstractConfigField *> _fields;

public:
    Changes thereAreChanges;

    BoolConfigField useCursorAsScaleCenter;
    BoolConfigField enableOneToOneScaling;
    IntConfigField maxPixelSize;
    IntConfigField minImageSize;

    IntConfigField moveMarginInPixels;

    BoolConfigField fixImageWhenWindowMaximized;
    BoolConfigField fixImageWhenWindowFullScreen;
    IntConfigField windowResizeStrategy;

    BoolConfigField enableHiding;
    BoolConfigField enableNormalazeFromMaximize;

    /*
    bool useDecreasingBestSmooth = true;
    bool useDecreasingSmooth = true;
    bool useIncreasingSmooth = false;
    */

    BoolConfigField enableHidingHoveredButtons;
    FloatConfigField guiScale;
    BoolConfigField enablePictureCount;
    ColorConfigField buttonsBackgroundColor;
    ColorConfigField windowBackgroundColor;

    BoolConfigField enableGesturesToScroll;

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
