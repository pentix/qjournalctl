#include "error.h"

Error::Error(QString msg)
{
    box.setText(msg);
    box.setIcon(QMessageBox::Icon::Critical);
}

void Error::showErrorBox()
{
    box.exec();
}
