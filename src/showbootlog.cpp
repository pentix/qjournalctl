/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016 by Patrick Eigensatz <patrick.eigensatz@gmail.com> 
 * Some rights reserved. See LICENSE.
 */


#include "src/showbootlog.h"
#include "ui_showbootlog.h"

#include <iostream>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>


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
            ui->label->setText("Showing journal of the current boot  (realtime following enabled)");
        } else {
            ui->label->setText("Showing journal of boot #" + bootid);
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


    // Fixing realtime bug: Maybe there isn't a single entry with
    // the selected properties. Therefore appendToBootLog() never
    // gets called, because it never gets the '-- No Entries --' output
    // from journalctl, leading to an out-of-date numerOfEntriesLabel.
    if(realtime){
        ui->numberOfEntriesLabel->setText("Showing <b>0</b> lines");
    }


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
            command = "journalctl -f --no-tail -p " + QString::number(maxPriority) + " -b " + bootid + sinceStr + untilStr;
        } else {
            command = "journalctl -a -p " + QString::number(maxPriority) + " -b " + bootid + sinceStr + untilStr;
        }
    }

    // Enable filtering by syslog identifiers
    command += identifierFlags;


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
        ui->numberOfEntriesLabel->setText("Showing <b>" + QString::number(ui->plainTextEdit->document()->lineCount()-1) + "</b> lines");
    } else {
        ui->numberOfEntriesLabel->setText("Showing <b>0</b> lines");
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

void ShowBootLog::on_filterButton_clicked()
{
    QString inputIdentifiers = ui->identifiersLineEdit->text();
    QStringList identifiers = inputIdentifiers.split(" ");

    identifierFlags = "";
    for(QString identifier : identifiers){
        if(identifier != ""){
            identifierFlags += " -t " + identifier;
        }
    }

    updateBootLog();
}
