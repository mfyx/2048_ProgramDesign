QT       += core gui
#QT += multimedia


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    #emphasize.cpp \
    block.cpp \
    initinterface.cpp \
    #input.cpp \
    main.cpp \
    mainwindow.cpp \
    matrix.cpp \
    rankwidget.cpp

HEADERS += \
    block.h \
    config.h \
    #emphasize.h \
    initinterface.h \
    #input.h \
    mainwindow.h \
    matrix.h \
    rankwidget.h \
    userinformation.h

FORMS += \
    initinterface.ui \
    mainwindow.ui \
    rankwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE += game2048.rc

RESOURCES += \
    res.qrc
