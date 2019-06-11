/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2018 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include "exceptions.h"

#include <QMessageBox>
#include <QDir>
#include <QDebug>

ConnectionDialog::ConnectionDialog(QWidget *parent, SSHConnectionSettings **settings, SSHConnectionSerializer *sshConnectionSerializer, bool createOnly) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    this->updateOnly = false;
    this->createOnly = createOnly;
    this->settings = settings;
    this->sshConnectionSerializer = sshConnectionSerializer;

    // Adjust keyfile path to standard ssh rsa key location
    ui->keyfileLineEdit->setText(QDir::homePath() + "/.ssh/id_rsa");

    // If we only want to use the Dialog to create a new connection, don't display the "Open" button
    if(createOnly){
        ui->openButton->hide();
        ui->saveOpenButton->setText("Save");
    }
}

// Construct Dialog from existing SSHConnectionSettings to edit values
ConnectionDialog::ConnectionDialog(SSHConnectionSerializer *sshConnectionSerializer, int id) :
    QDialog(),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);

    // Prepare UI
    setWindowTitle("Edit saved connection details");
    SSHConnectionSettings *sshSettings = sshConnectionSerializer->get(id);
    ui->connectionNameLineEdit->setText(sshSettings->getName());
    ui->hostnameLineEdit->setText(sshSettings->getHostname());
    ui->portLineEdit->setText(QString::number(*sshSettings->getPort()));
    ui->usernameLineEdit->setText(sshSettings->getUsername());
    ui->keyfileLineEdit->setText(sshSettings->getKeyfile());
    ui->authKeyfileRadio->setChecked(sshSettings->useKeyfile());

    // UI adjustments because of already existing connection entry
    ui->openButton->hide();
    ui->saveOpenButton->setText("Save");
    createOnly = true;

    // Remember update-specific data
    this->sshConnectionSerializer = sshConnectionSerializer;
    idToUpdate = id;
    updateOnly = true;
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

SSHConnectionSettings *ConnectionDialog::generateConnectionSettingsFromData()
{
    if(ui->hostnameLineEdit->text().trimmed() == ""){
        Exceptions::warning("Please provide a correct hostname!");
        return nullptr;
    }

    unsigned int port = ui->portLineEdit->text().toUInt();
    if(port == 0){
        port = 22;
    }

    if(port > 65635){
        Exceptions::warning("Please provide a correct port! The default port for ssh is 22.");
        return nullptr;
    }

    return new SSHConnectionSettings(ui->connectionNameLineEdit->text().trimmed(),
                                        ui->hostnameLineEdit->text().trimmed(),
                                        port,
                                        ui->usernameLineEdit->text().trimmed(),
                                        ui->keyfileLineEdit->text().trimmed(),
                                        ui->authKeyfileRadio->isChecked()
                                    );
}

void ConnectionDialog::on_openButton_clicked()
{
    // todo: Check for nullptr first
    // todo: free?
    *settings = generateConnectionSettingsFromData();
    close();
}

void ConnectionDialog::on_saveOpenButton_clicked()
{
    // todo: Check for nullptr first
    // todo: free?

    if(ui->connectionNameLineEdit->text().trimmed() == ""){
        return;
    }

    SSHConnectionSettings *newSettings = generateConnectionSettingsFromData();
    if(newSettings == nullptr){
        return;
    }

    // Prepare the new connection details
    if(!createOnly){
        *settings = newSettings;
    }

    // Add/Update the connection to the serializer
    if(updateOnly){
        // todo: free old settings?
        sshConnectionSerializer->update(idToUpdate, newSettings);

    } else {
        sshConnectionSerializer->add(newSettings);
    }

    close();
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
