/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2019 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "version.h"


AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);


	// Concat gitStr
	QString gitStr = "";

	if(QString(GITREV) != QString("0000000"))
		gitStr = QString("  (git commit: ") + GITREV + QString(")");

	ui->versionLabel->setText(QString("v") + QString(VERSION) + gitStr);

    // Set the about dialog as a fixed-size dialog
    this->setFixedSize(688, 487);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
	this->close();
}
