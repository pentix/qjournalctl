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

# It is assumed that VCPKG is installed under the current
# project folder. Otherwise, adjust this path with yours or
# just run
# qmake qjournalctl.pro CONFIG+=release CONFIG+=x86_64 VCPKG_FOLDER=%VCPKG_INSTALL_FOLDER%
!defined(VCPKG_FOLDER, var):VCPKG_FOLDER = $$_PRO_FILE_PWD_\vcpkg

SOURCES += src/main.cpp\
	src/connectiondialog.cpp \
	src/connection.cpp \
	src/connectionmanager.cpp \
	src/exceptions.cpp \
	src/local.cpp \
	src/mainwindow.cpp \
	src/aboutdialog.cpp \
	src/passworddialog.cpp \
	src/remote.cpp \
	src/showbootlog.cpp \
	src/sshconnectionserializer.cpp \
	src/sshconnectionsettings.cpp

HEADERS  += src/mainwindow.h \
	src/aboutdialog.h \
	src/connectiondialog.h \
	src/connection.h \
	src/connectionmanager.h \
	src/exceptions.h \
	src/local.h \
	src/passworddialog.h \
	src/remote.h \
	src/sshconnectionserializer.h \
	src/sshconnectionsettings.h \
	src/version.h \
	src/showbootlog.h

FORMS    += ui/mainwindow.ui \
	ui/aboutdialog.ui \
	ui/connectiondialog.ui \
	ui/connectionmanager.ui \
	ui/passworddialog.ui \
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
metainfo-file.path = /usr/share/metainfo
metainfo-file.files += qjournalctl.appdata.xml

INSTALLS += target desktop-file desktop-icon metainfo-file

CONFIG(x86_64) {
	    # 64 bit path
        win32: INCLUDEPATH += $$VCPKG_FOLDER\installed\x64-windows\include
        win32: LIBS += $$VCPKG_FOLDER\installed\x64-windows\lib\ssh.lib
} else {
        # 32 bit path
        win32: INCLUDEPATH += $$VCPKG_FOLDER\installed\x86-windows\include
        win32: LIBS += $$VCPKG_FOLDER\installed\x86-windows\lib\ssh.lib
}

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libssh
