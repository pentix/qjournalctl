/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2017 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_showBootLogButton_clicked();

	void on_listBootsButton_clicked();

	void on_actionAbout_triggered();

	void on_actionLoadBoots_triggered();

	void on_actionQuit_triggered();

	void on_tableView_doubleClicked(const QModelIndex &index);

	void on_actionShowCompleteJournal_triggered();

	void on_actionSizeOfTheJournalOnTheDisk_triggered();

	void on_tableView_clicked(const QModelIndex &index);

	void on_showCurrentBootLogButton_clicked();

	void on_actionShowCurrentBootLog_triggered();

	void on_realtimeCheckBox_stateChanged(int arg1);

	void on_reverseCheckBox_stateChanged(int arg1);

private:
	Ui::MainWindow *ui;
	QStandardItemModel *itemModel;

	QString lastSelection;

};

#endif // MAINWINDOW_H
