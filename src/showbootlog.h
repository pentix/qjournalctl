/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2017 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef SHOWBOOTLOG_H
#define SHOWBOOTLOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class ShowBootLog;
}

class ShowBootLog : public QDialog
{
	Q_OBJECT

public:
	explicit ShowBootLog(QWidget *parent = 0);
	explicit ShowBootLog(QWidget *parent = 0, bool completeJournal=false, bool realtime=false, bool reverse=false, QString bootid="");
	~ShowBootLog();


private slots:
	void on_pushButton_clicked();

	void on_sinceCheckBox_clicked();

	void on_untilCheckBox_clicked();

    void on_sinceDateTimeEdit_dateTimeChanged();

    void on_untilDateTimeEdit_dateTimeChanged();

	void on_horizontalSlider_sliderMoved(int position);

	void appendToBootLog();

	void on_filterButton_clicked();

private:
	void updateBootLog();

	Ui::ShowBootLog *ui;
	QProcess *journalProcess;

	QString bootid;
	bool sinceFlag=false, untilFlag=false;
	bool completeJournal=false;
	bool realtime=false;
	bool reverse=false;
	int maxPriority=3;
	int numberOfBytesRead=0;
	QString identifierFlags="";

};

#endif // SHOWBOOTLOG_H
