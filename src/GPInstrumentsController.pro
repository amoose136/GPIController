#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T10:15:22
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPInstrumentsController
TEMPLATE = app

INCLUDEPATH +="darkstyle"
INCLUDEPATH +="framelesswindow"

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES     += \
                main.cpp \
                gpicontroller.cpp

HEADERS     += \
                gpicontroller.h
                DarkStyle.h

FORMS       += \
                gpicontroller.ui

RESOURCES   += \
                darkstyle.qrc \
                framelesswindow.qrc


