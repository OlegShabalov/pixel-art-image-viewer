#ifndef GUIPAGE_HPP
#define GUIPAGE_HPP

#include <QWidget>
#include <QLabel>

#include "src/ConfigItem.hpp"



class GuiPage : public QWidget {
    Q_OBJECT

public:
    GuiPage(ConfigItem & config);

private Q_SLOTS:
    void _guiScaleChanged(int value);

private:
    ConfigItem & _config;

    QLabel * _guiScaleValue;
};



#endif // GUIPAGE_HPP
