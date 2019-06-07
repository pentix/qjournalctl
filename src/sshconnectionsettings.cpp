#include "sshconnectionsettings.h"

// ** Static helper functions **

// Allocates a new C-style string that can be passed to libssh.
// Needs to be freed afterwards!
const char *SSHConnectionSettings::qstringToChar(QString s)
{
    const char   *tmpStr    = s.toUtf8().constData();
    const size_t  tmpStrLen = strlen(tmpStr);
          char   *cStr      = (char *)malloc(tmpStrLen+1);

    strncpy(cStr, tmpStr, tmpStrLen+1);

    return cStr;
}



// ** Actual class code **

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
{
    free((void *)this->name);
    free((void *)this->hostname);
    free((void *)this->username);
    free((void *)this->keyfile);
}

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
