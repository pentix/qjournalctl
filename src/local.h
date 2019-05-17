#ifndef LOCAL_H
#define LOCAL_H

#include <QProcess>

class Local
{
public:
	QProcess *journalProcess;
	Local(QObject *qObject);
	~Local();
};

#endif // LOCAL_H
