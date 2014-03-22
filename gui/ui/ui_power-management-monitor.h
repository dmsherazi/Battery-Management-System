/********************************************************************************
** Form generated from reading UI file 'power-management-monitor.ui'
**
** Created: Sun Mar 23 08:32:34 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POWER_2D_MANAGEMENT_2D_MONITOR_H
#define UI_POWER_2D_MANAGEMENT_2D_MONITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_PowerManagementMonitorDialog
{
public:
    QLabel *title;
    QLabel *errorLabel;
    QwtPlot *qwtPlot1;
    QComboBox *sourceComboBox1;
    QLabel *sourceLabel1;
    QPushButton *closeButton;
    QSlider *offsetSlider1;
    QSlider *scaleSlider1;
    QLabel *rangeLabelHorizontal1;
    QLabel *rangeLabelVertical1;
    QwtPlot *qwtPlot2;
    QLabel *rangeLabelVertical2;
    QLabel *sourceLabel2;
    QSlider *scaleSlider2;
    QLabel *sampleLabel;
    QLabel *rangeLabelHorizontal2;
    QComboBox *sourceComboBox2;
    QSpinBox *sampleSpinBox;
    QSlider *offsetSlider2;
    QSlider *xoffsetSlider;

    void setupUi(QDialog *PowerManagementMonitorDialog)
    {
        if (PowerManagementMonitorDialog->objectName().isEmpty())
            PowerManagementMonitorDialog->setObjectName(QString::fromUtf8("PowerManagementMonitorDialog"));
        PowerManagementMonitorDialog->resize(682, 597);
        title = new QLabel(PowerManagementMonitorDialog);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(140, 12, 431, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Andale Mono"));
        font.setPointSize(18);
        title->setFont(font);
        errorLabel = new QLabel(PowerManagementMonitorDialog);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setGeometry(QRect(30, 560, 381, 20));
        qwtPlot1 = new QwtPlot(PowerManagementMonitorDialog);
        qwtPlot1->setObjectName(QString::fromUtf8("qwtPlot1"));
        qwtPlot1->setGeometry(QRect(20, 93, 551, 144));
        sourceComboBox1 = new QComboBox(PowerManagementMonitorDialog);
        sourceComboBox1->setObjectName(QString::fromUtf8("sourceComboBox1"));
        sourceComboBox1->setGeometry(QRect(230, 248, 161, 29));
        sourceLabel1 = new QLabel(PowerManagementMonitorDialog);
        sourceLabel1->setObjectName(QString::fromUtf8("sourceLabel1"));
        sourceLabel1->setGeometry(QRect(180, 254, 51, 17));
        closeButton = new QPushButton(PowerManagementMonitorDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(580, 557, 91, 27));
        offsetSlider1 = new QSlider(PowerManagementMonitorDialog);
        offsetSlider1->setObjectName(QString::fromUtf8("offsetSlider1"));
        offsetSlider1->setGeometry(QRect(590, 100, 20, 100));
        offsetSlider1->setValue(50);
        offsetSlider1->setOrientation(Qt::Vertical);
        scaleSlider1 = new QSlider(PowerManagementMonitorDialog);
        scaleSlider1->setObjectName(QString::fromUtf8("scaleSlider1"));
        scaleSlider1->setGeometry(QRect(620, 100, 20, 100));
        scaleSlider1->setMaximum(6);
        scaleSlider1->setPageStep(1);
        scaleSlider1->setValue(3);
        scaleSlider1->setSliderPosition(3);
        scaleSlider1->setOrientation(Qt::Vertical);
        rangeLabelHorizontal1 = new QLabel(PowerManagementMonitorDialog);
        rangeLabelHorizontal1->setObjectName(QString::fromUtf8("rangeLabelHorizontal1"));
        rangeLabelHorizontal1->setGeometry(QRect(580, 219, 31, 20));
        rangeLabelVertical1 = new QLabel(PowerManagementMonitorDialog);
        rangeLabelVertical1->setObjectName(QString::fromUtf8("rangeLabelVertical1"));
        rangeLabelVertical1->setGeometry(QRect(30, 65, 51, 20));
        qwtPlot2 = new QwtPlot(PowerManagementMonitorDialog);
        qwtPlot2->setObjectName(QString::fromUtf8("qwtPlot2"));
        qwtPlot2->setGeometry(QRect(20, 329, 551, 144));
        rangeLabelVertical2 = new QLabel(PowerManagementMonitorDialog);
        rangeLabelVertical2->setObjectName(QString::fromUtf8("rangeLabelVertical2"));
        rangeLabelVertical2->setGeometry(QRect(33, 301, 51, 20));
        sourceLabel2 = new QLabel(PowerManagementMonitorDialog);
        sourceLabel2->setObjectName(QString::fromUtf8("sourceLabel2"));
        sourceLabel2->setGeometry(QRect(190, 490, 51, 17));
        scaleSlider2 = new QSlider(PowerManagementMonitorDialog);
        scaleSlider2->setObjectName(QString::fromUtf8("scaleSlider2"));
        scaleSlider2->setGeometry(QRect(623, 336, 20, 100));
        scaleSlider2->setMaximum(6);
        scaleSlider2->setPageStep(1);
        scaleSlider2->setValue(3);
        scaleSlider2->setSliderPosition(3);
        scaleSlider2->setOrientation(Qt::Vertical);
        sampleLabel = new QLabel(PowerManagementMonitorDialog);
        sampleLabel->setObjectName(QString::fromUtf8("sampleLabel"));
        sampleLabel->setGeometry(QRect(510, 527, 91, 20));
        rangeLabelHorizontal2 = new QLabel(PowerManagementMonitorDialog);
        rangeLabelHorizontal2->setObjectName(QString::fromUtf8("rangeLabelHorizontal2"));
        rangeLabelHorizontal2->setGeometry(QRect(583, 455, 31, 20));
        sourceComboBox2 = new QComboBox(PowerManagementMonitorDialog);
        sourceComboBox2->setObjectName(QString::fromUtf8("sourceComboBox2"));
        sourceComboBox2->setGeometry(QRect(240, 484, 161, 29));
        sampleSpinBox = new QSpinBox(PowerManagementMonitorDialog);
        sampleSpinBox->setObjectName(QString::fromUtf8("sampleSpinBox"));
        sampleSpinBox->setGeometry(QRect(610, 523, 52, 25));
        sampleSpinBox->setMinimum(1);
        sampleSpinBox->setMaximum(5);
        sampleSpinBox->setValue(1);
        offsetSlider2 = new QSlider(PowerManagementMonitorDialog);
        offsetSlider2->setObjectName(QString::fromUtf8("offsetSlider2"));
        offsetSlider2->setGeometry(QRect(593, 336, 20, 100));
        offsetSlider2->setValue(50);
        offsetSlider2->setOrientation(Qt::Vertical);
        xoffsetSlider = new QSlider(PowerManagementMonitorDialog);
        xoffsetSlider->setObjectName(QString::fromUtf8("xoffsetSlider"));
        xoffsetSlider->setGeometry(QRect(240, 527, 160, 20));
        xoffsetSlider->setMaximum(20);
        xoffsetSlider->setSliderPosition(20);
        xoffsetSlider->setOrientation(Qt::Horizontal);

        retranslateUi(PowerManagementMonitorDialog);

        QMetaObject::connectSlotsByName(PowerManagementMonitorDialog);
    } // setupUi

    void retranslateUi(QDialog *PowerManagementMonitorDialog)
    {
        PowerManagementMonitorDialog->setWindowTitle(QApplication::translate("PowerManagementMonitorDialog", "Solar Power BMS Monitor", 0, QApplication::UnicodeUTF8));
        title->setText(QApplication::translate("PowerManagementMonitorDialog", "Solar Power Management Monitor", 0, QApplication::UnicodeUTF8));
        errorLabel->setText(QString());
        sourceLabel1->setText(QApplication::translate("PowerManagementMonitorDialog", "Source", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("PowerManagementMonitorDialog", "Close", 0, QApplication::UnicodeUTF8));
        rangeLabelHorizontal1->setText(QApplication::translate("PowerManagementMonitorDialog", "Sec", 0, QApplication::UnicodeUTF8));
        rangeLabelVertical1->setText(QApplication::translate("PowerManagementMonitorDialog", "Amps", 0, QApplication::UnicodeUTF8));
        rangeLabelVertical2->setText(QApplication::translate("PowerManagementMonitorDialog", "Amps", 0, QApplication::UnicodeUTF8));
        sourceLabel2->setText(QApplication::translate("PowerManagementMonitorDialog", "Source", 0, QApplication::UnicodeUTF8));
        sampleLabel->setText(QApplication::translate("PowerManagementMonitorDialog", "Sample Time", 0, QApplication::UnicodeUTF8));
        rangeLabelHorizontal2->setText(QApplication::translate("PowerManagementMonitorDialog", "Sec", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PowerManagementMonitorDialog: public Ui_PowerManagementMonitorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POWER_2D_MANAGEMENT_2D_MONITOR_H
