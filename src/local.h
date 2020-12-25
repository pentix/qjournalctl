/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2021 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef LOCAL_H
#define LOCAL_H

#include <QProcess>

class Local: public QObject
{
    Q_OBJECT

public:
    Local(QObject *qObject);
    ~Local();

    void run(QString);
    void close();
    bool isRunning();

private:
    QProcess *journalProcess;

signals:
    void localDataAvailable(QString);

public slots:
    void processHasData();

public:
    QString runAndWait(QString cmd);
};

#endif // LOCAL_H
