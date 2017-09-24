#-------------------------------------------------
#
# Project created by QtCreator 2017-07-12T13:44:54
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = desktop
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    connectiondialog.cpp \
    DeviceData.cpp \
    PasswordTabView.cpp \
    Password.cpp \
    Aes.cpp \
    RandomGenerator.cpp \
    KeyDialog.cpp \
    Ascii85.cpp \
    AddPwdDialog.cpp \
    OptionTabView.cpp \
    RestoreTabView.cpp \
    SerialInterface.cpp \
    SerialDevice.cpp \
    SerialCommand.cpp

HEADERS  += mainwindow.h \
    connectiondialog.h \
    DeviceData.h \
    PasswordTabView.h \
    Password.h \
    Aes.h \
    RandomGenerator.h \
    KeyDialog.h \
    Ascii85.h \
    AddPwdDialog.h \
    OptionTabView.h \
    RestoreTabView.h \
    SerialInterface.h \
    SerialDevice.h \
    SerialCommand.h

FORMS    += mainwindow.ui \
    connectiondialog.ui \
    PasswordTabView.ui \
    KeyDialog.ui \
    AddPwdDialog.ui \
    OptionTabView.ui \
    RestoreTabView.ui

DISTFILES += \
    SerialDevice.h.old \
    SerialDevice.cpp.old
