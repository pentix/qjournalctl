#ifndef ERROR_H
#define ERROR_H

#include <QMessageBox>

class Error
{
public:
    Error(QString errorMsg);
    void showErrorBox();

private:
    QMessageBox box;
};

#endif // ERROR_H
