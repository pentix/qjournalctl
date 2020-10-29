# QJournalctl v0.6.2 [![Build Status](https://travis-ci.org/pentix/qjournalctl.svg?branch=master)](https://travis-ci.org/pentix/qjournalctl)
### A Qt-based Graphical User Interface for systemd's journalctl command


#### About QJournalctl
systemd provides `journalctl` to display and analyze its journal. Think of
the journal as a giant log file for the whole system. Different programs
(like e.g. different software/services on your system, but also the kernel) write their log entries into systemd's
journal. journalctl supports a few flags/parameters to specify what should
be displayed. QJournalctl is just a graphical user interface which displays
the journal and lets the user simply change the parameters to check the log
quickly for specific reports or errors.

Maybe you want to checkout the [Changelog](https://github.com/pentix/qjournalctl/blob/master/CHANGELOG.md).

#### Build Dependencies
* Make sure your compiler supports (at least) C++11 (E.g. `g++` ≥ 4.8.1, `clang` ≥ 3.3)
* QJournalctl relies on Qt5, please ensure to have the Qt5 development libaries installed, when compiling! (E.g. `qtbase5-dev` for Debian/Ubuntu; on macOS these come with the `qt` Homebrew tap)
* To access remote hosts QJournalctl heavily relies on `libssh` ≥ [0.8.7](https://www.libssh.org/files/0.8/)


#### Build Dependencies for Debian, Ubuntu, et al.
Your distribution's supplied version of `libssh` might be too old for a successful build. You need
to build and install libssh yourself (< 2 minutes!)

`sudo apt-get install qtbase5-dev cmake`

`wget https://www.libssh.org/files/0.9/libssh-0.9.3.tar.xz`

`tar xf libssh-0.9.3.tar.xz ; cd libssh-0.9.3`

`mkdir build ; cd build`

`cmake .. && sudo make install`

`cd ../..`


#### Dependencies for macOS
To handle dependencies of macOS, we're going to use [homebrew](https://brew.sh/).
`brew install qt libssh`
As indicated in the caveats:

````
qt is keg-only, which means it was not symlinked into /usr/local,
because Qt 5 has CMake issues when linked.

If you need to have qt first in your PATH run:
  echo 'export PATH="/usr/local/opt/qt/bin:$PATH"' >> /Users/user_name/.bash_profile
````

before building QJournalctl you'll indeed need to run:
`export PATH="/usr/local/opt/qt/bin:$PATH"`
(ne need to make it persistent by using .bash_profile)


#### Building QJournalctl
1. Download the source code and extract it
2. Run `./autogen.sh`
3. Run `make -j5` to compile qjournalctl


#### ArchLinux and Manjaro
QJournalctl is available in the community repository:
`sudo pacman -S qjournalctl`


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


