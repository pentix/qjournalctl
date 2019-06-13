/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2019 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "passworddialog.h"
#include "ui_passworddialog.h"
#include "sshconnectionsettings.h"

PasswordDialog::PasswordDialog(QWidget *parent, QString text) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    allocatedPassword = nullptr;
    ui->setupUi(this);

    // Set the "Enter password:" text
    ui->textLabel->setText(text);
}

PasswordDialog::~PasswordDialog()
{
    if(allocatedPassword != nullptr){
        char *c = allocatedPassword;

        // (Try to) remove pw from heap
        while(*c != '\0'){
            *c = '\0';
            c++;
        }

        free((void *)allocatedPassword);
    }

    delete ui;
}

void PasswordDialog::on_acceptButton_clicked()
{
    close();
}

void PasswordDialog::on_passwordEdit_returnPressed()
{
    on_acceptButton_clicked();
}

const char *PasswordDialog::getPassword()
{
    allocatedPassword = (char *)SSHConnectionSettings::qstringToChar(ui->passwordEdit->text());
    return allocatedPassword;
}
