#ifndef CONNECTION_H
#define CONNECTION_H

#include "local.h"


class Connection : public QObject
{
	Q_OBJECT

public:
	QObject *qObject;
	Connection(QObject *qObject);
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
};

#endif // CONNECTION_H
