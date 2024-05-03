QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# CONFIG += c++11 console
# CONFIG -= app_bundle
QMAKE_LFLAGS += -static

SOURCES += \
    src/Application.cpp \
    src/ConfigField.cpp \
    src/ConfigItem.cpp \
    src/IconButton.cpp \
    src/Image.cpp \
    src/ImageLoader.cpp \
    src/Layout.cpp \
    src/main.cpp \
    src/widgets/ColorButton.cpp \
    src/widgets/ColorDialog.cpp \
    src/widgets/GuiPage.cpp \
    src/widgets/ImagePage.cpp \
    src/widgets/MainWidget.cpp \
    src/widgets/SettingsWidget.cpp \
    src/widgets/WinWindow.cpp \
    src/widgets/WindowPage.cpp

HEADERS += \
    src/Application.hpp \
    src/ConfigField.hpp \
    src/ConfigItem.hpp \
    src/IconButton.hpp \
    src/Image.hpp \
    src/ImageLoader.hpp \
    src/Layout.hpp \
    src/Math.hpp \
    src/widgets/ColorButton.hpp \
    src/widgets/ColorDialog.hpp \
    src/widgets/GuiPage.hpp \
    src/widgets/ImagePage.hpp \
    src/widgets/MainWidget.hpp \
    src/widgets/SettingsWidget.hpp \
    src/widgets/WinWindow.hpp \
    src/widgets/WindowPage.hpp

RESOURCES += \
    res.qrc



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
