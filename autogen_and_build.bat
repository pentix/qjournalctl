REM ---------------------------------------------------------------------
REM Batch file to make all Makefiles or all Visual Studio project files
REM ---------------------------------------------------------------------
set OLD_PATH=%PATH%
rem "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
rem set QTDIR=C:\Qt\Qt5.14.1\5.14.1\msvc2017
rem "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set QTDIR=C:\Qt\5.14.1\msvc2017_64
set PATH=%QTDIR%\bin;%PATH%
echo %PATH%
set QMAKE_BIN=qmake
set QT_SELECT=qt5
set QMAKESPEC=win32-msvc
set VCPKG_INSTALL_FOLDER=c:\tools\vcpkg
rem 32 bits
rem qmake qjournalctl.pro CONFIG+=release VCPKG_FOLDER=.

rem 64 bits
qmake qjournalctl.pro CONFIG+=release CONFIG+=x86_64 VCPKG_FOLDER=%VCPKG_INSTALL_FOLDER%

nmake
windeployqt release/qjournalctl.exe

rem 32 bits
rem copy %VCPKG_INSTALL_FOLDER%\vcpkg\packages\libssh_x86-windows\bin\ssh.dll release\ssh.dll
rem copy %VCPKG_INSTALL_FOLDER%\vcpkg\packages\pthreads_x86-windows\bin\ssh.dll release\pthreadVC3.dll

rem 64 bits
copy %VCPKG_INSTALL_FOLDER%\installed\x64-windows\bin\ssh.dll release\ssh.dll
copy %VCPKG_INSTALL_FOLDER%\installed\x64-windows\bin\pthreadVC3.dll release\pthreadVC3.dll

del release\*.obj
del release\*.cpp

REM Restore the new path
set PATH=%OLD_PATH%