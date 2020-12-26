/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2021 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

#include "sshconnectionsettings.h"
#include "sshconnectionserializer.h"

namespace Ui {
class ConnectionDialog;
}


class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent=nullptr,
                              SSHConnectionSettings **settings=nullptr,
                              SSHConnectionSerializer *sshConnectionSerializer=nullptr,
                              bool createOnly=false);

    explicit ConnectionDialog(SSHConnectionSerializer *sshConnectionSerializer,
                              int id);

    explicit ConnectionDialog(SSHConnectionSettings **settings,
                              SSHConnectionSerializer *sshConnectionSerializer);
    ~ConnectionDialog();

private slots:
    void on_openButton_clicked();
    void on_saveOpenButton_clicked();
    void on_hostnameLineEdit_returnPressed();
    void on_authKeyfileRadio_toggled(bool checked);
    void on_usernameLineEdit_returnPressed();

    void on_cancelButton_clicked();

private:
    Ui::ConnectionDialog *ui;
    SSHConnectionSettings **settings;

    bool createOnly;
    bool updateOnly;
    int idToUpdate;

    void prepareUIFromExistingSSHSettings(SSHConnectionSettings *sshSettings);
    SSHConnectionSettings *generateConnectionSettingsFromData();
    SSHConnectionSerializer *sshConnectionSerializer;
};

#endif // CONNECTIONDIALOG_H
