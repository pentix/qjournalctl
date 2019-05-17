#include "connection.h"
#include "local.h"

#include <QDebug>

Connection::Connection(QObject *qObject)
{
	this->qObject = qObject;
	this->localConnection = new Local(qObject);

	// Make sure we forward the local process output
	connect(localConnection->journalProcess, SIGNAL(readyRead()), this, SLOT(processHasData()));
}

Connection::~Connection()
{
	delete localConnection;
}

void Connection::run(QString cmd)
{
	// Make sure to terminate an already running process first
	if(isRunning()){
		localConnection->journalProcess->close();
	}

	localConnection->journalProcess->start(cmd);
}

bool Connection::isRunning()
{
	return (localConnection->journalProcess->state() == QProcess::Running
			|| localConnection->journalProcess->state() == QProcess::Starting);
}


void Connection::close()
{
	localConnection->journalProcess->close();
}


void Connection::processHasData()
{
	// Collect the process output, then forward this
	// event through the abstraction layers
	QByteArray read = localConnection->journalProcess->read(8192000);

	QString readString = QString(read);
	readString = readString.left(readString.size()-1);

	emit connectionDataAvailable(readString);
}
