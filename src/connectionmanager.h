/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2019 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QDialog>
#include <QVector>
#include <QStandardItemModel>
#include "sshconnectionserializer.h"

namespace Ui {
class ConnectionManager;
}

class ConnectionManager : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionManager(QWidget *parent=nullptr, SSHConnectionSerializer *sshConnectionSerializer=nullptr);
    ~ConnectionManager();

private slots:
    void on_newConnectionButton_clicked();

    void on_editConnectionButton_clicked();

    void on_removeConnectionButton_clicked();

    void on_listView_doubleClicked();

    void on_closeButton_clicked();

private:
    Ui::ConnectionManager *ui;
    QStandardItemModel *connectionsModel;
    SSHConnectionSerializer *sshConnectionSerializer;

    void refreshSavedConnections();
};

#endif // CONNECTIONMANAGER_H
