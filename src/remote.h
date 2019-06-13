/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2019 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef REMOTE_H
#define REMOTE_H

#include <thread>
#include <mutex>
#include <cassert>
#include <libssh/libssh.h>
#include <libssh/callbacks.h>
#include <QObject>

#include "sshconnectionsettings.h"

class Remote: public QObject
{
    Q_OBJECT

public:
    Remote(QObject *qObject, SSHConnectionSettings *sshSettings);
    ~Remote();

    void run(QString cmd);
    QString runAndWait(QString cmd);
    void close();
    bool isRunning();

private:
    std::thread *readerThread;
    volatile bool destroyAllThreads;
    QString sshCmd;

    std::recursive_mutex sshMutex;
    ssh_session ssh;
    ssh_channel sshChannel;
    void initSSHChannel();

signals:
    void remoteDataAvailable(QString);

public slots:
    void processHasData();
};

#endif // REMOTE_H
