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
    PasswordDialog(QWidget *parent = nullptr);
	~PasswordDialog();

    const char *getPassword();

private slots:
	void on_acceptButton_clicked();
	void on_passwordEdit_returnPressed();

private:
	Ui::PasswordDialog *ui;
};

#endif // PASSWORDDIALOG_H
