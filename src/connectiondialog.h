/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2018 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include "connectionsettings.h"
#include <QDialog>

namespace Ui {
class ConnectionDialog;
}


class ConnectionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ConnectionDialog(QWidget *parent=nullptr, ConnectionSettings **settings=nullptr);
	~ConnectionDialog();

private slots:
	void on_openButton_clicked();
	void on_saveOpenButton_clicked();

private:
	Ui::ConnectionDialog *ui;
	ConnectionSettings **settings;

	ConnectionSettings *generateConnectionSettingsFromData();
};

#endif // CONNECTIONDIALOG_H
