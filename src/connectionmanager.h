#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QDialog>
#include <QVector>
#include <QStandardItemModel>
#include "sshconnectionserializer.h"

namespace Ui {
class ConnectionManager;
}

class ConnectionManager : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionManager(QWidget *parent=nullptr, SSHConnectionSerializer *sshConnectionSerializer=nullptr);
    ~ConnectionManager();

private slots:
    void on_newConnectionButton_clicked();

private:
    Ui::ConnectionManager *ui;
    QStandardItemModel *connectionsModel;
    SSHConnectionSerializer *sshConnectionSerializer;
};

#endif // CONNECTIONMANAGER_H
