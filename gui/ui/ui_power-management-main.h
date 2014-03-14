/********************************************************************************
** Form generated from reading UI file 'power-management-main.ui'
**
** Created: Fri Feb 14 10:37:30 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POWER_2D_MANAGEMENT_2D_MAIN_H
#define UI_POWER_2D_MANAGEMENT_2D_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PowerManagementMainDialog
{
public:
    QLabel *title;
    QLabel *errorLabel;
    QSpinBox *displayfrequencySpinBox;
    QLabel *battery1Charge;
    QLabel *battery1ChargeLabel;
    QLabel *displayFreqLabel;
    QLabel *displayIntervalLabel;
    QLabel *battery2ChargeLabel;
    QLabel *battery3ChargeLabel;
    QGroupBox *groupBox;
    QRadioButton *load1Battery2;
    QRadioButton *load1Battery1;
    QRadioButton *load1Battery3;
    QFrame *load1line1;
    QFrame *load1line2;
    QFrame *load1line3;
    QGroupBox *groupBox_2;
    QRadioButton *load2Battery2;
    QRadioButton *load2Battery1;
    QRadioButton *load2Battery3;
    QFrame *load2line1;
    QFrame *load2line2;
    QFrame *load2line3;
    QGroupBox *groupBox_3;
    QRadioButton *panelBattery2;
    QRadioButton *panelBattery1;
    QRadioButton *panelBattery3;
    QFrame *panelline2;
    QFrame *panelline3;
    QFrame *panelline4;
    QFrame *battery1line1;
    QFrame *battery2line1;
    QFrame *battery3line1;
    QFrame *battery2line2;
    QFrame *battery3line2;
    QFrame *battery1line2;
    QFrame *battery2line3;
    QFrame *battery3line3;
    QFrame *battery1line3;
    QCheckBox *battery1CheckBox;
    QCheckBox *battery2CheckBox;
    QCheckBox *battery3CheckBox;
    QCheckBox *load1CheckBox;
    QCheckBox *load2CheckBox;
    QCheckBox *panelCheckBox;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QLabel *temperature;
    QLabel *battery2Charge;
    QLabel *battery3Charge;
    QLabel *battery1Image;
    QLabel *battery2Image;
    QLabel *battery3Image;
    QLabel *load1Image;
    QLabel *load2Image;
    QLabel *panelImage;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *monitorButton;
    QPushButton *configureButton;
    QPushButton *recordingButton;
    QPushButton *saveFileButton;
    QPushButton *closeFileButton;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QLabel *currentLabel;
    QLabel *battery1Current;
    QLabel *battery2Current;
    QLabel *battery3Current;
    QLabel *load1Current;
    QLabel *load2Current;
    QLabel *panelCurrent;
    QLabel *currentLabel_2;
    QLabel *voltageLabel;
    QLabel *battery1Voltage;
    QLabel *battery2Voltage;
    QLabel *battery3Voltage;
    QLabel *load1Voltage;
    QLabel *load2Voltage;
    QLabel *panelVoltage;
    QLabel *voltageLabel_2;
    QPushButton *battery1OverCurrent;
    QPushButton *battery2OverCurrent;
    QPushButton *battery3OverCurrent;
    QPushButton *load1OverCurrent;
    QPushButton *panelOverCurrent;
    QPushButton *load2OverCurrent;
    QCheckBox *autoTrackCheckBox;
    QLabel *battery1UnderVoltage;
    QLabel *battery2UnderVoltage;
    QLabel *battery3UnderVoltage;
    QLabel *load1UnderVoltage;
    QLabel *load2UnderVoltage;
    QLabel *panelUnderVoltage;
    QLabel *battery1Fill;
    QLabel *battery2Fill;
    QLabel *battery3Fill;
    QLabel *battery1Charging;
    QLabel *battery2Charging;
    QLabel *battery3Charging;
    QLabel *battery1Op;
    QLabel *battery2Op;
    QLabel *battery3Op;
    QPushButton *connectButton;

    void setupUi(QDialog *PowerManagementMainDialog)
    {
        if (PowerManagementMainDialog->objectName().isEmpty())
            PowerManagementMainDialog->setObjectName(QString::fromUtf8("PowerManagementMainDialog"));
        PowerManagementMainDialog->resize(682, 585);
        title = new QLabel(PowerManagementMainDialog);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(80, 13, 541, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Andale Mono"));
        font.setPointSize(18);
        title->setFont(font);
        errorLabel = new QLabel(PowerManagementMainDialog);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setGeometry(QRect(42, 555, 461, 20));
        displayfrequencySpinBox = new QSpinBox(PowerManagementMainDialog);
        displayfrequencySpinBox->setObjectName(QString::fromUtf8("displayfrequencySpinBox"));
        displayfrequencySpinBox->setGeometry(QRect(553, 543, 61, 23));
        battery1Charge = new QLabel(PowerManagementMainDialog);
        battery1Charge->setObjectName(QString::fromUtf8("battery1Charge"));
        battery1Charge->setGeometry(QRect(103, 106, 95, 52));
        QFont font1;
        font1.setPointSize(18);
        battery1Charge->setFont(font1);
        battery1Charge->setFrameShape(QFrame::Box);
        battery1Charge->setScaledContents(true);
        battery1Charge->setAlignment(Qt::AlignCenter);
        battery1ChargeLabel = new QLabel(PowerManagementMainDialog);
        battery1ChargeLabel->setObjectName(QString::fromUtf8("battery1ChargeLabel"));
        battery1ChargeLabel->setGeometry(QRect(94, 84, 121, 17));
        displayFreqLabel = new QLabel(PowerManagementMainDialog);
        displayFreqLabel->setObjectName(QString::fromUtf8("displayFreqLabel"));
        displayFreqLabel->setGeometry(QRect(523, 513, 121, 20));
        displayIntervalLabel = new QLabel(PowerManagementMainDialog);
        displayIntervalLabel->setObjectName(QString::fromUtf8("displayIntervalLabel"));
        displayIntervalLabel->setGeometry(QRect(623, 547, 64, 17));
        battery2ChargeLabel = new QLabel(PowerManagementMainDialog);
        battery2ChargeLabel->setObjectName(QString::fromUtf8("battery2ChargeLabel"));
        battery2ChargeLabel->setGeometry(QRect(269, 84, 121, 17));
        battery3ChargeLabel = new QLabel(PowerManagementMainDialog);
        battery3ChargeLabel->setObjectName(QString::fromUtf8("battery3ChargeLabel"));
        battery3ChargeLabel->setGeometry(QRect(449, 85, 121, 17));
        groupBox = new QGroupBox(PowerManagementMainDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(359, 207, 21, 101));
        groupBox->setLayoutDirection(Qt::RightToLeft);
        groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        load1Battery2 = new QRadioButton(groupBox);
        load1Battery2->setObjectName(QString::fromUtf8("load1Battery2"));
        load1Battery2->setGeometry(QRect(0, 50, 21, 22));
        load1Battery1 = new QRadioButton(groupBox);
        load1Battery1->setObjectName(QString::fromUtf8("load1Battery1"));
        load1Battery1->setGeometry(QRect(0, 20, 21, 22));
        load1Battery1->setChecked(true);
        load1Battery3 = new QRadioButton(groupBox);
        load1Battery3->setObjectName(QString::fromUtf8("load1Battery3"));
        load1Battery3->setGeometry(QRect(0, 80, 21, 22));
        load1line1 = new QFrame(groupBox);
        load1line1->setObjectName(QString::fromUtf8("load1line1"));
        load1line1->setGeometry(QRect(2, 35, 20, 20));
        load1line1->setFrameShadow(QFrame::Plain);
        load1line1->setLineWidth(3);
        load1line1->setFrameShape(QFrame::VLine);
        load1line2 = new QFrame(groupBox);
        load1line2->setObjectName(QString::fromUtf8("load1line2"));
        load1line2->setGeometry(QRect(2, 64, 20, 20));
        load1line2->setFrameShadow(QFrame::Plain);
        load1line2->setLineWidth(3);
        load1line2->setFrameShape(QFrame::VLine);
        load1line3 = new QFrame(groupBox);
        load1line3->setObjectName(QString::fromUtf8("load1line3"));
        load1line3->setGeometry(QRect(2, 10, 20, 16));
        load1line3->setFrameShadow(QFrame::Plain);
        load1line3->setLineWidth(3);
        load1line3->setFrameShape(QFrame::VLine);
        load1line2->raise();
        load1line3->raise();
        load1line1->raise();
        load1Battery2->raise();
        load1Battery3->raise();
        load1Battery1->raise();
        groupBox_2 = new QGroupBox(PowerManagementMainDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(419, 214, 61, 96));
        load2Battery2 = new QRadioButton(groupBox_2);
        load2Battery2->setObjectName(QString::fromUtf8("load2Battery2"));
        load2Battery2->setGeometry(QRect(22, 42, 21, 22));
        load2Battery1 = new QRadioButton(groupBox_2);
        load2Battery1->setObjectName(QString::fromUtf8("load2Battery1"));
        load2Battery1->setGeometry(QRect(22, 10, 21, 22));
        load2Battery1->setChecked(true);
        load2Battery3 = new QRadioButton(groupBox_2);
        load2Battery3->setObjectName(QString::fromUtf8("load2Battery3"));
        load2Battery3->setGeometry(QRect(22, 75, 21, 16));
        load2line1 = new QFrame(groupBox_2);
        load2line1->setObjectName(QString::fromUtf8("load2line1"));
        load2line1->setGeometry(QRect(20, 25, 20, 20));
        load2line1->setFrameShadow(QFrame::Plain);
        load2line1->setLineWidth(3);
        load2line1->setFrameShape(QFrame::VLine);
        load2line2 = new QFrame(groupBox_2);
        load2line2->setObjectName(QString::fromUtf8("load2line2"));
        load2line2->setGeometry(QRect(20, 54, 20, 20));
        load2line2->setFrameShadow(QFrame::Plain);
        load2line2->setLineWidth(3);
        load2line2->setFrameShape(QFrame::VLine);
        load2line3 = new QFrame(groupBox_2);
        load2line3->setObjectName(QString::fromUtf8("load2line3"));
        load2line3->setGeometry(QRect(20, -5, 20, 20));
        load2line3->setFrameShadow(QFrame::Plain);
        load2line3->setLineWidth(3);
        load2line3->setFrameShape(QFrame::VLine);
        load2line1->raise();
        load2line2->raise();
        load2line3->raise();
        load2Battery2->raise();
        load2Battery3->raise();
        load2Battery1->raise();
        groupBox_3 = new QGroupBox(PowerManagementMainDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(499, 215, 61, 98));
        panelBattery2 = new QRadioButton(groupBox_3);
        panelBattery2->setObjectName(QString::fromUtf8("panelBattery2"));
        panelBattery2->setGeometry(QRect(22, 40, 21, 22));
        panelBattery1 = new QRadioButton(groupBox_3);
        panelBattery1->setObjectName(QString::fromUtf8("panelBattery1"));
        panelBattery1->setGeometry(QRect(22, 10, 21, 22));
        panelBattery1->setChecked(true);
        panelBattery3 = new QRadioButton(groupBox_3);
        panelBattery3->setObjectName(QString::fromUtf8("panelBattery3"));
        panelBattery3->setGeometry(QRect(22, 70, 21, 22));
        panelline2 = new QFrame(groupBox_3);
        panelline2->setObjectName(QString::fromUtf8("panelline2"));
        panelline2->setGeometry(QRect(19, 26, 20, 20));
        panelline2->setFrameShadow(QFrame::Plain);
        panelline2->setLineWidth(3);
        panelline2->setFrameShape(QFrame::VLine);
        panelline3 = new QFrame(groupBox_3);
        panelline3->setObjectName(QString::fromUtf8("panelline3"));
        panelline3->setGeometry(QRect(19, 56, 20, 20));
        panelline3->setFrameShadow(QFrame::Plain);
        panelline3->setLineWidth(3);
        panelline3->setFrameShape(QFrame::VLine);
        panelline4 = new QFrame(groupBox_3);
        panelline4->setObjectName(QString::fromUtf8("panelline4"));
        panelline4->setGeometry(QRect(20, 85, 20, 13));
        panelline4->setFrameShadow(QFrame::Plain);
        panelline4->setLineWidth(4);
        panelline4->setFrameShape(QFrame::VLine);
        panelline3->raise();
        panelBattery3->raise();
        panelline2->raise();
        panelBattery2->raise();
        panelBattery1->raise();
        panelline4->raise();
        battery1line1 = new QFrame(PowerManagementMainDialog);
        battery1line1->setObjectName(QString::fromUtf8("battery1line1"));
        battery1line1->setGeometry(QRect(379, 227, 61, 16));
        battery1line1->setFrameShadow(QFrame::Plain);
        battery1line1->setLineWidth(3);
        battery1line1->setFrameShape(QFrame::HLine);
        battery2line1 = new QFrame(PowerManagementMainDialog);
        battery2line1->setObjectName(QString::fromUtf8("battery2line1"));
        battery2line1->setGeometry(QRect(379, 257, 61, 16));
        battery2line1->setFrameShadow(QFrame::Plain);
        battery2line1->setLineWidth(3);
        battery2line1->setFrameShape(QFrame::HLine);
        battery3line1 = new QFrame(PowerManagementMainDialog);
        battery3line1->setObjectName(QString::fromUtf8("battery3line1"));
        battery3line1->setGeometry(QRect(379, 287, 61, 16));
        battery3line1->setFrameShadow(QFrame::Plain);
        battery3line1->setLineWidth(3);
        battery3line1->setFrameShape(QFrame::HLine);
        battery2line2 = new QFrame(PowerManagementMainDialog);
        battery2line2->setObjectName(QString::fromUtf8("battery2line2"));
        battery2line2->setGeometry(QRect(455, 257, 65, 16));
        battery2line2->setFrameShadow(QFrame::Plain);
        battery2line2->setLineWidth(3);
        battery2line2->setFrameShape(QFrame::HLine);
        battery3line2 = new QFrame(PowerManagementMainDialog);
        battery3line2->setObjectName(QString::fromUtf8("battery3line2"));
        battery3line2->setGeometry(QRect(455, 287, 65, 16));
        battery3line2->setFrameShadow(QFrame::Plain);
        battery3line2->setLineWidth(3);
        battery3line2->setFrameShape(QFrame::HLine);
        battery1line2 = new QFrame(PowerManagementMainDialog);
        battery1line2->setObjectName(QString::fromUtf8("battery1line2"));
        battery1line2->setGeometry(QRect(455, 227, 65, 16));
        battery1line2->setFrameShadow(QFrame::Plain);
        battery1line2->setLineWidth(3);
        battery1line2->setFrameShape(QFrame::HLine);
        battery2line3 = new QFrame(PowerManagementMainDialog);
        battery2line3->setObjectName(QString::fromUtf8("battery2line3"));
        battery2line3->setGeometry(QRect(204, 257, 161, 16));
        battery2line3->setFrameShadow(QFrame::Plain);
        battery2line3->setLineWidth(3);
        battery2line3->setFrameShape(QFrame::HLine);
        battery3line3 = new QFrame(PowerManagementMainDialog);
        battery3line3->setObjectName(QString::fromUtf8("battery3line3"));
        battery3line3->setGeometry(QRect(284, 287, 79, 16));
        battery3line3->setFrameShadow(QFrame::Plain);
        battery3line3->setLineWidth(3);
        battery3line3->setFrameShape(QFrame::HLine);
        battery1line3 = new QFrame(PowerManagementMainDialog);
        battery1line3->setObjectName(QString::fromUtf8("battery1line3"));
        battery1line3->setGeometry(QRect(124, 227, 241, 16));
        battery1line3->setFrameShadow(QFrame::Plain);
        battery1line3->setLineWidth(3);
        battery1line3->setFrameShape(QFrame::HLine);
        battery1CheckBox = new QCheckBox(PowerManagementMainDialog);
        battery1CheckBox->setObjectName(QString::fromUtf8("battery1CheckBox"));
        battery1CheckBox->setGeometry(QRect(96, 349, 71, 22));
        battery1CheckBox->setLayoutDirection(Qt::RightToLeft);
        battery1CheckBox->setChecked(false);
        battery2CheckBox = new QCheckBox(PowerManagementMainDialog);
        battery2CheckBox->setObjectName(QString::fromUtf8("battery2CheckBox"));
        battery2CheckBox->setGeometry(QRect(177, 349, 71, 22));
        battery2CheckBox->setLayoutDirection(Qt::RightToLeft);
        battery3CheckBox = new QCheckBox(PowerManagementMainDialog);
        battery3CheckBox->setObjectName(QString::fromUtf8("battery3CheckBox"));
        battery3CheckBox->setGeometry(QRect(257, 349, 71, 22));
        battery3CheckBox->setLayoutDirection(Qt::RightToLeft);
        load1CheckBox = new QCheckBox(PowerManagementMainDialog);
        load1CheckBox->setObjectName(QString::fromUtf8("load1CheckBox"));
        load1CheckBox->setGeometry(QRect(329, 349, 81, 22));
        load1CheckBox->setLayoutDirection(Qt::RightToLeft);
        load1CheckBox->setChecked(false);
        load2CheckBox = new QCheckBox(PowerManagementMainDialog);
        load2CheckBox->setObjectName(QString::fromUtf8("load2CheckBox"));
        load2CheckBox->setGeometry(QRect(413, 349, 81, 22));
        load2CheckBox->setLayoutDirection(Qt::RightToLeft);
        load2CheckBox->setChecked(false);
        panelCheckBox = new QCheckBox(PowerManagementMainDialog);
        panelCheckBox->setObjectName(QString::fromUtf8("panelCheckBox"));
        panelCheckBox->setGeometry(QRect(497, 350, 71, 22));
        panelCheckBox->setLayoutDirection(Qt::RightToLeft);
        panelCheckBox->setChecked(false);
        line = new QFrame(PowerManagementMainDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(115, 235, 20, 73));
        line->setFrameShadow(QFrame::Plain);
        line->setLineWidth(4);
        line->setFrameShape(QFrame::VLine);
        line_2 = new QFrame(PowerManagementMainDialog);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(195, 265, 20, 42));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setLineWidth(4);
        line_2->setFrameShape(QFrame::VLine);
        line_3 = new QFrame(PowerManagementMainDialog);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(275, 296, 20, 13));
        line_3->setFrameShadow(QFrame::Plain);
        line_3->setLineWidth(4);
        line_3->setFrameShape(QFrame::VLine);
        temperature = new QLabel(PowerManagementMainDialog);
        temperature->setObjectName(QString::fromUtf8("temperature"));
        temperature->setGeometry(QRect(287, 52, 77, 24));
        QFont font2;
        font2.setPointSize(14);
        temperature->setFont(font2);
        temperature->setFrameShape(QFrame::Box);
        temperature->setScaledContents(true);
        battery2Charge = new QLabel(PowerManagementMainDialog);
        battery2Charge->setObjectName(QString::fromUtf8("battery2Charge"));
        battery2Charge->setGeometry(QRect(278, 106, 95, 52));
        battery2Charge->setFont(font1);
        battery2Charge->setFrameShape(QFrame::Box);
        battery2Charge->setScaledContents(true);
        battery2Charge->setAlignment(Qt::AlignCenter);
        battery3Charge = new QLabel(PowerManagementMainDialog);
        battery3Charge->setObjectName(QString::fromUtf8("battery3Charge"));
        battery3Charge->setGeometry(QRect(455, 106, 95, 52));
        battery3Charge->setFont(font1);
        battery3Charge->setFrameShape(QFrame::Box);
        battery3Charge->setScaledContents(true);
        battery3Charge->setAlignment(Qt::AlignCenter);
        battery1Image = new QLabel(PowerManagementMainDialog);
        battery1Image->setObjectName(QString::fromUtf8("battery1Image"));
        battery1Image->setGeometry(QRect(105, 310, 41, 29));
        battery1Image->setPixmap(QPixmap(QString::fromUtf8(":/main/battery2.png")));
        battery2Image = new QLabel(PowerManagementMainDialog);
        battery2Image->setObjectName(QString::fromUtf8("battery2Image"));
        battery2Image->setGeometry(QRect(185, 309, 41, 29));
        battery2Image->setPixmap(QPixmap(QString::fromUtf8(":/main/battery2.png")));
        battery3Image = new QLabel(PowerManagementMainDialog);
        battery3Image->setObjectName(QString::fromUtf8("battery3Image"));
        battery3Image->setGeometry(QRect(265, 310, 41, 29));
        battery3Image->setPixmap(QPixmap(QString::fromUtf8(":/main/battery2.png")));
        load1Image = new QLabel(PowerManagementMainDialog);
        load1Image->setObjectName(QString::fromUtf8("load1Image"));
        load1Image->setGeometry(QRect(355, 185, 41, 29));
        load1Image->setPixmap(QPixmap(QString::fromUtf8(":/main/light2.png")));
        load2Image = new QLabel(PowerManagementMainDialog);
        load2Image->setObjectName(QString::fromUtf8("load2Image"));
        load2Image->setGeometry(QRect(435, 184, 41, 29));
        load2Image->setPixmap(QPixmap(QString::fromUtf8(":/main/light2.png")));
        panelImage = new QLabel(PowerManagementMainDialog);
        panelImage->setObjectName(QString::fromUtf8("panelImage"));
        panelImage->setGeometry(QRect(505, 312, 41, 29));
        panelImage->setPixmap(QPixmap(QString::fromUtf8(":/main/panel4.png")));
        layoutWidget = new QWidget(PowerManagementMainDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(93, 477, 471, 29));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        monitorButton = new QPushButton(layoutWidget);
        monitorButton->setObjectName(QString::fromUtf8("monitorButton"));

        horizontalLayout->addWidget(monitorButton);

        configureButton = new QPushButton(layoutWidget);
        configureButton->setObjectName(QString::fromUtf8("configureButton"));

        horizontalLayout->addWidget(configureButton);

        recordingButton = new QPushButton(layoutWidget);
        recordingButton->setObjectName(QString::fromUtf8("recordingButton"));

        horizontalLayout->addWidget(recordingButton);

        saveFileButton = new QPushButton(layoutWidget);
        saveFileButton->setObjectName(QString::fromUtf8("saveFileButton"));

        horizontalLayout->addWidget(saveFileButton);

        closeFileButton = new QPushButton(layoutWidget);
        closeFileButton->setObjectName(QString::fromUtf8("closeFileButton"));

        horizontalLayout->addWidget(closeFileButton);

        layoutWidget1 = new QWidget(PowerManagementMainDialog);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(13, 405, 631, 62));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        currentLabel = new QLabel(layoutWidget1);
        currentLabel->setObjectName(QString::fromUtf8("currentLabel"));
        currentLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(currentLabel, 0, 0, 1, 1);

        battery1Current = new QLabel(layoutWidget1);
        battery1Current->setObjectName(QString::fromUtf8("battery1Current"));
        QFont font3;
        font3.setPointSize(12);
        battery1Current->setFont(font3);
        battery1Current->setFrameShape(QFrame::Box);

        gridLayout->addWidget(battery1Current, 0, 1, 1, 1);

        battery2Current = new QLabel(layoutWidget1);
        battery2Current->setObjectName(QString::fromUtf8("battery2Current"));
        battery2Current->setFont(font3);
        battery2Current->setFrameShape(QFrame::Box);

        gridLayout->addWidget(battery2Current, 0, 2, 1, 1);

        battery3Current = new QLabel(layoutWidget1);
        battery3Current->setObjectName(QString::fromUtf8("battery3Current"));
        battery3Current->setFont(font3);
        battery3Current->setFrameShape(QFrame::Box);

        gridLayout->addWidget(battery3Current, 0, 3, 1, 1);

        load1Current = new QLabel(layoutWidget1);
        load1Current->setObjectName(QString::fromUtf8("load1Current"));
        load1Current->setFont(font3);
        load1Current->setFrameShape(QFrame::Box);

        gridLayout->addWidget(load1Current, 0, 4, 1, 1);

        load2Current = new QLabel(layoutWidget1);
        load2Current->setObjectName(QString::fromUtf8("load2Current"));
        load2Current->setFont(font3);
        load2Current->setFrameShape(QFrame::Box);

        gridLayout->addWidget(load2Current, 0, 5, 1, 1);

        panelCurrent = new QLabel(layoutWidget1);
        panelCurrent->setObjectName(QString::fromUtf8("panelCurrent"));
        panelCurrent->setFont(font3);
        panelCurrent->setFrameShape(QFrame::Box);

        gridLayout->addWidget(panelCurrent, 0, 6, 1, 1);

        currentLabel_2 = new QLabel(layoutWidget1);
        currentLabel_2->setObjectName(QString::fromUtf8("currentLabel_2"));
        currentLabel_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(currentLabel_2, 0, 7, 1, 1);

        voltageLabel = new QLabel(layoutWidget1);
        voltageLabel->setObjectName(QString::fromUtf8("voltageLabel"));
        voltageLabel->setLayoutDirection(Qt::LeftToRight);
        voltageLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(voltageLabel, 1, 0, 1, 1);

        battery1Voltage = new QLabel(layoutWidget1);
        battery1Voltage->setObjectName(QString::fromUtf8("battery1Voltage"));
        battery1Voltage->setFont(font3);
        battery1Voltage->setFrameShape(QFrame::Box);

        gridLayout->addWidget(battery1Voltage, 1, 1, 1, 1);

        battery2Voltage = new QLabel(layoutWidget1);
        battery2Voltage->setObjectName(QString::fromUtf8("battery2Voltage"));
        battery2Voltage->setFont(font3);
        battery2Voltage->setFrameShape(QFrame::Box);

        gridLayout->addWidget(battery2Voltage, 1, 2, 1, 1);

        battery3Voltage = new QLabel(layoutWidget1);
        battery3Voltage->setObjectName(QString::fromUtf8("battery3Voltage"));
        battery3Voltage->setFont(font3);
        battery3Voltage->setFrameShape(QFrame::Box);

        gridLayout->addWidget(battery3Voltage, 1, 3, 1, 1);

        load1Voltage = new QLabel(layoutWidget1);
        load1Voltage->setObjectName(QString::fromUtf8("load1Voltage"));
        load1Voltage->setFont(font3);
        load1Voltage->setFrameShape(QFrame::Box);

        gridLayout->addWidget(load1Voltage, 1, 4, 1, 1);

        load2Voltage = new QLabel(layoutWidget1);
        load2Voltage->setObjectName(QString::fromUtf8("load2Voltage"));
        load2Voltage->setFont(font3);
        load2Voltage->setFrameShape(QFrame::Box);

        gridLayout->addWidget(load2Voltage, 1, 5, 1, 1);

        panelVoltage = new QLabel(layoutWidget1);
        panelVoltage->setObjectName(QString::fromUtf8("panelVoltage"));
        panelVoltage->setFont(font3);
        panelVoltage->setFrameShape(QFrame::Box);

        gridLayout->addWidget(panelVoltage, 1, 6, 1, 1);

        voltageLabel_2 = new QLabel(layoutWidget1);
        voltageLabel_2->setObjectName(QString::fromUtf8("voltageLabel_2"));
        voltageLabel_2->setLayoutDirection(Qt::LeftToRight);
        voltageLabel_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(voltageLabel_2, 1, 7, 1, 1);

        battery1OverCurrent = new QPushButton(PowerManagementMainDialog);
        battery1OverCurrent->setObjectName(QString::fromUtf8("battery1OverCurrent"));
        battery1OverCurrent->setGeometry(QRect(110, 370, 16, 16));
        QFont font4;
        font4.setPointSize(6);
        battery1OverCurrent->setFont(font4);
        battery2OverCurrent = new QPushButton(PowerManagementMainDialog);
        battery2OverCurrent->setObjectName(QString::fromUtf8("battery2OverCurrent"));
        battery2OverCurrent->setGeometry(QRect(190, 370, 16, 16));
        battery2OverCurrent->setFont(font4);
        battery2OverCurrent->setAutoFillBackground(true);
        battery3OverCurrent = new QPushButton(PowerManagementMainDialog);
        battery3OverCurrent->setObjectName(QString::fromUtf8("battery3OverCurrent"));
        battery3OverCurrent->setGeometry(QRect(270, 370, 16, 16));
        battery3OverCurrent->setFont(font4);
        load1OverCurrent = new QPushButton(PowerManagementMainDialog);
        load1OverCurrent->setObjectName(QString::fromUtf8("load1OverCurrent"));
        load1OverCurrent->setGeometry(QRect(350, 370, 16, 16));
        load1OverCurrent->setFont(font4);
        panelOverCurrent = new QPushButton(PowerManagementMainDialog);
        panelOverCurrent->setObjectName(QString::fromUtf8("panelOverCurrent"));
        panelOverCurrent->setGeometry(QRect(515, 370, 16, 16));
        panelOverCurrent->setFont(font4);
        load2OverCurrent = new QPushButton(PowerManagementMainDialog);
        load2OverCurrent->setObjectName(QString::fromUtf8("load2OverCurrent"));
        load2OverCurrent->setGeometry(QRect(435, 370, 16, 16));
        load2OverCurrent->setFont(font4);
        autoTrackCheckBox = new QCheckBox(PowerManagementMainDialog);
        autoTrackCheckBox->setObjectName(QString::fromUtf8("autoTrackCheckBox"));
        autoTrackCheckBox->setGeometry(QRect(252, 515, 151, 22));
        autoTrackCheckBox->setLayoutDirection(Qt::RightToLeft);
        autoTrackCheckBox->setChecked(false);
        battery1UnderVoltage = new QLabel(PowerManagementMainDialog);
        battery1UnderVoltage->setObjectName(QString::fromUtf8("battery1UnderVoltage"));
        battery1UnderVoltage->setGeometry(QRect(125, 370, 16, 17));
        battery1UnderVoltage->setFont(font4);
        battery1UnderVoltage->setFrameShape(QFrame::StyledPanel);
        battery1UnderVoltage->setFrameShadow(QFrame::Raised);
        battery2UnderVoltage = new QLabel(PowerManagementMainDialog);
        battery2UnderVoltage->setObjectName(QString::fromUtf8("battery2UnderVoltage"));
        battery2UnderVoltage->setGeometry(QRect(205, 370, 16, 17));
        battery2UnderVoltage->setFont(font4);
        battery2UnderVoltage->setFrameShape(QFrame::StyledPanel);
        battery2UnderVoltage->setFrameShadow(QFrame::Raised);
        battery3UnderVoltage = new QLabel(PowerManagementMainDialog);
        battery3UnderVoltage->setObjectName(QString::fromUtf8("battery3UnderVoltage"));
        battery3UnderVoltage->setGeometry(QRect(285, 370, 16, 17));
        battery3UnderVoltage->setFont(font4);
        battery3UnderVoltage->setFrameShape(QFrame::StyledPanel);
        battery3UnderVoltage->setFrameShadow(QFrame::Raised);
        load1UnderVoltage = new QLabel(PowerManagementMainDialog);
        load1UnderVoltage->setObjectName(QString::fromUtf8("load1UnderVoltage"));
        load1UnderVoltage->setGeometry(QRect(365, 370, 16, 17));
        load1UnderVoltage->setFont(font4);
        load1UnderVoltage->setFrameShape(QFrame::StyledPanel);
        load1UnderVoltage->setFrameShadow(QFrame::Raised);
        load2UnderVoltage = new QLabel(PowerManagementMainDialog);
        load2UnderVoltage->setObjectName(QString::fromUtf8("load2UnderVoltage"));
        load2UnderVoltage->setGeometry(QRect(450, 370, 16, 17));
        load2UnderVoltage->setFont(font4);
        load2UnderVoltage->setFrameShape(QFrame::StyledPanel);
        load2UnderVoltage->setFrameShadow(QFrame::Raised);
        panelUnderVoltage = new QLabel(PowerManagementMainDialog);
        panelUnderVoltage->setObjectName(QString::fromUtf8("panelUnderVoltage"));
        panelUnderVoltage->setGeometry(QRect(530, 370, 16, 17));
        panelUnderVoltage->setFont(font4);
        panelUnderVoltage->setFrameShape(QFrame::StyledPanel);
        panelUnderVoltage->setFrameShadow(QFrame::Raised);
        battery1Fill = new QLabel(PowerManagementMainDialog);
        battery1Fill->setObjectName(QString::fromUtf8("battery1Fill"));
        battery1Fill->setGeometry(QRect(130, 160, 16, 17));
        battery1Fill->setFont(font4);
        battery1Fill->setFrameShape(QFrame::StyledPanel);
        battery1Fill->setFrameShadow(QFrame::Raised);
        battery2Fill = new QLabel(PowerManagementMainDialog);
        battery2Fill->setObjectName(QString::fromUtf8("battery2Fill"));
        battery2Fill->setGeometry(QRect(300, 160, 16, 17));
        battery2Fill->setFont(font4);
        battery2Fill->setFrameShape(QFrame::StyledPanel);
        battery2Fill->setFrameShadow(QFrame::Raised);
        battery3Fill = new QLabel(PowerManagementMainDialog);
        battery3Fill->setObjectName(QString::fromUtf8("battery3Fill"));
        battery3Fill->setGeometry(QRect(480, 160, 16, 17));
        battery3Fill->setFont(font4);
        battery3Fill->setFrameShape(QFrame::StyledPanel);
        battery3Fill->setFrameShadow(QFrame::Raised);
        battery1Charging = new QLabel(PowerManagementMainDialog);
        battery1Charging->setObjectName(QString::fromUtf8("battery1Charging"));
        battery1Charging->setGeometry(QRect(145, 160, 16, 17));
        battery1Charging->setFont(font4);
        battery1Charging->setFrameShape(QFrame::StyledPanel);
        battery1Charging->setFrameShadow(QFrame::Raised);
        battery2Charging = new QLabel(PowerManagementMainDialog);
        battery2Charging->setObjectName(QString::fromUtf8("battery2Charging"));
        battery2Charging->setGeometry(QRect(315, 160, 16, 17));
        battery2Charging->setFont(font4);
        battery2Charging->setFrameShape(QFrame::StyledPanel);
        battery2Charging->setFrameShadow(QFrame::Raised);
        battery3Charging = new QLabel(PowerManagementMainDialog);
        battery3Charging->setObjectName(QString::fromUtf8("battery3Charging"));
        battery3Charging->setGeometry(QRect(495, 160, 16, 17));
        battery3Charging->setFont(font4);
        battery3Charging->setFrameShape(QFrame::StyledPanel);
        battery3Charging->setFrameShadow(QFrame::Raised);
        battery1Op = new QLabel(PowerManagementMainDialog);
        battery1Op->setObjectName(QString::fromUtf8("battery1Op"));
        battery1Op->setGeometry(QRect(160, 160, 16, 17));
        battery1Op->setFont(font4);
        battery1Op->setFrameShape(QFrame::StyledPanel);
        battery1Op->setFrameShadow(QFrame::Raised);
        battery2Op = new QLabel(PowerManagementMainDialog);
        battery2Op->setObjectName(QString::fromUtf8("battery2Op"));
        battery2Op->setGeometry(QRect(330, 160, 16, 17));
        battery2Op->setFont(font4);
        battery2Op->setFrameShape(QFrame::StyledPanel);
        battery2Op->setFrameShadow(QFrame::Raised);
        battery3Op = new QLabel(PowerManagementMainDialog);
        battery3Op->setObjectName(QString::fromUtf8("battery3Op"));
        battery3Op->setGeometry(QRect(510, 160, 16, 17));
        battery3Op->setFont(font4);
        battery3Op->setFrameShape(QFrame::StyledPanel);
        battery3Op->setFrameShadow(QFrame::Raised);
        connectButton = new QPushButton(PowerManagementMainDialog);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(94, 515, 89, 27));

        retranslateUi(PowerManagementMainDialog);

        QMetaObject::connectSlotsByName(PowerManagementMainDialog);
    } // setupUi

    void retranslateUi(QDialog *PowerManagementMainDialog)
    {
        PowerManagementMainDialog->setWindowTitle(QApplication::translate("PowerManagementMainDialog", "Solar Power Management", 0, QApplication::UnicodeUTF8));
        title->setText(QApplication::translate("PowerManagementMainDialog", "Solar Power/Battery Management System", 0, QApplication::UnicodeUTF8));
        errorLabel->setText(QString());
        battery1Charge->setText(QString());
        battery1ChargeLabel->setText(QApplication::translate("PowerManagementMainDialog", "Battery 1 Charge", 0, QApplication::UnicodeUTF8));
        displayFreqLabel->setText(QApplication::translate("PowerManagementMainDialog", "Display Frequency", 0, QApplication::UnicodeUTF8));
        displayIntervalLabel->setText(QApplication::translate("PowerManagementMainDialog", "0.1 sec", 0, QApplication::UnicodeUTF8));
        battery2ChargeLabel->setText(QApplication::translate("PowerManagementMainDialog", "Battery 2 Charge", 0, QApplication::UnicodeUTF8));
        battery3ChargeLabel->setText(QApplication::translate("PowerManagementMainDialog", "Battery 3 Charge", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        load1Battery2->setText(QString());
        load1Battery1->setText(QString());
        load1Battery3->setText(QString());
        groupBox_2->setTitle(QString());
        load2Battery2->setText(QString());
        load2Battery1->setText(QString());
        load2Battery3->setText(QString());
        groupBox_3->setTitle(QString());
        panelBattery2->setText(QString());
        panelBattery1->setText(QString());
        panelBattery3->setText(QString());
        battery1CheckBox->setText(QApplication::translate("PowerManagementMainDialog", "Batt 1", 0, QApplication::UnicodeUTF8));
        battery2CheckBox->setText(QApplication::translate("PowerManagementMainDialog", "Batt 2", 0, QApplication::UnicodeUTF8));
        battery3CheckBox->setText(QApplication::translate("PowerManagementMainDialog", "Batt 3", 0, QApplication::UnicodeUTF8));
        load1CheckBox->setText(QApplication::translate("PowerManagementMainDialog", "Load 1", 0, QApplication::UnicodeUTF8));
        load2CheckBox->setText(QApplication::translate("PowerManagementMainDialog", "Load 2", 0, QApplication::UnicodeUTF8));
        panelCheckBox->setText(QApplication::translate("PowerManagementMainDialog", "Panel", 0, QApplication::UnicodeUTF8));
        temperature->setText(QString());
        battery2Charge->setText(QString());
        battery3Charge->setText(QString());
        battery1Image->setText(QString());
        battery2Image->setText(QString());
        battery3Image->setText(QString());
        load1Image->setText(QString());
        load2Image->setText(QString());
        panelImage->setText(QString());
#ifndef QT_NO_TOOLTIP
        monitorButton->setToolTip(QApplication::translate("PowerManagementMainDialog", "<html><head/><body><p>Visual monitoring of system variables.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        monitorButton->setText(QApplication::translate("PowerManagementMainDialog", "Monitor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        configureButton->setToolTip(QApplication::translate("PowerManagementMainDialog", "<html><head/><body><p>System configuration and advanced configuration choices for algorithms.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        configureButton->setText(QApplication::translate("PowerManagementMainDialog", "Configure", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        recordingButton->setToolTip(QApplication::translate("PowerManagementMainDialog", "<html><head/><body><p>Open or close files on the BMS for recording history, and start and pause recording actions.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        recordingButton->setText(QApplication::translate("PowerManagementMainDialog", "Record", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        saveFileButton->setToolTip(QApplication::translate("PowerManagementMainDialog", "<html><head/><body><p>Save incoming data to a local file</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        saveFileButton->setText(QApplication::translate("PowerManagementMainDialog", "Open File", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        closeFileButton->setToolTip(QApplication::translate("PowerManagementMainDialog", "<html><head/><body><p>Save incoming data to a local file</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        closeFileButton->setText(QApplication::translate("PowerManagementMainDialog", "Close File", 0, QApplication::UnicodeUTF8));
        currentLabel->setText(QApplication::translate("PowerManagementMainDialog", "Current", 0, QApplication::UnicodeUTF8));
        battery1Current->setText(QString());
        battery2Current->setText(QString());
        battery3Current->setText(QString());
        load1Current->setText(QString());
        load2Current->setText(QString());
        panelCurrent->setText(QString());
        currentLabel_2->setText(QApplication::translate("PowerManagementMainDialog", "Current", 0, QApplication::UnicodeUTF8));
        voltageLabel->setText(QApplication::translate("PowerManagementMainDialog", "Voltage", 0, QApplication::UnicodeUTF8));
        battery1Voltage->setText(QString());
        battery2Voltage->setText(QString());
        battery3Voltage->setText(QString());
        load1Voltage->setText(QString());
        load2Voltage->setText(QString());
        panelVoltage->setText(QString());
        voltageLabel_2->setText(QApplication::translate("PowerManagementMainDialog", "Voltage", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        battery1OverCurrent->setToolTip(QApplication::translate("PowerManagementMainDialog", "OverCurrent Reset", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery1OverCurrent->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery2OverCurrent->setToolTip(QApplication::translate("PowerManagementMainDialog", "OverCurrent Reset", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery2OverCurrent->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery3OverCurrent->setToolTip(QApplication::translate("PowerManagementMainDialog", "OverCurrent Reset", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery3OverCurrent->setText(QString());
#ifndef QT_NO_TOOLTIP
        load1OverCurrent->setToolTip(QApplication::translate("PowerManagementMainDialog", "OverCurrent Reset", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        load1OverCurrent->setText(QString());
#ifndef QT_NO_TOOLTIP
        panelOverCurrent->setToolTip(QApplication::translate("PowerManagementMainDialog", "OverCurrent Reset", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        panelOverCurrent->setText(QString());
#ifndef QT_NO_TOOLTIP
        load2OverCurrent->setToolTip(QApplication::translate("PowerManagementMainDialog", "OverCurrent Reset", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        load2OverCurrent->setText(QString());
        autoTrackCheckBox->setText(QApplication::translate("PowerManagementMainDialog", "Automatic Tracking", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        battery1UnderVoltage->setToolTip(QApplication::translate("PowerManagementMainDialog", "UnderVoltage", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery1UnderVoltage->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery2UnderVoltage->setToolTip(QApplication::translate("PowerManagementMainDialog", "UnderVoltage", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery2UnderVoltage->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery3UnderVoltage->setToolTip(QApplication::translate("PowerManagementMainDialog", "UnderVoltage", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery3UnderVoltage->setText(QString());
#ifndef QT_NO_TOOLTIP
        load1UnderVoltage->setToolTip(QApplication::translate("PowerManagementMainDialog", "UnderVoltage", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        load1UnderVoltage->setText(QString());
#ifndef QT_NO_TOOLTIP
        load2UnderVoltage->setToolTip(QApplication::translate("PowerManagementMainDialog", "UnderVoltage", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        load2UnderVoltage->setText(QString());
#ifndef QT_NO_TOOLTIP
        panelUnderVoltage->setToolTip(QApplication::translate("PowerManagementMainDialog", "UnderVoltage", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        panelUnderVoltage->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery1Fill->setToolTip(QApplication::translate("PowerManagementMainDialog", "Fill State - Normal, Low, Critical", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery1Fill->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery2Fill->setToolTip(QApplication::translate("PowerManagementMainDialog", "Fill State - Normal, Low, Critical", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery2Fill->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery3Fill->setToolTip(QApplication::translate("PowerManagementMainDialog", "Fill State - Normal, Low, Critical", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery3Fill->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery1Charging->setToolTip(QApplication::translate("PowerManagementMainDialog", "Charging State - Bulk, Absorption, Float", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery1Charging->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery2Charging->setToolTip(QApplication::translate("PowerManagementMainDialog", "Charging State - Bulk, Absorption, Float", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery2Charging->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery3Charging->setToolTip(QApplication::translate("PowerManagementMainDialog", "Charging State - Bulk, Absorption, Float", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery3Charging->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery1Op->setToolTip(QApplication::translate("PowerManagementMainDialog", "Operational State - Loaded, Charging, Isolated", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery1Op->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery2Op->setToolTip(QApplication::translate("PowerManagementMainDialog", "Operational State - Loaded, Charging, Isolated", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery2Op->setText(QString());
#ifndef QT_NO_TOOLTIP
        battery3Op->setToolTip(QApplication::translate("PowerManagementMainDialog", "Operational State - Loaded, Charging, Isolated", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        battery3Op->setText(QString());
#ifndef QT_NO_TOOLTIP
        connectButton->setToolTip(QApplication::translate("PowerManagementMainDialog", "Connect to the TCP address and port as configured", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        connectButton->setText(QApplication::translate("PowerManagementMainDialog", "Connect", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PowerManagementMainDialog: public Ui_PowerManagementMainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POWER_2D_MANAGEMENT_2D_MAIN_H
