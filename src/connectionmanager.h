#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QDialog>
#include <QVector>
#include "sshconnectionsettings.h"

namespace Ui {
class ConnectionManager;
}

class ConnectionManager : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionManager(QWidget *parent=nullptr, QVector<SSHConnectionSettings> *savedConnections=nullptr);
    ~ConnectionManager();

private:
    Ui::ConnectionManager *ui;
};

#endif // CONNECTIONMANAGER_H