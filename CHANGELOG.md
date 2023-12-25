## Changelog

#### v0.6.4 (2023, Dec. 25)
* Support loading a journald DB folder (#70)
* Fix several small issues in the ShowBootLog / MainWindow module
* Add support for Systemd251+ (#81)
* Fix and improve Debian packaging

#### v0.6.3 (2020, Dec. 25)
* Added support for Windows and macOS (#41 by @alacasta, #64 by @Marcool04)
* Added connection indicator for SSH (#61)
* Minor bug fixes (#42, #52)
* Improved pipeline for compilation and deployment

#### v0.6.2 (2019, Dec. 12)
* Fixed the cropped host label in the main window
* Fixed possible null pointer issue on disconnecting from a remote host

#### v0.6.1 (2019, Dec. 11)
* Fixed a possible crash due to different libssh initialization behaviour

#### v0.6 (2019, Jun. 13)
* Quite some code restructuring and cleanup (#15) which led to:
* SSH Support! Connect to any remote host using a password or the public key infrastructure
  and display/search its journal. Save connection details you need to use often and load / edit them later on (#17)
* This requires `libssh`, which is now a build dependency. Information can be found in the `README` (#33)
* Fixed some build issues when old qmake was present (#20)
* Fixed some UI issues (#19, #21, #22)


#### v0.5.1 (2018, Feb. 18)
* Remove the trailing newline when realtime following is enabled (#16)
* Resized ShowBootLog window and other small design fixes to increase readability
* identifiersLineEdit gets cleared when entering a non-suggested identifier 


#### v0.5 (2018, Feb. 13)
* `make install` creates a .desktop file in `/usr/share/applications/` (#7)
* Added autocompletion for filtering by syslog identifiers. When loading
  the journal, a list of all identifiers is created (even if you're
  following the journal in realtime) and QJournalctl suggests you
  already appeared identifiers when filtering. (#9)
* Compile against C++11 by default (#10)
* Press [Ctrl][F] to search the journal for a normal string or a RegExp (#11)
* Added a button to export the filtered journal or only the current selection (#12)
* Added more keyboard shortcuts in the main window (Ctrl+A, Ctrl+B, Ctrl+L)


#### v0.4 (2017, Oct. 22)
* Added the possibility to reverse the journalctl output
* Display the log size additionally in bytes
* Added this changelog to the about dialog


#### v0.3 (2016, Sep. 17)
* Show an error message if no boots were found (#2)
* Implemented a "Show current boot" button (#3)
* UI improvements


#### v0.21 (2016, Aug. 28)
* Created a icon for QJournalctl
* Add a quit shortcut and the corresponding menu item (#1)
* Mentioned the contributions of @dopsi


#### v0.2 (2016, Aug. 13) 
* UI improvements
* Filtering the journal by identifiers


#### v0.1 (2016, Aug. 07,  initial release)
* Listing all boots
* Showing the journal of a specific boot
* Showing the complete journal
* Filter by date or priority
