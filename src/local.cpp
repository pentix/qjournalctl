#include "local.h"

#include <QProcess>

Local::Local(QObject *qObject)
{
	journalProcess = new QProcess(qObject);
}


Local::~Local()
{
	delete journalProcess;
}
