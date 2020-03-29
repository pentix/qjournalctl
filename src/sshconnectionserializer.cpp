/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2019 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "sshconnectionserializer.h"

SSHConnectionSerializer::SSHConnectionSerializer()
{
    // Get the correct native representation of the configuration path
    QString savedConnectionsFilePath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/";
    savedConnectionsFilePath = QDir::toNativeSeparators(savedConnectionsFilePath);

    // Windows might store settings in application specific folders that do not yet exist
    #ifdef WIN32
        if(!QDir().exists(savedConnectionsFilePath)){
            QDir().mkdir(savedConnectionsFilePath);
        }
    #endif

    // SSH connections are stored into the 'qjournalctl' file inside the received file path
    savedConnectionsFile.setFileName(savedConnectionsFilePath + "qjournalctl");

    // Read existing connections if there are already saved settings, otherwise return
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
