QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# CONFIG += c++11 console
# CONFIG -= app_bundle

SOURCES += \
    Application.cpp \
    ConfigItem.cpp \
    IconButton.cpp \
    Image.cpp \
    ImageLoader.cpp \
    Layout.cpp \
    main.cpp \
    src/widgets/MainWidget.cpp \
    src/widgets/WinWindow.cpp

HEADERS += \
    Application.hpp \
    ConfigItem.hpp \
    IconButton.hpp \
    Image.hpp \
    ImageLoader.hpp \
    Layout.hpp \
    Math.hpp \
    src/widgets/MainWidget.hpp \
    src/widgets/WinWindow.hpp

RESOURCES += \
    res.qrc



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
