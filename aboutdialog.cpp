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

    if(GITREV != "")
        gitStr = QString(" (git revision: ") + GITREV + QString(")");

    ui->versionLabel->setText(QString("v") + QString(VERSION) + gitStr);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
    this->close();
}
