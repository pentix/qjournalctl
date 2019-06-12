/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2018 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "showbootlog.h"
#include "connectiondialog.h"
#include "connectionmanager.h"
#include "exceptions.h"

#include <QProcess>
#include <QMessageBox>
#include <QShortcut>
#include <QDebug>

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

    // Load saved connections
    sshConnectionSerializer = new SSHConnectionSerializer();
    refreshSavedConnectionsMenu();

    // Create default (local) connection
    currentConnection = new Connection(this);
    currentConnectionSettings = nullptr;
}

MainWindow::~MainWindow()
{
    delete sshConnectionSerializer;
    delete currentConnection;
    delete ui;
}

void MainWindow::refreshSavedConnectionsMenu()
{
    // First remove old "savedConnection" entries
    for(QAction *menuEntry : savedConnectionsActions){
        ui->menuRemote->removeAction(menuEntry);
        delete menuEntry;
    }
    savedConnectionsActions.clear();

    // Now insert the new connections into the "Remote" menu
    int id=0;
    for(SSHConnectionSettings *sshSettings : *sshConnectionSerializer->getConnectionsVector()){
        QAction *newEntry = new QAction(sshSettings->getName());
        savedConnectionsActions.push_back(newEntry);
        ui->menuRemote->addAction(newEntry);

        // Use a lambda-functor to pass the argument
        // Greetings to https://stackoverflow.com/a/22411267/2628569
        connect(newEntry, &QAction::triggered, this, [this,id]{ connectToSavedConnection(id); });

        id++;
    }
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
    ui->reverseCheckBox->setEnabled(true);

}


// Get selected boot information
void MainWindow::on_showBootLogButton_clicked()
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();

    if(!selection->hasSelection())
        return;


    QModelIndex ind = selection->selectedRows().at(0);
    QStandardItem *mod = bootModel->item(ind.row(), 0);

    ShowBootLog *b = new ShowBootLog(this, false, ui->realtimeCheckBox->isChecked(), ui->reverseCheckBox->isChecked(), mod->text(), currentConnection);
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

void MainWindow::on_tableView_doubleClicked()
{
    this->on_showBootLogButton_clicked();
}


void MainWindow::on_actionShowCompleteJournal_triggered()
{
    ShowBootLog *b = new ShowBootLog(this, true, true, false, "", currentConnection);
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



void MainWindow::on_tableView_clicked()
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
        ui->reverseCheckBox->setChecked(false);
    } else {
        ui->realtimeCheckBox->setEnabled(false);
        ui->realtimeCheckBox->setChecked(false);
    }
}


// Show the current boot

void MainWindow::on_showCurrentBootLogButton_clicked()
{
    ShowBootLog *b = new ShowBootLog(this, false, true, false, "", currentConnection);
    b->show();
}


void MainWindow::on_actionShowCurrentBootLog_triggered()
{
    // Same as "showCurrentBootLogButton"
    on_showCurrentBootLogButton_clicked();
}

void MainWindow::on_realtimeCheckBox_stateChanged(int arg1)
{
    // Disable the "reverse" option if "follow" has been checked
    if(arg1 == Qt::Checked){
        ui->reverseCheckBox->setChecked(false);
    }
}

void MainWindow::on_reverseCheckBox_stateChanged(int arg1)
{
    // Disable the "follow" option if "reverse" has been checked
    if(arg1 == Qt::Checked){
        ui->realtimeCheckBox->setChecked(false);
    }
}

void MainWindow::on_actionOpen_a_new_SSH_connection_triggered()
{
    bool ok;

    try {
        ConnectionDialog connectionDialog(this, &currentConnectionSettings, sshConnectionSerializer);
        connectionDialog.exec();

        // Try to setup the connection
        ok = setupRemoteConnection();

        if(!ok){
            // Try again until it works or user aborts (-> Throws Error)
            while(!ok){
                ConnectionDialog newDialog(&currentConnectionSettings, sshConnectionSerializer);
                newDialog.exec();
                ok = setupRemoteConnection();
            }
        }
    } catch (Error *) {
        return;
    }

    // { Everything worked }

    // Update "savedConnections menu"
    refreshSavedConnectionsMenu();
}

bool MainWindow::setupRemoteConnection()
{
    Connection *newConnection;
    if(currentConnectionSettings == nullptr){
        ui->label->setText("QJournalctl");

        // User aborted!
        throw new Error();
    }


    // Connection Details are correct and available, continue to connect!
    while(true){
        try {
            newConnection = new Connection(this, currentConnectionSettings);
        } catch (Error *error) {
            error->showErrorBox();

            // If retry option isn't given or not chosen, quit here
            bool quit = !error->userWantsRetry();
            delete error;

            if(quit){
                // Not able to continue
                return false;
            } else {
                // Retry!
                continue;
            }
        }

        // No errors occurred -> exit loop to continue with setup
        break;
    }

    // Delete current connection only on success
    delete currentConnection;
    currentConnection = newConnection;

    // Update connection label
    ui->label->setText("QJournalctl @ " + QString::fromUtf8(currentConnectionSettings->getHostname()));
    ui->actionDisconnect_from_current_host->setEnabled(true);
    return true;
}

void MainWindow::on_actionDisconnect_from_current_host_triggered()
{
    currentConnection = new Connection(this);
    ui->actionDisconnect_from_current_host->setDisabled(true);
    ui->label->setText("QJournalctl");
}

void MainWindow::on_actionEdit_saved_connections_triggered()
{
    ConnectionManager m(nullptr, sshConnectionSerializer);
    m.exec();

    // Update "savedConnections menu"
    refreshSavedConnectionsMenu();
}

void MainWindow::connectToSavedConnection(int id)
{
    currentConnectionSettings = sshConnectionSerializer->get(id);
    setupRemoteConnection();
}
