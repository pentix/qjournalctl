/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2018 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include <QMessageBox>

ConnectionDialog::ConnectionDialog(QWidget *parent, ConnectionSettings **settings) :
	QDialog(parent),
	ui(new Ui::ConnectionDialog)
{
	ui->setupUi(this);
	this->settings = settings;
}

ConnectionDialog::~ConnectionDialog()
{
	delete ui;
}

ConnectionSettings *ConnectionDialog::generateConnectionSettingsFromData()
{
	QMessageBox *incompleteMessageBox = new QMessageBox(QMessageBox::Warning, "Incomplete data", "Please provide a correct hostname!");

	if(ui->hostnameLineEdit->text().trimmed() == ""){
		incompleteMessageBox->show();
		return nullptr;
	}

	int port = ui->portLineEdit->text().toInt();
	if(port == 0)
		port = 22;

	if(port < 0 || port > 65635){
		incompleteMessageBox->setText("Please provide a correct port! The default port for ssh is 22.");
		incompleteMessageBox->show();
		return nullptr;
	}

	return new ConnectionSettings(ui->connectionNameLineEdit->text().replace("\"", "\\\"").trimmed(),
											   ui->hostnameLineEdit->text().replace("\"", "\\\"").trimmed(),
											   (unsigned short)port,
											   ui->usernameLineEdit->text().replace("\"", "\\\"").trimmed()
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
