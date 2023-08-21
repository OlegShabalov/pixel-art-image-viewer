#include "SettingsWidget.hpp"

#include <QGridLayout>
#include <QScrollBar>
#include <QShortcut>



SettingsWidget::SettingsWidget(QWidget * parentWindow, ConfigItem & config)
    : WinWindow("localconfig/winpos", "SettWidget")
    , _config(config)
{
    setParent(parentWindow);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    QGridLayout * layout = new QGridLayout(this);

    _menu = new QListWidget(this);
    _menu->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    layout->addWidget(_menu, 1, 0);

    _title = new QLabel(this);
    layout->addWidget(_title, 0, 1);

    _pages = new QStackedWidget(this);
    layout->addWidget(_pages, 1, 1);

    _setTitle(parentWindow->windowTitle());
    connect(parentWindow, &QWidget::windowTitleChanged,
            this, &SettingsWidget::_setTitle);

    connect(_menu, &QListWidget::currentRowChanged,
            this, &SettingsWidget::_selectPage);

    _createShortcuts();

    _createPages();
}

void SettingsWidget::addPage(QWidget * widget, const QString & title) {
    _pages->addWidget(widget);
    _menu->addItem(title);
    _menu->item(0)->setSelected(true);

    _menu->setMaximumWidth(_menu->sizeHintForColumn(0) + _menu->frameWidth() +
                           _menu->verticalScrollBar()->height() + 2);
}

void SettingsWidget::_selectPage(int index) {
    _pages->setCurrentIndex(index);
    _title->setText("<h2>" + _menu->item(index)->text() + "</h2>");
}

void SettingsWidget::_setTitle(const QString & title) {
    setWindowTitle("Settings: " + title);
}

void SettingsWidget::_createShortcuts() {
    QShortcut * s;

    s = new QShortcut(_config.keyScreenStateDown, this, this, &QWidget::close);
    s->setAutoRepeat(false);
    s = new QShortcut(_config.keyClose, this, this, &QWidget::close);
    s->setAutoRepeat(false);
}



void SettingsWidget::_createPages() {
    addPage(new QLabel("Test1"), "Test1");
    addPage(new QLabel("Test2"), "Test2");
}
