/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2021 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef SHOWBOOTLOG_H
#define SHOWBOOTLOG_H

#include <QDialog>
#include <QProcess>
#include <QTextDocument>
#include <QSet>

#include "connection.h"
#include "sshconnectionsettings.h"

namespace Ui {
class ShowBootLog;
}

class ShowBootLog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowBootLog(QWidget *parent = nullptr);
    explicit ShowBootLog(QWidget *parent = nullptr, bool completeJournal=false, bool realtime=false, bool reverse=false, QString bootid="", Connection *connection=nullptr);
    ~ShowBootLog();


private slots:
    void on_closeButton_clicked();

    void on_sinceCheckBox_clicked();

    void on_untilCheckBox_clicked();

    void on_sinceDateTimeEdit_dateTimeChanged();

    void on_untilDateTimeEdit_dateTimeChanged();

    void on_horizontalSlider_sliderMoved(int position);

    void appendToBootLog(QString readString);

    void on_filterButton_clicked();

    void on_exportButton_clicked();

    void on_find_keyshortcut_triggered();

    void on_escape_keyshortcut_triggered();

    void on_findLineEdit_returnPressed();

    void on_identifiersLineEdit_returnPressed();

    void on_clearButton_clicked();

    void on_plainTextEdit_selectionChanged();

    void on_exportSelectionButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_verboseCheckBox_stateChanged(int arg1);

private:
    void updateBootLog(bool keepIdentifiers=false);

    Ui::ShowBootLog *ui;
    Connection *connection;

    // Only journalctl options
    QString bootid;
    bool sinceFlag=false, untilFlag=false;
    bool verboseflag=false;
    bool completeJournal=false;
    bool realtime=false;
    bool reverse=false;
    int maxPriority=3;

    // Internal display variables
    int numberOfBytesRead=0;
    bool empty=true;
    QString identifierFlags="";
    QSet<QString> allIdentifiers;
    QSet<QString> acceptedIdentifiers;

    void execute_find(QRegExp regexp, QTextDocument::FindFlags findFlags);
    void execute_find(QString string, QTextDocument::FindFlags findFlags);
    void acceptIdentifier(void);

};

#endif // SHOWBOOTLOG_H
