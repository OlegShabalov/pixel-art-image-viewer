#include "SettingsWidget.hpp"

#include <QGridLayout>
#include <QScrollBar>
#include <QShortcut>
#include <QHBoxLayout>
#include <QPushButton>

#include "ImagePage.hpp"
#include "GuiPage.hpp"
#include "WindowPage.hpp"



SettingsWidget::SettingsWidget(QWidget * parentWindow, ConfigItem & config)
    : WinWindow("localconfig/winpos", "SettWidget")
    , _config(config)
{
    _backupConfig.copyData(config);

    setParent(parentWindow);
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    QGridLayout * layout = new QGridLayout(this);

    _menu = new QListWidget(this);
    _menu->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    layout->addWidget(_menu, 1, 0);

    _title = new QLabel(this);
    layout->addWidget(_title, 0, 1);

    _pages = new QStackedWidget(this);
    layout->addWidget(_pages, 1, 1);

    QHBoxLayout * buttonsLayout = new QHBoxLayout;
    layout->addLayout(buttonsLayout, 2, 1);

    _createPages();

    QPushButton * resetButton = new QPushButton(tr("By default"), this);
    connect(resetButton, &QPushButton::clicked,
            this, &SettingsWidget::_byDefaultButtonSlot);
    buttonsLayout->addWidget(resetButton);

    buttonsLayout->addStretch();

    QPushButton * applyButton = new QPushButton(tr("Apply"), this);
    applyButton->setEnabled(false);
    connect(&config.thereAreChanges, &Changes::changed,
            applyButton, &QWidget::setEnabled);
    connect(applyButton, &QPushButton::clicked,
            this, &SettingsWidget::_applyButtonSlot);
    buttonsLayout->addWidget(applyButton);

    QPushButton * cancelButton = new QPushButton(tr("Cancel"), this);
    cancelButton->setAutoDefault(true);
    cancelButton->setDefault(true);
    cancelButton->setShortcut(Qt::Key_Return);
    connect(cancelButton, &QPushButton::clicked, this, &QWidget::close);
    buttonsLayout->addWidget(cancelButton);

    _setTitle(parentWindow->windowTitle());
    connect(parentWindow, &QWidget::windowTitleChanged,
            this, &SettingsWidget::_setTitle);

    connect(_menu, &QListWidget::currentRowChanged,
            this, &SettingsWidget::_selectPage);

    _createShortcuts();
}

void SettingsWidget::addPage(QWidget * widget, const QString & title) {
    _pages->addWidget(widget);
    _menu->addItem(title);
    _menu->item(0)->setSelected(true);

    _menu->setMaximumWidth(_menu->sizeHintForColumn(0) + _menu->frameWidth() +
                           _menu->verticalScrollBar()->height() + 2);
}



void SettingsWidget::closeEvent(QCloseEvent * event) {
    if (_config.thereAreChanges) {
        _config.copyData(_backupConfig);
        _config.thereAreChanges.set(false);
    }
    WinWindow::closeEvent(event);
}



void SettingsWidget::_selectPage(int index) {
    _pages->setCurrentIndex(index);
    _title->setText("<h2>" + _menu->item(index)->text() + "</h2>");
}

void SettingsWidget::_setTitle(const QString & title) {
    setWindowTitle("Settings: " + title);
}

void SettingsWidget::_byDefaultButtonSlot() {
    ConfigItem defaultConfig;
    _config.copyData(defaultConfig);
}

void SettingsWidget::_applyButtonSlot() {
    if (_config.thereAreChanges) {
        _backupConfig.copyData(_config);
        _config.thereAreChanges.set(false);
        _config.saveData("localconfig/settings");
    }
    close();
}

void SettingsWidget::_createShortcuts() {
    QShortcut * s;

    s = new QShortcut(_config.keyScreenStateDown, this, this, &QWidget::close);
    s->setAutoRepeat(false);
    s = new QShortcut(_config.keyClose, this, this, &QWidget::close);
    s->setAutoRepeat(false);
}



void SettingsWidget::_createPages() {
    addPage(new ImagePage(_config), tr("Image"));
    addPage(new GuiPage(_config), tr("GUI"));
    addPage(new WindowPage(_config), tr("Window"));
}
