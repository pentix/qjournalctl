# QJournalctl v0.5
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

#### How to get QJournalctl
##### Build Dependencies
* Make sure your compiler supports (at least) C++11 (E.g. `g++` ≥ 4.8.1, `clang` ≥ 3.3)
* Also QJournalctl relies on Qt 5, ensure to have the Qt5 development libaries (E.g. `qtbase5-dev` for Debian/Ubuntu) installed, when compiling


##### Building QJournalctl
1. Download the source code and extract it
2. Run `./autogen.sh`
3. Run `make` to compile qjournalctl


##### ArchLinux
QJournalctl is available on the AUR. You can either download the corresponding PKGBUILD or install the package via `yaourt -S qjournalctl`

If you like this project or if you want to see qjournalctl outside of the AUR in the official Arch community repository you can also vote for it!

[Visit the AUR page for more information](https://aur.archlinux.org/packages/qjournalctl/)



#### Screenshots
##### Use the filter option and regular expressions to find what you are looking for
![Use the filter option and regular expressions to find what you are looking for](https://user-images.githubusercontent.com/3193006/36170961-3fbc1ed0-1101-11e8-9123-ceda9a1b6c8c.gif)

##### Finally found the interesting log entries? Directly export your selection!
![Finally found the interesting log entries? Directly export your selection!](https://user-images.githubusercontent.com/3193006/36171008-5f55351a-1101-11e8-8885-f17723944868.gif)

##### Search the journal while it keeps itself up-to-date
![Searching the journal in realtime](https://image.prntscr.com/image/lWwWdJV2Qk_nLOKxVl54xg.png "Searching the journal")

##### The current boot is not enough? Search the entire (!) journal of all boots or select a previous boot
![Show all saved boot logs or the complete journal](http://image.prntscr.com/image/3d7ba5b4d684489db4184b5cd97743c9.png "Show all saved boot logs or the complete journal")


