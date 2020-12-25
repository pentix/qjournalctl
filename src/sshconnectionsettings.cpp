/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2021 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "sshconnectionsettings.h"

// ** Static helper functions **

// Allocates a new C-style string that can be passed to libssh.
// Needs to be freed afterwards!
const char *SSHConnectionSettings::qstringToChar(QString s)
{
    const int     tmpStrLen = s.toStdString().size();
          char   *cStr      = (char *)malloc(tmpStrLen+1);

    strncpy(cStr, s.toStdString().c_str(), tmpStrLen+1);
    cStr[tmpStrLen] = '\0';
    return cStr;
}



// ** Actual class code **
SSHConnectionSettings::SSHConnectionSettings()
{
    // Need this for the class to work with QVector
}

SSHConnectionSettings::SSHConnectionSettings(QString name, QString hostname, unsigned int port, QString username, QString keyfile, bool useKeyfile)
{
    this->name = qstringToChar(name);
    this->hostname = qstringToChar(hostname);
    this->port = port;
    this->username = qstringToChar(username);
    this->keyfile = qstringToChar(keyfile);
    this->keyfileAuthentication = useKeyfile;
}

SSHConnectionSettings::~SSHConnectionSettings()
{/*
   todo: fix this

    free((void *)this->name);
    free((void *)this->hostname);
    free((void *)this->username);
    free((void *)this->keyfile);
*/}

const char *SSHConnectionSettings::getHostname() const
{
     return hostname;
}

const char *SSHConnectionSettings::getName() const
{
    return name;
}

const unsigned int *SSHConnectionSettings::getPort() const
{
    return &port;
}

const char *SSHConnectionSettings::getUsername() const
{
    return username;
}

const char *SSHConnectionSettings::getKeyfile() const
{
    return keyfile;
}

bool SSHConnectionSettings::useKeyfile() const
{
    return keyfileAuthentication;
}
