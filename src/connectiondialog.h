/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2018 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
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
    explicit ConnectionDialog(QWidget *parent=nullptr, SSHConnectionSettings **settings=nullptr, SSHConnectionSerializer *sshConnectionSerializer=nullptr);
    ~ConnectionDialog();

private slots:
    void on_openButton_clicked();
    void on_saveOpenButton_clicked();
    void on_hostnameLineEdit_returnPressed();

    void on_authKeyfileRadio_toggled(bool checked);

    void on_usernameLineEdit_returnPressed();

private:
    Ui::ConnectionDialog *ui;
    SSHConnectionSettings **settings;

    SSHConnectionSettings *generateConnectionSettingsFromData();
    SSHConnectionSerializer *sshConnectionSerializer;
};

#endif // CONNECTIONDIALOG_H
