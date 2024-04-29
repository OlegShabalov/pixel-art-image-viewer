#ifndef COLORBUTTON_HPP
#define COLORBUTTON_HPP

#include <QPushButton>



class ColorButton : public QPushButton {
    Q_OBJECT

public:
    ColorButton(const QColor & color, QWidget * parent = nullptr);

public Q_SLOTS:
    void setColor(const QColor & color);

protected:
    void paintEvent(QPaintEvent * event) override;

private:
    QColor _color;
};



#endif // COLORBUTTON_HPP
