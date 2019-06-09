#include "connectionmanager.h"
#include "ui_connectionmanager.h"
#include "connectiondialog.h"

ConnectionManager::ConnectionManager(QWidget *parent, SSHConnectionSerializer *sshConnectionSerializer) :
    QDialog(parent),
    ui(new Ui::ConnectionManager)
{
    ui->setupUi(this);

    this->sshConnectionSerializer = sshConnectionSerializer;

    connectionsModel = new QStandardItemModel(0, 1);
    connectionsModel->setHorizontalHeaderItem(0, new QStandardItem("Connection"));

    // Add known connections
    for(SSHConnectionSettings settings : *sshConnectionSerializer->getConnectionsVector()){
        connectionsModel->appendRow(new QStandardItem(settings.getName()));
    }

    ui->listView->setModel(connectionsModel);
}

ConnectionManager::~ConnectionManager()
{
    delete connectionsModel;
    delete ui;
}

void ConnectionManager::on_newConnectionButton_clicked()
{
    // We can omit current SSHSettings since we don't want to connect to another host from there
    ConnectionDialog dialog(nullptr, nullptr, sshConnectionSerializer, true);
    dialog.exec();
}
