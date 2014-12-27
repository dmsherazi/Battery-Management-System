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
/* Ask for control settings */
    socket->write("dS\n\r");
/* Ask for charge parameter settings */
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

Resistance, type and capacity of the batteries are returned.
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
    QString typeSet1 = "pT1";
    typeSet1.append(QString("%1").arg(PowerManagementConfigUi.battery1TypeCombo
                ->currentIndex(),1));
    typeSet1.append(QString("%1").arg(PowerManagementConfigUi.battery1CapacitySpinBox
                ->value(),-0));
    socket->write(typeSet1.append("\n\r").toAscii().constData());
    QString typeSet2 = "pT2";
    typeSet2.append(QString("%1").arg(PowerManagementConfigUi.battery2TypeCombo
                ->currentIndex(),1));
    typeSet2.append(QString("%1").arg(PowerManagementConfigUi.battery2CapacitySpinBox
                ->value(),-0));
    socket->write(typeSet2.append("\n\r").toAscii().constData());
    QString typeSet3 = "pT3";
    typeSet3.append(QString("%1").arg(PowerManagementConfigUi.battery3TypeCombo
                ->currentIndex(),1));
    typeSet3.append(QString("%1").arg(PowerManagementConfigUi.battery3CapacitySpinBox
                ->value(),-0));
    socket->write(typeSet3.append("\n\r").toAscii().constData());
/* Write to FLASH */
    socket->write("aW\n\r");
/* Refresh display of set parameters */
    on_queryBatteryButton_clicked();
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
    socket->write(command.append(QString("%1").arg(option,2)).append("\n\r")
                         .toAscii().constData());
}

//-----------------------------------------------------------------------------
/** @brief Set Charge Algorithm Options

*/

void PowerManagementConfigGui::on_setChargeOptionButton_clicked()
{
    int restTime = PowerManagementConfigUi.restTimerSpinBox->value();
    int absorptionTime = PowerManagementConfigUi.absorptionTimeSpinBox->value();
    int dutyCycleMin = PowerManagementConfigUi.minimumDutyCycleSpinBox->value();
    int floatTime = PowerManagementConfigUi.floatDelaySpinBox->value();
    int floatSoC = PowerManagementConfigUi.floatBulkSoCSpinBox->value();
}

//-----------------------------------------------------------------------------
/** @brief Set Absorption Phase Muted

This stops the charge algorithm from entering absorption phase for the purpose
of reducing EMI.
*/

void PowerManagementConfigGui::on_absorptionMuteCheckbox_clicked()
{
    if (PowerManagementConfigUi.absorptionMuteCheckbox->isChecked())
    {
    }
    else
    {
    }
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
            QString absorptionVoltage = QString("%1 V").arg(breakdown[2]
                                         .simplified().toFloat()/256,0,'f',2);
            float bulkCurrentScale = breakdown[1].simplified().toFloat();
            if (battery == '1')
            {
                PowerManagementConfigUi.battery1AbsorptionVoltage
                    ->setText(absorptionVoltage);
                float battery1Capacity = PowerManagementConfigUi.
                            battery1CapacitySpinBox->value();
                PowerManagementConfigUi.battery1AbsorptionCurrent
                    ->setText(QString("%1 A")
                        .arg(battery1Capacity/bulkCurrentScale,0,'f',2));
            }
            else if (battery == '2')
            {
                PowerManagementConfigUi.battery2AbsorptionVoltage
                    ->setText(absorptionVoltage);
                float battery2Capacity = PowerManagementConfigUi.
                            battery2CapacitySpinBox->value();
                PowerManagementConfigUi.battery2AbsorptionCurrent
                    ->setText(QString("%1 A")
                        .arg(battery2Capacity/bulkCurrentScale,0,'f',2));
            }
            else if (battery == '3')
            {
                PowerManagementConfigUi.battery3AbsorptionVoltage
                    ->setText(absorptionVoltage);
                float battery3Capacity = PowerManagementConfigUi.
                            battery3CapacitySpinBox->value();
                PowerManagementConfigUi.battery3AbsorptionCurrent
                    ->setText(QString("%1 A")
                        .arg(battery3Capacity/bulkCurrentScale,0,'f',2));
            }
            break;
        }
// Show battery float phase voltage and current limits
        case 'F':
        {
            if (size < 3) break;
            QString floatVoltage = QString("%1 V").arg(breakdown[2]
                                         .simplified().toFloat()/256,0,'f',2);
            float floatCurrentScale = breakdown[1].simplified().toFloat();
            if (battery == '1')
            {
                PowerManagementConfigUi.battery1FloatVoltage
                    ->setText(floatVoltage);
                float battery1Capacity = PowerManagementConfigUi.
                            battery1CapacitySpinBox->value();
                PowerManagementConfigUi.battery1FloatCurrent
                    ->setText(QString("%1 A")
                        .arg(battery1Capacity/floatCurrentScale,0,'f',2));
            }
            else if (battery == '2')
            {
                PowerManagementConfigUi.battery2FloatVoltage
                    ->setText(floatVoltage);
                float battery2Capacity = PowerManagementConfigUi.
                            battery2CapacitySpinBox->value();
                PowerManagementConfigUi.battery2FloatCurrent
                    ->setText(QString("%1 A")
                        .arg(battery2Capacity/floatCurrentScale,0,'f',2));
            }
            else if (battery == '3')
            {
                PowerManagementConfigUi.battery3FloatVoltage
                    ->setText(floatVoltage);
                float battery3Capacity = PowerManagementConfigUi.
                            battery3CapacitySpinBox->value();
                PowerManagementConfigUi.battery3FloatCurrent
                    ->setText(QString("%1 A")
                        .arg(battery3Capacity/floatCurrentScale,0,'f',2));
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
            bool avoidLoad = (((controlByte >> 7) & 0x01) > 0);
            if (avoidLoad)
                PowerManagementConfigUi.loadChargeCheckBox->setChecked(true);
            else
                PowerManagementConfigUi.loadChargeCheckBox->setChecked(false);
            bool maintainIsolate = (((controlByte >> 8) & 0x01) > 0);
            if (maintainIsolate)
                PowerManagementConfigUi.isolationMaintainCheckBox->setChecked(true);
            else
                PowerManagementConfigUi.isolationMaintainCheckBox->setChecked(false);
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

