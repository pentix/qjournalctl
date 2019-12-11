/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2019 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QVector>

#include "connection.h"
#include "sshconnectionsettings.h"
#include "sshconnectionserializer.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_showBootLogButton_clicked();

    void on_listBootsButton_clicked();

    void on_actionAbout_triggered();

    void on_actionLoadBoots_triggered();

    void on_actionQuit_triggered();

    void on_tableView_doubleClicked();

    void on_actionShowCompleteJournal_triggered();

    void on_actionSizeOfTheJournalOnTheDisk_triggered();

    void on_tableView_clicked();

    void on_showCurrentBootLogButton_clicked();

    void on_actionShowCurrentBootLog_triggered();

    void on_realtimeCheckBox_stateChanged(int arg1);

    void on_reverseCheckBox_stateChanged(int arg1);

    void on_actionOpen_a_new_SSH_connection_triggered();

    void on_actionDisconnect_from_current_host_triggered();

    void on_actionEdit_saved_connections_triggered();

    void connectToSavedConnection(int id);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *bootModel;
    QStandardItemModel *itemModel;

    QString lastSelection;
    Connection *currentConnection;
    SSHConnectionSettings *currentConnectionSettings;
    SSHConnectionSerializer *sshConnectionSerializer;
    QVector<QAction *> savedConnectionsActions;

    void refreshSavedConnectionsMenu();
    bool setupRemoteConnection();
};

#endif // MAINWINDOW_H
