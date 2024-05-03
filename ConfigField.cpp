#include "ConfigField.hpp"

#include "ConfigItem.hpp"

#include <QCheckBox>
#include <QSpinBox>
#include <QSettings>



AbstractConfigField::AbstractConfigField(const QString & id, ConfigItem * configItem)
    : _id(id)
    , _configItem(configItem)
{
    configItem->_addField(this);
}

void AbstractConfigField::_notifyConfigItem() {
    _configItem->_change();
}





BoolConfigField::BoolConfigField(bool value, const QString & id,
                                 ConfigItem * configItem)
    : AbstractConfigField(id, configItem)
    , _value(value)
{  }

void BoolConfigField::copy(const AbstractConfigField * from) {
    set(static_cast<const BoolConfigField *>(from)->_value);
}

void BoolConfigField::load(QSettings & settings) {
    _value = settings.value(_id, _value).toBool();
}
void BoolConfigField::save(QSettings & settings) {
    settings.setValue(_id, _value);
}

void BoolConfigField::connectCheckBox(QCheckBox * checkBox) {
    checkBox->setChecked(_value);
    connect(checkBox, &QCheckBox::toggled, this, &BoolConfigField::set);
    connect(this, &BoolConfigField::changed, checkBox, &QCheckBox::setChecked);
}

void BoolConfigField::set(bool value) {
    if (value != _value) {
        _value = value;
        Q_EMIT changed(value);
        _notifyConfigItem();
    }
}





IntConfigField::IntConfigField(int value, const QString & id,
                               ConfigItem * configItem)
    : AbstractConfigField(id, configItem)
    , _value(value)
{  }

void IntConfigField::copy(const AbstractConfigField * from) {
    set(static_cast<const IntConfigField *>(from)->_value);
}

void IntConfigField::load(QSettings & settings) {
    _value = settings.value(_id, _value).toInt();
}
void IntConfigField::save(QSettings & settings) {
    settings.setValue(_id, _value);
}

void IntConfigField::connectSpinBox(QSpinBox * spinBox) {
    spinBox->setValue(_value);
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &IntConfigField::set);
    connect(this, &IntConfigField::changed, spinBox, &QSpinBox::setValue);
}

void IntConfigField::set(int value) {
    if (value != _value) {
        _value = value;
        Q_EMIT changed(value);
        _notifyConfigItem();
    }
}





FloatConfigField::FloatConfigField(float value, const QString & id,
                                   ConfigItem * configItem)
    : AbstractConfigField(id, configItem)
    , _value(value)
{  }

void FloatConfigField::copy(const AbstractConfigField * from) {
    set(static_cast<const FloatConfigField *>(from)->_value);
}

void FloatConfigField::load(QSettings & settings) {
    _value = settings.value(_id, _value).toFloat();
}
void FloatConfigField::save(QSettings & settings) {
    settings.setValue(_id, _value);
}

void FloatConfigField::set(float value) {
    if (value != _value) {
        _value = value;
        Q_EMIT changed(value);
        _notifyConfigItem();
    }
}





ColorConfigField::ColorConfigField(const QColor & value, const QString & id,
                                   ConfigItem * configItem)
    : AbstractConfigField(id, configItem)
    , _value(value)
{  }

void ColorConfigField::copy(const AbstractConfigField * from) {
    set(static_cast<const ColorConfigField *>(from)->_value);
}

void ColorConfigField::load(QSettings & settings) {
    _value = settings.value(_id, _value).value<QColor>();
}
void ColorConfigField::save(QSettings & settings) {
    settings.setValue(_id, _value);
}

void ColorConfigField::set(const QColor & value) {
    if (value != _value) {
        _value = value;
        Q_EMIT changed(value);
        _notifyConfigItem();
    }
}
