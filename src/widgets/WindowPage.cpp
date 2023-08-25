#include "WindowPage.hpp"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QScrollArea>



WindowPage::WindowPage(ConfigItem & config)
    : _config(config)
{
    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    QWidget * content = new QWidget;



    QVBoxLayout * layout = new QVBoxLayout(content);

    QGroupBox * windowBox = new QGroupBox(tr("Minimize button"), this);
    layout->addWidget(windowBox);

    QVBoxLayout * windowLayout = new QVBoxLayout;
    windowBox->setLayout(windowLayout);

    QCheckBox * enableNormalaze = new QCheckBox(tr(
        "Enable normalization from maximized window state"
    ), this);
    enableNormalaze->setChecked(config.enableNormalazeFromMaximize);
    windowLayout->addWidget(enableNormalaze);
    connect(enableNormalaze, &QCheckBox::toggled,
            &config, &ConfigItem::setEnableNormalazeFromMaximize);

    QCheckBox * enableHiding =
        new QCheckBox(tr("Enable window minimization"), this);
    enableHiding->setChecked(config.enableHiding);
    windowLayout->addWidget(enableHiding);
    connect(enableHiding, &QCheckBox::toggled,
            &config, &ConfigItem::setEnableHiding);



    layout->addStretch();



    QScrollArea * scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(content);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);
}
