# QJournalctl v0.4
### A Qt-based Graphical User Interface for systemd's journalctl command 



#### About QJournalctl 
systemd provides `journalctl` to display and analyze its journal. Think of
the journal as a giant log file for the whole system. Different programs
(like KDE Apps, but also the kernel) write their log entries into systemd's
journal. journalctl supports a few flags/parameters to specify what should
be displayed. QJournalctl is just a graphical user interface which displays
the journal and lets the user simply change the parameters to check the log
for specific reports or errors.

Maybe you want to checkout the [Changelog](https://github.com/pentix/qjournalctl/blob/master/CHANGELOG.md).


#### How to get qjournalctl
##### ArchLinux
qjournalctl is available on the AUR. You can either download the corresponding PKGBUILD or install the package via `yaourt -S qjournalctl`

If you like this project or if you want to see qjournalctl outside of the AUR in the official Arch community repository you can also vote for it!

[Visit the AUR page for more information](https://aur.archlinux.org/packages/qjournalctl/)


##### Other distros
1. Download the source code and extract it
2. Run `./autogen.sh`
3. Run `make` to compile qjournalctl


#### Screenshots
![Searching the journal in realtime](http://image.prntscr.com/image/1c42499604ae4a69882af8869efac24e.png "Searching the journal")

![Looking through an old boot log](http://image.prntscr.com/image/13de56e1fa74401283ec2593afa51a89.png "Looking through an old boot log")

![Show all saved boot logs or the complete journal](http://image.prntscr.com/image/3d7ba5b4d684489db4184b5cd97743c9.png "Show all saved boot logs or the complete journal")


