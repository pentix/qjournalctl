#include "passworddialog.h"
#include "ui_passworddialog.h"

PasswordDialog::PasswordDialog(QWidget *parent, QString *target) :
	QDialog(parent),
	ui(new Ui::PasswordDialog)
{
	ui->setupUi(this);
	this->target = target;
}

PasswordDialog::~PasswordDialog()
{
	delete ui;
}

void PasswordDialog::on_acceptButton_clicked()
{
	*target = ui->passwordEdit->text();
	close();
}

void PasswordDialog::on_passwordEdit_returnPressed()
{
	on_acceptButton_clicked();
}
