#ifndef SETTINGSWIDGET_HPP
#define SETTINGSWIDGET_HPP

#include <QListWidget>
#include <QLabel>
#include <QStackedWidget>

#include "WinWindow.hpp"
#include "ConfigItem.hpp"



class SettingsWidget : public WinWindow {
    Q_OBJECT

public:
    SettingsWidget(QWidget * parentWindow, ConfigItem & config);

    void addPage(QWidget * widget, const QString & title);

protected:
    void closeEvent(QCloseEvent * event) override;

private Q_SLOTS:
    void _selectPage(int index);
    void _setTitle(const QString & title);
    void _byDefaultButtonSlot();
    void _applyButtonSlot();

private:
    void _createShortcuts();
    void _createPages();

private:
    QListWidget * _menu;
    QLabel * _title;
    QStackedWidget * _pages;

    ConfigItem & _config;
    ConfigItem _backupConfig;
};



#endif // SETTINGSWIDGET_HPP
