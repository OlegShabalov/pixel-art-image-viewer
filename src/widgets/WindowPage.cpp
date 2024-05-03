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
    windowLayout->addWidget(enableNormalaze);
    config.enableNormalazeFromMaximize.connectCheckBox(enableNormalaze);

    QCheckBox * enableHiding =
        new QCheckBox(tr("Enable window minimization"), this);
    windowLayout->addWidget(enableHiding);
    config.enableHiding.connectCheckBox(enableHiding);



    layout->addStretch();



    QScrollArea * scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(content);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);
}
