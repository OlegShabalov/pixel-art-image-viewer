#ifndef WINDOWPAGE_HPP
#define WINDOWPAGE_HPP

#include <QWidget>

#include "src/ConfigItem.hpp"



class WindowPage : public QWidget {
    Q_OBJECT

public:
    WindowPage(ConfigItem & config);

private:
    ConfigItem & _config;
};



#endif // WINDOWPAGE_HPP
