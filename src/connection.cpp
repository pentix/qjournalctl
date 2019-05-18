#include "connection.h"
#include "local.h"

#include <QDebug>

// Constructor for a local connection
Connection::Connection(QObject *qObject)
{
	this->qObject = qObject;
	this->localConnection = new Local(qObject);
	this->remote = false;

	// Make sure we forward the local process output
	connect(localConnection, SIGNAL(localDataAvailable(QString)), this, SLOT(processData(QString)));
}

// Constructor for a remote SSH connection
Connection::Connection(QObject *qObject, QString hostname, QString username)
{
	this->qObject = qObject;
	this->remoteConnection = new Remote(qObject, hostname, username);
	this->remote = false;

	// Make sure we forward the local process output
	connect(localConnection, SIGNAL(localDataAvailable(QString)), this, SLOT(processData(QString)));
}

Connection::~Connection()
{
	if(remote){
		delete remoteConnection;
	} else {
		delete localConnection;
	}
}

void Connection::processData(QString data)
{
	if(remote){

	} else {
		emit connectionDataAvailable(data);
	}
}

void Connection::run(QString cmd)
{
	if(remote){

	} else {
		localConnection->run(cmd);
	}

}

bool Connection::isRemote()
{
	return remote;
}

bool Connection::isRunning()
{
	if(remote){
		return false;
	} else {
		return localConnection->isRunning();
	}
}

void Connection::close()
{
	if(remote){

	} else {
		localConnection->close();
	}
}


