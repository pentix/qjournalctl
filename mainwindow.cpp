#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

#include <QProcess>
#include <iostream>

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


    QStandardItemModel *bootModel = new QStandardItemModel(30, 5, this);
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



    ui->tableView->setModel(bootModel);

    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(true);


}

void MainWindow::on_pushButton_clicked()
{
    QProcess process;
    process.start("journalctl -b -5");
    process.waitForFinished(-1);

    QString stdout = process.readAllStandardOutput();
    QStringList lines = stdout.split("\n", QString::SkipEmptyParts);


    /*QStandardItemModel *bootModel = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns
    bootModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
    bootModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
    bootModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));*/




    QStandardItemModel *bootModel = new QStandardItemModel(30, 9, this);
    bootModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Boot No.")));
    bootModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Boot No.")));
    bootModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Boot No.")));
    bootModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Boot No.")));
    bootModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Boot No.")));
    bootModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Boot No.")));
    bootModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Boot No.")));
    bootModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Boot No.")));
    bootModel->setHorizontalHeaderItem(8, new QStandardItem(QString("Boot No.")));

   /* for(int i=0; i<lines.size(); i++){
        QString line = QString(lines.at(i).toLocal8Bit().constData());
        QStringList columns = line.split(" ");

        for(int j=0; j<columns.size(); j++){
            QStandardItem *item = new QStandardItem(columns.at(j));
            bootModel->appendRow(item);
        }

    } */



    ui->tableView->setModel(bootModel);

}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *a = new AboutDialog(this);
    a->show();
}
