/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016 by Patrick Eigensatz <patrick.eigensatz@gmail.com> 
 * Some rights reserved. See LICENSE.
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "showbootlog.h"

#include <QProcess>
#include <QMessageBox>
#include <QShortcut>
#include <iostream>

#include <QMessageBox>

using namespace std;


// List of boot dates
QStandardItemModel *bootModel;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Set Item Model
    itemModel = new QStandardItemModel(this);
    ui->tableView->setModel(itemModel);

    QShortcut *shortcutQuit = new QShortcut(QKeySequence("Ctrl+Q"),this);
    connect(shortcutQuit, SIGNAL(activated()), ui->actionQuit, SLOT(trigger()));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_listBootsButton_clicked()
{
    QProcess process;
    process.start("journalctl --list-boots");
    process.waitForFinished(-1);

    QString stdout = process.readAllStandardOutput();
    if (stdout.length() == 0) {
        QMessageBox message_box;
        message_box.critical(nullptr, "Error", "No boots have been found :\n"+process.readAllStandardError());
        message_box.setFixedSize(500, 200);
        message_box.show();
        return;
    }
    QStringList lines = stdout.split("\n", QString::SkipEmptyParts);


    bootModel = new QStandardItemModel(1, 5, this);
    bootModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Boot No.")));
    bootModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Boot ID")));
    bootModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Day")));
    bootModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Date")));
    bootModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Time")));

    for(int i=0; i<lines.size(); i++){
        QString line = QString(lines.at(i).toLocal8Bit().constData());
        QStringList columns = line.split(" ", QString::SkipEmptyParts);

        for(int j=0; j<5; j++){
            QStandardItem *item = new QStandardItem(columns.at(j).toLocal8Bit().constData());
            bootModel->setItem(lines.size()-i-1, j, item);
        }

    }

    ui->tableView->verticalHeader()->setVisible(false);


    // Connect Model-View
    ui->tableView->setModel(bootModel);

    // Resize columns to their content
    ui->tableView->resizeColumnsToContents();


    ui->listBootsButton->setEnabled(false);
    ui->actionLoadBoots->setEnabled(false);
    ui->showBootLogButton->setEnabled(true);


}


// Get selected boot information
void MainWindow::on_showBootLogButton_clicked()
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();

    if(!selection->hasSelection())
        return;


    QModelIndex ind = selection->selectedRows().at(0);
    QStandardItem *mod = bootModel->item(ind.row(), 0);

    ShowBootLog *b = new ShowBootLog(this, false, ui->realtimeCheckBox->isChecked(), mod->text());
    b->show();

}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *a = new AboutDialog(this);
    a->show();
}


void MainWindow::on_actionLoadBoots_triggered()
{
    // Load system boots
    this->on_listBootsButton_clicked();
}

void MainWindow::on_actionQuit_triggered()
{
    // Quit
    this->close();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    this->on_showBootLogButton_clicked();
}


void MainWindow::on_actionShowCompleteJournal_triggered()
{
    ShowBootLog *b = new ShowBootLog(this, true, false, "");
    b->show();
}



void MainWindow::on_actionSizeOfTheJournalOnTheDisk_triggered()
{
    QProcess p;
    p.start("journalctl --disk-usage");
    p.waitForFinished(-1);

    QMessageBox msgBox;

    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("\n" + p.readAllStandardOutput());

    msgBox.exec();

}



void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();

    if(!selection->hasSelection())
        return;


    QModelIndex ind = selection->selectedRows().at(0);
    QStandardItem *mod = bootModel->item(ind.row(), 0);

    // Avoid singleClick-execution when the user double clicked
    if(mod->text() == this->lastSelection){
        return;
    }

    lastSelection = mod->text();


    if(mod->text() == "0"){
        ui->realtimeCheckBox->setEnabled(true);
        ui->realtimeCheckBox->setChecked(true);
    } else {
        ui->realtimeCheckBox->setEnabled(false);
        ui->realtimeCheckBox->setChecked(false);
    }
}
