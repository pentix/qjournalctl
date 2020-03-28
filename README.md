# QJournalctl v0.6.2 

[![Build Status](https://travis-ci.org/pentix/qjournalctl.svg?branch=master)](https://travis-ci.org/pentix/qjournalctl)
[![Build Status](https://ci.appveyor.com/api/projects/status/32r7s2skrgm9ubva?svg=true)](https://ci.appveyor.com/api/projects/status/32r7s2skrgm9ubva?svg=true)


### A Qt-based Graphical User Interface for systemd's journalctl command 


## About QJournalctl 
systemd provides `journalctl` to display and analyze its journal. Think of
the journal as a giant log file for the whole system. Different programs
(like e.g. different software/services on your system, but also the kernel) write their log entries into systemd's
journal. journalctl supports a few flags/parameters to specify what should
be displayed. QJournalctl is just a graphical user interface which displays
the journal and lets the user simply change the parameters to check the log
quickly for specific reports or errors.

Maybe you want to checkout the [Changelog](https://github.com/pentix/qjournalctl/blob/master/CHANGELOG.md).


## QJournalctl for Linux

### ArchLinux and Manjaro
QJournalctl is available in the community repository for the **Archlinux** and **Manjaro** Distributions:

```bash
sudo pacman -S qjournalctl
```
For other distributions, it is required to build it from sources.

### Build Dependencies
* Make sure your compiler supports (at least) C++11 (E.g. `g++` ≥ 4.8.1, `clang` ≥ 3.3)
* QJournalctl relies on Qt5, please ensure to have the Qt5 development libaries (E.g. `qtbase5-dev` for Debian/Ubuntu) installed, when compiling!
* To access remote hosts QJournalctl heavily relies on `libssh` ≥ [0.8.7](https://www.libssh.org/files/0.8/)

### Build Dependencies (Old Distros) 
Your distribution's supplied version of `libssh` might be too old for a successful build. You need
to build and install libssh yourself (< 2 minutes!)

`sudo apt-get install qtbase5-dev cmake`

`wget https://www.libssh.org/files/0.9/libssh-0.9.3.tar.xz`

`tar xf libssh-0.9.3.tar.xz ; cd libssh-0.9.3`

`mkdir build ; cd build`

`cmake .. && sudo make install`

`cd ../..`


### Building QJournalctl
1. Download the source code and extract it
2. Run `./autogen.sh`
3. Run `make -j5` to compile qjournalctl


## QJournalctl for Windows

### Build Dependencies

To buuild QJournalctl for Windows, it is needed
- MSVC 2017 C++ Build Tools for x64/x86 (>= `v141`). They are part of the Microsoft Visual Studio Community 2017 and can be also installed in the version 2019 (https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=vs-2019)
- Qt Open-Source >= `5.13.2`
- vcpkg >= `2020.01` (https://github.com/microsoft/vcpkg)

### Installing `vcpkg`
1. Checkout the vcpkg github repository within the folder of the `qjournalctl` repository (`<qjournalctlfoldr>/vcpkg/`) from https://github.com/microsoft/vcpkg
2. Open a `cmd` and go to `<qjournalctlfoldr>/vcpkg/`
3. run `./bootstrap-vcpkg.bat`. With this, you will get vcpkg ready to be used

### Install the QJournalctl Dependencies
`QJournalctl` is based on `libssh`. It is possible to obtain it using `vcpkg`:
1. Navigate to the `vcpkg` folder (`<qjournalctlfoldr>/vcpkg/`)
2. Get the `libssh` dependencies for the target needed:
```
vcpkg install libssh:x64-windows
```

Now, the `libssh` binaries as well as its dependencies can be found at `<repository_root>/vcpkg/packages/`

### Building QJournalctl

It is possible to build `QJournalctl` using two different manners
- Using Qt Creator. 
- Using the `autogen_and_build.bat` script.

#### Build using Qt Creator

First, it is needed to configure qtcreator by enabling the MSVC toolchain. Configure it for 32 and 64 bits, release and debug (see my screenshot) by navigating to `projects > Manage Kits > add ...`

Once you have it installed, 
- Click in the "screen" icon and select the msvc 2017 64 bits release configuration
- Now, Build at `Build > Build project "qjournalctl"`

#### Build using `autogen_and_build.bat`
Considering that the *Install the QJournalctl Dependencies* steps are already performed, first adjust `autogen_and_build.bat` with your own paths
- Modify the variable `VCPKG_INSTALL_FOLDER` defined in the `autogen_and_build.bat` with the path where you have vcpkg, (i.e. set `VCPKG_INSTALL_FOLDER= <yourqjournalctlrepopath>\vcpkg`)
- Modify also the path where the Qt for the given toolchain path is found (`QTDIR` may be for instance `C:\Qt\Qt5.14.1\5.14.1\msvc2017_64`)

From this point

1. Open a Visual Studio console (programs > msvc 2019 native x64 build tools) or open a console (`cmd`) and run `<visual_studio_install_path>\2017\Community\VC\Auxiliary\Build\vcvarsall.bat x64`:

```
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

2. Run `autogen_and_build.bat`

#### Where to find the built application?

The application can be found at `release/` folder.

**Note** The process can be repeated for the `x86` platform by modifying the `autogen_and_build.bat` script in the commented sections


#### Screenshots
##### Use the ssh feature to connect to remote hosts and read their logs; Save frequently used connections and benefit from password or (encrypted) keyfile support
![Use the ssh feature to connect to remote hosts and read their logs](https://user-images.githubusercontent.com/3193006/69094424-9d3e7900-0a50-11ea-94e3-5165b25d93cb.png)

##### Use the filter option and regular expressions to find what you are looking for
![Use the filter option and regular expressions to find what you are looking for](https://user-images.githubusercontent.com/3193006/36170961-3fbc1ed0-1101-11e8-9123-ceda9a1b6c8c.gif)

##### Finally found the interesting log entries? Directly export your selection!
![Finally found the interesting log entries? Directly export your selection!](https://user-images.githubusercontent.com/3193006/36171008-5f55351a-1101-11e8-8885-f17723944868.gif)

##### Search the journal while it keeps itself up-to-date
![Searching the journal in realtime](https://image.prntscr.com/image/lWwWdJV2Qk_nLOKxVl54xg.png "Searching the journal")

##### The current boot is not enough? Search the entire (!) journal of all boots or select a previous boot
![Show all saved boot logs or the complete journal](http://image.prntscr.com/image/3d7ba5b4d684489db4184b5cd97743c9.png "Show all saved boot logs or the complete journal")


