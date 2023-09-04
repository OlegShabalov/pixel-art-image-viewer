#include "GuiPage.hpp"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSlider>
#include <QScrollArea>



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
    connect(guiScale, &QSlider::valueChanged,
            this, &GuiPage::_guiScaleChanged);

    _guiScaleValue = new QLabel(this);
    scaleLayout->addWidget(_guiScaleValue);

    guiScale->setValue(_config.guiScale * 100);



    QCheckBox * enableHidingHoveredButtons =
        new QCheckBox(tr("Enable hiding hovered buttons"), this);
    enableHidingHoveredButtons->setChecked(config.enableHidingHoveredButtons);
    guiLayout->addWidget(enableHidingHoveredButtons);
    connect(enableHidingHoveredButtons, &QCheckBox::toggled,
            &config, &ConfigItem::setEnableHidingHoveredButtons);



    QGroupBox * titleBox = new QGroupBox(tr("Title"), this);
    layout->addWidget(titleBox);

    QVBoxLayout * titleLayout = new QVBoxLayout;
    titleBox->setLayout(titleLayout);

    QCheckBox * enablePictureCount =
        new QCheckBox(tr("Enable image count"), this);
    enablePictureCount->setChecked(config.enablePictureCount);
    titleLayout->addWidget(enablePictureCount);
    connect(enablePictureCount, &QCheckBox::toggled,
            &config, &ConfigItem::setEnablePictureCount);



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
    _config.setGuiScale(scale);
}
