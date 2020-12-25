/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2021 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef SSHCONNECTIONSERIALIZER_H
#define SSHCONNECTIONSERIALIZER_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QVector>

#include "connection.h"

class SSHConnectionSerializer
{
public:
    SSHConnectionSerializer();
    ~SSHConnectionSerializer();

    void add(SSHConnectionSettings *sshSettings);
    SSHConnectionSettings *get(int id);
    void update(int id, SSHConnectionSettings *sshSettings);
    void remove(int id);
    QVector<SSHConnectionSettings *> *getConnectionsVector();

    static QJsonObject sshSettingsToJSON(SSHConnectionSettings *sshSettings);
    static SSHConnectionSettings *jsonToSSHSettings(QString name, QJsonObject jsonSettings);

private:
    QFile savedConnectionsFile;
    QVector<SSHConnectionSettings *> savedConnections;
    bool modifiedConnectionsFile;
};

#endif // SSHCONNECTIONSERIALIZER_H
