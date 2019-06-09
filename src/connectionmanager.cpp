#include "connectionmanager.h"
#include "ui_connectionmanager.h"

ConnectionManager::ConnectionManager(QWidget *parent, QVector<SSHConnectionSettings> *savedConnections) :
    QDialog(parent),
    ui(new Ui::ConnectionManager)
{
    ui->setupUi(this);
}

ConnectionManager::~ConnectionManager()
{
    delete ui;
}
