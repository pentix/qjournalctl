#include "passworddialog.h"
#include "ui_passworddialog.h"
#include "sshconnectionsettings.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    allocatedPassword = nullptr;
    ui->setupUi(this);
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
