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
    journalProcess = new QProcess(this);
}


ShowBootLog::ShowBootLog(QWidget *parent, bool completeJournal, bool realtime, QString bootid) :
    QDialog(parent),
    ui(new Ui::ShowBootLog)
{

    // Call simple constructor first
    ui->setupUi(this);
    journalProcess = new QProcess(this);


    this->bootid = bootid;
    this->completeJournal = completeJournal;
    this->realtime = realtime;

    ui->sinceDateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(-60));
    ui->untilDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    if(completeJournal){
        ui->label->setText("Complete systemd journal");
    } else {
        if(realtime){
            ui->label->setText("Journal:  Current boot  (realtime following enabled)");
        } else {
            ui->label->setText(ui->label->text() + bootid);
        }
    }

    updateBootLog();
}

ShowBootLog::~ShowBootLog()
{
    if(journalProcess->state() == QProcess::Running){
        journalProcess->close();
    }


    delete ui;
    delete journalProcess;
}

void ShowBootLog::on_pushButton_clicked()
{
    close();
}


void ShowBootLog::updateBootLog()
{
    // Clear log!
    ui->plainTextEdit->clear();


    QString sinceStr = "";
    if(sinceFlag){
        sinceStr = " --since \"" + ui->sinceDateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:00") + "\"";
    }

    QString untilStr = "";
    if(untilFlag){
        untilStr = " --until \"" + ui->untilDateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:00") + "\"";
    }


    QString command = "";
    if(this->completeJournal){
        command = "journalctl -a -p " + QString::number(maxPriority) + sinceStr + untilStr;
    } else {
        if(this->realtime){
            command = "journalctl -f -p " + QString::number(maxPriority) + " -b " + bootid + sinceStr + untilStr;
        } else {
            command = "journalctl -a -p " + QString::number(maxPriority) + " -b " + bootid + sinceStr + untilStr;
        }
    }


    // Connect readyRead signal to appendToBootLog slot
    // or close already opened process
    if(journalProcess->state() == QProcess::NotRunning){
        connect(journalProcess, SIGNAL(readyRead()), this, SLOT(appendToBootLog()));
    } else {
        journalProcess->close();
    }

    journalProcess->start(command);

}


void ShowBootLog::appendToBootLog()
{
    QByteArray read = journalProcess->read(4096000);
    ui->plainTextEdit->document()->setPlainText(ui->plainTextEdit->toPlainText() + QString(read));

    ui->plainTextEdit->ensureCursorVisible();

    // Update "numberOfEntries" label
    if(ui->plainTextEdit->toPlainText() != "-- No entries --\n"){
        ui->numberOfEntriesLabel->setText("Showing <b>" + QString::number(ui->plainTextEdit->document()->lineCount()-1) + "</b> entries");
    } else {
        ui->numberOfEntriesLabel->setText("Showing <b>0</b> entries");
    }

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
