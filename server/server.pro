#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T09:05:33
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    socket.cpp \
    host.cpp

HEADERS += \
    socket.h \
    host.h

INCLUDEPATH += /usr/include/cppconn

LIBS += /usr/lib/libmysqlcppconn-static.a
LIBS += /usr/lib/libmysqlcppconn.a /usr/lib/x86_64-linux-gnu/libmysqlclient.so.18.0.0
