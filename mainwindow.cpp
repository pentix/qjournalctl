#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Set Item Model
    itemModel = new QStandardItemModel(this);
    ui->listView1->setModel(itemModel);
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


    for(int i=0; i<lines.size(); i++){
        QStandardItem *item = new QStandardItem(lines.at(i).toLocal8Bit().constData());
        itemModel->appendRow(item);
    }

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


    QStandardItemModel *bootModel = new QStandardItemModel(this);
    for(int i=0; i<lines.size(); i++){
        QStandardItem *item = new QStandardItem(lines.at(i).toLocal8Bit().constData());
        bootModel->appendRow(item);
    }

    ui->listView1->setModel(bootModel);

}
