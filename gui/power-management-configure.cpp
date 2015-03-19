/*       Power Management Configure Window

All configuration items are dealt with here under a set of tabs that handle
algorithmic configurations, calibration and general operational configuration.
*/
/****************************************************************************
 *   Copyright (C) 2013 by Ken Sarkies                                      *
 *   ksarkies@internode.on.net                                              *
 *                                                                          *
 *   This file is part of power-management                                  *
 *                                                                          *
 *   power-management is free software; you can redistribute it and/or      *
 *   modify it under the terms of the GNU General Public License as         *
 *   published by the Free Software Foundation; either version 2 of the     *
 *   License, or (at your option) any later version.                        *
 *                                                                          *
 *   power-management is distributed in the hope that it will be useful,    *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with serial-response-tool if not, write to the                   *
 *   Free Software Foundation, Inc.,                                        *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.              *
 ***************************************************************************/

#include "power-management-main.h"
#include "power-management-configure.h"
#include "serialport.h"
#include <QApplication>
#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QtNetwork>
#include <QTcpSocket>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

//-----------------------------------------------------------------------------
/** Power Management Configuration Window Constructor

@param[in] socket TCP Socket object pointer
@param[in] parent Parent widget.
*/

#ifdef SERIAL
PowerManagementConfigGui::PowerManagementConfigGui(SerialPort* p, QWidget* parent)
                                                    : QDialog(parent)
{
    socket = p;
#else
PowerManagementConfigGui::PowerManagementConfigGui(QTcpSocket* tcpSocket, QWidget* parent)
                                                    : QDialog(parent)
{
    socket = tcpSocket;
#endif
// Build the User Interface display from the Ui class in ui_mainwindowform.h
    PowerManagementConfigUi.setupUi(this);
    PowerManagementConfigUi.battery1AbsorptionCurrent->setDecimals(2);
    PowerManagementConfigUi.battery2AbsorptionCurrent->setDecimals(2);
    PowerManagementConfigUi.battery3AbsorptionCurrent->setDecimals(2);
    PowerManagementConfigUi.battery1AbsorptionVoltage->setDecimals(2);
    PowerManagementConfigUi.battery2AbsorptionVoltage->setDecimals(2);
    PowerManagementConfigUi.battery3AbsorptionVoltage->setDecimals(2);
    PowerManagementConfigUi.battery1FloatVoltage->setDecimals(2);
    PowerManagementConfigUi.battery2FloatVoltage->setDecimals(2);
    PowerManagementConfigUi.battery3FloatVoltage->setDecimals(2);
    PowerManagementConfigUi.battery1FloatCurrent->setDecimals(2);
    PowerManagementConfigUi.battery2FloatCurrent->setDecimals(2);
    PowerManagementConfigUi.battery3FloatCurrent->setDecimals(2);
// Set the calibrate progress bar to invisible
    PowerManagementConfigUi.calibrateProgressBar->setVisible(false);
    PowerManagementConfigUi.calibrateProgressBar->setValue(0);
// Set the battery type combobox entries and default value
    PowerManagementConfigUi.battery1TypeCombo->addItem("Wet Cell");
    PowerManagementConfigUi.battery1TypeCombo->addItem("Gel Cell");
    PowerManagementConfigUi.battery1TypeCombo->addItem("AGM Cell");
    PowerManagementConfigUi.battery1TypeCombo->setCurrentIndex(1);
    PowerManagementConfigUi.battery2TypeCombo->addItem("Wet Cell");
    PowerManagementConfigUi.battery2TypeCombo->addItem("Gel Cell");
    PowerManagementConfigUi.battery2TypeCombo->addItem("AGM Cell");
    PowerManagementConfigUi.battery2TypeCombo->setCurrentIndex(1);
    PowerManagementConfigUi.battery3TypeCombo->addItem("Wet Cell");
    PowerManagementConfigUi.battery3TypeCombo->addItem("Gel Cell");
    PowerManagementConfigUi.battery3TypeCombo->addItem("AGM Cell");
    PowerManagementConfigUi.battery3TypeCombo->setCurrentIndex(1);
// Set the resistance display default (with Unicode Omega)
    PowerManagementConfigUi.battery1Resistance
            ->setText(QString("0 m").append(QChar(0x03A9)));
    PowerManagementConfigUi.battery2Resistance
            ->setText(QString("0 m").append(QChar(0x03A9)));
    PowerManagementConfigUi.battery3Resistance
            ->setText(QString("0 m").append(QChar(0x03A9)));
/* Ask for battery parameters to fill display */
    on_queryBatteryButton_clicked();
/* Ask for switch control settings */
    socket->write("dS\n\r");
/* Ask for monitor strategy parameter settings */
    socket->write("dT\n\r");
/* Ask for charger strategy parameter settings */
    socket->write("dC\n\r");
}

PowerManagementConfigGui::~PowerManagementConfigGui()
{
}

//-----------------------------------------------------------------------------
/** @brief Initiate Calibration

This causes the end device to initiate the calibration sequence. The window
cannot be closed until a result is received or a timeout period is reached.
*/

void PowerManagementConfigGui::on_calibrateButton_clicked()
{
    PowerManagementConfigUi.calibrateProgressBar->setVisible(true);
    this->setEnabled(false);
    QApplication::processEvents();
    socket->write("pC\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Query Battery Parameters

Resistance, type and capacity and parameters of the batteries are returned.
*/

void PowerManagementConfigGui::on_queryBatteryButton_clicked()
{
    socket->write("dB1\n\r");
    socket->write("dB2\n\r");
    socket->write("dB3\n\r");
}
//-----------------------------------------------------------------------------
/** @brief Set Battery Parameters

Resistance, type and capacity of the batteries are set from the GUI values.
*/

void PowerManagementConfigGui::on_setBatteryButton_clicked()
{
    int battery1Capacity = PowerManagementConfigUi.battery1CapacitySpinBox
                ->value();
    int battery2Capacity = PowerManagementConfigUi.battery2CapacitySpinBox
                ->value();
    int battery3Capacity = PowerManagementConfigUi.battery3CapacitySpinBox
                ->value();
// Set type and capacity. Capacity is an integer unscaled.
    QString typeSet1 = "pT1";
    typeSet1.append(QString("%1").arg(PowerManagementConfigUi.battery1TypeCombo
                ->currentIndex(),1));
    typeSet1.append(QString("%1").arg(battery1Capacity,-0));
    socket->write(typeSet1.append("\n\r").toAscii().constData());
    QString typeSet2 = "pT2";
    typeSet2.append(QString("%1").arg(PowerManagementConfigUi.battery2TypeCombo
                ->currentIndex(),1));
    typeSet2.append(QString("%1").arg(battery2Capacity,-0));
    socket->write(typeSet2.append("\n\r").toAscii().constData());
    QString typeSet3 = "pT3";
    typeSet3.append(QString("%1").arg(PowerManagementConfigUi.battery3TypeCombo
                ->currentIndex(),1));
    typeSet3.append(QString("%1").arg(battery3Capacity,-0));
    socket->write(typeSet3.append("\n\r").toAscii().constData());
/* Set bulk current limit scales. These are the scaling factors relating the
battery capacity to the bulk current limit. */
    QString bulkISet1 = "pI1";
    bulkISet1.append(QString("%1")
                .arg((unsigned int)(battery1Capacity/
                 PowerManagementConfigUi.battery1AbsorptionCurrent->value()),-0));
    socket->write(bulkISet1.append("\n\r").toAscii().constData());
    QString bulkISet2 = "pI2";
    bulkISet2.append(QString("%1")
                .arg((unsigned int)(battery2Capacity/
                 PowerManagementConfigUi.battery2AbsorptionCurrent->value()),-0));
    socket->write(bulkISet2.append("\n\r").toAscii().constData());
    QString bulkISet3 = "pI3";
    bulkISet3.append(QString("%1")
                .arg((unsigned int)(battery3Capacity/
                 PowerManagementConfigUi.battery3AbsorptionCurrent->value()),-0));
    socket->write(bulkISet3.append("\n\r").toAscii().constData());
// Set gassing voltage limits
    QString gassingVSet1 = "pA1";
    gassingVSet1.append(QString("%1")
                .arg((unsigned int)(PowerManagementConfigUi.battery1AbsorptionVoltage
                ->value()*256),-0));
    socket->write(gassingVSet1.append("\n\r").toAscii().constData());
    QString gassingVSet2 = "pA2";
    gassingVSet2.append(QString("%1")
                .arg((unsigned int)(PowerManagementConfigUi.battery2AbsorptionVoltage
                ->value()*256),-0));
    socket->write(gassingVSet2.append("\n\r").toAscii().constData());
    QString gassingVSet3 = "pA3";
    gassingVSet3.append(QString("%1")
                .arg((unsigned int)(PowerManagementConfigUi.battery3AbsorptionVoltage
                ->value()*256),-0));
    socket->write(gassingVSet3.append("\n\r").toAscii().constData());
// Set float voltage limits
    QString floatVSet1 = "pF1";
    floatVSet1.append(QString("%1")
                .arg((unsigned int)(PowerManagementConfigUi.battery1FloatVoltage
                ->value()*256),-0));
    socket->write(floatVSet1.append("\n\r").toAscii().constData());
    QString floatVSet2 = "pF2";
    floatVSet2.append(QString("%1")
                .arg((unsigned int)(PowerManagementConfigUi.battery2FloatVoltage
                ->value()*256),-0));
    socket->write(floatVSet2.append("\n\r").toAscii().constData());
    QString floatVSet3 = "pF3";
    floatVSet3.append(QString("%1")
                .arg((unsigned int)(PowerManagementConfigUi.battery3FloatVoltage
                ->value()*256),-0));
    socket->write(floatVSet3.append("\n\r").toAscii().constData());
/* Set float current scales. These are the scaling factors relating the
battery capacity to the float current trigger. */
    QString floatISet1 = "pf1";
    floatISet1.append(QString("%1")
                .arg((unsigned int)(battery1Capacity/
                 PowerManagementConfigUi.battery1FloatCurrent->value()),-0));
    socket->write(floatISet1.append("\n\r").toAscii().constData());
    QString floatISet2 = "pf2";
    floatISet2.append(QString("%1")
                .arg((unsigned int)(battery2Capacity/
                 PowerManagementConfigUi.battery2FloatCurrent->value()),-0));
    socket->write(floatISet2.append("\n\r").toAscii().constData());
    QString floatISet3 = "pf3";
    floatISet3.append(QString("%1")
                .arg((unsigned int)(battery3Capacity/
                 PowerManagementConfigUi.battery3FloatCurrent->value()),-0));
    socket->write(floatISet3.append("\n\r").toAscii().constData());
/* Write to FLASH */
    socket->write("aW\n\r");
/* Refresh display of set parameters */
    on_queryBatteryButton_clicked();
}

//-----------------------------------------------------------------------------
/** @brief Battery 1 type changed

When the type is changed, update the corresponding battery parameters with
defaults.

Here there are two parameters assumed, the float scale which is the fractional
C at which the trigger to float occurs, and the fractional C at which the bulk
charge current is limited.
*/

void PowerManagementConfigGui::on_battery1TypeCombo_activated(int index)
{
    float floatScale = 50;
    float bulkScale = 5;
    int capacity = PowerManagementConfigUi.battery1CapacitySpinBox->value();
    if (index == 0)
    {
        PowerManagementConfigUi.battery1AbsorptionCurrent->
                                setValue((float)capacity/bulkScale);
        PowerManagementConfigUi.battery1AbsorptionVoltage->setValue(14.5);
        PowerManagementConfigUi.battery1FloatVoltage->setValue(13.2);
        PowerManagementConfigUi.battery1FloatCurrent->
                                setValue((float)capacity/floatScale);
    }
    if (index == 1)
    {
        PowerManagementConfigUi.battery1AbsorptionCurrent->
                                setValue((float)capacity/bulkScale);
        PowerManagementConfigUi.battery1AbsorptionVoltage->setValue(14.6);
        PowerManagementConfigUi.battery1FloatVoltage->setValue(13.6);
        PowerManagementConfigUi.battery1FloatCurrent->
                                setValue((float)capacity/floatScale);
    }
    if (index == 2)
    {
        PowerManagementConfigUi.battery1AbsorptionCurrent->
                                setValue((float)capacity/bulkScale);
        PowerManagementConfigUi.battery1AbsorptionVoltage->setValue(14.1);
        PowerManagementConfigUi.battery1FloatVoltage->setValue(13.8);
        PowerManagementConfigUi.battery1FloatCurrent->
                                setValue((float)capacity/floatScale);
    }
}

//-----------------------------------------------------------------------------
/** @brief Battery 1 capacity changed

When the capacity is changed, update the corresponding battery parameters with
defaults.

Here there are two parameters assumed, the float scale which is the fractional
C at which the trigger to float occurs, and the fractional C at which the bulk
charge current is limited.
*/

void PowerManagementConfigGui::on_battery1CapacitySpinBox_valueChanged(int i)
{
// Simply call the type combobox function to reset all parameters.
    int index = PowerManagementConfigUi.battery1TypeCombo->currentIndex();
    on_battery1TypeCombo_activated(index);
}

//-----------------------------------------------------------------------------
/** @brief Battery 2 type changed

When the type is changed, update the corresponding battery parameters with
defaults.
*/

void PowerManagementConfigGui::on_battery2TypeCombo_activated(int index)
{
    float floatScale = 50;
    float bulkScale = 5;
    int capacity = PowerManagementConfigUi.battery2CapacitySpinBox->value();
    if (index == 0)
    {
        PowerManagementConfigUi.battery2AbsorptionCurrent->
                                setValue((float)capacity/bulkScale);
        PowerManagementConfigUi.battery2AbsorptionVoltage->setValue(14.5);
        PowerManagementConfigUi.battery2FloatVoltage->setValue(13.2);
        PowerManagementConfigUi.battery2FloatCurrent->
                                setValue((float)capacity/floatScale);
    }
    if (index == 1)
    {
        PowerManagementConfigUi.battery2AbsorptionCurrent->
                                setValue((float)capacity/bulkScale);
        PowerManagementConfigUi.battery2AbsorptionVoltage->setValue(14.6);
        PowerManagementConfigUi.battery2FloatVoltage->setValue(13.6);
        PowerManagementConfigUi.battery2FloatCurrent->
                                setValue((float)capacity/floatScale);
    }
    if (index == 2)
    {
        PowerManagementConfigUi.battery2AbsorptionCurrent->
                                setValue((float)capacity/bulkScale);
        PowerManagementConfigUi.battery2AbsorptionVoltage->setValue(14.1);
        PowerManagementConfigUi.battery2FloatVoltage->setValue(13.8);
        PowerManagementConfigUi.battery2FloatCurrent->
                                setValue((float)capacity/floatScale);
    }
}

//-----------------------------------------------------------------------------
/** @brief Battery 2 capacity changed

When the capacity is changed, update the corresponding battery parameters with
defaults.

Here there are two parameters assumed, the float scale which is the fractional
C at which the trigger to float occurs, and the fractional C at which the bulk
charge current is limited.
*/

void PowerManagementConfigGui::on_battery2CapacitySpinBox_valueChanged(int i)
{
// Simply call the type combobox function to reset all parameters.
    int index = PowerManagementConfigUi.battery2TypeCombo->currentIndex();
    on_battery2TypeCombo_activated(index);
}

//-----------------------------------------------------------------------------
/** @brief Battery 3 type changed

When the type is changed, update the corresponding battery parameters with
defaults.
*/

void PowerManagementConfigGui::on_battery3TypeCombo_activated(int index)
{
    float floatScale = 50;
    float bulkScale = 5;
    int capacity = PowerManagementConfigUi.battery3CapacitySpinBox->value();
    if (index == 0)
    {
        PowerManagementConfigUi.battery3AbsorptionCurrent->
                                setValue((float)capacity/bulkScale);
        PowerManagementConfigUi.battery3AbsorptionCurrent->setValue(14.5);
        PowerManagementConfigUi.battery3FloatVoltage->setValue(13.2);
        PowerManagementConfigUi.battery3FloatCurrent->
                                setValue((float)capacity/floatScale);
    }
    if (index == 1)
    {
        PowerManagementConfigUi.battery3AbsorptionCurrent->
                                setValue((float)capacity/bulkScale);
        PowerManagementConfigUi.battery3AbsorptionVoltage->setValue(14.6);
        PowerManagementConfigUi.battery3FloatVoltage->setValue(13.6);
        PowerManagementConfigUi.battery3FloatCurrent->
                                setValue((float)capacity/floatScale);
    }
    if (index == 2)
    {
        PowerManagementConfigUi.battery3AbsorptionCurrent->
                                setValue((float)capacity/bulkScale);
        PowerManagementConfigUi.battery3AbsorptionVoltage->setValue(14.1);
        PowerManagementConfigUi.battery3FloatVoltage->setValue(13.8);
        PowerManagementConfigUi.battery3FloatCurrent->
                                setValue((float)capacity/floatScale);
    }
}

//-----------------------------------------------------------------------------
/** @brief Battery 3 capacity changed

When the capacity is changed, update the corresponding battery parameters with
defaults.

Here there are two parameters assumed, the float scale which is the fractional
C at which the trigger to float occurs, and the fractional C at which the bulk
charge current is limited.
*/

void PowerManagementConfigGui::on_battery3CapacitySpinBox_valueChanged(int i)
{
// Simply call the type combobox function to reset all parameters.
    int index = PowerManagementConfigUi.battery3TypeCombo->currentIndex();
    on_battery3TypeCombo_activated(index);
}

//-----------------------------------------------------------------------------
/** @brief Set Tracking Strategy Options

0x01 The option allowing or preventing loads connected to the charging battery.
0x02 The option allowing or preventing a battery being maintained in isolation.
*/

void PowerManagementConfigGui::on_setTrackOptionButton_clicked()
{
    int option = 0;
    if (PowerManagementConfigUi.loadChargeCheckBox->isChecked())
    {
        option |= 0x01;
    }
    else
    {
        option &= ~0x01;
    }
    if (PowerManagementConfigUi.isolationMaintainCheckBox->isChecked())
    {
        option |= 0x02;
    }
    else
    {
        option &= ~0x02;
    }
    QString command = "ps";
    socket->write(command.append(QString("%1").arg(option,1)).append("\n\r")
                         .toAscii().constData());
    command = "pv";
    int lowVoltage = (int)(PowerManagementConfigUi.
                                lowVoltageDoubleSpinBox->value()*256);
    socket->write(command.append(QString("%1").arg(lowVoltage,2)).append("\n\r")
                         .toAscii().constData());
    command = "pV";
    int criticalVoltage = (int)(PowerManagementConfigUi.
                                criticalVoltageDoubleSpinBox->value()*256);
    socket->write(command.append(QString("%1").arg(criticalVoltage,2)).append("\n\r")
                         .toAscii().constData());
    command = "px";
    int lowSoC = (int)(PowerManagementConfigUi.
                                lowSoCSpinBox->value());
    socket->write(command.append(QString("%1").arg(lowSoC,2)).append("\n\r")
                         .toAscii().constData());
    command = "pX";
    int criticalSoC = (int)(PowerManagementConfigUi.
                                criticalSoCSpinBox->value());
    socket->write(command.append(QString("%1").arg(criticalSoC,2)).append("\n\r")
                         .toAscii().constData());
/* Write to FLASH */
    socket->write("aW\n\r");
/* Ask for monitor strategy parameter settings */
    socket->write("dT\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Set Charge Algorithm Options

*/

void PowerManagementConfigGui::on_setChargeOptionButton_clicked()
{
    on_absorptionMuteCheckbox_clicked();
    QString command = "pR";
    int restTime = PowerManagementConfigUi.restTimeSpinBox->value();
    socket->write(command.append(QString("%1").arg(restTime,2)).append("\n\r")
                         .toAscii().constData());
    command = "pG";
    int absorptionTime = PowerManagementConfigUi.absorptionTimeSpinBox->value();
    socket->write(command.append(QString("%1").arg(absorptionTime,2)).append("\n\r")
                         .toAscii().constData());
    command = "pD";
    int dutyCycleMin = PowerManagementConfigUi.minimumDutyCycleSpinBox->value();
    socket->write(command.append(QString("%1").arg(dutyCycleMin,2)).append("\n\r")
                         .toAscii().constData());
    command = "pe";
    int floatTime = PowerManagementConfigUi.floatDelaySpinBox->value();
    socket->write(command.append(QString("%1").arg(floatTime,2)).append("\n\r")
                         .toAscii().constData());
    command = "pB";
    int floatSoC = PowerManagementConfigUi.floatBulkSoCSpinBox->value();
    socket->write(command.append(QString("%1").arg(floatSoC,2)).append("\n\r")
                         .toAscii().constData());
/* Write to FLASH */
    socket->write("aW\n\r");
/* Ask for charger strategy parameter settings */
    socket->write("dC\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Set Absorption Phase Muted

This stops the charge algorithm from entering absorption phase for the purpose
of reducing EMI.
*/

void PowerManagementConfigGui::on_absorptionMuteCheckbox_clicked()
{
    int option = 0;
    if (PowerManagementConfigUi.absorptionMuteCheckbox->isChecked())
    {
        option |= 0x01;
    }
    else
    {
        option &= ~0x01;
    }
    QString command = "pS";
    socket->write(command.append(QString("%1").arg(option,1)).append("\n\r")
                         .toAscii().constData());
}

//-----------------------------------------------------------------------------
/** @brief Set the Time

The current time is read and transmitted to set time in the remote system.
*/

void PowerManagementConfigGui::on_timeSetButton_clicked()
{
    QDateTime localDateTime = QDateTime::currentDateTime();
    localDateTime.setTimeSpec(Qt::UTC);
    socket->write("pH");
    socket->write(localDateTime.toString(Qt::ISODate).append("\n\r")
                               .toAscii().constData());
}

//-----------------------------------------------------------------------------
/** @brief Enable Debug Messages

*/

void PowerManagementConfigGui::on_debugMessageCheckbox_clicked()
{
    if (PowerManagementConfigUi.debugMessageCheckbox->isChecked())
        socket->write("pd+\n\r");
    else
        socket->write("pd-\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Enable Data Messages

*/

void PowerManagementConfigGui::on_dataMessageCheckbox_clicked()
{
    if (PowerManagementConfigUi.dataMessageCheckbox->isChecked())
        socket->write("pM+\n\r");
    else
        socket->write("pM-\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Send Echo Request

*/

void PowerManagementConfigGui::on_echoTestButton_clicked()
{
    socket->write("aE\n\r");
    socket->write("dS\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Reset or Set the Battery 1 Missing Status

*/

void PowerManagementConfigGui::on_resetMissing1Button_clicked()
{
    if (PowerManagementConfigUi.resetMissing1Button->text() == "X")
    {
        PowerManagementConfigUi.resetMissing1Button->setText("");
        socket->write("pm1-\n\r");
    }
    else
    {
        PowerManagementConfigUi.resetMissing1Button->setText("X");
        socket->write("pm1+\n\r");
    }
}

//-----------------------------------------------------------------------------
/** @brief Reset the Battery 2 Missing Status

*/

void PowerManagementConfigGui::on_resetMissing2Button_clicked()
{
    if (PowerManagementConfigUi.resetMissing2Button->text() == "X")
    {
        PowerManagementConfigUi.resetMissing2Button->setText("");
        socket->write("pm2-\n\r");
    }
    else
    {
        PowerManagementConfigUi.resetMissing2Button->setText("X");
        socket->write("pm2+\n\r");
    }
}

//-----------------------------------------------------------------------------
/** @brief Reset the Battery 3 Missing Status

*/

void PowerManagementConfigGui::on_resetMissing3Button_clicked()
{
    if (PowerManagementConfigUi.resetMissing3Button->text() == "X")
    {
        PowerManagementConfigUi.resetMissing3Button->setText("");
        socket->write("pm3-\n\r");
    }
    else
    {
        PowerManagementConfigUi.resetMissing3Button->setText("X");
        socket->write("pm3+\n\r");
    }
}

//-----------------------------------------------------------------------------
/** @brief Close Window

*/

void PowerManagementConfigGui::on_closeButton_clicked()
{
    this->close();
}

//-----------------------------------------------------------------------------
/** @brief Process a Message.

After a command is sent, response messages from the remote are passed here for
processing. Appropriate fields on the form are updated.

@todo This interprets response as done in main. See if this can be made
independent of formats.
*/

void PowerManagementConfigGui::onMessageReceived(const QString &response)
{
    QStringList breakdown = response.split(",");
    int size = breakdown.size();
    if (size <= 0) return;
    QChar command = breakdown[0].at(1);
    QChar battery = breakdown[0].at(2);
    QChar parameter = breakdown[0].at(2);
    int controlByte = 0;
    if (size > 1) controlByte = breakdown[1].simplified().toInt();
// Error Code
    switch (command.toAscii())
    {
// Show Measured Quiescent Current
        case 'Q':
        {
            if (size < 2) break;
            quiescentCurrent = breakdown[1].simplified();
            int test = breakdown[2].simplified().toInt();
            if (test < 6)
                PowerManagementConfigUi.calibrateProgressBar->setValue(test+1);
            else
            {
                PowerManagementConfigUi.quiescentCurrent
                        ->setText(QString("%1 A").arg(quiescentCurrent.
                                  toFloat()/256,0,'f',3));
                this->setEnabled(true);
                PowerManagementConfigUi.calibrateProgressBar->setVisible(false);
                PowerManagementConfigUi.calibrateProgressBar->setValue(0);
            }
            break;
        }
// Show measured battery resistance
        case 'R':
        {
            if (size < 2) break;
            QString batteryResistance = QString("%1 m").arg(breakdown[1]
                                         .simplified().toFloat()/65.536,0,'f',0)
                                         .append(QChar(0x03A9));
            if (battery == '1')
                PowerManagementConfigUi.battery1Resistance
                    ->setText(batteryResistance);
            else if (battery == '2')
                PowerManagementConfigUi.battery2Resistance
                    ->setText(batteryResistance);
            else if (battery == '3')
                PowerManagementConfigUi.battery3Resistance
                    ->setText(batteryResistance);
            break;
        }
// Show battery type and capacity
        case 'T':
        {
            if (size < 3) break;
            int batteryType = breakdown[1].simplified().toInt();
            int batteryCapacity = breakdown[2].simplified().toInt();
            if (battery == '1')
            {
                PowerManagementConfigUi.battery1TypeCombo
                    ->setCurrentIndex(batteryType);
                PowerManagementConfigUi.battery1CapacitySpinBox
                    ->setValue(batteryCapacity);
            }
            else if (battery == '2')
            {
                PowerManagementConfigUi.battery2TypeCombo
                    ->setCurrentIndex(batteryType);
                PowerManagementConfigUi.battery2CapacitySpinBox
                    ->setValue(batteryCapacity);
            }
            else if (battery == '3')
            {
                PowerManagementConfigUi.battery3TypeCombo
                    ->setCurrentIndex(batteryType);
                PowerManagementConfigUi.battery3CapacitySpinBox
                    ->setValue(batteryCapacity);
            }
            break;
        }
// Show battery absorption phase voltage and current limits
        case 'A':
        {
            if (size < 3) break;
            float absorptionVoltage = breakdown[2].simplified().toFloat()/256;
            float bulkCurrentScale = breakdown[1].simplified().toFloat();
            if (battery == '1')
            {
                PowerManagementConfigUi.battery1AbsorptionVoltage
                    ->setValue(absorptionVoltage);
                float battery1Capacity = PowerManagementConfigUi.
                            battery1CapacitySpinBox->value();
                PowerManagementConfigUi.battery1AbsorptionCurrent
                    ->setValue(battery1Capacity/bulkCurrentScale);
            }
            else if (battery == '2')
            {
                PowerManagementConfigUi.battery2AbsorptionVoltage
                    ->setValue(absorptionVoltage);
                float battery2Capacity = PowerManagementConfigUi.
                            battery2CapacitySpinBox->value();
                PowerManagementConfigUi.battery2AbsorptionCurrent
                    ->setValue(battery2Capacity/bulkCurrentScale);
            }
            else if (battery == '3')
            {
                PowerManagementConfigUi.battery3AbsorptionVoltage
                    ->setValue(absorptionVoltage);
                float battery3Capacity = PowerManagementConfigUi.
                            battery3CapacitySpinBox->value();
                PowerManagementConfigUi.battery3AbsorptionCurrent
                    ->setValue(battery3Capacity/bulkCurrentScale);
            }
            break;
        }
// Show battery float phase voltage and current limits
        case 'F':
        {
            if (size < 3) break;
            float floatVoltage = breakdown[2].simplified().toFloat()/256;
            float floatCurrentScale = breakdown[1].simplified().toFloat();
            if (battery == '1')
            {
                PowerManagementConfigUi.battery1FloatVoltage
                    ->setValue(floatVoltage);
                float battery1Capacity = PowerManagementConfigUi.
                            battery1CapacitySpinBox->value();
                PowerManagementConfigUi.battery1FloatCurrent
                    ->setValue(battery1Capacity/floatCurrentScale);
            }
            else if (battery == '2')
            {
                PowerManagementConfigUi.battery2FloatVoltage
                    ->setValue(floatVoltage);
                float battery2Capacity = PowerManagementConfigUi.
                            battery2CapacitySpinBox->value();
                PowerManagementConfigUi.battery2FloatCurrent
                    ->setValue(battery2Capacity/floatCurrentScale);
            }
            else if (battery == '3')
            {
                PowerManagementConfigUi.battery3FloatVoltage
                    ->setValue(floatVoltage);
                float battery3Capacity = PowerManagementConfigUi.
                            battery3CapacitySpinBox->value();
                PowerManagementConfigUi.battery3FloatCurrent
                    ->setValue(battery3Capacity/floatCurrentScale);
            }
            break;
        }
/* Show control settings */
        case 'D':
        {
            if (size < 2) break;
            bool dataMessage = ((controlByte & (1<<3)) > 0);
            if (dataMessage)
                PowerManagementConfigUi.dataMessageCheckbox->setChecked(true);
            else
                PowerManagementConfigUi.dataMessageCheckbox->setChecked(false);
            bool debugMessage = ((controlByte & (1<<4)) > 0);
            if (debugMessage)
                PowerManagementConfigUi.debugMessageCheckbox->setChecked(true);
            else
                PowerManagementConfigUi.debugMessageCheckbox->setChecked(false);
            break;
        }
// Show current time settings from the system
        case 'H':
        {
            if (size < 2) break;
            QDateTime systemTime =
                QDateTime::fromString(breakdown[1].simplified(),Qt::ISODate);
            PowerManagementConfigUi.date->setText(systemTime.date().toString("dd.MM.yyyy"));
            PowerManagementConfigUi.time->setText(systemTime.time().toString("H.mm.ss"));
            break;
        }
// Operational State values for "reset missing" buttons.
        case 'O':
        {
            int healthState = (controlByte >> 6) & 0x03;
            if (battery == '1')
            {
                if (healthState == 0)
                {
                    PowerManagementConfigUi.resetMissing1Button->
                        setStyleSheet("background-color:lightgreen;");
                    PowerManagementConfigUi.resetMissing1Button->setText("");
                }
                else if (healthState == 1)
                {
                    PowerManagementConfigUi.resetMissing1Button->
                        setStyleSheet("background-color:orange;");
                    PowerManagementConfigUi.resetMissing1Button->setText("");
                }
                else if (healthState == 2)
                {
                    PowerManagementConfigUi.resetMissing1Button->
                        setStyleSheet("background-color:white;");
                    PowerManagementConfigUi.resetMissing1Button->setText("X");
                }
            }
            else if (battery == '2')
            {
                if (healthState == 0)
                {
                    PowerManagementConfigUi.resetMissing2Button->
                        setStyleSheet("background-color:lightgreen;");
                    PowerManagementConfigUi.resetMissing2Button->setText("");
                }
                else if (healthState == 1)
                {
                    PowerManagementConfigUi.resetMissing2Button->
                        setStyleSheet("background-color:orange;");
                    PowerManagementConfigUi.resetMissing2Button->setText("");
                }
                else if (healthState == 2)
                {
                    PowerManagementConfigUi.resetMissing2Button->
                        setStyleSheet("background-color:white;");
                    PowerManagementConfigUi.resetMissing2Button->setText("X");
                }
            }
            else if (battery == '3')
            {
                if (healthState == 0)
                {
                    PowerManagementConfigUi.resetMissing3Button->
                        setStyleSheet("background-color:lightgreen;");
                    PowerManagementConfigUi.resetMissing3Button->setText("");
                }
                else if (healthState == 1)
                {
                    PowerManagementConfigUi.resetMissing3Button->
                        setStyleSheet("background-color:orange;");
                    PowerManagementConfigUi.resetMissing3Button->setText("");
                }
                else if (healthState == 2)
                {
                    PowerManagementConfigUi.resetMissing3Button->
                        setStyleSheet("background-color:white;");
                    PowerManagementConfigUi.resetMissing3Button->setText("X");
                }
            }
            break;
        }
// Show tracking parameters
        case 't':
        {
            if (size < 3) break;
// Low voltage and critical voltage thresholds.
            if (parameter == 'V')
            {
                float lowVoltage = (float)breakdown[1].simplified().toInt()/256;
                float criticalVoltage = (float)breakdown[2].simplified().toInt()/256;
                PowerManagementConfigUi.lowVoltageDoubleSpinBox
                    ->setValue(lowVoltage);
                PowerManagementConfigUi.criticalVoltageDoubleSpinBox
                    ->setValue(criticalVoltage);
            }
// Low SoC and critical SoC thresholds.
            else if (parameter == 'S')
            {
                int lowSoC = (float)breakdown[1].simplified().toInt();
                int criticalSoC = (float)breakdown[2].simplified().toInt();
                PowerManagementConfigUi.lowSoCSpinBox
                    ->setValue(lowSoC);
                PowerManagementConfigUi.criticalSoCSpinBox
                    ->setValue(criticalSoC);
            }
/* Monitor strategy byte. Bit 0 is to allow charger and load on the same
battery; bit 1 is to maintain an isolated battery in normal conditions. */
            else if (parameter == 's')
            {
                int monitorStrategy = (float)breakdown[1].simplified().toInt();
                bool separateLoad = (monitorStrategy & 1) > 0;
                PowerManagementConfigUi.loadChargeCheckBox
                    ->setChecked(separateLoad);
                bool preserveIsolation = (monitorStrategy & 2) > 0;
                PowerManagementConfigUi.isolationMaintainCheckBox
                    ->setChecked(preserveIsolation);
            }
// SoC below which float phase is changed to bulk phase.
            else if (parameter == 'F')
            {
                int floatBulkSoC = (float)breakdown[1].simplified().toInt();
                PowerManagementConfigUi.floatBulkSoCSpinBox
                    ->setValue(floatBulkSoC);
            }
            break;
        }
// Show charger parameters
        case 'c':
        {
            if (size < 3) break;
            if (parameter == 'R')
            {
                int restTime = (float)breakdown[1].simplified().toInt();
                int absorptionTime = (float)breakdown[2].simplified().toInt();
                PowerManagementConfigUi.restTimeSpinBox
                    ->setValue(restTime);
                PowerManagementConfigUi.absorptionTimeSpinBox
                    ->setValue(absorptionTime);
            }
            else if (parameter == 'D')
            {
                int dutyCycleMin = (float)breakdown[1].simplified().toInt();
                PowerManagementConfigUi.minimumDutyCycleSpinBox
                    ->setValue(dutyCycleMin);
            }
            else if (parameter == 'F')
            {
                int floatTime = (float)breakdown[1].simplified().toInt();
                int floatSoC = (float)breakdown[2].simplified().toInt();
                PowerManagementConfigUi.floatDelaySpinBox
                    ->setValue(floatTime);
                PowerManagementConfigUi.floatBulkSoCSpinBox
                    ->setValue(floatSoC);
            }
/* Charger strategy byte. Bit 0 is to suppress the absortion phase for EMI. */
            else if (parameter == 's')
            {
                int chargerStrategy = (float)breakdown[1].simplified().toInt();
                bool suppressAbsorptionPhase = (chargerStrategy & 1) > 0;
                PowerManagementConfigUi.absorptionMuteCheckbox
                    ->setChecked(suppressAbsorptionPhase);
            }
            break;
        }
    }
}

//-----------------------------------------------------------------------------
/** @brief Error Message

@returns a message when the device didn't respond.
*/

QString PowerManagementConfigGui::error()
{
    return errorMessage;
}

//-----------------------------------------------------------------------------
/** @brief Show an error condition in the Error label.

@param[in] message Message to be displayed.
*/

void PowerManagementConfigGui::displayErrorMessage(const QString message)
{
    PowerManagementConfigUi.errorLabel->setText(message);
}

