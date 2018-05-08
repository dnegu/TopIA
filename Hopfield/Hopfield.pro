#-------------------------------------------------
#
# Project created by QtCreator 2017-05-29T22:14:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hopfield
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    matriz.hpp

FORMS    += mainwindow.ui

INCLUDEPATH += "/usr/local/include/"

LIBS += `pkg-config --libs opencv` -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
