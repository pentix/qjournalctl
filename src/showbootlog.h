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
    explicit ShowBootLog(QWidget *parent = 0, bool completeJournal=false, QString bootid="");
    ~ShowBootLog();

private slots:
    void on_pushButton_clicked();

    void on_sinceCheckBox_clicked();

    void on_untilCheckBox_clicked();

    void on_sinceDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_untilDateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_horizontalSlider_sliderMoved(int position);

private:
    void updateBootLog();

    Ui::ShowBootLog *ui;
    QString bootid;
    bool sinceFlag=false, untilFlag=false;
    bool completeJournal=false;
    int maxPriority=1;

};

#endif // SHOWBOOTLOG_H
