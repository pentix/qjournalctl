#include "src/showbootlog.h"
#include "ui_showbootlog.h"

#include <iostream>
#include <QProcess>
#include <QMessageBox>
#include <QtConcurrent/qtconcurrentrun.h>


using namespace QtConcurrent;



ShowBootLog::ShowBootLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowBootLog)
{
    ui->setupUi(this);
}


ShowBootLog::ShowBootLog(QWidget *parent, bool completeJournal, QString bootid) :
    QDialog(parent),
    ui(new Ui::ShowBootLog)
{

    ui->setupUi(this);
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
}

void ShowBootLog::on_pushButton_clicked()
{
    close();
}




void realtimeRead(QProcess &process, QPlainTextEdit &plainTextEdit){
    QByteArray output;

    while(true){
        process.waitForReadyRead();
        output = process.read(4096000);

        plainTextEdit.document()->setPlainText(plainTextEdit.toPlainText() + QString(output));

    }
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


    QProcess process;
    process.start(command);

    ui->plainTextEdit->clear();

    QFuture<void> rtr = run(realtimeRead, process, ui->plainTextEdit);


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
