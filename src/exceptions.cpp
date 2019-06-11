#include "exceptions.h"

Error::Error()
{
    this->silent = true;
}

Error::Error(const QString &msg, const bool retryPossible)
{
    this->silent = false;
    this->retryPossible = retryPossible;

    box.setText(msg);
    box.setIcon(QMessageBox::Icon::Critical);

    if(retryPossible){
        yes = box.addButton(QMessageBox::Yes);
              box.addButton(QMessageBox::No);
    }
}

void Error::showErrorBox()
{
    // A silent exception doesn't display any message box
    if(silent){
        return;
    }

    box.exec();
    if((QPushButton *)box.clickedButton() == yes){
        userSelectedRetry = true;
    } else {
        userSelectedRetry = false;
    }
}

bool Error::userWantsRetry()
{
    return userSelectedRetry;
}


void Exceptions::warning(const QString &warningMessage)
{
    QMessageBox box(QMessageBox::Warning, "", warningMessage);
    box.exec();
}

bool Exceptions::userAcceptsWarning(const QString &warningMessage)
{
    QMessageBox box(QMessageBox::Warning, "", warningMessage);
    QPushButton *yes = box.addButton(QMessageBox::Yes);
                       box.addButton(QMessageBox::No);
    box.exec();

    return (QPushButton *)box.clickedButton() == yes;
}
