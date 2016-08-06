#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_showBootLogButton_clicked();

    void on_listBootsButton_clicked();

    void on_actionAbout_triggered();

    void on_actionLoadBoots_triggered();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_actionShowCompleteJournal_triggered();

    void on_actionSizeOfTheJournalOnTheDisk_triggered();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *itemModel;

};

#endif // MAINWINDOW_H
