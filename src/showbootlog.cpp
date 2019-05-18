/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2018 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "showbootlog.h"
#include "ui_showbootlog.h"

#include "local.h"
#include "connection.h"


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
}


// todo: Fix this mess!
ShowBootLog::ShowBootLog(QWidget *parent, bool completeJournal, bool realtime, bool reverse, QString bootid, Connection *connection) :
	QDialog(parent),
	ui(new Ui::ShowBootLog)
{

	// Call simple constructor first
	ui->setupUi(this);
	this->connection = connection;

	// Set save icon for the export buttons
	ui->exportButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
	ui->exportSelectionButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));

	// Create find and escape shortcut
	new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this, SLOT(on_find_keyshortcut_triggered()));
	new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(on_escape_keyshortcut_triggered()));

	// Set local display variables
	this->bootid = bootid;
	this->completeJournal = completeJournal;
	this->realtime = realtime;
	this->reverse = reverse;

	// UI default values
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
	if(connection->isRunning()){
		connection->close();
	}

	delete connection;
	delete ui;
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
		// Reset all previously accepted but also read identifiers   (clear the filter but also do a full reload!)
		this->allIdentifiers.clear();
		this->acceptedIdentifiers.clear();
		identifierFlags = "";

		// Also reset the UI parts
		ui->acceptedIdentifierLabel->setText("");

	} else {
		// Regenerate all flags
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


	// As soon as the connection has data available, we want to append it to the boot log.
	// If the connection is already open, we close it!
	if(!connection->isRunning()){
		connect(connection, SIGNAL(connectionDataAvailable(QString)), this, SLOT(appendToBootLog(QString)));
	} else {
		connection->close();
	}

	// Reset byte counter
	numberOfBytesRead = 0;
	connection->run(command);
}

void ShowBootLog::acceptIdentifier(void){
	this->acceptedIdentifiers.insert(ui->identifiersLineEdit->text());
	updateBootLog(true);
	ui->identifiersLineEdit->clear();
	ui->identifiersLineEdit->setFocus();
}


void ShowBootLog::appendToBootLog(QString readString)
{
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

	// Iterate over all found identifiers
	while(matches.hasNext()){
		QRegularExpressionMatch identifier = matches.next();
		identifierSet.insert(identifier.captured(1));
	}

	this->allIdentifiers += identifierSet;
	QCompleter *completer = new QCompleter(this->allIdentifiers.toList());
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setCompletionMode(QCompleter::PopupCompletion);
	ui->identifiersLineEdit->setCompleter(completer);

	// Clearing the LineEdit using clear() does not work due to the focus being
	// on the completer popup. (Some strange Qt behaviour)
	// Solution: Stackoverflow, again!  https://stackoverflow.com/a/11905995/2628569
	QObject::connect(completer, SIGNAL(activated(const QString&)),
					 ui->identifiersLineEdit, SLOT(clear()),
					 Qt::QueuedConnection);
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

void ShowBootLog::on_horizontalSlider_valueChanged(int value)
{
	ShowBootLog::on_horizontalSlider_sliderMoved(value);
}

void ShowBootLog::on_filterButton_clicked()
{
	acceptIdentifier();
	ui->identifiersLineEdit->clear();

	updateBootLog(true);
}

void writeToExportFile(QString fileName, const char *data){
	if(fileName != ""){
		QFile *exportFile = new QFile(fileName);
		exportFile->open(QFile::ReadWrite);
		exportFile->write(data);
		exportFile->close();
	}
}


void ShowBootLog::on_exportButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Export filtered journal entries");
	writeToExportFile(fileName, ui->plainTextEdit->toPlainText().toLocal8Bit().data());
}


void ShowBootLog::on_find_keyshortcut_triggered()
{
	ui->findBox->setVisible(true);
	ui->findLineEdit->setFocus();
}

void ShowBootLog::on_escape_keyshortcut_triggered()
{
	if(ui->identifiersLineEdit->hasFocus()){
		on_clearButton_clicked();
	} else if(ui->findLineEdit->hasFocus() || ui->useRegexpCheckBox->hasFocus() || ui->ignoreCaseCheckBox->hasFocus()){
		ui->findBox->setVisible(false);
	}
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


void ShowBootLog::on_plainTextEdit_selectionChanged()
{
	// Get the selected text and decide whether to show the the exportSelectionButton or not
	QString selection = ui->plainTextEdit->textCursor().selectedText();
	if(selection != ""){
		ui->exportSelectionButton->setVisible(true);
	} else {
		ui->exportSelectionButton->setVisible(false);
	}
}


void ShowBootLog::on_exportSelectionButton_clicked()
{
	QString selection = ui->plainTextEdit->textCursor().selectedText();
	QString fileName = QFileDialog::getSaveFileName(this, "Export selected journal entries");

	writeToExportFile(fileName, selection.toLocal8Bit().data());
}

