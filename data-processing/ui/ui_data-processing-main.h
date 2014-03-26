/********************************************************************************
** Form generated from reading UI file 'data-processing-main.ui'
**
** Created: Sun Mar 23 11:07:27 2014
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
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataProcessingMainWindow
{
public:
    QAction *actionOpen_Datafile;
    QWidget *centralwidget;
    QPushButton *openFileButton;
    QLabel *label;
    QDateTimeEdit *startTime;
    QDateTimeEdit *endTime;
    QComboBox *recordType_1;
    QComboBox *recordType_2;
    QComboBox *recordType_3;
    QPushButton *extractButton;
    QSpinBox *intervalSpinBox;
    QLabel *label_2;
    QComboBox *intervalType;
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
        openFileButton->setGeometry(QRect(26, 75, 91, 27));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(26, 15, 615, 41));
        QFont font;
        font.setPointSize(18);
        label->setFont(font);
        startTime = new QDateTimeEdit(centralwidget);
        startTime->setObjectName(QString::fromUtf8("startTime"));
        startTime->setGeometry(QRect(140, 75, 194, 25));
        endTime = new QDateTimeEdit(centralwidget);
        endTime->setObjectName(QString::fromUtf8("endTime"));
        endTime->setGeometry(QRect(356, 75, 194, 25));
        recordType_1 = new QComboBox(centralwidget);
        recordType_1->setObjectName(QString::fromUtf8("recordType_1"));
        recordType_1->setGeometry(QRect(140, 115, 133, 29));
        recordType_2 = new QComboBox(centralwidget);
        recordType_2->setObjectName(QString::fromUtf8("recordType_2"));
        recordType_2->setGeometry(QRect(290, 115, 133, 29));
        recordType_3 = new QComboBox(centralwidget);
        recordType_3->setObjectName(QString::fromUtf8("recordType_3"));
        recordType_3->setGeometry(QRect(434, 115, 133, 29));
        extractButton = new QPushButton(centralwidget);
        extractButton->setObjectName(QString::fromUtf8("extractButton"));
        extractButton->setGeometry(QRect(26, 155, 91, 27));
        intervalSpinBox = new QSpinBox(centralwidget);
        intervalSpinBox->setObjectName(QString::fromUtf8("intervalSpinBox"));
        intervalSpinBox->setGeometry(QRect(140, 155, 52, 25));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(214, 160, 64, 17));
        intervalType = new QComboBox(centralwidget);
        intervalType->setObjectName(QString::fromUtf8("intervalType"));
        intervalType->setGeometry(QRect(290, 155, 133, 29));
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
#ifndef QT_NO_TOOLTIP
        openFileButton->setToolTip(QApplication::translate("DataProcessingMainWindow", "Open a txt file for processing", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        openFileButton->setText(QApplication::translate("DataProcessingMainWindow", "Open", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DataProcessingMainWindow", "Battery Management System Data Processing Tool", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        extractButton->setToolTip(QApplication::translate("DataProcessingMainWindow", "Extract selected data from the file to a csv", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        extractButton->setText(QApplication::translate("DataProcessingMainWindow", "Extract", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        intervalSpinBox->setToolTip(QApplication::translate("DataProcessingMainWindow", "Select the sample interval for data extraction", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("DataProcessingMainWindow", "Interval", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        intervalType->setToolTip(QApplication::translate("DataProcessingMainWindow", "Select whether to use average, maximum or first sample in the interval", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        menuFile->setTitle(QApplication::translate("DataProcessingMainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DataProcessingMainWindow: public Ui_DataProcessingMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATA_2D_PROCESSING_2D_MAIN_H
