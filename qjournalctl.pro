#-------------------------------------------------
#
# Project created by QtCreator 2016-05-07T16:07:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qjournalctl
TEMPLATE = app


CONFIG += c++11

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/aboutdialog.cpp \
    src/showbootlog.cpp

HEADERS  += src/mainwindow.h \
    src/aboutdialog.h \
    src/version.h \
    src/showbootlog.h

FORMS    += ui/mainwindow.ui \
    ui/aboutdialog.ui \
    ui/showbootlog.ui

DISTFILES += \
    ui/icon.xpm

RESOURCES += \
    ui/icon.qrc


# This prevents qmake from failing when trying to install
# the desktop environment files
QMAKE_STRIP = echo
target.path = /usr/bin

# Desktop environment files
desktop-file.path = /usr/share/applications
desktop-file.files += ui/qjournalctl.desktop
desktop-icon.path = /usr/share/pixmaps
desktop-icon.files += ui/qjournalctl.png

INSTALLS += target desktop-file desktop-icon
