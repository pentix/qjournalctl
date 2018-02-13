/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2018 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "src/showbootlog.h"
#include "ui_showbootlog.h"

#include <iostream>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QIcon>
#include <QStyle>
#include <QPushButton>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QShortcut>
#include <QLabel>
#include <QLineEdit>
#include <QTextDocument>
#include <QCheckBox>
#include <QSet>
#include <QRegularExpression>
#include <QCompleter>
#include <QThread>


ShowBootLog::ShowBootLog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::ShowBootLog)
{
	ui->setupUi(this);
	journalProcess = new QProcess(this);
}


ShowBootLog::ShowBootLog(QWidget *parent, bool completeJournal, bool realtime, bool reverse, QString bootid) :
	QDialog(parent),
	ui(new Ui::ShowBootLog)
{

	// Call simple constructor first
	ui->setupUi(this);
	journalProcess = new QProcess(this);

    // Set save icon for the export button
    ui->exportButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));

    // Create find and escape shortcut
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this, SLOT(on_find_keyshortcut_triggered()));
    new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(on_find_hide_keyshortcut_triggered()));


	this->bootid = bootid;
	this->completeJournal = completeJournal;
	this->realtime = realtime;
	this->reverse = reverse;

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

void ShowBootLog::on_closeButton_clicked()
{
	close();
}


void ShowBootLog::updateBootLog(bool keepIdentifiers)
{
	// Clear log!
	ui->plainTextEdit->clear();

    if(!keepIdentifiers){
        qDebug() << "Clearing identifiers" << endl;
        this->allIdentifiers.clear();
        ui->acceptedIdentifierLabel->setText("");
        identifierFlags = "";
    } else {
        identifierFlags = "";
        QString acceptedIdentifierLabelText = "";
        for(QString identifier : this->acceptedIdentifiers){
            identifierFlags += " -t " + identifier;
            acceptedIdentifierLabelText += identifier + "  ";
        }

        ui->acceptedIdentifierLabel->setText(acceptedIdentifierLabelText);
    }

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

	if(this->reverse){
		command = command + " -r";
    }

	// Enable filtering by syslog identifiers
	command += identifierFlags;

    qDebug() << command << endl;

	// Connect readyRead signal to appendToBootLog slot
	// or close already opened process
	if(journalProcess->state() == QProcess::NotRunning){
		connect(journalProcess, SIGNAL(readyRead()), this, SLOT(appendToBootLog()));
	} else {
		journalProcess->close();
	}

	// Reset byte counter
	numberOfBytesRead = 0;

	journalProcess->start(command);

}

void ShowBootLog::acceptIdentifier(void){
    qDebug() << "Accept identifier " << ui->identifiersLineEdit->text();
    this->acceptedIdentifiers.insert(ui->identifiersLineEdit->text());
    ui->identifiersLineEdit->clear();
    ui->identifiersLineEdit->setText("");
    updateBootLog(true);
}


void ShowBootLog::appendToBootLog()
{
    QByteArray read = journalProcess->read(8192000);
	QString readString = QString(read);

	// Append string to the UI and increment byte counter
	ui->plainTextEdit->appendPlainText(readString);
	numberOfBytesRead += readString.size();

	ui->plainTextEdit->ensureCursorVisible();

	// Update "numberOfEntries" label
	if(ui->plainTextEdit->toPlainText() != "-- No entries --\n"){
		ui->numberOfEntriesLabel->setText("Showing <b>" + QString::number(ui->plainTextEdit->document()->lineCount()-1) + "</b> lines ("+QString::number(numberOfBytesRead) + " bytes)");
	} else {
		ui->numberOfEntriesLabel->setText("Showing <b>0</b> lines (0 bytes)");
	}

    // Collect identifiers for auto completion
    QRegularExpression identifierRegexp = QRegularExpression(" ([a-zA-Z0-9\\_]+)\\[\\d+\\]\\: ", QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator matches = identifierRegexp.globalMatch(readString);

    QSet<QString> identifierSet;

    while(matches.hasNext()){
        QRegularExpressionMatch identifier = matches.next();
        identifierSet.insert(identifier.captured(1));
    }

    this->allIdentifiers += identifierSet;
    QCompleter *completer = new QCompleter(this->allIdentifiers.toList());
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    ui->identifiersLineEdit->setCompleter(completer);

}


void ShowBootLog::on_sinceCheckBox_clicked()
{
	sinceFlag = !sinceFlag;
	ui->sinceDateTimeEdit->setEnabled(sinceFlag);
    updateBootLog(true);
}

void ShowBootLog::on_untilCheckBox_clicked()
{
	untilFlag = !untilFlag;
	ui->untilDateTimeEdit->setEnabled(untilFlag);
    updateBootLog(true);
}

void ShowBootLog::on_sinceDateTimeEdit_dateTimeChanged()
{
    updateBootLog(true);
}

void ShowBootLog::on_untilDateTimeEdit_dateTimeChanged()
{
    updateBootLog(true);
}

void ShowBootLog::on_horizontalSlider_sliderMoved(int position)
{
	maxPriority = position;
    updateBootLog(true);
}

void ShowBootLog::on_filterButton_clicked()
{
    /// TODO: accept here?
    // acceptIdentifers();
    updateBootLog(true);
}

void ShowBootLog::on_exportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export filtered journal entries");
    if(fileName != ""){
        QFile *exportFile = new QFile(fileName);
        exportFile->open(QFile::ReadWrite);
        exportFile->write(ui->plainTextEdit->toPlainText().toLocal8Bit().data());
        exportFile->close();
    }
}

void ShowBootLog::on_find_keyshortcut_triggered()
{
    ui->findBox->setVisible(true);
    ui->findLineEdit->setFocus();
}

void ShowBootLog::on_find_hide_keyshortcut_triggered()
{
    ui->findBox->setVisible(false);
}

void ShowBootLog::execute_find(QRegExp regexp, QTextDocument::FindFlags findFlags) {
    if(!regexp.isValid()){
        ui->findStatusLabel->setText("Invalid RegExp!");
        ui->findStatusLabel->setStyleSheet("color: #F00;");
        return;
    }

    if(!ui->plainTextEdit->find(regexp, findFlags)){
        QTextCursor cur = ui->plainTextEdit->textCursor();
        cur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
        ui->plainTextEdit->setTextCursor(cur);
        ui->findStatusLabel->setText("Search started from the beginning");

        if(!ui->plainTextEdit->find(regexp, findFlags)){
            ui->findStatusLabel->setText("Not found");
            ui->findStatusLabel->setStyleSheet("color: #F00;");
        }
    }
}

void ShowBootLog::execute_find(QString string, QTextDocument::FindFlags findFlags) {
    if(!ui->plainTextEdit->find(string, findFlags)){
        QTextCursor cur = ui->plainTextEdit->textCursor();
        cur.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
        ui->plainTextEdit->setTextCursor(cur);
        ui->findStatusLabel->setText("Search started from the beginning");

        if(!ui->plainTextEdit->find(string, findFlags)){
            ui->findStatusLabel->setText("Not found");
            ui->findStatusLabel->setStyleSheet("color: #F00;");
        }
    }
}

void ShowBootLog::on_findLineEdit_returnPressed()
{
    ui->findStatusLabel->setText("");
    ui->findStatusLabel->setStyleSheet("color: #000;");

    bool ignoreCase = (ui->ignoreCaseCheckBox->checkState() == Qt::Checked);
    bool useRegexp = (ui->useRegexpCheckBox->checkState() == Qt::Checked);

    QTextDocument::FindFlags ignoreCaseFlags;
    if(!ignoreCase){
        ignoreCaseFlags = QTextDocument::FindCaseSensitively;
    }

    if(useRegexp) {
        QRegExp regexp = QRegExp(ui->findLineEdit->text());
        if(ignoreCase) {
            regexp.setCaseSensitivity(Qt::CaseInsensitive);
        } else {
            regexp.setCaseSensitivity(Qt::CaseSensitive);
        }

        execute_find(regexp, ignoreCaseFlags);
    } else {
        execute_find(ui->findLineEdit->text(), ignoreCaseFlags);
    }
}


void ShowBootLog::on_identifiersLineEdit_returnPressed()
{
    acceptIdentifier();
}


void ShowBootLog::on_clearButton_clicked()
{
    ui->acceptedIdentifierLabel->setText("");
    ui->identifiersLineEdit->clear();
    this->acceptedIdentifiers.clear();
    updateBootLog(false);
}
