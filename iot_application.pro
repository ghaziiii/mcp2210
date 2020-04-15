#-------------------------------------------------
#
# Project created by QtCreator 2018-04-23T10:57:54
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iot_application
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mcp2210.cpp \
    hid.c

HEADERS  += mainwindow.h \
    mcp2210.h \
    hidapi.h

FORMS    += mainwindow.ui
unix {
packagesExist(libudev) {
CONFIG += link_pkgconfig
DEFINES += LINK_LIBUDEV
PKGCONFIG += libudev
}
}
