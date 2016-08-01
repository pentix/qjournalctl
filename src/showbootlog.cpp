#include "src/showbootlog.h"
#include "ui_showbootlog.h"

#include <iostream>
#include <QProcess>
#include <QMessageBox>


ShowBootLog::ShowBootLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowBootLog)
{
    ui->setupUi(this);
    journalProcess = new QProcess;
}


ShowBootLog::ShowBootLog(QWidget *parent, bool completeJournal, QString bootid) :
    QDialog(parent),
    ui(new Ui::ShowBootLog)
{

    // Call simple constructor first
    this->ShowBootLog(parent);


    this->bootid = bootid;
    this->completeJournal = completeJournal;

    ui->sinceDateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(-60));
    ui->untilDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    if(!completeJournal){
        ui->label->setText(ui->label->text() + " " + bootid);
    } else {
        ui->label->setText("Complete systemd journal");
    }

    updateBootLog();
}

ShowBootLog::~ShowBootLog()
{
    delete ui;
    delete journalProcess;
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


    QString command = "";
    if(!this->completeJournal){
        command = "journalctl -p " + QString::number(maxPriority) + " -b " + bootid + sinceStr + untilStr;
    } else {
        command = "journalctl -p " + QString::number(maxPriority) + sinceStr + untilStr;
    }


    QProcess *process = new QProcess;

    connect(process, SIGNAL(readyRead()), this, SLOT(appendToBootLog());

    process->start(command);


}


void ShowBootLog::appendToBootLog()
{
    ui->plainTextEdit->document()->setPlainText(ui->plainTextEdit->toPlainText() + );
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

void ShowBootLog::on_horizontalSlider_sliderMoved(int position)
{
    maxPriority = position;
    updateBootLog();
}
