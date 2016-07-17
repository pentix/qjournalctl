#-------------------------------------------------
#
# Project created by QtCreator 2016-05-07T16:07:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = journalctlgui
TEMPLATE = app


SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/aboutdialog.cpp

HEADERS  += src/mainwindow.h \
    src/aboutdialog.h \
    src/version.h

FORMS    += ui/mainwindow.ui \
    ui/aboutdialog.ui
