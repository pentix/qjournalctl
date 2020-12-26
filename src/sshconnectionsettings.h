/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2021 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef SSHCONNECTIONSETTINGS_H
#define SSHCONNECTIONSETTINGS_H

#include <QString>
#include <cstring>

class SSHConnectionSettings
{
public:
   SSHConnectionSettings();
   SSHConnectionSettings(QString name, QString hostname, unsigned int port, QString username, QString keyfile, bool useKeyfile);
   ~SSHConnectionSettings();

   const char *getName() const;
   const char *getHostname() const;
   const unsigned int *getPort() const;
   const char *getUsername() const;
   const char *getKeyfile() const;
   bool useKeyfile() const;

   static const char *qstringToChar(QString s);

private:
   const char *name;
   const char *hostname;
   unsigned int port;	// Note: libssh requires an unsigned int
   const char *username;
   const char *keyfile;
   bool keyfileAuthentication;
};

#endif // SSHCONNECTIONSETTINGS_H
