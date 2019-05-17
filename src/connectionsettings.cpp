#include "connectionsettings.h"

// Create a remote connection
ConnectionSettings::ConnectionSettings(QString name, QString hostname, unsigned short port, QString username)
{
		this->name = name;
		this->hostname = hostname;
		this->port = port;
		this->username = username;

		this->local = false;
}

// Create a local connection
ConnectionSettings::ConnectionSettings(){
		this->local = true;
}

QString ConnectionSettings::getHostname() const
{
		return hostname;
}

QString ConnectionSettings::getName() const
{
		return name;
}

QString ConnectionSettings::getUsername() const
{
		return username;
}

bool ConnectionSettings::isLocal() const
{
		return local;
}

QString ConnectionSettings::getShellCommand()
{
		if(local){
				return "/bin/sh";
		}

		QString command = "ssh -t -t ";

		if(port != 22)
				command += "-p " + QString(port);

		if(username != "")
				command += "\""+username+"\"@";

		command += "\"" + hostname + "\" ";

		return command;
}
