/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2021 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    PasswordDialog(QWidget *parent=nullptr, QString text="Enter password:");
    ~PasswordDialog();

    const char *getPassword();

private slots:
    void on_acceptButton_clicked();
    void on_passwordEdit_returnPressed();

private:
    Ui::PasswordDialog *ui;
    char *allocatedPassword;
};

#endif // PASSWORDDIALOG_H
