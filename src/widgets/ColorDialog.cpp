#include "ColorDialog.hpp"

#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QBitmap>
#include <QGridLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>



///////////////////////////////////////////////////////////////////////////////

namespace {

class ColorSelector;
class HueSelector;
class AlphaSelector;
class ColorPreview;

} // namespace

///////////////////////////////////////////////////////////////////////////////

class ColorDialogPrivate {
    friend class ColorDialog;

    QColor currentColor;
    ColorDialog * colorDialog;

    ColorSelector * colorSelector;
    HueSelector * hueSelector;
    AlphaSelector * alphaSelector;
    ColorPreview * colorPreview;

    QWidget * columnWidget;

    QGridLayout * spinBoxesLayout;

    QSpinBox * redSpinBox;
    QSpinBox * greenSpinBox;
    QSpinBox * blueSpinBox;
    QSpinBox * alphaSpinBox;

    QLabel * alphaLabel;

    QPushButton * okButton;
    QPushButton * cancelButton;

    bool ignoreSpinBoxesSignals = false;
    bool enabledAlpha = true;

    void setColorFromSpinBoxes();
    void setInputColor(const QColor & c);

public:
    void setOutputColor(const QColor & c);

    void updateColorSelectorMinSize();
};

///////////////////////////////////////////////////////////////////////////////

namespace {

class ColorPreview : public QWidget {
public:
    ColorPreview(QWidget * parent)
        : QWidget(parent)
    {
        setMinimumSize(96, 96);
    }

    void setPrivateData(ColorDialogPrivate * privateData) {
        _privateData = privateData;
    }

    void setInputColor(const QColor & color) {
        _color = color;
        _privateData->setOutputColor(color);
        update();
    }

    void setOutputColor(const QColor & color) {
        _color = color;
        update();
    }

    void enableAlpha(bool enable) {
        if (_enabledAlpha != enable) {
            _enabledAlpha = enable;
            update();
        }
    }

protected:
    void paintEvent(QPaintEvent * event) override {
        Q_UNUSED(event)

        QPainter painter(this);
        painter.setPen(Qt::NoPen);

        if (_enabledAlpha) {
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setBrush(QColor(200, 200, 200));

            bool odd = false;
            for (int ix = 0; ix < width(); ix += 6) {
                int iy = (odd ? 6 : 0);
                odd = !odd;
                for (; iy < height(); iy += 12) {
                    painter.drawRect(ix, iy, 6, 6);
                }
            }

            painter.setRenderHint(QPainter::Antialiasing, false);

            QColor noAlpha = _color;
            noAlpha.setAlphaF(1.0);
            painter.setBrush(QBrush(noAlpha));
            painter.drawRect(0, 0, width(), height() / 2.0f);

            painter.setBrush(QBrush(_color));
            painter.drawRect(0, height() / 2.0f, width(), height() / 2.0f);
        }
        else {
            QColor noAlpha = _color;
            noAlpha.setAlphaF(1.0);
            painter.setBrush(QBrush(noAlpha));
            painter.drawRect(0, 0, width(), height());
        }

    }

private:
    QColor _color;
    bool _enabledAlpha = true;
    ColorDialogPrivate * _privateData;
};

///////////////////////////////////////////////////////////////////////////////

// Because of to the use of a mask, the edge of the circle is drawn
// with ragged edges. To fix this, the edge of the circle is drawn twice:
// in the internal and external widgets.

class AlphaSelectorInternal : public QWidget {
public:
    AlphaSelectorInternal(QWidget * parent)
        : QWidget(parent)
    {  }

    void setColor(unsigned char r, unsigned char g, unsigned char b) {
        _r = r;
        _g = g;
        _b = b;
    }

    void setAlpha(float alpha) {
        _alpha = alpha;
    }

    QColor color() const {
        return QColor(_r, _g, _b, _alpha * 255);
    }

protected:
    void paintEvent(QPaintEvent * event) override {
        Q_UNUSED(event)

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(200, 200, 200));

        for (int h = 0; h < height(); h += 12) {
            painter.drawRect(0 + 6, h, 6, 6);
            painter.drawRect(0 + 6 + 6, h + 6, 6, 6);
        }

        painter.setRenderHint(QPainter::Antialiasing, false);

        QLinearGradient gradient(QPointF(0, 0), QPointF(0, height()));
        gradient.setColorAt(0, QColor(_r, _g, _b, 0));
        gradient.setColorAt(1, QColor(_r, _g, _b, 255));

        painter.setBrush(QBrush(gradient));
        painter.drawRect(0, 0, width(), height());

        const int x = width()  * 0.5f   - 10;
        const int y = height() * _alpha - 10;

        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(QPen(QColor(70, 70, 70), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(x, y, 20, 20);



        QBitmap mask(width(), height());
        mask.clear();
        QPainter maskPainter(&mask);

        maskPainter.setPen(Qt::NoPen);
        maskPainter.setBrush(QBrush(Qt::color1));

        maskPainter.drawRect(0 + 6, 0, width() - 12, height());
        maskPainter.drawEllipse(x, y, 20, 20);

        setMask(mask);
    }

private:
    float _alpha;
    unsigned char _r;
    unsigned char _g;
    unsigned char _b;
};

class AlphaSelector : public QWidget {
public:
    AlphaSelector(QWidget * parent)
        : QWidget(parent)
    {
        setFixedWidth(24);
        _alpha = 0.5;

        QVBoxLayout * layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        _internal = new AlphaSelectorInternal(this);
        layout->addWidget(_internal);
        _internal->setAlpha(_alpha);
    }

    void setColorPreview(ColorPreview * colorPreview) {
        _colorPreview = colorPreview;
    }

    void setItputColor(const QColor & color) {
        _internal->setColor(color.red(), color.green(), color.blue());
        _colorPreview->setInputColor(_internal->color());
        update();
    }

    void setOutputColor(const QColor & color) {
        _alpha = color.alphaF();
        _internal->setAlpha(_alpha);
        _internal->setColor(color.red(), color.green(), color.blue());
        update();
    }

protected:
    void paintEvent(QPaintEvent * event) override {
        Q_UNUSED(event)

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        const int x = width()  * 0.5f   - 10;
        const int y = height() * _alpha - 10;

        painter.setPen(QPen(QColor(70, 70, 70), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(x, y, 20, 20);
    }

    void mousePressEvent(QMouseEvent * event) override {
        if (event->button() == Qt::LeftButton) {
            _alpha = event->pos().y() / static_cast<float>(height());
            _alpha = qBound(0.0f, _alpha, 1.0f);
            _internal->setAlpha(_alpha);
            _colorPreview->setInputColor(_internal->color());
            update();
            event->accept();
            return;
        }
        event->ignore();
    }

    void mouseMoveEvent(QMouseEvent * event) override {
        _alpha = event->pos().y() / static_cast<float>(height());
        _alpha = qBound(0.0f, _alpha, 1.0f);
        _internal->setAlpha(_alpha);
        _colorPreview->setInputColor(_internal->color());
        update();
        event->accept();
    }

private:
    float _alpha;
    AlphaSelectorInternal * _internal;
    ColorPreview * _colorPreview;
};

///////////////////////////////////////////////////////////////////////////////

class ColorSelector : public QWidget {
public:
    ColorSelector(QWidget * parent)
        : QWidget(parent)
    {  }

    void setAlphaSelector(AlphaSelector * alphaSelector) {
        _alphaSelector = alphaSelector;
    }

    void setItputColor(float hue) {
        _hue = hue;
        _updateColor();
        update();
    }

    void setOutputColor(const QColor & color) {
        _outputColor = color;
        const QColor temp = color.toHsv();
        _hue = qMax(temp.hsvHueF(), 0.0);
        _pointX = temp.saturationF();
        _pointY = 1 - temp.valueF();
        update();
    }

protected:
    void paintEvent(QPaintEvent * event) override {
        Q_UNUSED(event)

        QPainter painter(this);

        const int lineSize = 1;

        for (int h = 0; h < height(); h += lineSize) {
            const float k = 1 - (h / static_cast<float>(height()));

            QLinearGradient gradient(QPointF(0, 0), QPointF(width(), 1));
            gradient.setColorAt(0, QColor::fromHsvF(_hue, 0, k));
            gradient.setColorAt(1, QColor::fromHsvF(_hue, 1, k));

            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(gradient));
            painter.drawRect(0, h, width(), lineSize);
        }

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(Qt::NoBrush);

        const int x = width()  * _pointX - 10;
        const int y = height() * _pointY - 10;

        if (_pointY < 0.4f - _pointX * 0.3f) {
            painter.setPen(QPen(QColor(70, 70, 70), 2));
            painter.setBrush(QBrush(_outputColor));
            painter.drawEllipse(x, y, 20, 20);
        }
        else {
            painter.setPen(QPen(Qt::white, 2));
            painter.setBrush(QBrush(_outputColor));
            painter.drawEllipse(x, y, 20, 20);
        }
    }

    void mousePressEvent(QMouseEvent * event) override {
        if (event->button() == Qt::LeftButton) {
            _pointX = event->pos().x() / static_cast<float>(width());
            _pointY = event->pos().y() / static_cast<float>(height());
            _pointX = qBound(0.0f, _pointX, 1.0f);
            _pointY = qBound(0.0f, _pointY, 1.0f);
            _updateColor();
            update();
            event->accept();
            return;
        }
        event->ignore();
    }

    void mouseMoveEvent(QMouseEvent * event) override {
        _pointX = event->pos().x() / static_cast<float>(width());
        _pointY = event->pos().y() / static_cast<float>(height());
        _pointX = qBound(0.0f, _pointX, 1.0f);
        _pointY = qBound(0.0f, _pointY, 1.0f);
        _updateColor();
        update();
        event->accept();
    }

private:
    void _updateColor() {
        _outputColor = QColor::fromHsvF(_hue, _pointX, 1 - _pointY).toRgb();
        _alphaSelector->setItputColor(_outputColor);
    }

private:
    float _pointX;
    float _pointY;
    float _hue;

    QColor _outputColor;

    AlphaSelector * _alphaSelector;
};

///////////////////////////////////////////////////////////////////////////////

class HueSelector : public QWidget {
public:
    HueSelector(QWidget * parent)
        : QWidget(parent)
    {
        setFixedWidth(24);
    }

    void setColorSelector(ColorSelector * colorSelector) {
        _colorSelector = colorSelector;
    }

    void setOutputColor(const QColor & color) {
        _hue = qMax(color.toHsv().hsvHueF(), 0.0);
        update();
    }

protected:
    void paintEvent(QPaintEvent * event) override {
        Q_UNUSED(event)

        QPainter painter(this);

        const int lineSize = 1;

        for (int h = 0; h < height(); h += lineSize) {
            const float k = 1 - (h / static_cast<float>(height()));

            const QColor color = QColor::fromHsvF(k, 1, 1);

            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(color));
            painter.drawRect(0 + 6, h, width() - 12, lineSize);
        }

        const int x = width()  * 0.5f          - 10;
        const int y = height() * (1.0f - _hue) - 10;

        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(QColor(70, 70, 70), 2));
        painter.setBrush(QBrush(QColor::fromHsvF(_hue, 1, 1)));
        painter.drawEllipse(x, y, 20, 20);
    }

    void mousePressEvent(QMouseEvent * event) override {
        if (event->button() == Qt::LeftButton) {
            _hue = 1.0f - (event->pos().y() / static_cast<float>(height()));
            _hue = qBound(0.0f, _hue, 1.0f);
            _colorSelector->setItputColor(_hue);
            update();
            event->accept();
            return;
        }
        event->ignore();
    }

    void mouseMoveEvent(QMouseEvent * event) override {
        _hue = 1.0f - (event->pos().y() / static_cast<float>(height()));
        _hue = qBound(0.0f, _hue, 1.0f);
        _colorSelector->setItputColor(_hue);
        update();
        event->accept();
    }

private:
    float _hue;
    ColorSelector * _colorSelector;
};

} // namespace

///////////////////////////////////////////////////////////////////////////////

void ColorDialogPrivate::setColorFromSpinBoxes() {
    if (ignoreSpinBoxesSignals) return;

    currentColor = QColor(redSpinBox->value(),
                          greenSpinBox->value(),
                          blueSpinBox->value(),
                          alphaSpinBox->value());

    hueSelector->setOutputColor(currentColor);
    colorSelector->setOutputColor(currentColor);
    alphaSelector->setOutputColor(currentColor);
    colorPreview->setOutputColor(currentColor);

    if (!enabledAlpha) {
        currentColor.setAlpha(255);
    }

    Q_EMIT colorDialog->currentColorChanged(currentColor);
}

void ColorDialogPrivate::setInputColor(const QColor & c) {
    // from outside

    hueSelector->setOutputColor(c);
    colorSelector->setOutputColor(c);
    alphaSelector->setOutputColor(c);
    colorPreview->setOutputColor(c);

    ignoreSpinBoxesSignals = true;
    redSpinBox->setValue(c.red());
    greenSpinBox->setValue(c.green());
    blueSpinBox->setValue(c.blue());
    alphaSpinBox->setValue(c.alpha());
    ignoreSpinBoxesSignals = false;

    currentColor = c;
    if (!enabledAlpha) {
        currentColor.setAlpha(255);
    }

    Q_EMIT colorDialog->currentColorChanged(currentColor);
}

void ColorDialogPrivate::setOutputColor(const QColor & c) {
    // from color preview

    ignoreSpinBoxesSignals = true;
    redSpinBox->setValue(c.red());
    greenSpinBox->setValue(c.green());
    blueSpinBox->setValue(c.blue());
    alphaSpinBox->setValue(c.alpha());
    ignoreSpinBoxesSignals = false;

    currentColor = c;
    if (!enabledAlpha) {
        currentColor.setAlpha(255);
    }

    Q_EMIT colorDialog->currentColorChanged(currentColor);
}

void ColorDialogPrivate::updateColorSelectorMinSize() {
    const int mh = columnWidget->minimumSizeHint().height();
    colorSelector->setMinimumSize(mh, mh);
}

///////////////////////////////////////////////////////////////////////////////

ColorDialog::ColorDialog(QWidget * parent)
    : ColorDialog(Qt::white, parent)
{  }

ColorDialog::ColorDialog(const QColor & initial, QWidget * parent)
    : QDialog(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setFocus();

    _d = new ColorDialogPrivate;
    _d->colorDialog = this;

    QHBoxLayout * layout = new QHBoxLayout(this);

    _d->colorSelector = new ColorSelector(this);
    layout->addWidget(_d->colorSelector, 1);

    _d->hueSelector = new HueSelector(this);
    _d->hueSelector->setColorSelector(_d->colorSelector);
    layout->addWidget(_d->hueSelector);

    _d->alphaSelector = new AlphaSelector(this);
    _d->colorSelector->setAlphaSelector(_d->alphaSelector);
    layout->addWidget(_d->alphaSelector);

    _d->columnWidget = new QWidget(this);
    layout->addWidget(_d->columnWidget);

    QVBoxLayout * rLayout = new QVBoxLayout(_d->columnWidget);
    rLayout->setContentsMargins(0, 0, 0, 0);

    _d->colorPreview = new ColorPreview(this);
    _d->colorPreview->setPrivateData(_d);
    _d->alphaSelector->setColorPreview(_d->colorPreview);
    rLayout->addWidget(_d->colorPreview, 0);

    _d->spinBoxesLayout = new QGridLayout;
    rLayout->addLayout(_d->spinBoxesLayout, 0);

    auto fun = [=](){
        _d->setColorFromSpinBoxes();
    };

    QLabel * label;

    label = new QLabel(tr("R:"), this);
    _d->spinBoxesLayout->addWidget(label, 0, 0, Qt::AlignRight);
    _d->redSpinBox = new QSpinBox(this);
    _d->redSpinBox->setRange(0, 255);
    connect(_d->redSpinBox, &QSpinBox::textChanged, this, fun);
    _d->spinBoxesLayout->addWidget(_d->redSpinBox, 0, 1);

    label = new QLabel(tr("G:"), this);
    _d->spinBoxesLayout->addWidget(label, 1, 0, Qt::AlignRight);
    _d->greenSpinBox = new QSpinBox(this);
    _d->greenSpinBox->setRange(0, 255);
    connect(_d->greenSpinBox, &QSpinBox::textChanged, this, fun);
    _d->spinBoxesLayout->addWidget(_d->greenSpinBox, 1, 1);

    label = new QLabel(tr("B:"), this);
    _d->spinBoxesLayout->addWidget(label, 2, 0, Qt::AlignRight);
    _d->blueSpinBox = new QSpinBox(this);
    _d->blueSpinBox->setRange(0, 255);
    connect(_d->blueSpinBox, &QSpinBox::textChanged, this, fun);
    _d->spinBoxesLayout->addWidget(_d->blueSpinBox, 2, 1);

    _d->alphaLabel = new QLabel(tr("A:"), this);
    _d->spinBoxesLayout->addWidget(_d->alphaLabel, 3, 0, Qt::AlignRight);
    _d->alphaSpinBox = new QSpinBox(this);
    _d->alphaSpinBox->setRange(0, 255);
    connect(_d->alphaSpinBox, &QSpinBox::textChanged, this, fun);
    _d->spinBoxesLayout->addWidget(_d->alphaSpinBox, 3, 1);

    rLayout->addStretch();

    _d->cancelButton = new QPushButton(tr("Cancel"), this);
    connect(_d->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    _d->okButton = new QPushButton(tr("OK"), this);
    _d->okButton->setDefault(true);
    connect(_d->okButton, &QPushButton::clicked, this, &QDialog::accept);

    rLayout->addWidget(_d->cancelButton);
    rLayout->addWidget(_d->okButton);

    _d->updateColorSelectorMinSize();

    setCurrentColor(initial);
}

ColorDialog::~ColorDialog() {
    delete _d;
}

QColor ColorDialog::currentColor() const {
    return _d->currentColor;
}

void ColorDialog::setCurrentColor(const QColor & color) {
    _d->setInputColor(color);
}

void ColorDialog::setEnabledAlphaChannel(bool enabled) {
    if (_d->enabledAlpha == enabled) return;
    _d->enabledAlpha = enabled;

    if (enabled) {
        _d->colorPreview->enableAlpha(true);

        _d->spinBoxesLayout->addWidget(_d->alphaLabel, 3, 0, Qt::AlignRight);
        _d->spinBoxesLayout->addWidget(_d->alphaSpinBox, 3, 1);

        _d->alphaSelector->show();
        _d->alphaSpinBox->show();
        _d->alphaLabel->show();

        _d->updateColorSelectorMinSize();
        _d->setColorFromSpinBoxes();
    }
    else {
        _d->colorPreview->enableAlpha(false);

        _d->spinBoxesLayout->removeWidget(_d->alphaSpinBox);
        _d->spinBoxesLayout->removeWidget(_d->alphaLabel);

        _d->alphaSelector->hide();
        _d->alphaSpinBox->hide();
        _d->alphaLabel->hide();

        _d->updateColorSelectorMinSize();
        _d->setColorFromSpinBoxes();
    }
}

void ColorDialog::setEnabledNoButtons(bool enabled) {
    if (enabled) {
        _d->okButton->hide();
        _d->cancelButton->hide();

        _d->updateColorSelectorMinSize();
    }
    else {
        _d->okButton->show();
        _d->cancelButton->show();

        _d->updateColorSelectorMinSize();
    }
}

QColor ColorDialog::getColor(
    const QColor & initial,
    QWidget * parent,
    const QString & title,
    bool enableAlphaChannel,
    bool enableNoButtonsMode)
{
    ColorDialog colorDialog(initial, parent);

    if (!title.isEmpty()) {
        colorDialog.setWindowTitle(title);
    }

    colorDialog.setEnabledAlphaChannel(enableAlphaChannel);
    colorDialog.setEnabledNoButtons(enableNoButtonsMode);

    int res = colorDialog.exec();

    if (res == QDialog::Accepted) {
        return colorDialog.currentColor();
    }
    else {
        return initial;
    }
}
