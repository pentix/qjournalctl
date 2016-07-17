#include "src/showbootlog.h"
#include "ui_showbootlog.h"

#include <QProcess>

ShowBootLog::ShowBootLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowBootLog)
{
    ui->setupUi(this);
}


ShowBootLog::ShowBootLog(QWidget *parent, QString bootid) :
    QDialog(parent),
    ui(new Ui::ShowBootLog)
{

    ui->setupUi(this);
    ui->label->setText(ui->label->text() + " boot id: " + bootid);


    QProcess process;
    process.start("journalctl -b " + bootid);
    process.waitForFinished(-1);

    QString stdout = process.readAllStandardOutput();
    ui->plainTextEdit->document()->setPlainText(stdout);
}



ShowBootLog::~ShowBootLog()
{
    delete ui;
}

void ShowBootLog::on_pushButton_clicked()
{
    close();
}
