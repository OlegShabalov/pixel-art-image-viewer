#ifndef CONFIGFIELD_HPP
#define CONFIGFIELD_HPP

#include <QObject>
#include <QColor>



class ConfigItem;

class QCheckBox;
class QSpinBox;
class QSettings;



class AbstractConfigField : public QObject {
    Q_OBJECT
public:
    AbstractConfigField(const QString & id, ConfigItem * configItem);

    virtual void copy(const AbstractConfigField * from) = 0;
    virtual void load(QSettings & settings) = 0;
    virtual void save(QSettings & settings) = 0;

protected:
    void _notifyConfigItem();

protected:
    QString _id;

private:
    ConfigItem * _configItem;
};





class BoolConfigField : public AbstractConfigField {
    Q_OBJECT
public:
    BoolConfigField(bool value, const QString & id, ConfigItem * configItem);
    inline operator bool () const { return _value; }

    void copy(const AbstractConfigField * from) override;
    void load(QSettings & settings) override;
    void save(QSettings & settings) override;

    void connectCheckBox(QCheckBox * checkBox);

public Q_SLOTS:
    void set(bool value);

Q_SIGNALS:
    void changed(bool value);

private:
    bool _value;
};





class IntConfigField : public AbstractConfigField {
    Q_OBJECT
public:
    IntConfigField(int value, const QString & id, ConfigItem * configItem);
    inline operator int () const { return _value; }

    void copy(const AbstractConfigField * from) override;
    void load(QSettings & settings) override;
    void save(QSettings & settings) override;

    void connectSpinBox(QSpinBox * spinBox);

public Q_SLOTS:
    void set(int value);

Q_SIGNALS:
    void changed(int value);

private:
    int _value;
};





class FloatConfigField : public AbstractConfigField {
    Q_OBJECT
public:
    FloatConfigField(float value, const QString & id, ConfigItem * configItem);
    inline operator float () const { return _value; }

    void copy(const AbstractConfigField * from) override;
    void load(QSettings & settings) override;
    void save(QSettings & settings) override;

public Q_SLOTS:
    void set(float value);

Q_SIGNALS:
    void changed(float value);

private:
    float _value;
};





class ColorConfigField : public AbstractConfigField {
    Q_OBJECT
public:
    ColorConfigField(const QColor & value, const QString & id,
                     ConfigItem * configItem);
    inline const QColor & operator () () const { return _value; }

    void copy(const AbstractConfigField * from) override;
    void load(QSettings & settings) override;
    void save(QSettings & settings) override;

public Q_SLOTS:
    void set(const QColor & value);

Q_SIGNALS:
    void changed(const QColor & value);

private:
    QColor _value;
};






#endif // CONFIGFIELD_HPP
