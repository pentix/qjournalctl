#ifndef SHOWBOOTLOG_H
#define SHOWBOOTLOG_H

#include <QDialog>

namespace Ui {
class ShowBootLog;
}

class ShowBootLog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowBootLog(QWidget *parent = 0);
    explicit ShowBootLog(QWidget *parent = 0, QString bootid="");
    ~ShowBootLog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ShowBootLog *ui;
};

#endif // SHOWBOOTLOG_H
