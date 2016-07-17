#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "showbootlog.h"

#include <QProcess>
#include <QMessageBox>
#include <iostream>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_2_clicked()
{
    QProcess process;
    process.start("journalctl --list-boots");
    process.waitForFinished(-1);

    QString stdout = process.readAllStandardOutput();
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
            bootModel->setItem(i, j, item);
        }

    }



    // Connect Model-View
    ui->tableView->setModel(bootModel);

    // Resize columns to their content
    ui->tableView->resizeColumnsToContents();


    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(true);


}


// Get selected boot information
void MainWindow::on_pushButton_clicked()
{
    QItemSelectionModel *selection = ui->tableView->selectionModel();

    if(!selection->hasSelection())
        return;


    QModelIndex ind = selection->selectedRows().at(0);
    QStandardItem *mod = bootModel->item(ind.row(), 0);


    QMessageBox::information(this, "Selection", "Auswahl auf Zeile " + mod->text(), QMessageBox::Ok);

    ShowBootLog *b = new ShowBootLog(this, mod->text());
    b->show();

}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *a = new AboutDialog(this);
    a->show();
}


void MainWindow::on_actionHilfe_triggered()
{
    // Load system boots
    this->on_pushButton_2_clicked();
}
