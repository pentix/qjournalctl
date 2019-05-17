#ifndef CONNECTIONSETTINGS_H
#define CONNECTIONSETTINGS_H

#include <QString>

class ConnectionSettings
{
public:
   ConnectionSettings(QString name, QString hostname, unsigned short port, QString username);
   ConnectionSettings();

   QString getName() const;
   QString getHostname() const;
   unsigned short getPort() const;
   QString getUsername() const;
   bool isLocal() const;
   QString getShellCommand();

private:
   QString name;
   QString hostname;
   unsigned short port;
   QString username;
   bool local;
};

#endif // CONNECTIONSETTINGS_H
