REM ---------------------------------------------------------------------
REM Batch file to make all Makefiles or all Visual Studio project files
REM ---------------------------------------------------------------------



REM ---------------------------------------------------------------------
REM            Modify this section to fit your toolchain before
REM                     trying to build qjournalctl
REM ---------------------------------------------------------------------

rem "Set ARCH to x86 only if you want to build the 32 bit version"
set ARCH=x86_64

rem "Set the appropriate Qt directory"
set QTDIR=C:\Qt\5.13.2\msvc2017_64

rem "Set the appropriate VCPKG folder"
set VCPKG_INSTALL_FOLDER=c:\tools\vcpkg


REM ----------------------------------------------------------------------




set OLD_PATH=%PATH%

REM Determine Visual Studio Version if newer > 2017
set VS_VERSION=2017
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\" (
	set VS_VERSION=2019
)

if "%ARCH%"=="x86" (
	call "C:\Program Files (x86)\Microsoft Visual Studio\%VS_VERSION%\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
)

if "%ARCH%"=="x86_64" (
	call "C:\Program Files (x86)\Microsoft Visual Studio\%VS_VERSION%\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

set PATH=%QTDIR%\bin;%PATH%
set QT_SELECT=qt5
set QMAKESPEC=win32-msvc

rem 32 bits
if "%ARCH%"=="x86" (
	qmake qjournalctl.pro CONFIG+=release VCPKG_FOLDER=%VCPKG_INSTALL_FOLDER%
)

rem 64 bits
if "%ARCH%"=="x86_64" (
	qmake qjournalctl.pro CONFIG+=release CONFIG+=%ARCH% VCPKG_FOLDER=%VCPKG_INSTALL_FOLDER%
)

nmake
windeployqt release/qjournalctl.exe

rem 32 bits
if "%ARCH%"=="x86" (
	copy %VCPKG_INSTALL_FOLDER%\vcpkg\packages\libssh_x86-windows\bin\ssh.dll release\ssh.dll
	copy %VCPKG_INSTALL_FOLDER%\vcpkg\packages\pthreads_x86-windows\bin\ssh.dll release\pthreadVC3.dll
)

rem 64 bits
if "%ARCH%"=="x86_64" (
	copy %VCPKG_INSTALL_FOLDER%\installed\x64-windows\bin\ssh.dll release\ssh.dll
	copy %VCPKG_INSTALL_FOLDER%\installed\x64-windows\bin\pthreadVC3.dll release\pthreadVC3.dll
)

	
del release\*.obj
del release\*.cpp

REM Restore the new path
set PATH=%OLD_PATH%