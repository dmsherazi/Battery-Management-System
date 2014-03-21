/********************************************************************************
** Form generated from reading UI file 'data-processing-main.ui'
**
** Created: Sat Mar 22 09:44:53 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATA_2D_PROCESSING_2D_MAIN_H
#define UI_DATA_2D_PROCESSING_2D_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataProcessingMainWindow
{
public:
    QAction *actionOpen_Datafile;
    QWidget *centralwidget;
    QPushButton *openFileButton;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *DataProcessingMainWindow)
    {
        if (DataProcessingMainWindow->objectName().isEmpty())
            DataProcessingMainWindow->setObjectName(QString::fromUtf8("DataProcessingMainWindow"));
        DataProcessingMainWindow->resize(800, 600);
        actionOpen_Datafile = new QAction(DataProcessingMainWindow);
        actionOpen_Datafile->setObjectName(QString::fromUtf8("actionOpen_Datafile"));
        centralwidget = new QWidget(DataProcessingMainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        openFileButton = new QPushButton(centralwidget);
        openFileButton->setObjectName(QString::fromUtf8("openFileButton"));
        openFileButton->setGeometry(QRect(32, 75, 91, 27));
        DataProcessingMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(DataProcessingMainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 27));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        DataProcessingMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(DataProcessingMainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        DataProcessingMainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_Datafile);

        retranslateUi(DataProcessingMainWindow);

        QMetaObject::connectSlotsByName(DataProcessingMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DataProcessingMainWindow)
    {
        DataProcessingMainWindow->setWindowTitle(QApplication::translate("DataProcessingMainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen_Datafile->setText(QApplication::translate("DataProcessingMainWindow", "Open Datafile", 0, QApplication::UnicodeUTF8));
        openFileButton->setText(QApplication::translate("DataProcessingMainWindow", "Open", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("DataProcessingMainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DataProcessingMainWindow: public Ui_DataProcessingMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATA_2D_PROCESSING_2D_MAIN_H
