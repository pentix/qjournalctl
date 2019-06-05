#ifndef SSHCONNECTIONSETTINGS_H
#define SSHCONNECTIONSETTINGS_H

#include <QString>

class SSHConnectionSettings
{
public:
   SSHConnectionSettings(QString name, QString hostname, unsigned int port, QString username);
   SSHConnectionSettings();

   QString getName() const;
   QString getHostname() const;
   unsigned int getPort() const;
   QString getUsername() const;

private:
   QString name;
   QString hostname;
   unsigned int port;	// Note: libssh requires an unsigned int
   QString username;
};

#endif // SSHCONNECTIONSETTINGS_H
