include(../mere-auth-lib/mere-auth-lib.pri)
#include(../../mere-utils/mere-utils-lib/mere-utils-lib.pri)

QT += core
QT -= gui

CONFIG += c++11
CONFIG += console

TARGET = mere-auth-cli
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS MERE_AUTH

SOURCES += \
        src/cli.cpp \
        src/main.cpp

LIBS += -lmere-utils
INCLUDEPATH += /usr/local/include

#
# Install
#
unix
{
    target.path = /usr/local/bin
    INSTALLS += target
}

HEADERS += \
    src/cli.h
