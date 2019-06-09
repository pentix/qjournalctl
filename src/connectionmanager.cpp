#include "connectionmanager.h"
#include "ui_connectionmanager.h"

ConnectionManager::ConnectionManager(QWidget *parent, QVector<SSHConnectionSettings> *savedConnections) :
    QDialog(parent),
    ui(new Ui::ConnectionManager)
{
    ui->setupUi(this);
    connectionsModel = new QStandardItemModel(0, 1);
    connectionsModel->setHorizontalHeaderItem(0, new QStandardItem("Connection"));

    // Add known connections
    for(SSHConnectionSettings settings : *savedConnections){
        connectionsModel->appendRow(new QStandardItem(settings.getName()));
    }

    ui->listView->setModel(connectionsModel);
}

ConnectionManager::~ConnectionManager()
{
    delete connectionsModel;
    delete ui;
}
