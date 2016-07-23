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


    this->bootid = bootid;


    ui->label->setText(ui->label->text() + " boot id: " + bootid);
    ui->sinceDateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(-60));
    ui->untilDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    updateBootLog();
}

ShowBootLog::~ShowBootLog()
{
    delete ui;
}

void ShowBootLog::on_pushButton_clicked()
{
    close();
}





void ShowBootLog::updateBootLog()
{
    QString sinceStr = "";
    if(sinceFlag){
        sinceStr = " --since \"" + ui->sinceDateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:00") + "\"";
    }

    QString untilStr = "";
    if(untilFlag){
        untilStr = " --until \"" + ui->untilDateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:00") + "\"";
    }


    QProcess process;
    process.start("journalctl -b " + bootid + sinceStr + untilStr);
    process.waitForFinished(-1);

    QString stdout = process.readAllStandardOutput();
    ui->plainTextEdit->document()->setPlainText(stdout);
}


void ShowBootLog::on_sinceCheckBox_clicked()
{
    sinceFlag = !sinceFlag;
    ui->sinceDateTimeEdit->setEnabled(sinceFlag);
    updateBootLog();
}

void ShowBootLog::on_untilCheckBox_clicked()
{
    untilFlag = !untilFlag;
    ui->untilDateTimeEdit->setEnabled(untilFlag);
    updateBootLog();
}

void ShowBootLog::on_sinceDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    updateBootLog();
}

void ShowBootLog::on_untilDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    updateBootLog();
}
