/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionHilfe;
    QAction *actionTest;
    QWidget *centralWidget;
    QLabel *label;
    QListView *listView1;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QMenu *menuJournalctl_Analyzer_Demo;
    QMenu *menuOK;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1084, 658);
        actionHilfe = new QAction(MainWindow);
        actionHilfe->setObjectName(QStringLiteral("actionHilfe"));
        actionTest = new QAction(MainWindow);
        actionTest->setObjectName(QStringLiteral("actionTest"));
        actionTest->setCheckable(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 161, 31));
        QFont font;
        font.setPointSize(16);
        label->setFont(font);
        listView1 = new QListView(centralWidget);
        listView1->setObjectName(QStringLiteral("listView1"));
        listView1->setGeometry(QRect(20, 120, 1031, 471));
        listView1->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView1->setResizeMode(QListView::Adjust);
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(20, 60, 191, 31));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setEnabled(false);
        pushButton->setGeometry(QRect(230, 60, 151, 31));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1084, 19));
        menuJournalctl_Analyzer_Demo = new QMenu(menuBar);
        menuJournalctl_Analyzer_Demo->setObjectName(QStringLiteral("menuJournalctl_Analyzer_Demo"));
        menuOK = new QMenu(menuBar);
        menuOK->setObjectName(QStringLiteral("menuOK"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuJournalctl_Analyzer_Demo->menuAction());
        menuBar->addAction(menuOK->menuAction());
        menuJournalctl_Analyzer_Demo->addSeparator();
        menuJournalctl_Analyzer_Demo->addSeparator();
        menuJournalctl_Analyzer_Demo->addSeparator();
        menuJournalctl_Analyzer_Demo->addAction(actionHilfe);
        menuJournalctl_Analyzer_Demo->addAction(actionTest);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionHilfe->setText(QApplication::translate("MainWindow", "List available boots", 0));
        actionTest->setText(QApplication::translate("MainWindow", "Test", 0));
        label->setText(QApplication::translate("MainWindow", "journalctl", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "List Available Boots", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Show Boot Log", 0));
        menuJournalctl_Analyzer_Demo->setTitle(QApplication::translate("MainWindow", "Load", 0));
        menuOK->setTitle(QApplication::translate("MainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
