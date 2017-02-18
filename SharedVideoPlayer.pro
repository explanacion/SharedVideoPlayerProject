#-------------------------------------------------
#
# Project created by QtCreator 2016-12-12T19:49:54
#
#-------------------------------------------------

QT       += core gui avwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SharedVideoPlayer
TEMPLATE = app


SOURCES += main.cpp\
        sharedvideo.cpp

HEADERS  += sharedvideo.h

FORMS    += sharedvideo.ui

LIBS += -L"C:\Qt\projects\QtAV-depends-windows-x86+x64\lib"
INCLUDEPATH += "C:\Qt\projects\QtAV-depends-windows-x86+x64\include"

