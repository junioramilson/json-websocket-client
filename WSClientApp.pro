QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/JSON/JSONConfigParser.cpp \
    src/JSON/JSONFileSerializer.cpp \
    src/Widgets/WSMainWindow.cpp \
    src/Widgets/WSClientTabWidget.cpp \
    src/WebSocket/WebSocketClient.cpp \
    src/main.cpp

HEADERS += \
    src/JSON/JSONConfigParser.h \
    src/JSON/JSONFileSerializer.h \
    src/Widgets/JSSintaxHighlighter.h \
    src/Widgets/WSMainWindow.h \
    src/Widgets/WSClientTabWidget.h \
    src/WebSocket/WebSocketClient.h

FORMS += \
    src/UI/WSClientTabWidget.ui \
    src/UI/Main.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
