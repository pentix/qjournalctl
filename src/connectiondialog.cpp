/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2018 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include <QMessageBox>
#include <QDir>

ConnectionDialog::ConnectionDialog(QWidget *parent, SSHConnectionSettings **settings) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    this->settings = settings;

    // Adjust keyfile path to standard ssh rsa key location
    ui->keyfileLineEdit->setText(QDir::homePath() + "/.ssh/id_rsa");
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

SSHConnectionSettings *ConnectionDialog::generateConnectionSettingsFromData()
{
    QMessageBox *incompleteMessageBox = new QMessageBox(QMessageBox::Warning, "Incomplete data", "Please provide a correct hostname!");

    if(ui->hostnameLineEdit->text().trimmed() == ""){
        incompleteMessageBox->show();
        return nullptr;
    }

    unsigned int port = ui->portLineEdit->text().toUInt();
    if(port == 0)
        port = 22;

    if(port > 65635){
        incompleteMessageBox->setText("Please provide a correct port! The default port for ssh is 22.");
        incompleteMessageBox->show();
        return nullptr;
    }

    return new SSHConnectionSettings(ui->connectionNameLineEdit->text().replace("\"", "\\\"").trimmed(),
                                        ui->hostnameLineEdit->text().replace("\"", "\\\"").trimmed(),
                                        port,
                                        ui->usernameLineEdit->text().replace("\"", "\\\"").trimmed(),
                                        ui->keyfileLineEdit->text().replace("\"", "\\\"").trimmed(),
                                        ui->authKeyfileRadio->isChecked()
                                    );
}

void ConnectionDialog::on_openButton_clicked()
{
    *settings = generateConnectionSettingsFromData();
    close();
}

void ConnectionDialog::on_saveOpenButton_clicked()
{


}


void ConnectionDialog::on_hostnameLineEdit_returnPressed()
{
    on_openButton_clicked();
}

void ConnectionDialog::on_usernameLineEdit_returnPressed()
{
    on_openButton_clicked();
}

void ConnectionDialog::on_authKeyfileRadio_toggled(bool checked)
{
    ui->keyfileLineEdit->setEnabled(checked);
}
