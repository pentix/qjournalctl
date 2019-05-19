#ifndef REMOTE_H
#define REMOTE_H

#include <assert.h>
#include <libssh/libssh.h>
#include <QObject>

class Remote: public QObject
{
	Q_OBJECT

public:
	Remote(QObject *qObject, QString hostnameString, QString usernameString);
	~Remote();

	void run(QString);
	void close();
	bool isRunning();

private:
	ssh_session ssh;

signals:
	void remoteDataAvailable(QString);

public slots:
	void processHasData();
};

#endif // REMOTE_H
