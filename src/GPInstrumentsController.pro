#-------------------------------------------------
#
# Project created by QtCreator 2017-10-23T10:15:22
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPIController
TEMPLATE = app
CONFIG += static
INCLUDEPATH +="framelesswindow"
win32:RC_ICONS += images/autosampler256.ico

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES     += \
                main.cpp \
                gpicontroller.cpp \
                framelesswindow/framelesswindow.cpp \
                framelesswindow/windowdragger.cpp \
                serialbuffer.cpp \
                qtcspinbox.cpp \
                superqlineedit.cpp \
                qtcintspinbox.cpp \
                superqdatetimeedit.cpp

HEADERS     += \
                gpicontroller.h \
                DarkStyle.h \
                framelesswindow/windowdragger.h \
                framelesswindow/framelesswindow.h \
                serialBuffer.h \
                qtcspinbox.h \
                superqlineedit.h \
                qtcintspinbox.h \
                superqdatetimeedit.h


FORMS       += \
                gpicontroller.ui \
                framelesswindow/framelesswindow.ui

RESOURCES   += \
                darkstyle.qrc \
                framelesswindow.qrc


