/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2021 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef CONNECTION_H
#define CONNECTION_H

#include "local.h"
#include "remote.h"


enum CONNECTION_PARAM {
    LOCAL_DIRECTORY
};


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
    void run(QString);
    void setParam(CONNECTION_PARAM, QString);
    QString runAndWait(QString);

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
