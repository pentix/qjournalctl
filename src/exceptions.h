/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2019 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QMessageBox>

class Error
{
public:
    Error();
    Error(const QString &errorMsg, const bool retryPossible=false);
    void showErrorBox();
    bool userWantsRetry();

private:
    QMessageBox box;
    QPushButton *yes;

    bool silent;
    bool retryPossible;
    bool userSelectedRetry;
};



namespace Exceptions {
    void warning(const QString &warningMessage);
    bool userAcceptsWarning(const QString &warningMessage);
}

#endif // EXCEPTIONS_H
