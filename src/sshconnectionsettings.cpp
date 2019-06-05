#include "sshconnectionsettings.h"

SSHConnectionSettings::SSHConnectionSettings(QString name, QString hostname, unsigned int port, QString username)
{
		this->name = name;
		this->hostname = hostname;
		this->port = port;
		this->username = username;
}

QString SSHConnectionSettings::getHostname() const
{
		return hostname;
}

QString SSHConnectionSettings::getName() const
{
		return name;
}

unsigned int SSHConnectionSettings::getPort() const
{
		return port;
}

QString SSHConnectionSettings::getUsername() const
{
		return username;
}
