#include "GuiPage.hpp"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSlider>
#include <QScrollArea>

#include "ColorButton.hpp"
#include "ColorDialog.hpp"



GuiPage::GuiPage(ConfigItem & config)
    : _config(config)
{
    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    QWidget * content = new QWidget;

    QVBoxLayout * layout = new QVBoxLayout(content);



    QGroupBox * guiBox = new QGroupBox(tr("GUI"), this);
    layout->addWidget(guiBox);



    QVBoxLayout * guiLayout = new QVBoxLayout;
    guiBox->setLayout(guiLayout);

    QHBoxLayout * scaleLayout = new QHBoxLayout;
    guiLayout->addLayout(scaleLayout);

    QLabel * guiScaleLabel = new QLabel(tr("Scale:"), this);
    scaleLayout->addWidget(guiScaleLabel);

    QSlider * guiScale = new QSlider(Qt::Horizontal, this);
    guiScale->setRange(0.1f * 100, 4 * 100.0f);
    guiLayout->addWidget(guiScale);
    scaleLayout->addWidget(guiScale);
    guiScale->setValue(_config.guiScale * 100);
    connect(guiScale, &QSlider::valueChanged,
            this, &GuiPage::_guiScaleChanged);
    connect(&config.guiScale, &FloatConfigField::changed,
            this, [guiScale](float value)
    {
        guiScale->setValue(value * 100.0f);
    });

    _guiScaleValue = new QLabel(this);
    scaleLayout->addWidget(_guiScaleValue);




    QCheckBox * enableHidingHoveredButtons =
        new QCheckBox(tr("Enable hiding hovered buttons"), this);
    guiLayout->addWidget(enableHidingHoveredButtons);
    config.enableHidingHoveredButtons
        .connectCheckBox(enableHidingHoveredButtons);



    QGroupBox * titleBox = new QGroupBox(tr("Title"), this);
    layout->addWidget(titleBox);

    QVBoxLayout * titleLayout = new QVBoxLayout;
    titleBox->setLayout(titleLayout);

    QCheckBox * enablePictureCount =
        new QCheckBox(tr("Enable image count"), this);
    titleLayout->addWidget(enablePictureCount);
    config.enablePictureCount.connectCheckBox(enablePictureCount);



    QGroupBox * colorBox = new QGroupBox(tr("Colors"), this);
    layout->addWidget(colorBox);

    QGridLayout * colorLayout = new QGridLayout;
    colorLayout->setColumnStretch(2, 1);
    colorBox->setLayout(colorLayout);



    QLabel * backgroundColorLabel =
        new QLabel(tr("Buttons background color:"), this);
    colorLayout->addWidget(backgroundColorLabel, 0, 0);

    ColorButton * backgroundColorButton =
        new ColorButton(_config.buttonsBackgroundColor());

    connect(&config.buttonsBackgroundColor, &ColorConfigField::changed,
            backgroundColorButton, &ColorButton::setColor);
    connect(backgroundColorButton, &QPushButton::clicked, this, [this](){
        ColorDialog * colorDialog =
            new ColorDialog(_config.buttonsBackgroundColor(), this);
        colorDialog->setAttribute(Qt::WA_DeleteOnClose);
        colorDialog->setWindowTitle(tr("Select color"));

        connect(colorDialog, &ColorDialog::currentColorChanged,
                &_config.buttonsBackgroundColor, &ColorConfigField::set);

        QColor backup = _config.buttonsBackgroundColor();
        this->topLevelWidget()->hide();
        if (colorDialog->exec() == QDialog::Rejected) {
            _config.buttonsBackgroundColor.set(backup);
        }
        this->topLevelWidget()->show();
    });
    colorLayout->addWidget(backgroundColorButton, 0, 1);



    QLabel * windowBackgroundColorLabel =
        new QLabel(tr("Window background color:"), this);
    colorLayout->addWidget(windowBackgroundColorLabel, 1, 0);

    ColorButton * windowBackgroundColorButton =
        new ColorButton(_config.windowBackgroundColor());

    connect(&config.windowBackgroundColor, &ColorConfigField::changed,
            windowBackgroundColorButton, &ColorButton::setColor);
    connect(windowBackgroundColorButton, &QPushButton::clicked, this, [this](){
        ColorDialog * colorDialog =
            new ColorDialog(_config.windowBackgroundColor(), this);
        colorDialog->setAttribute(Qt::WA_DeleteOnClose);
        colorDialog->setEnabledAlphaChannel(false);
        colorDialog->setWindowTitle(tr("Select color"));

        connect(colorDialog, &ColorDialog::currentColorChanged,
                &_config.windowBackgroundColor, &ColorConfigField::set);

        QColor backup = _config.windowBackgroundColor();
        this->topLevelWidget()->hide();
        if (colorDialog->exec() == QDialog::Rejected) {
            _config.windowBackgroundColor.set(backup);
        }
        this->topLevelWidget()->show();
    });
    colorLayout->addWidget(windowBackgroundColorButton, 1, 1);



    layout->addStretch();

    QScrollArea * scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(content);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);
}



void GuiPage::_guiScaleChanged(int value) {
    const float scale = value / 100.0;
    _guiScaleValue->setText(QString::number(scale, 'f', 2));
    _config.guiScale.set(scale);
}
