#ifndef IMAGEPAGE_HPP
#define IMAGEPAGE_HPP

#include <QWidget>
#include <QSpinBox>

#include "ConfigItem.hpp"



class ImagePage : public QWidget {
    Q_OBJECT

public:
    ImagePage(ConfigItem & config);

private Q_SLOTS:
    void _maxPixelSizeChanged(int value);
    void _minImageSizeChanged(int value);

private:
    ConfigItem & _config;

    QSpinBox * _maxPixelSizeSpinBox;
    QSpinBox * _minImageSizeSpinBox;
};



#endif // IMAGEPAGE_HPP
