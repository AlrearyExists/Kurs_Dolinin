#-------------------------------------------------
#
# Project created by QtCreator 2013-04-07T15:44:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qbilliard.cpp \
    myqwtplot.cpp

HEADERS  += mainwindow.h \
    qbilliard.h \
    myqwtplot.h



FORMS    += mainwindow.ui

INCLUDEPATH += /usr/include/qwt

LIBS += -lqwt

CONFIG += qwt
