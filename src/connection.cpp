#include "connection.h"
#include "local.h"

#include <QDebug>

Connection::Connection(QObject *qObject)
{
	this->qObject = qObject;
	this->localConnection = new Local(qObject);

	// Make sure we forward the local process output
	connect(localConnection, SIGNAL(localDataAvailable(QString)), this, SLOT(processData(QString)));
}

Connection::~Connection()
{
	delete localConnection;
}

void Connection::processData(QString data)
{
	emit connectionDataAvailable(data);
}

void Connection::run(QString cmd)
{
	localConnection->run(cmd);
}

bool Connection::isRunning()
{
	return localConnection->isRunning();
}

void Connection::close()
{
	localConnection->close();
}


