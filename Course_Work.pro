QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminpanelwindow.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    myregex.cpp \
    registrationwindow.cpp \
    resultswindow.cpp \
    tabledatagateway.cpp \
    ticket.cpp

HEADERS += \
    adminpanelwindow.h \
    loginwindow.h \
    mainwindow.h \
    myconsts.h \
    myregex.h \
    registrationwindow.h \
    resultswindow.h \
    tabledatagateway.h \
    ticket.h

FORMS += \
    adminpanelwindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    registrationwindow.ui \
    resultswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pictures.qrc

DESTDIR = ../bin
