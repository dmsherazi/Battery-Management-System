/********************************************************************************
** Form generated from reading UI file 'power-management-configure.ui'
**
** Created: Wed Mar 26 18:49:46 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POWER_2D_MANAGEMENT_2D_CONFIGURE_H
#define UI_POWER_2D_MANAGEMENT_2D_CONFIGURE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PowerManagementConfigDialog
{
public:
    QLabel *title;
    QTabWidget *tabWidget;
    QWidget *generalTab;
    QPushButton *timeSetButton;
    QLabel *date;
    QLabel *time;
    QLabel *dateTimeLabel;
    QLabel *generalLabel;
    QCheckBox *debugMessageCheckbox;
    QCheckBox *dataMessageCheckbox;
    QPushButton *echoTestButton;
    QWidget *batteriesTab;
    QPushButton *queryBatteryButton;
    QLabel *battery1Label;
    QLabel *battery2Label;
    QLabel *battery3Label;
    QLabel *battery1Resistance;
    QLabel *battery3Resistance;
    QLabel *battery2Resistance;
    QLabel *AHLabel_1;
    QSpinBox *battery1CapacitySpinBox;
    QLabel *AHLabel_2;
    QSpinBox *battery2CapacitySpinBox;
    QLabel *AHLabel_3;
    QSpinBox *battery3CapacitySpinBox;
    QPushButton *setBatteryButton;
    QLabel *resistanceLabel;
    QLabel *capacityLabel;
    QComboBox *battery1TypeCombo;
    QComboBox *battery2TypeCombo;
    QComboBox *battery3TypeCombo;
    QLabel *typeLabel;
    QLabel *batteriesLabel;
    QLabel *battery1AbsorptionVoltage;
    QLabel *battery2AbsorptionVoltage;
    QLabel *battery3AbsorptionVoltage;
    QLabel *battery1FloatVoltage;
    QLabel *battery2FloatVoltage;
    QLabel *battery3FloatVoltage;
    QLabel *battery3FloatCurrent;
    QLabel *battery2FloatCurrent;
    QLabel *battery1FloatCurrent;
    QLabel *absorptionVoltageLabel;
    QLabel *floatVoltageLabel;
    QLabel *floatCurrentLabel;
    QLabel *battery2AbsorptionCurrent;
    QLabel *battery3AbsorptionCurrent;
    QLabel *battery1AbsorptionCurrent;
    QLabel *absorptionCurrentLabel;
    QWidget *calibrationTab;
    QPushButton *calibrateButton;
    QLabel *socLabel;
    QLabel *calibrationLabel;
    QLabel *qcLabel;
    QLabel *quiescentCurrent;
    QProgressBar *calibrateProgressBar;
    QWidget *socTab;
    QCheckBox *loadChargeCheckBox;
    QLabel *loadChargeText;
    QCheckBox *isolationMaintainCheckBox;
    QLabel *isolationMaintainText;
    QCheckBox *accelChargeCheckBox;
    QLabel *accelChargeText;
    QPushButton *setOptionButton;
    QLabel *errorLabel;
    QPushButton *closeButton;

    void setupUi(QDialog *PowerManagementConfigDialog)
    {
        if (PowerManagementConfigDialog->objectName().isEmpty())
            PowerManagementConfigDialog->setObjectName(QString::fromUtf8("PowerManagementConfigDialog"));
        PowerManagementConfigDialog->resize(682, 570);
        title = new QLabel(PowerManagementConfigDialog);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(150, 12, 371, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Andale Mono"));
        font.setPointSize(18);
        title->setFont(font);
        tabWidget = new QTabWidget(PowerManagementConfigDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 50, 651, 471));
        generalTab = new QWidget();
        generalTab->setObjectName(QString::fromUtf8("generalTab"));
        timeSetButton = new QPushButton(generalTab);
        timeSetButton->setObjectName(QString::fromUtf8("timeSetButton"));
        timeSetButton->setGeometry(QRect(248, 130, 101, 27));
        date = new QLabel(generalTab);
        date->setObjectName(QString::fromUtf8("date"));
        date->setGeometry(QRect(178, 90, 111, 26));
        date->setFrameShape(QFrame::StyledPanel);
        date->setFrameShadow(QFrame::Raised);
        date->setLineWidth(3);
        date->setMidLineWidth(1);
        time = new QLabel(generalTab);
        time->setObjectName(QString::fromUtf8("time"));
        time->setGeometry(QRect(303, 90, 111, 26));
        time->setFrameShape(QFrame::StyledPanel);
        time->setFrameShadow(QFrame::Raised);
        time->setLineWidth(3);
        time->setMidLineWidth(1);
        dateTimeLabel = new QLabel(generalTab);
        dateTimeLabel->setObjectName(QString::fromUtf8("dateTimeLabel"));
        dateTimeLabel->setGeometry(QRect(148, 65, 316, 20));
        generalLabel = new QLabel(generalTab);
        generalLabel->setObjectName(QString::fromUtf8("generalLabel"));
        generalLabel->setGeometry(QRect(194, 5, 239, 41));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        generalLabel->setFont(font1);
        generalLabel->setAlignment(Qt::AlignCenter);
        generalLabel->setWordWrap(true);
        debugMessageCheckbox = new QCheckBox(generalTab);
        debugMessageCheckbox->setObjectName(QString::fromUtf8("debugMessageCheckbox"));
        debugMessageCheckbox->setGeometry(QRect(208, 175, 187, 22));
        dataMessageCheckbox = new QCheckBox(generalTab);
        dataMessageCheckbox->setObjectName(QString::fromUtf8("dataMessageCheckbox"));
        dataMessageCheckbox->setGeometry(QRect(214, 205, 185, 22));
        dataMessageCheckbox->setChecked(true);
        echoTestButton = new QPushButton(generalTab);
        echoTestButton->setObjectName(QString::fromUtf8("echoTestButton"));
        echoTestButton->setGeometry(QRect(250, 240, 91, 27));
        tabWidget->addTab(generalTab, QString());
        batteriesTab = new QWidget();
        batteriesTab->setObjectName(QString::fromUtf8("batteriesTab"));
        queryBatteryButton = new QPushButton(batteriesTab);
        queryBatteryButton->setObjectName(QString::fromUtf8("queryBatteryButton"));
        queryBatteryButton->setGeometry(QRect(30, 70, 91, 27));
        battery1Label = new QLabel(batteriesTab);
        battery1Label->setObjectName(QString::fromUtf8("battery1Label"));
        battery1Label->setGeometry(QRect(166, 51, 64, 17));
        battery2Label = new QLabel(batteriesTab);
        battery2Label->setObjectName(QString::fromUtf8("battery2Label"));
        battery2Label->setGeometry(QRect(276, 51, 64, 17));
        battery3Label = new QLabel(batteriesTab);
        battery3Label->setObjectName(QString::fromUtf8("battery3Label"));
        battery3Label->setGeometry(QRect(386, 50, 64, 17));
        battery1Resistance = new QLabel(batteriesTab);
        battery1Resistance->setObjectName(QString::fromUtf8("battery1Resistance"));
        battery1Resistance->setGeometry(QRect(160, 70, 71, 31));
        battery1Resistance->setFrameShape(QFrame::Box);
        battery3Resistance = new QLabel(batteriesTab);
        battery3Resistance->setObjectName(QString::fromUtf8("battery3Resistance"));
        battery3Resistance->setGeometry(QRect(380, 70, 71, 31));
        battery3Resistance->setFrameShape(QFrame::Box);
        battery2Resistance = new QLabel(batteriesTab);
        battery2Resistance->setObjectName(QString::fromUtf8("battery2Resistance"));
        battery2Resistance->setGeometry(QRect(270, 70, 71, 31));
        battery2Resistance->setFrameShape(QFrame::Box);
        AHLabel_1 = new QLabel(batteriesTab);
        AHLabel_1->setObjectName(QString::fromUtf8("AHLabel_1"));
        AHLabel_1->setGeometry(QRect(210, 120, 31, 17));
        AHLabel_1->setAlignment(Qt::AlignCenter);
        battery1CapacitySpinBox = new QSpinBox(batteriesTab);
        battery1CapacitySpinBox->setObjectName(QString::fromUtf8("battery1CapacitySpinBox"));
        battery1CapacitySpinBox->setGeometry(QRect(160, 115, 52, 25));
        battery1CapacitySpinBox->setMinimum(40);
        battery1CapacitySpinBox->setMaximum(200);
        battery1CapacitySpinBox->setSingleStep(10);
        battery1CapacitySpinBox->setValue(54);
        AHLabel_2 = new QLabel(batteriesTab);
        AHLabel_2->setObjectName(QString::fromUtf8("AHLabel_2"));
        AHLabel_2->setGeometry(QRect(320, 120, 31, 17));
        AHLabel_2->setAlignment(Qt::AlignCenter);
        battery2CapacitySpinBox = new QSpinBox(batteriesTab);
        battery2CapacitySpinBox->setObjectName(QString::fromUtf8("battery2CapacitySpinBox"));
        battery2CapacitySpinBox->setGeometry(QRect(270, 115, 52, 25));
        battery2CapacitySpinBox->setMinimum(40);
        battery2CapacitySpinBox->setMaximum(200);
        battery2CapacitySpinBox->setSingleStep(10);
        battery2CapacitySpinBox->setValue(80);
        AHLabel_3 = new QLabel(batteriesTab);
        AHLabel_3->setObjectName(QString::fromUtf8("AHLabel_3"));
        AHLabel_3->setGeometry(QRect(430, 120, 31, 17));
        AHLabel_3->setAlignment(Qt::AlignCenter);
        battery3CapacitySpinBox = new QSpinBox(batteriesTab);
        battery3CapacitySpinBox->setObjectName(QString::fromUtf8("battery3CapacitySpinBox"));
        battery3CapacitySpinBox->setGeometry(QRect(380, 115, 52, 25));
        battery3CapacitySpinBox->setMinimum(40);
        battery3CapacitySpinBox->setMaximum(200);
        battery3CapacitySpinBox->setSingleStep(10);
        battery3CapacitySpinBox->setValue(54);
        setBatteryButton = new QPushButton(batteriesTab);
        setBatteryButton->setObjectName(QString::fromUtf8("setBatteryButton"));
        setBatteryButton->setGeometry(QRect(30, 110, 91, 27));
        resistanceLabel = new QLabel(batteriesTab);
        resistanceLabel->setObjectName(QString::fromUtf8("resistanceLabel"));
        resistanceLabel->setGeometry(QRect(470, 76, 71, 17));
        capacityLabel = new QLabel(batteriesTab);
        capacityLabel->setObjectName(QString::fromUtf8("capacityLabel"));
        capacityLabel->setGeometry(QRect(470, 117, 71, 17));
        battery1TypeCombo = new QComboBox(batteriesTab);
        battery1TypeCombo->setObjectName(QString::fromUtf8("battery1TypeCombo"));
        battery1TypeCombo->setGeometry(QRect(150, 152, 87, 29));
        battery2TypeCombo = new QComboBox(batteriesTab);
        battery2TypeCombo->setObjectName(QString::fromUtf8("battery2TypeCombo"));
        battery2TypeCombo->setGeometry(QRect(260, 153, 87, 29));
        battery3TypeCombo = new QComboBox(batteriesTab);
        battery3TypeCombo->setObjectName(QString::fromUtf8("battery3TypeCombo"));
        battery3TypeCombo->setGeometry(QRect(370, 152, 87, 29));
        typeLabel = new QLabel(batteriesTab);
        typeLabel->setObjectName(QString::fromUtf8("typeLabel"));
        typeLabel->setGeometry(QRect(470, 157, 71, 17));
        batteriesLabel = new QLabel(batteriesTab);
        batteriesLabel->setObjectName(QString::fromUtf8("batteriesLabel"));
        batteriesLabel->setGeometry(QRect(88, 5, 431, 41));
        batteriesLabel->setFont(font1);
        batteriesLabel->setAlignment(Qt::AlignCenter);
        batteriesLabel->setWordWrap(true);
        battery1AbsorptionVoltage = new QLabel(batteriesTab);
        battery1AbsorptionVoltage->setObjectName(QString::fromUtf8("battery1AbsorptionVoltage"));
        battery1AbsorptionVoltage->setGeometry(QRect(154, 195, 79, 31));
        battery1AbsorptionVoltage->setFrameShape(QFrame::Box);
        battery2AbsorptionVoltage = new QLabel(batteriesTab);
        battery2AbsorptionVoltage->setObjectName(QString::fromUtf8("battery2AbsorptionVoltage"));
        battery2AbsorptionVoltage->setGeometry(QRect(264, 195, 79, 31));
        battery2AbsorptionVoltage->setFrameShape(QFrame::Box);
        battery3AbsorptionVoltage = new QLabel(batteriesTab);
        battery3AbsorptionVoltage->setObjectName(QString::fromUtf8("battery3AbsorptionVoltage"));
        battery3AbsorptionVoltage->setGeometry(QRect(374, 195, 79, 31));
        battery3AbsorptionVoltage->setFrameShape(QFrame::Box);
        battery1FloatVoltage = new QLabel(batteriesTab);
        battery1FloatVoltage->setObjectName(QString::fromUtf8("battery1FloatVoltage"));
        battery1FloatVoltage->setGeometry(QRect(154, 275, 79, 31));
        battery1FloatVoltage->setFrameShape(QFrame::Box);
        battery2FloatVoltage = new QLabel(batteriesTab);
        battery2FloatVoltage->setObjectName(QString::fromUtf8("battery2FloatVoltage"));
        battery2FloatVoltage->setGeometry(QRect(264, 275, 79, 31));
        battery2FloatVoltage->setFrameShape(QFrame::Box);
        battery3FloatVoltage = new QLabel(batteriesTab);
        battery3FloatVoltage->setObjectName(QString::fromUtf8("battery3FloatVoltage"));
        battery3FloatVoltage->setGeometry(QRect(374, 275, 79, 31));
        battery3FloatVoltage->setFrameShape(QFrame::Box);
        battery3FloatCurrent = new QLabel(batteriesTab);
        battery3FloatCurrent->setObjectName(QString::fromUtf8("battery3FloatCurrent"));
        battery3FloatCurrent->setGeometry(QRect(374, 315, 79, 31));
        battery3FloatCurrent->setFrameShape(QFrame::Box);
        battery2FloatCurrent = new QLabel(batteriesTab);
        battery2FloatCurrent->setObjectName(QString::fromUtf8("battery2FloatCurrent"));
        battery2FloatCurrent->setGeometry(QRect(264, 315, 79, 31));
        battery2FloatCurrent->setFrameShape(QFrame::Box);
        battery1FloatCurrent = new QLabel(batteriesTab);
        battery1FloatCurrent->setObjectName(QString::fromUtf8("battery1FloatCurrent"));
        battery1FloatCurrent->setGeometry(QRect(154, 315, 79, 31));
        battery1FloatCurrent->setFrameShape(QFrame::Box);
        absorptionVoltageLabel = new QLabel(batteriesTab);
        absorptionVoltageLabel->setObjectName(QString::fromUtf8("absorptionVoltageLabel"));
        absorptionVoltageLabel->setGeometry(QRect(472, 200, 135, 17));
        floatVoltageLabel = new QLabel(batteriesTab);
        floatVoltageLabel->setObjectName(QString::fromUtf8("floatVoltageLabel"));
        floatVoltageLabel->setGeometry(QRect(476, 280, 95, 17));
        floatCurrentLabel = new QLabel(batteriesTab);
        floatCurrentLabel->setObjectName(QString::fromUtf8("floatCurrentLabel"));
        floatCurrentLabel->setGeometry(QRect(476, 320, 97, 17));
        battery2AbsorptionCurrent = new QLabel(batteriesTab);
        battery2AbsorptionCurrent->setObjectName(QString::fromUtf8("battery2AbsorptionCurrent"));
        battery2AbsorptionCurrent->setGeometry(QRect(264, 235, 79, 31));
        battery2AbsorptionCurrent->setFrameShape(QFrame::Box);
        battery3AbsorptionCurrent = new QLabel(batteriesTab);
        battery3AbsorptionCurrent->setObjectName(QString::fromUtf8("battery3AbsorptionCurrent"));
        battery3AbsorptionCurrent->setGeometry(QRect(374, 235, 79, 31));
        battery3AbsorptionCurrent->setFrameShape(QFrame::Box);
        battery1AbsorptionCurrent = new QLabel(batteriesTab);
        battery1AbsorptionCurrent->setObjectName(QString::fromUtf8("battery1AbsorptionCurrent"));
        battery1AbsorptionCurrent->setGeometry(QRect(154, 235, 79, 31));
        battery1AbsorptionCurrent->setFrameShape(QFrame::Box);
        absorptionCurrentLabel = new QLabel(batteriesTab);
        absorptionCurrentLabel->setObjectName(QString::fromUtf8("absorptionCurrentLabel"));
        absorptionCurrentLabel->setGeometry(QRect(472, 240, 135, 17));
        tabWidget->addTab(batteriesTab, QString());
        calibrationTab = new QWidget();
        calibrationTab->setObjectName(QString::fromUtf8("calibrationTab"));
        calibrateButton = new QPushButton(calibrationTab);
        calibrateButton->setObjectName(QString::fromUtf8("calibrateButton"));
        calibrateButton->setGeometry(QRect(260, 180, 91, 27));
        socLabel = new QLabel(calibrationTab);
        socLabel->setObjectName(QString::fromUtf8("socLabel"));
        socLabel->setGeometry(QRect(90, 40, 461, 91));
        socLabel->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);
        socLabel->setWordWrap(true);
        calibrationLabel = new QLabel(calibrationTab);
        calibrationLabel->setObjectName(QString::fromUtf8("calibrationLabel"));
        calibrationLabel->setGeometry(QRect(96, 5, 431, 41));
        calibrationLabel->setFont(font1);
        calibrationLabel->setAlignment(Qt::AlignCenter);
        calibrationLabel->setWordWrap(true);
        qcLabel = new QLabel(calibrationTab);
        qcLabel->setObjectName(QString::fromUtf8("qcLabel"));
        qcLabel->setGeometry(QRect(240, 240, 131, 17));
        quiescentCurrent = new QLabel(calibrationTab);
        quiescentCurrent->setObjectName(QString::fromUtf8("quiescentCurrent"));
        quiescentCurrent->setGeometry(QRect(250, 260, 111, 31));
        QFont font2;
        font2.setPointSize(17);
        quiescentCurrent->setFont(font2);
        quiescentCurrent->setFrameShape(QFrame::Box);
        calibrateProgressBar = new QProgressBar(calibrationTab);
        calibrateProgressBar->setObjectName(QString::fromUtf8("calibrateProgressBar"));
        calibrateProgressBar->setEnabled(false);
        calibrateProgressBar->setGeometry(QRect(250, 210, 118, 23));
        calibrateProgressBar->setMaximum(7);
        calibrateProgressBar->setValue(-1);
        tabWidget->addTab(calibrationTab, QString());
        socTab = new QWidget();
        socTab->setObjectName(QString::fromUtf8("socTab"));
        loadChargeCheckBox = new QCheckBox(socTab);
        loadChargeCheckBox->setObjectName(QString::fromUtf8("loadChargeCheckBox"));
        loadChargeCheckBox->setGeometry(QRect(68, 30, 273, 22));
        QFont font3;
        font3.setBold(true);
        font3.setWeight(75);
        loadChargeCheckBox->setFont(font3);
        loadChargeCheckBox->setChecked(true);
        loadChargeText = new QLabel(socTab);
        loadChargeText->setObjectName(QString::fromUtf8("loadChargeText"));
        loadChargeText->setGeometry(QRect(86, 55, 487, 101));
        loadChargeText->setAutoFillBackground(false);
        loadChargeText->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        loadChargeText->setAlignment(Qt::AlignJustify|Qt::AlignTop);
        loadChargeText->setWordWrap(true);
        isolationMaintainCheckBox = new QCheckBox(socTab);
        isolationMaintainCheckBox->setObjectName(QString::fromUtf8("isolationMaintainCheckBox"));
        isolationMaintainCheckBox->setGeometry(QRect(64, 160, 295, 22));
        isolationMaintainCheckBox->setFont(font3);
        isolationMaintainCheckBox->setChecked(true);
        isolationMaintainText = new QLabel(socTab);
        isolationMaintainText->setObjectName(QString::fromUtf8("isolationMaintainText"));
        isolationMaintainText->setGeometry(QRect(86, 185, 487, 66));
        isolationMaintainText->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        isolationMaintainText->setAlignment(Qt::AlignJustify|Qt::AlignTop);
        isolationMaintainText->setWordWrap(true);
        accelChargeCheckBox = new QCheckBox(socTab);
        accelChargeCheckBox->setObjectName(QString::fromUtf8("accelChargeCheckBox"));
        accelChargeCheckBox->setGeometry(QRect(60, 260, 241, 22));
        accelChargeCheckBox->setFont(font3);
        accelChargeCheckBox->setChecked(true);
        accelChargeText = new QLabel(socTab);
        accelChargeText->setObjectName(QString::fromUtf8("accelChargeText"));
        accelChargeText->setGeometry(QRect(82, 285, 487, 56));
        accelChargeText->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        accelChargeText->setAlignment(Qt::AlignJustify|Qt::AlignTop);
        accelChargeText->setWordWrap(true);
        setOptionButton = new QPushButton(socTab);
        setOptionButton->setObjectName(QString::fromUtf8("setOptionButton"));
        setOptionButton->setGeometry(QRect(264, 380, 91, 27));
        tabWidget->addTab(socTab, QString());
        errorLabel = new QLabel(PowerManagementConfigDialog);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setGeometry(QRect(20, 530, 511, 20));
        closeButton = new QPushButton(PowerManagementConfigDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(562, 532, 91, 27));

        retranslateUi(PowerManagementConfigDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PowerManagementConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *PowerManagementConfigDialog)
    {
        PowerManagementConfigDialog->setWindowTitle(QApplication::translate("PowerManagementConfigDialog", "Solar Power BMS Configuration", 0, QApplication::UnicodeUTF8));
        title->setText(QApplication::translate("PowerManagementConfigDialog", "Solar Power Configuration", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        timeSetButton->setToolTip(QApplication::translate("PowerManagementConfigDialog", "Set the time/date in the remote system from the local time.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        timeSetButton->setText(QApplication::translate("PowerManagementConfigDialog", "Set Time", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        date->setToolTip(QApplication::translate("PowerManagementConfigDialog", "Time/Date in the remote system.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        date->setText(QString());
#ifndef QT_NO_TOOLTIP
        time->setToolTip(QApplication::translate("PowerManagementConfigDialog", "Time/Date in the remote system.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        time->setText(QString());
        dateTimeLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Date and time reported by the remote system", 0, QApplication::UnicodeUTF8));
        generalLabel->setText(QApplication::translate("PowerManagementConfigDialog", "General Configuration", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        debugMessageCheckbox->setToolTip(QApplication::translate("PowerManagementConfigDialog", "Enable Debug Messages to be sent from remote", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        debugMessageCheckbox->setText(QApplication::translate("PowerManagementConfigDialog", "Enable Debug Messages", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        dataMessageCheckbox->setToolTip(QApplication::translate("PowerManagementConfigDialog", "Enable Data Messages. Disabling will stop the main display from showing real-time data and will stoip local recording.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        dataMessageCheckbox->setText(QApplication::translate("PowerManagementConfigDialog", "Enable Data Messages", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        echoTestButton->setToolTip(QApplication::translate("PowerManagementConfigDialog", "Request an ID to be sent from the remote.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        echoTestButton->setText(QApplication::translate("PowerManagementConfigDialog", "Echo Test", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(generalTab), QApplication::translate("PowerManagementConfigDialog", "General", 0, QApplication::UnicodeUTF8));
        queryBatteryButton->setText(QApplication::translate("PowerManagementConfigDialog", "Query", 0, QApplication::UnicodeUTF8));
        battery1Label->setText(QApplication::translate("PowerManagementConfigDialog", "Battery 1", 0, QApplication::UnicodeUTF8));
        battery2Label->setText(QApplication::translate("PowerManagementConfigDialog", "Battery 2", 0, QApplication::UnicodeUTF8));
        battery3Label->setText(QApplication::translate("PowerManagementConfigDialog", "Battery 3", 0, QApplication::UnicodeUTF8));
        battery1Resistance->setText(QString());
        battery3Resistance->setText(QString());
        battery2Resistance->setText(QString());
        AHLabel_1->setText(QApplication::translate("PowerManagementConfigDialog", "AH", 0, QApplication::UnicodeUTF8));
        AHLabel_2->setText(QApplication::translate("PowerManagementConfigDialog", "AH", 0, QApplication::UnicodeUTF8));
        AHLabel_3->setText(QApplication::translate("PowerManagementConfigDialog", "AH", 0, QApplication::UnicodeUTF8));
        setBatteryButton->setText(QApplication::translate("PowerManagementConfigDialog", "Set", 0, QApplication::UnicodeUTF8));
        resistanceLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Resistance", 0, QApplication::UnicodeUTF8));
        capacityLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Capacity", 0, QApplication::UnicodeUTF8));
        typeLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Type", 0, QApplication::UnicodeUTF8));
        batteriesLabel->setText(QApplication::translate("PowerManagementConfigDialog", "View and set Battery model parameters.", 0, QApplication::UnicodeUTF8));
        battery1AbsorptionVoltage->setText(QString());
        battery2AbsorptionVoltage->setText(QString());
        battery3AbsorptionVoltage->setText(QString());
        battery1FloatVoltage->setText(QString());
        battery2FloatVoltage->setText(QString());
        battery3FloatVoltage->setText(QString());
        battery3FloatCurrent->setText(QString());
        battery2FloatCurrent->setText(QString());
        battery1FloatCurrent->setText(QString());
        absorptionVoltageLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Absorption Voltage", 0, QApplication::UnicodeUTF8));
        floatVoltageLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Float Voltage", 0, QApplication::UnicodeUTF8));
        floatCurrentLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Float Current", 0, QApplication::UnicodeUTF8));
        battery2AbsorptionCurrent->setText(QString());
        battery3AbsorptionCurrent->setText(QString());
        battery1AbsorptionCurrent->setText(QString());
        absorptionCurrentLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Absorption Current", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(batteriesTab), QApplication::translate("PowerManagementConfigDialog", "Batteries", 0, QApplication::UnicodeUTF8));
        calibrateButton->setText(QApplication::translate("PowerManagementConfigDialog", "Calibrate", 0, QApplication::UnicodeUTF8));
        socLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Calibration must be done with all loads physically disconnected and the solar panel connected, but not linked to a battery, over a period of about an hour. This ensures that only quiescent current is flowing and the panel interface has power. Requires 35 seconds to complete.", 0, QApplication::UnicodeUTF8));
        calibrationLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Zero Calibration of  Current Measurements", 0, QApplication::UnicodeUTF8));
        qcLabel->setText(QApplication::translate("PowerManagementConfigDialog", "Quiescent Current", 0, QApplication::UnicodeUTF8));
        quiescentCurrent->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(calibrationTab), QApplication::translate("PowerManagementConfigDialog", "Calibration", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        loadChargeCheckBox->setToolTip(QApplication::translate("PowerManagementConfigDialog", "This allows a load to be connected to the battery under charge.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        loadChargeCheckBox->setText(QApplication::translate("PowerManagementConfigDialog", "Avoid Load on Charging Battery", 0, QApplication::UnicodeUTF8));
        loadChargeText->setText(QApplication::translate("PowerManagementConfigDialog", "Normally a load is not connected to the battery under charge as this option will result in charging current being leaked to the other batteries. This may not be a problem for those batteries, but will mean that the battery under charge receives less current than would be desirable. An advantage is that during charging, the load will receive a higher voltage when batteries are low.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        isolationMaintainCheckBox->setToolTip(QApplication::translate("PowerManagementConfigDialog", "Accelerate return to voltage limit when charging voltage is excessive.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        isolationMaintainCheckBox->setText(QApplication::translate("PowerManagementConfigDialog", "Maintain Batteries under Isolation", 0, QApplication::UnicodeUTF8));
        isolationMaintainText->setText(QApplication::translate("PowerManagementConfigDialog", "When all batteries are in normal charge state this option will cause the charger and load to be allocated so that at least one battery can be kept in isolation for resetting its charge state measurement. This can result in lower charging efficiency or load voltage.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        accelChargeCheckBox->setToolTip(QApplication::translate("PowerManagementConfigDialog", "Accelerate return to voltage limit when charging voltage is excessive.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        accelChargeCheckBox->setText(QApplication::translate("PowerManagementConfigDialog", "Use Charge Acceleration", 0, QApplication::UnicodeUTF8));
        accelChargeText->setText(QApplication::translate("PowerManagementConfigDialog", "When the battery terminal voltage exceeds the absorption or float limit, the current is reduced at a rate proportional to the voltage difference, thus pulling the voltage back faster as the difference increases.", 0, QApplication::UnicodeUTF8));
        setOptionButton->setText(QApplication::translate("PowerManagementConfigDialog", "Set", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(socTab), QApplication::translate("PowerManagementConfigDialog", "SoC Tracking", 0, QApplication::UnicodeUTF8));
        errorLabel->setText(QString());
        closeButton->setText(QApplication::translate("PowerManagementConfigDialog", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PowerManagementConfigDialog: public Ui_PowerManagementConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POWER_2D_MANAGEMENT_2D_CONFIGURE_H
