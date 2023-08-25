#include "ImagePage.hpp"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QComboBox>
#include <QScrollArea>



ImagePage::ImagePage(ConfigItem & config)
    : _config(config)
{
    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    QWidget * content = new QWidget;



    QVBoxLayout * layout = new QVBoxLayout(content);

    QGroupBox * scalingBox = new QGroupBox(tr("Scaling"), this);
    layout->addWidget(scalingBox);

    QVBoxLayout * scalingLayout = new QVBoxLayout;
    scalingBox->setLayout(scalingLayout);

    QCheckBox * useCursorAsScaleCenter =
        new QCheckBox(tr("Use cursor as scaling center"), this);
    useCursorAsScaleCenter->setChecked(config.useCursorAsScaleCenter);
    useCursorAsScaleCenter->setToolTip(tr(
        "Use the cursor position as the image scaling center when scaling\n"
        "with the mouse or touchpad. When this option is disabled, the\n"
        "center of the window is used for scaling."
    ));
    scalingLayout->addWidget(useCursorAsScaleCenter);
    connect(useCursorAsScaleCenter, &QCheckBox::toggled,
            &config, &ConfigItem::setUseCursorAsScaleCenter);



    QGridLayout * minMaxSizeLayout = new QGridLayout;
    minMaxSizeLayout->setColumnStretch(2, 1);
    scalingLayout->addLayout(minMaxSizeLayout);

    QLabel * maxPixelSizeLabel = new QLabel(tr("Max pixel size:"), this);
    maxPixelSizeLabel->setToolTip(tr(
        "The maximum size of the image pixel in screen pixels.\n"
        "This parameter sets how much you can zoom in on the image.\n"
        "The maximum pixel size must be greater than the minimum image size."
    ));
    minMaxSizeLayout->addWidget(maxPixelSizeLabel, 0, 0);

    _maxPixelSizeSpinBox = new QSpinBox(this);
    _maxPixelSizeSpinBox->setRange(1, 4000);
    _maxPixelSizeSpinBox->setSuffix(tr(" px"));
    _maxPixelSizeSpinBox->setSingleStep(10);
    _maxPixelSizeSpinBox->setValue(config.maxPixelSize);
    minMaxSizeLayout->addWidget(_maxPixelSizeSpinBox, 0, 1, Qt::AlignLeft);
    connect(_maxPixelSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ImagePage::_maxPixelSizeChanged);

    QLabel * minImageSizeLabel = new QLabel(tr("Min image size:"), this);
    minImageSizeLabel->setToolTip(tr(
        "Minimum image size in screen pixels.\n"
        "This parameter sets how much you can zoom out on the image.\n"
        "The maximum pixel size must be greater than the minimum image size."
    ));
    minMaxSizeLayout->addWidget(minImageSizeLabel, 1, 0);

    _minImageSizeSpinBox = new QSpinBox(this);
    _minImageSizeSpinBox->setRange(1, 4000);
    _minImageSizeSpinBox->setSuffix(tr(" px"));
    _minImageSizeSpinBox->setSingleStep(10);
    _minImageSizeSpinBox->setValue(config.minImageSize);
    minMaxSizeLayout->addWidget(_minImageSizeSpinBox, 1, 1, Qt::AlignLeft);
    connect(_minImageSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ImagePage::_minImageSizeChanged);



    QCheckBox * enableOneToOneScaling =
        new QCheckBox(tr("Allow one to one scaling"), this);
    enableOneToOneScaling->setChecked(_config.enableOneToOneScaling);
    enableOneToOneScaling->setToolTip(tr(
        "The minimum image size may become smaller than\n"
        "the specified one if the image is small."
    ));
    minMaxSizeLayout->addWidget(enableOneToOneScaling, 1, 2);
    connect(enableOneToOneScaling, &QCheckBox::toggled,
            &config, &ConfigItem::setEnableOneToOneScaling);



    QGroupBox * movingBox = new QGroupBox(tr("Moving"), this);
    layout->addWidget(movingBox);

    QGridLayout * movingLayout = new QGridLayout;
    movingLayout->setColumnStretch(1, 1);
    movingBox->setLayout(movingLayout);

    QLabel * marginLabel = new QLabel(tr("Margin:"), this);
    marginLabel->setToolTip(tr(
        "Value of zero means that the image can be\n"
        "completely moved beyond the edge of the window."
    ));
    movingLayout->addWidget(marginLabel, 0, 0);

    QSpinBox * marginSpinBox = new QSpinBox(this);
    marginSpinBox->setRange(0, 1000);
    marginSpinBox->setSuffix(tr(" px"));
    marginSpinBox->setSingleStep(5);
    marginSpinBox->setValue(config.moveMarginInPixels);
    movingLayout->addWidget(marginSpinBox, 0, 1, Qt::AlignLeft);
    connect(marginSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            &config, &ConfigItem::setMoveMarginInPixels);



    QGroupBox * windowBox = new QGroupBox(tr("Window resizing"), this);
    layout->addWidget(windowBox);

    QVBoxLayout * windowLayout = new QVBoxLayout;
    windowBox->setLayout(windowLayout);

    QCheckBox * fixImageWhenMaximized =
        new QCheckBox(tr("Reset image position when maximizing window"), this);
    fixImageWhenMaximized->setChecked(config.fixImageWhenWindowMaximized);
    windowLayout->addWidget(fixImageWhenMaximized);
    connect(fixImageWhenMaximized, &QCheckBox::toggled,
            &config, &ConfigItem::setFixImageWhenWindowMaximized);

    QCheckBox * fixImageWhenFullScreen =
        new QCheckBox(tr("Reset image position in fullscreen"), this);
    fixImageWhenFullScreen->setChecked(config.fixImageWhenWindowFullScreen);
    windowLayout->addWidget(fixImageWhenFullScreen);
    connect(fixImageWhenFullScreen, &QCheckBox::toggled,
            &config, &ConfigItem::setFixImageWhenWindowFullScreen);

    QHBoxLayout * hLayout = new QHBoxLayout;
    windowLayout->addLayout(hLayout);

    QLabel * windowResizeStrategyLabel =
        new QLabel(tr("On window resize:"), this);
    hLayout->addWidget(windowResizeStrategyLabel);

    QComboBox * windowResizeStrategy = new QComboBox(this);
    windowResizeStrategy->addItem(tr("Do nothing"));
    windowResizeStrategy->addItem(tr("Keep image centered"));
    windowResizeStrategy->addItem(tr("Scale image"));
    if (config.windowResizeStrategy == ConfigItem::FocusOnCenter) {
        windowResizeStrategy->setCurrentIndex(1);
    }
    else if (config.windowResizeStrategy == ConfigItem::ScaleFragment) {
        windowResizeStrategy->setCurrentIndex(2);
    }
    else {
        windowResizeStrategy->setCurrentIndex(0);
    }
    hLayout->addWidget(windowResizeStrategy);
    connect(windowResizeStrategy,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index)
    {
        if (index == 1) {
            _config.setWindowResizeStrategy(ConfigItem::FocusOnCenter);
        }
        else if (index == 2) {
            _config.setWindowResizeStrategy(ConfigItem::ScaleFragment);
        }
        else {
            _config.setWindowResizeStrategy(ConfigItem::NoStrategy);
        }
    });
    hLayout->addStretch();



    layout->addStretch();



    QScrollArea * scrollArea = new QScrollArea(this);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidget(content);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);
}



void ImagePage::_maxPixelSizeChanged(int max) {
    _config.setMaxPixelSize(max);
    if (max < _minImageSizeSpinBox->value()) {
        _minImageSizeSpinBox->setValue(max);
    }
}
void ImagePage::_minImageSizeChanged(int min) {
    _config.setMinImageSize(min);
    if (min > _maxPixelSizeSpinBox->value()) {
        _maxPixelSizeSpinBox->setValue(min);
    }
}
