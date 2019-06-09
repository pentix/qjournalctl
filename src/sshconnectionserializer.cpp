#include <QStandardPaths>
#include <QFile>
#include <QDebug>

#include "sshconnectionserializer.h"

SSHConnectionSerializer::SSHConnectionSerializer()
{
    QString savedConnectionsFilePath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    savedConnectionsFile.setFileName(savedConnectionsFilePath + "/qjournalctl");

    // File IO or exit
    modifiedConnectionsFile = false;
    if(!savedConnectionsFile.exists()) {
        return;
    }
    savedConnectionsFile.open(QFile::ReadOnly);
    QByteArray savedConnectionsData = savedConnectionsFile.readAll();
    savedConnectionsFile.close();

    // Parsing
    QJsonDocument doc = QJsonDocument::fromJson(savedConnectionsData);
    QJsonObject connectionsDoc = doc.object().value("connections").toObject();

    for(QString name : connectionsDoc.keys()){
        savedConnections.push_back(jsonToSSHSettings(name, connectionsDoc[name].toObject()));
    }
}

SSHConnectionSerializer::~SSHConnectionSerializer()
{
    // If no SSHConnectionSettings were modified, just exit!
    if(!modifiedConnectionsFile){
        return;
    }

    // Otherwise serialize all settings to JSON and save it!
    QJsonObject connectionsJson;
    for(SSHConnectionSettings *settings : savedConnections){
        qDebug() << "Saving connection: " << settings->getName();
        connectionsJson[settings->getName()] = sshSettingsToJSON(settings);
    }

    // For compatibility reasons, wrap this as a "connections" object
    QJsonObject rootJson;
    rootJson["connections"] = connectionsJson;

    // File IO
    QJsonDocument settingsDocument(rootJson);
    savedConnectionsFile.open(QFile::ReadWrite | QFile::Truncate);
    savedConnectionsFile.write(settingsDocument.toJson(QJsonDocument::Indented));
    savedConnectionsFile.close();
}

QVector<SSHConnectionSettings *> *SSHConnectionSerializer::getConnectionsVector()
{
    return &savedConnections;
}

void SSHConnectionSerializer::add(SSHConnectionSettings *sshSettings)
{
    savedConnections.push_front(sshSettings);
    modifiedConnectionsFile = true;
}

SSHConnectionSettings *SSHConnectionSerializer::get(int id)
{
    return savedConnections[id];
}

void SSHConnectionSerializer::update(int id, SSHConnectionSettings *sshSettings)
{
    savedConnections[id] = sshSettings;
    modifiedConnectionsFile = true;
}

void SSHConnectionSerializer::remove(int id)
{
    savedConnections.remove(id);
    modifiedConnectionsFile = true;
}

QJsonObject SSHConnectionSerializer::sshSettingsToJSON(SSHConnectionSettings *sshSettings)
{
    QJsonObject jsonSettings;
    jsonSettings.insert("hostname", QJsonValue(sshSettings->getHostname()));
    jsonSettings.insert("port", QJsonValue((int)*sshSettings->getPort()));
    jsonSettings.insert("username", QJsonValue(sshSettings->getUsername()));
    jsonSettings.insert("keyfile", QJsonValue(sshSettings->getKeyfile()));
    jsonSettings.insert("useKeyfile", QJsonValue(sshSettings->useKeyfile()));

    return jsonSettings;
}

SSHConnectionSettings *SSHConnectionSerializer::jsonToSSHSettings(QString name, QJsonObject jsonSettings)
{
    QString hostname = jsonSettings["hostname"].toString();
    unsigned int port = (unsigned int)jsonSettings["port"].toInt();
    QString username = jsonSettings["username"].toString();
    QString keyfile = jsonSettings["keyfile"].toString();
    bool useKeyfile = jsonSettings["useKeyfile"].toBool();

    return new SSHConnectionSettings(name, hostname, port, username, keyfile, useKeyfile);
}
