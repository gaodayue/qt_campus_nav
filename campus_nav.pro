#-------------------------------------------------
#
# Project created by QtCreator 2013-12-14T15:11:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = campus_nav
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graph.cpp \
    mapping.cpp \
    position.cpp

HEADERS  += mainwindow.h \
    graph.h \
    mapping.h \
    position.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
