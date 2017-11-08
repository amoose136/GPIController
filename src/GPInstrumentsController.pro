#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T10:15:22
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPInstrumentsController
TEMPLATE = app

INCLUDEPATH +="framelesswindow"

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES     += \
                main.cpp \
                gpicontroller.cpp \
                framelesswindow/framelesswindow.cpp \
                framelesswindow/windowdragger.cpp \
                serialbuffer.cpp

HEADERS     += \
                gpicontroller.h \
                DarkStyle.h \
                framelesswindow/windowdragger.h \
                framelesswindow/framelesswindow.h \
                serialBuffer.h


FORMS       += \
                gpicontroller.ui \
                framelesswindow/framelesswindow.ui

RESOURCES   += \
                darkstyle.qrc \
                framelesswindow.qrc


