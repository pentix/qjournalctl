#include "passworddialog.h"
#include "ui_passworddialog.h"
#include "sshconnectionsettings.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PasswordDialog)
{
	ui->setupUi(this);
}

PasswordDialog::~PasswordDialog()
{
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
    return SSHConnectionSettings::qstringToChar(ui->passwordEdit->text());
}
