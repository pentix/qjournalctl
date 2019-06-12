#include "local.h"

#include <QProcess>
#include <QDebug>

Local::Local(QObject *qObject)
{
    journalProcess = new QProcess(qObject);
    connect(journalProcess, SIGNAL(readyRead()), this, SLOT(processHasData()));
}


Local::~Local()
{
    delete journalProcess;
}


void Local::run(QString cmd)
{
    // Make sure to terminate an already running process first
    if(isRunning()){
        journalProcess->close();
    }

    journalProcess->start(cmd);
}

QString Local::runAndWait(QString cmd)
{
    QProcess p;
    p.start(cmd);
    p.waitForFinished(-1);

    return QString(p.readAllStandardOutput());
}

void Local::close()
{
    journalProcess->close();
}


bool Local::isRunning()
{
    return (journalProcess->state() == QProcess::Running
            || journalProcess->state() == QProcess::Starting);
}


void Local::processHasData()
{
    // Collect the process output, then forward this
    // event through the abstraction layers
    QByteArray read = journalProcess->read(8192000);

    QString readString = QString(read);
    readString = readString.left(readString.size()-1);

    emit localDataAvailable(readString);
}
