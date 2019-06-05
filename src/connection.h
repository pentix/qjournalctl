#ifndef CONNECTION_H
#define CONNECTION_H

#include "local.h"
#include "remote.h"

class Connection : public QObject
{
	Q_OBJECT

public:
	QObject *qObject;
	// Local connection
	Connection(QObject *qObject);

	// Remote connection
	Connection(QObject *qObject, SSHConnectionSettings *sshSettings);
	~Connection();

	bool isRemote();
	bool isRunning();
	void close();
	void run(QString cmd);

signals:
	void connectionDataAvailable(QString);

public slots:
	void processData(QString);

private:
	Local *localConnection;
	Remote *remoteConnection;
	bool remote;
};

#endif // CONNECTION_H
