/*       Power Management Main Window

This is a cutdown version of the Battery Management System for use with the
BeagleBone touch screen LCD monitoring unit.

Here the data stream from the microcontroller is received and saved to a file.
SoC values are displayed directly. Controls can be set to change the switching
between batteries, loads and panel. When a change is made the new values are
transmitted to the microcontroller. Other windows are called up for more
detailed monitoring and for configuration.

@date 30 September 2014
*/
/****************************************************************************
 *   Copyright (C) 2013 by Ken Sarkies                                      *
 *   ksarkies@trinity.asn.au                                                *
 *                                                                          *
 *   This file is part of Power Management GUI                              *
 *                                                                          *
 *   Power Management GUI is free software; you can redistribute it and/or  *
 *   modify it under the terms of the GNU General Public License as         *
 *   published by the Free Software Foundation; either version 2 of the     *
 *   License, or (at your option) any later version.                        *
 *                                                                          *
 *   Power Management GUI is distributed in the hope that it will be useful,*
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with Power Management GUI if not, write to the                   *
 *   Free Software Foundation, Inc.,                                        *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.              *
 ***************************************************************************/

#include "power-management.h"
#include "power-management-main.h"
#include "serialport.h"
#include <QApplication>
#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>
#include <QCloseEvent>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

//-----------------------------------------------------------------------------
/** Power Management Main Window Constructor

@param[in] parent Parent widget.
*/

PowerManagementGui::PowerManagementGui(QWidget* parent) : QDialog(parent)
{
// Build the User Interface display from the Ui class in ui_mainwindowform.h
    PowerManagementMainUi.setupUi(this);
    socket = new SerialPort(SERIAL_PORT);
    connect(socket, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
// Attempt to initialise the serial port with the default setting
    synchronized = false;
    baudrate = 6;
    if (socket->initPort(baudrate,100))
        synchronized = true;
    else
        errorMessage = QString("Unable to access the serial port\n"
                            "Check the connections and power.\n"
                            "You may need root privileges?");
    saveFile.clear();
    response.clear();

    initGui();
    initCalibration();
    initRecording();
}

PowerManagementGui::~PowerManagementGui()
{
/* Turn off microcontroller communications */
    socket->write("pc-\n\r");
    if (socket != NULL) socket->close();
    if (socket != NULL) delete socket;
}

//-----------------------------------------------------------------------------
/** @brief Initialise GUI

*/

void PowerManagementGui::initGui()
{
    PowerManagementMainUi.recordFileName->installEventFilter(this);
    PowerManagementMainUi.battery1CapacityEdit->installEventFilter(this);
    PowerManagementMainUi.battery2CapacityEdit->installEventFilter(this);
    PowerManagementMainUi.battery3CapacityEdit->installEventFilter(this);
    PowerManagementMainUi.chargeParameterEdit_1->installEventFilter(this);
    PowerManagementMainUi.chargeParameterEdit_2->installEventFilter(this);
    PowerManagementMainUi.chargeParameterEdit_3->installEventFilter(this);
    PowerManagementMainUi.lowVoltageEdit->installEventFilter(this);
    PowerManagementMainUi.criticalVoltageEdit->installEventFilter(this);
    PowerManagementMainUi.lowSoCEdit->installEventFilter(this);
    PowerManagementMainUi.criticalSoCEdit->installEventFilter(this);
// Uncheck all buttons in case the microcontroller doesn't respond.
    PowerManagementMainUi.load1Battery1->setAutoExclusive(false);
    PowerManagementMainUi.load1Battery1->setChecked(false);
    PowerManagementMainUi.load1Battery1->setAutoExclusive(true);
    PowerManagementMainUi.load1Battery1->setEnabled(false);
    PowerManagementMainUi.load2Battery1->setAutoExclusive(false);
    PowerManagementMainUi.load2Battery1->setChecked(false);
    PowerManagementMainUi.load2Battery1->setAutoExclusive(true);
    PowerManagementMainUi.load2Battery1->setEnabled(false);
    PowerManagementMainUi.panelBattery1->setAutoExclusive(false);
    PowerManagementMainUi.panelBattery1->setChecked(false);
    PowerManagementMainUi.panelBattery1->setAutoExclusive(true);
    PowerManagementMainUi.panelBattery1->setEnabled(false);
    PowerManagementMainUi.load1Battery2->setAutoExclusive(false);
    PowerManagementMainUi.load1Battery2->setChecked(false);
    PowerManagementMainUi.load1Battery2->setAutoExclusive(true);
    PowerManagementMainUi.load1Battery2->setEnabled(false);
    PowerManagementMainUi.load2Battery2->setAutoExclusive(false);
    PowerManagementMainUi.load2Battery2->setChecked(false);
    PowerManagementMainUi.load2Battery2->setAutoExclusive(true);
    PowerManagementMainUi.load2Battery2->setEnabled(false);
    PowerManagementMainUi.panelBattery2->setAutoExclusive(false);
    PowerManagementMainUi.panelBattery2->setChecked(false);
    PowerManagementMainUi.panelBattery2->setAutoExclusive(true);
    PowerManagementMainUi.panelBattery2->setEnabled(false);
    PowerManagementMainUi.load1Battery3->setAutoExclusive(false);
    PowerManagementMainUi.load1Battery3->setChecked(false);
    PowerManagementMainUi.load1Battery3->setAutoExclusive(true);
    PowerManagementMainUi.load1Battery3->setEnabled(false);
    PowerManagementMainUi.load2Battery3->setAutoExclusive(false);
    PowerManagementMainUi.load2Battery3->setChecked(false);
    PowerManagementMainUi.load2Battery3->setAutoExclusive(true);
    PowerManagementMainUi.load2Battery3->setEnabled(false);
    PowerManagementMainUi.panelBattery3->setAutoExclusive(false);
    PowerManagementMainUi.panelBattery3->setChecked(false);
    PowerManagementMainUi.panelBattery3->setAutoExclusive(true);
    PowerManagementMainUi.panelBattery3->setEnabled(false);
    PowerManagementMainUi.load1Current->clear();
    PowerManagementMainUi.load1Voltage->clear();
    PowerManagementMainUi.load2Current->clear();
    PowerManagementMainUi.load2Voltage->clear();
    PowerManagementMainUi.panelCurrent->clear();
    PowerManagementMainUi.panelVoltage->clear();
    PowerManagementMainUi.battery1CheckBox->setChecked(false);
    PowerManagementMainUi.battery2CheckBox->setChecked(false);
    PowerManagementMainUi.battery3CheckBox->setChecked(false);
    PowerManagementMainUi.load1CheckBox->setChecked(false);
    PowerManagementMainUi.load2CheckBox->setChecked(false);
    PowerManagementMainUi.panelCheckBox->setChecked(false);

    PowerManagementMainUi.battery1SoCReset->
        setStyleSheet("background-color:lightpink;");
    PowerManagementMainUi.battery1SoCReset->
        setText("R");
    PowerManagementMainUi.battery2SoCReset->
        setStyleSheet("background-color:lightpink;");
    PowerManagementMainUi.battery2SoCReset->
        setText("R");
    PowerManagementMainUi.battery3SoCReset->
        setStyleSheet("background-color:lightpink;");
    PowerManagementMainUi.battery3SoCReset->
        setText("R");

    disableKeypad();

/* Turn on microcontroller communications */
    socket->write("pc+\n\r");
/* This should cause the microcontroller to respond with all data */
    socket->write("dS\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Handle incoming serial data

This is called when data appears in the serial buffer. Data is pulled in until
a newline occurs, at which point the assembled command QString is processed.

All incoming messages are processed here and passed to other windows as
appropriate.
*/

void PowerManagementGui::onDataAvailable()
{
    QByteArray data = socket->readAll();
    int n=0;
    while (n < data.size())
    {
        if ((data.at(n) != '\r') && (data.at(n) != '\n')) response += data.at(n);
        if (data.at(n) == '\n')
        {
// The current time is saved to ms precision followed by the line.
            tick.restart();
            processResponse(response);
            response.clear();
        }
        n++;
    }
}

//-----------------------------------------------------------------------------
/** @brief Process the incoming serial data

Parse the line and take action on the command received.
*/

void PowerManagementGui::processResponse(const QString response)
{
    QStringList breakdown = response.split(",");
    int size = breakdown.size();
    QString firstField;
    firstField = breakdown[0].simplified();
    QString secondField;
    if (size > 1) secondField = breakdown[1].simplified();
    QString current, voltage;
// Load 1 current/voltage values
    if (! saveFile.isEmpty()) saveLine(response);
    if ((size > 0) && (firstField == "dL1"))
    {
        getCurrentVoltage(breakdown,&current,&voltage);
        if (PowerManagementMainUi.load1CheckBox->isChecked())
        {
            if (testIndicator(load1UnderVoltage) || testIndicator(load1OverCurrent))
            {
                PowerManagementMainUi.load1Current->setText(QString("---"));
                PowerManagementMainUi.load1Voltage->setText(QString("---"));
            }
            else
            {
                if (size > 1)
                    PowerManagementMainUi.load1Current->setText(current);
                if (size > 2)
                    PowerManagementMainUi.load1Voltage->setText(voltage);
            }
        }
        else
        {
            PowerManagementMainUi.load1Current->clear();
            PowerManagementMainUi.load1Voltage->clear();
        }
    }
// Load 2 current/voltage values
    if ((size > 0) && (firstField == "dL2"))
    {
        getCurrentVoltage(breakdown,&current,&voltage);
        if (PowerManagementMainUi.load2CheckBox->isChecked())
        {
            if (testIndicator(load2UnderVoltage) || testIndicator(load2OverCurrent))
            {
                PowerManagementMainUi.load2Current->setText(QString("---"));
                PowerManagementMainUi.load2Voltage->setText(QString("---"));
            }
            else
            {
                if (size > 1)
                    PowerManagementMainUi.load2Current->setText(current);
                if (size > 2)
                    PowerManagementMainUi.load2Voltage->setText(voltage);
            }
        }
        else
        {
            PowerManagementMainUi.load2Current->clear();
            PowerManagementMainUi.load2Voltage->clear();
        }
    }
// Panel current/voltage values
    if ((size > 0) && (firstField == "dM1"))
    {
        getCurrentVoltage(breakdown,&current,&voltage);
        if (PowerManagementMainUi.panelCheckBox->isChecked())
        {
            if (testIndicator(panelUnderVoltage) || testIndicator(panelOverCurrent))
            {
                PowerManagementMainUi.panelCurrent->setText(QString("---"));
                PowerManagementMainUi.panelVoltage->setText(QString("---"));
            }
            else
            {
                if (size > 1)
                    PowerManagementMainUi.panelCurrent->setText(current);
                if (size > 2)
                    PowerManagementMainUi.panelVoltage->setText(voltage);
            }
        }
        else
        {
            PowerManagementMainUi.panelCurrent->clear();
            PowerManagementMainUi.panelVoltage->clear();
        }
    }
// Battery 1 current/voltage values
    if ((size > 0) && (firstField == "dB1"))
    {
        getCurrentVoltage(breakdown,&current,&voltage);
        if (PowerManagementMainUi.battery1CheckBox->isChecked())
        {
            if (testIndicator(battery1UnderVoltage) || testIndicator(battery1OverCurrent))
            {
                PowerManagementMainUi.battery1Current->setText(QString("---"));
                PowerManagementMainUi.battery1Voltage->setText(QString("---"));
            }
            else
            {
                if (size > 1)
                    PowerManagementMainUi.battery1Current->setText(current);
                if (size > 2)
                    PowerManagementMainUi.battery1Voltage->setText(voltage);
            }
        }
        else
        {
            PowerManagementMainUi.battery1Current->clear();
            PowerManagementMainUi.battery1Voltage->clear();
        }
    }
// Battery 2 current/voltage values
    if ((size > 0) && (firstField == "dB2"))
    {
        getCurrentVoltage(breakdown,&current,&voltage);
        if (PowerManagementMainUi.battery2CheckBox->isChecked())
        {
            if (testIndicator(battery2UnderVoltage) || testIndicator(battery2OverCurrent))
            {
                PowerManagementMainUi.battery2Current->setText(QString("---"));
                PowerManagementMainUi.battery2Voltage->setText(QString("---"));
            }
            else
            {
                if (size > 1)
                    PowerManagementMainUi.battery2Current->setText(current);
                if (size > 2)
                    PowerManagementMainUi.battery2Voltage->setText(voltage);
            }
        }
        else
        {
            PowerManagementMainUi.battery2Current->clear();
            PowerManagementMainUi.battery2Voltage->clear();
        }
    }
// Battery 3 current/voltage values
    if ((size > 0) && (firstField == "dB3"))
    {
        getCurrentVoltage(breakdown,&current,&voltage);
        if (PowerManagementMainUi.battery3CheckBox->isChecked())
        {
            if (testIndicator(battery3UnderVoltage) || testIndicator(battery3OverCurrent))
            {
                PowerManagementMainUi.battery3Current->setText(QString("---"));
                PowerManagementMainUi.battery3Voltage->setText(QString("---"));
            }
            else
            {
                if (size > 1)
                    PowerManagementMainUi.battery3Current->setText(current);
                if (size > 2)
                    PowerManagementMainUi.battery3Voltage->setText(voltage);
            }
        }
        else
        {
            PowerManagementMainUi.battery3Current->clear();
            PowerManagementMainUi.battery3Voltage->clear();
        }
    }
// Restore the current software settings.
// Bit 0 = autotrack
    if ((size > 0) && (firstField == "dD"))
    {
        bool autoTrackOn = ((secondField.toInt() & 0x01) > 0);
        PowerManagementMainUi.autoTrackCheckBox->setChecked(autoTrackOn);
        disableRadioButtons(autoTrackOn);
    }
// Read all the microcontroller's switch settings and set display accordingly
// Upper case S is used for initialization and after calibration.
// Disable unused batteries and associated buttons, and set checkboxes.
// Lower case s is used for autotrack to allow switch settings to be observed.
// In that case the original settings of the checkboxes are preserved.
    if ((size > 0) && ((firstField == "dS") || (firstField == "ds")))
    {
        unsigned int settings = secondField.toInt();
        unsigned int load1Setting = (settings & 0x03);
        unsigned int load2Setting = ((settings >> 2) & 0x03);
        unsigned int panelSetting = ((settings >> 4) & 0x03);
        bool battery1Enabled = ((load1Setting == 1) || (load2Setting == 1)\
                                       || (panelSetting == 1));
        bool battery2Enabled = ((load1Setting == 2) || (load2Setting == 2)\
                                       || (panelSetting == 2));
        bool battery3Enabled = ((load1Setting == 3) || (load2Setting == 3)\
                                       || (panelSetting == 3));
// Disable a battery if none of the load/panels are selected for it
        if (firstField == "dS")
        {
            PowerManagementMainUi.battery1CheckBox->setChecked(battery1Enabled);
            PowerManagementMainUi.battery2CheckBox->setChecked(battery2Enabled);
            PowerManagementMainUi.battery3CheckBox->setChecked(battery3Enabled);
            PowerManagementMainUi.load1Battery1->setEnabled(battery1Enabled);
            PowerManagementMainUi.load1Battery2->setEnabled(battery2Enabled);
            PowerManagementMainUi.load1Battery3->setEnabled(battery3Enabled);
            PowerManagementMainUi.load2Battery1->setEnabled(battery1Enabled);
            PowerManagementMainUi.load2Battery2->setEnabled(battery2Enabled);
            PowerManagementMainUi.load2Battery3->setEnabled(battery3Enabled);
            PowerManagementMainUi.panelBattery1->setEnabled(battery1Enabled);
            PowerManagementMainUi.panelBattery2->setEnabled(battery2Enabled);
            PowerManagementMainUi.panelBattery3->setEnabled(battery3Enabled);
        }
// Set each of the switch settings
        if (firstField == "dS")
            PowerManagementMainUi.load1CheckBox->setChecked(true);
        bool load1Battery1enabled = PowerManagementMainUi.load1Battery1->isEnabled();
        bool load1Battery2enabled = PowerManagementMainUi.load1Battery2->isEnabled();
        bool load1Battery3enabled = PowerManagementMainUi.load1Battery3->isEnabled();
        PowerManagementMainUi.load1Battery1->setEnabled(true);
        PowerManagementMainUi.load1Battery2->setEnabled(true);
        PowerManagementMainUi.load1Battery3->setEnabled(true);
        switch (load1Setting)
        {
// No battery allocated to load 1
            case 0:
                PowerManagementMainUi.load1Battery1->setAutoExclusive(false);
                PowerManagementMainUi.load1Battery1->setChecked(false);
                PowerManagementMainUi.load1Battery1->setAutoExclusive(true);
                PowerManagementMainUi.load1Battery2->setAutoExclusive(false);
                PowerManagementMainUi.load1Battery2->setChecked(false);
                PowerManagementMainUi.load1Battery2->setAutoExclusive(true);
                PowerManagementMainUi.load1Battery3->setAutoExclusive(false);
                PowerManagementMainUi.load1Battery3->setChecked(false);
                PowerManagementMainUi.load1Battery3->setAutoExclusive(true);
                break;
            case 1:
                PowerManagementMainUi.load1Battery1->setAutoExclusive(true);
                PowerManagementMainUi.load1Battery1->setChecked(true);
                break;            
            case 2:
                PowerManagementMainUi.load1Battery2->setAutoExclusive(true);
                PowerManagementMainUi.load1Battery2->setChecked(true);
                break;            
            case 3:
                PowerManagementMainUi.load1Battery3->setAutoExclusive(true);
                PowerManagementMainUi.load1Battery3->setChecked(true);
                break;
            }
        if (! load1Battery1enabled) PowerManagementMainUi.load1Battery1->setEnabled(false);
        if (! load1Battery2enabled) PowerManagementMainUi.load1Battery2->setEnabled(false);
        if (! load1Battery3enabled) PowerManagementMainUi.load1Battery3->setEnabled(false);

        if (firstField == "dS")
            PowerManagementMainUi.load2CheckBox->setChecked(true);
        bool load2Battery1enabled = PowerManagementMainUi.load2Battery1->isEnabled();
        bool load2Battery2enabled = PowerManagementMainUi.load2Battery2->isEnabled();
        bool load2Battery3enabled = PowerManagementMainUi.load2Battery3->isEnabled();
        PowerManagementMainUi.load2Battery1->setEnabled(true);
        PowerManagementMainUi.load2Battery2->setEnabled(true);
        PowerManagementMainUi.load2Battery3->setEnabled(true);
        switch (load2Setting)
            {
// No battery allocated to load 2
            case 0:
                PowerManagementMainUi.load2Battery1->setAutoExclusive(false);
                PowerManagementMainUi.load2Battery1->setChecked(false);
                PowerManagementMainUi.load2Battery1->setAutoExclusive(true);
                PowerManagementMainUi.load2Battery2->setAutoExclusive(false);
                PowerManagementMainUi.load2Battery2->setChecked(false);
                PowerManagementMainUi.load2Battery2->setAutoExclusive(true);
                PowerManagementMainUi.load2Battery3->setAutoExclusive(false);
                PowerManagementMainUi.load2Battery3->setChecked(false);
                PowerManagementMainUi.load2Battery3->setAutoExclusive(true);
                break;
            case 1:
                PowerManagementMainUi.load2Battery1->setAutoExclusive(true);
                PowerManagementMainUi.load2Battery1->setChecked(true);
                break;            
            case 2:
                PowerManagementMainUi.load2Battery2->setAutoExclusive(true);
                PowerManagementMainUi.load2Battery2->setChecked(true);
                break;            
            case 3:
                PowerManagementMainUi.load2Battery3->setAutoExclusive(true);
                PowerManagementMainUi.load2Battery3->setChecked(true);
                break;
            }
        if (! load2Battery1enabled) PowerManagementMainUi.load2Battery1->setEnabled(false);
        if (! load2Battery2enabled) PowerManagementMainUi.load2Battery2->setEnabled(false);
        if (! load2Battery3enabled) PowerManagementMainUi.load2Battery3->setEnabled(false);

        if (firstField == "dS")
            PowerManagementMainUi.panelBattery1->setChecked(true);
        bool panelBattery1enabled = PowerManagementMainUi.panelBattery1->isEnabled();
        bool panelBattery2enabled = PowerManagementMainUi.panelBattery2->isEnabled();
        bool panelBattery3enabled = PowerManagementMainUi.panelBattery3->isEnabled();
        PowerManagementMainUi.panelBattery1->setEnabled(true);
        PowerManagementMainUi.panelBattery2->setEnabled(true);
        PowerManagementMainUi.panelBattery3->setEnabled(true);
        switch (panelSetting)
            {
// No battery allocated to the panel
            case 0:
                PowerManagementMainUi.panelBattery1->setAutoExclusive(false);
                PowerManagementMainUi.panelBattery1->setChecked(false);
                PowerManagementMainUi.panelBattery1->setAutoExclusive(true);
                PowerManagementMainUi.panelBattery2->setAutoExclusive(false);
                PowerManagementMainUi.panelBattery2->setChecked(false);
                PowerManagementMainUi.panelBattery2->setAutoExclusive(true);
                PowerManagementMainUi.panelBattery3->setAutoExclusive(false);
                PowerManagementMainUi.panelBattery3->setChecked(false);
                PowerManagementMainUi.panelBattery3->setAutoExclusive(true);
                break;
// Battery x allocated to the panel
            case 1:
                PowerManagementMainUi.panelBattery1->setAutoExclusive(true);
                PowerManagementMainUi.panelBattery1->setChecked(true);
                break;            
            case 2:
                PowerManagementMainUi.panelBattery2->setAutoExclusive(true);
                PowerManagementMainUi.panelBattery2->setChecked(true);
                break;            
            case 3:
                PowerManagementMainUi.panelBattery3->setAutoExclusive(true);
                PowerManagementMainUi.panelBattery3->setChecked(true);
                break;
        }
        if (! panelBattery1enabled) PowerManagementMainUi.panelBattery1->setEnabled(false);
        if (! panelBattery2enabled) PowerManagementMainUi.panelBattery2->setEnabled(false);
        if (! panelBattery3enabled) PowerManagementMainUi.panelBattery3->setEnabled(false);
    }
// Overload and undervoltage indicators from the I/Fs
// Battery 1, Battery 2, Battery 3, Load 1, Load 2, Panel
// ON is low.
    if ((size > 0) && (firstField == "dI"))
    {
        indicators = secondField.toInt();
        if (testIndicator(battery1OverCurrent))
        {
            PowerManagementMainUi.battery1OverCurrent->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.battery1OverCurrent->
                setText("OC");
        }
        else
        {
            PowerManagementMainUi.battery1OverCurrent->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery1OverCurrent->
                setText("");
        }
        if (testIndicator(battery1UnderVoltage))
        {
            PowerManagementMainUi.battery1UnderVoltage->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.battery1UnderVoltage->
                setText("UV");
        }
        else
        {
            PowerManagementMainUi.battery1UnderVoltage->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery1UnderVoltage->
                setText("");
        }
        if (testIndicator(battery2OverCurrent))
        {
            PowerManagementMainUi.battery2OverCurrent->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.battery2OverCurrent->
                setText("OC");
        }
        else
        {
            PowerManagementMainUi.battery2OverCurrent->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery2OverCurrent->
                setText("");
        }
        if (testIndicator(battery2UnderVoltage))
        {
            PowerManagementMainUi.battery2UnderVoltage->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.battery2UnderVoltage->
                setText("UV");
        }
        else
        {
            PowerManagementMainUi.battery2UnderVoltage->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery2UnderVoltage->
                setText("");
        }
        if (testIndicator(battery3OverCurrent))
        {
            PowerManagementMainUi.battery3OverCurrent->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.battery3OverCurrent->
                setText("OC");
        }
        else
        {
            PowerManagementMainUi.battery3OverCurrent->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery3OverCurrent->
                setText("");
        }
        if (testIndicator(battery3UnderVoltage))
        {
            PowerManagementMainUi.battery3UnderVoltage->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.battery3UnderVoltage->
                setText("UV");
        }
        else
        {
            PowerManagementMainUi.battery3UnderVoltage->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery3UnderVoltage->
                setText("");
        }
        if (testIndicator(load1OverCurrent))
        {
            PowerManagementMainUi.load1OverCurrent->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.load1OverCurrent->
                setText("OC");
        }
        else
        {
            PowerManagementMainUi.load1OverCurrent->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.load1OverCurrent->
                setText("");
        }
        if (testIndicator(load1UnderVoltage))
        {
            PowerManagementMainUi.load1UnderVoltage->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.load1UnderVoltage->
                setText("UV");
        }
        else
        {
            PowerManagementMainUi.load1UnderVoltage->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.load1UnderVoltage->
                setText("");
        }
        if (testIndicator(load2OverCurrent))
        {
            PowerManagementMainUi.load2OverCurrent->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.load2OverCurrent->
                setText("OC");
        }
        else
        {
            PowerManagementMainUi.load2OverCurrent->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.load2OverCurrent->
                setText("");
        }
        if (testIndicator(load2UnderVoltage))
        {
            PowerManagementMainUi.load2UnderVoltage->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.load2UnderVoltage->
                setText("UV");
        }
        else
        {
            PowerManagementMainUi.load2UnderVoltage->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.load2UnderVoltage->
                setText("");
        }
        if (testIndicator(panelOverCurrent))
        {
            PowerManagementMainUi.panelOverCurrent->
                setStyleSheet("color:white; background-color:red;");
            PowerManagementMainUi.panelOverCurrent->
                setText("OC");
        }
        else
        {
            PowerManagementMainUi.panelOverCurrent->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.panelOverCurrent->
                setText("");
        }
        if (testIndicator(panelUnderVoltage))
        {
            PowerManagementMainUi.panelUnderVoltage->
                setStyleSheet("color:white; color:white; background-color:red;");
            PowerManagementMainUi.panelUnderVoltage->
                setText("UV");
        }
        else
        {
            PowerManagementMainUi.panelUnderVoltage->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.panelUnderVoltage->
                setText("");
        }
    }
/* Battery 1 Fill, Health and Operational State Indicators */
    if ((size > 0) && (firstField == "dO1"))
    {
        int opState = secondField.toInt() & 0x03;
        int fillState = (secondField.toInt() >> 2) & 0x03;
        int chargingState = (secondField.toInt() >> 4) & 0x03;
        int healthState = (secondField.toInt() >> 6) & 0x03;
        if (fillState == 0)         // Normal
        {
            PowerManagementMainUi.battery1Fill->
                setStyleSheet("background-color:lightgreen;");
        }
        else if (fillState == 1)    // Low
        {
            PowerManagementMainUi.battery1Fill->
                setStyleSheet("background-color:yellow;");
        }
        else if (fillState == 2)    // Critical
        {
            PowerManagementMainUi.battery1Fill->
                setStyleSheet("background-color:red;");
        }
        else if (fillState == 3)    // Faulty
        {
            PowerManagementMainUi.battery1Fill->
                setStyleSheet("background-color:black;");
        }
        else                        // Invalid
        {
            PowerManagementMainUi.battery1Fill->
                setStyleSheet("background-color:white;");
        }
        if (PowerManagementMainUi.autoTrackCheckBox->isChecked())
        {
            if (opState == 0)
            {
                PowerManagementMainUi.battery1Op->
                    setText("L");
            }
            else if (opState == 1)
            {
                PowerManagementMainUi.battery1Op->
                    setText("C");
            }
            else
            {
                PowerManagementMainUi.battery1Op->
                    setText("I");
            }
        }
        else
        {
            PowerManagementMainUi.battery1Op->
                setText("");
        }
        if (chargingState == 0)
        {
            PowerManagementMainUi.battery1Charging->
                setStyleSheet("background-color:orange;");
            PowerManagementMainUi.battery1Charging->
                setText("B");
        }
        else if (chargingState == 1)
        {
            PowerManagementMainUi.battery1Charging->
                setStyleSheet("background-color:yellow;");
            PowerManagementMainUi.battery1Charging->
                setText("A");
        }
        else if (chargingState == 2)
        {
            PowerManagementMainUi.battery1Charging->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery1Charging->
                setText("F");
        }
        else if (chargingState == 3)
        {
            PowerManagementMainUi.battery1Charging->
                setStyleSheet("background-color:pink;");
            PowerManagementMainUi.battery1Charging->
                setText("R");
        }
        else if (chargingState == 4)
        {
            PowerManagementMainUi.battery1Charging->
                setStyleSheet("background-color:lightblue;");
            PowerManagementMainUi.battery1Charging->
                setText("E");
        }
        else
        {
            PowerManagementMainUi.battery1Charging->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery1Charging->
                setText(" ");
        }
        if (healthState == 0)
        {
            PowerManagementMainUi.battery1Health->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery1Health->
                setText("");
        }
        else if (healthState == 1)
        {
            PowerManagementMainUi.battery1Health->
                setStyleSheet("background-color:orange;");
            PowerManagementMainUi.battery1Health->
                setText("F");
        }
        else
        {
            PowerManagementMainUi.battery1Health->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery1Health->
                setText("X");
            PowerManagementMainUi.battery1Charging->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery1Charging->
                setText("");
            PowerManagementMainUi.battery1Fill->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery1Fill->
                setText("");
            PowerManagementMainUi.battery1Op->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery1Op->
                setText("");
            PowerManagementMainUi.battery1Charge->
                setText(QString(""));
            PowerManagementMainUi.battery1Current->setText(QString(""));
            PowerManagementMainUi.battery1Voltage->setText(QString(""));
        }
    }
/* Battery 2 Fill, Health and Operational State Indicators */
    if ((size > 0) && (firstField == "dO2"))
    {
        int opState = secondField.toInt() & 0x03;
        int fillState = (secondField.toInt() >> 2) & 0x03;
        int chargingState = (secondField.toInt() >> 4) & 0x03;
        int healthState = (secondField.toInt() >> 6) & 0x03;
        if (fillState == 0)         // Normal
        {
            PowerManagementMainUi.battery2Fill->
                setStyleSheet("background-color:lightgreen;");
        }
        else if (fillState == 1)    // Low
        {
            PowerManagementMainUi.battery2Fill->
                setStyleSheet("background-color:yellow;");
        }
        else if (fillState == 2)    // Critical
        {
            PowerManagementMainUi.battery2Fill->
                setStyleSheet("background-color:red;");
        }
        else if (fillState == 3)    // Faulty
        {
            PowerManagementMainUi.battery2Fill->
                setStyleSheet("background-color:black;");
        }
        else                        // Invalid
        {
            PowerManagementMainUi.battery2Fill->
                setStyleSheet("background-color:white;");
        }
        if (PowerManagementMainUi.autoTrackCheckBox->isChecked())
        {
            if (opState == 0)
            {
                PowerManagementMainUi.battery2Op->
                    setText("L");
            }
            else if (opState == 1)
            {
                PowerManagementMainUi.battery2Op->
                    setText("C");
            }
            else
            {
                PowerManagementMainUi.battery2Op->
                    setText("I");
            }
        }
        else
        {
            PowerManagementMainUi.battery2Op->
                setText("");
        }
        if (chargingState == 0)
        {
            PowerManagementMainUi.battery2Charging->
                setStyleSheet("background-color:orange;");
            PowerManagementMainUi.battery2Charging->
                setText("B");
        }
        else if (chargingState == 1)
        {
            PowerManagementMainUi.battery2Charging->
                setStyleSheet("background-color:yellow;");
            PowerManagementMainUi.battery2Charging->
                setText("A");
        }
        else if (chargingState == 2)
        {
            PowerManagementMainUi.battery2Charging->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery2Charging->
                setText("F");
        }
        else if (chargingState == 3)
        {
            PowerManagementMainUi.battery2Charging->
                setStyleSheet("background-color:pink;");
            PowerManagementMainUi.battery2Charging->
                setText("R");
        }
        else if (chargingState == 4)
        {
            PowerManagementMainUi.battery2Charging->
                setStyleSheet("background-color:lightblue;");
            PowerManagementMainUi.battery2Charging->
                setText("E");
        }
        else
        {
            PowerManagementMainUi.battery2Charging->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery2Charging->
                setText(" ");
        }
        if (healthState == 0)
        {
            PowerManagementMainUi.battery2Health->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery2Health->
                setText("");
        }
        else if (healthState == 1)
        {
            PowerManagementMainUi.battery2Health->
                setStyleSheet("background-color:orange;");
            PowerManagementMainUi.battery2Health->
                setText("F");
        }
        else
        {
            PowerManagementMainUi.battery2Health->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery2Health->
                setText("X");
            PowerManagementMainUi.battery2Charging->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery2Charging->
                setText("");
            PowerManagementMainUi.battery2Fill->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery2Fill->
                setText("");
            PowerManagementMainUi.battery2Op->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery2Op->
                setText("");
            PowerManagementMainUi.battery2Charge->
                setText(QString(""));
            PowerManagementMainUi.battery2Current->setText(QString(""));
            PowerManagementMainUi.battery2Voltage->setText(QString(""));
        }
    }
/* Battery 3 Fill, Health and Operational State Indicators */
    if ((size > 0) && (firstField == "dO3"))
    {
        int opState = secondField.toInt() & 0x03;
        int fillState = (secondField.toInt() >> 2) & 0x03;
        int chargingState = (secondField.toInt() >> 4) & 0x03;
        int healthState = (secondField.toInt() >> 6) & 0x03;
        if (fillState == 0)         // Normal
        {
            PowerManagementMainUi.battery3Fill->
                setStyleSheet("background-color:lightgreen;");
        }
        else if (fillState == 1)    // Low
        {
            PowerManagementMainUi.battery3Fill->
                setStyleSheet("background-color:yellow;");
        }
        else if (fillState == 2)    // Critical
        {
            PowerManagementMainUi.battery3Fill->
                setStyleSheet("background-color:red;");
        }
        else if (fillState == 3)    // Faulty
        {
            PowerManagementMainUi.battery3Fill->
                setStyleSheet("background-color:black;");
        }
        else                        // Invalid
        {
            PowerManagementMainUi.battery3Fill->
                setStyleSheet("background-color:white;");
        }
        if (PowerManagementMainUi.autoTrackCheckBox->isChecked())
        {
            if (opState == 0)
            {
                PowerManagementMainUi.battery3Op->
                    setText("L");
            }
            else if (opState == 1)
            {
                PowerManagementMainUi.battery3Op->
                    setText("C");
            }
            else
            {
                PowerManagementMainUi.battery3Op->
                    setText("I");
            }
        }
        else
        {
            PowerManagementMainUi.battery3Op->
                setText("");
        }
        if (chargingState == 0)
        {
            PowerManagementMainUi.battery3Charging->
                setStyleSheet("background-color:orange;");
            PowerManagementMainUi.battery3Charging->
                setText("B");
        }
        else if (chargingState == 1)
        {
            PowerManagementMainUi.battery3Charging->
                setStyleSheet("background-color:yellow;");
            PowerManagementMainUi.battery3Charging->
                setText("A");
        }
        else if (chargingState == 2)
        {
            PowerManagementMainUi.battery3Charging->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery3Charging->
                setText("F");
        }
        else if (chargingState == 3)
        {
            PowerManagementMainUi.battery3Charging->
                setStyleSheet("background-color:pink;");
            PowerManagementMainUi.battery3Charging->
                setText("R");
        }
        else if (chargingState == 4)
        {
            PowerManagementMainUi.battery3Charging->
                setStyleSheet("background-color:lightblue;");
            PowerManagementMainUi.battery3Charging->
                setText("E");
        }
        else
        {
            PowerManagementMainUi.battery3Charging->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery3Charging->
                setText(" ");
        }
        if (healthState == 0)
        {
            PowerManagementMainUi.battery3Health->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery3Health->
                setText("");
        }
        else if (healthState == 1)
        {
            PowerManagementMainUi.battery3Health->
                setStyleSheet("background-color:orange;");
            PowerManagementMainUi.battery3Health->
                setText("F");
        }
        else
        {
            PowerManagementMainUi.battery3Health->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery3Health->
                setText("X");
            PowerManagementMainUi.battery3Charging->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery3Charging->
                setText("");
            PowerManagementMainUi.battery3Fill->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery3Fill->
                setText("");
            PowerManagementMainUi.battery3Op->
                setStyleSheet("background-color:white;");
            PowerManagementMainUi.battery3Op->
                setText("");
            PowerManagementMainUi.battery3Charge->
                setText(QString(""));
            PowerManagementMainUi.battery3Current->setText(QString(""));
            PowerManagementMainUi.battery3Voltage->setText(QString(""));
        }
    }
/* SoC estimates */
    if ((size > 0) && (firstField == "dC1"))
    {
        if (PowerManagementMainUi.battery1CheckBox->isChecked())
        {
            if (size > 1) PowerManagementMainUi.battery1Charge
                ->setText(QString("%1").arg(secondField
                        .toFloat()/256,0,'f',0).append('%'));
        }
        else
        {
            PowerManagementMainUi.battery1Charge->clear();
        }
    }
    if ((size > 0) && (firstField == "dC2"))
    {
        if (PowerManagementMainUi.battery2CheckBox->isChecked())
        {
            if (size > 1) PowerManagementMainUi.battery2Charge
                ->setText(QString("%1").arg(secondField
                        .toFloat()/256,0,'f',0).append('%'));
        }
        else
        {
            PowerManagementMainUi.battery2Charge->clear();
        }
    }
    if ((size > 0) && (firstField == "dC3"))
    {
        if (PowerManagementMainUi.battery3CheckBox->isChecked())
        {
            if (size > 1) PowerManagementMainUi.battery3Charge
                ->setText(QString("%1").arg(secondField
                        .toFloat()/256,0,'f',0).append('%'));
        }
        else
        {
            PowerManagementMainUi.battery3Charge->clear();
        }
    }
    if ((size > 0) && (firstField == "dT"))
    {
        if (size > 1) PowerManagementMainUi.temperature
            ->setText(QString("%1").arg(secondField
                .toFloat()/256,0,'f',1).append(QChar(0x00B0)).append("C"));
    }
/* Messages for the File Task start with f */
    if ((size > 0) && (firstField.left(1) == "f"))
    {
        recordMessageReceived(response);
    }
/* Messages for the Configure Task start with p or dS */
    if ((size > 0) && ((firstField.left(1) == "p") || (firstField.left(2) == "dO")))
    {
        configureMessageReceived(response);
    }
    if ((size > 0) && (firstField.left(2) == "dD"))
    {
        configureMessageReceived(response);
    }
/* This allows debug messages to be displayed on the terminal. */
    if ((size > 0) && (firstField.left(1) == "D"))
    {
        qDebug() << response;
        if (! saveFile.isEmpty()) saveLine(response);
    }
}

//-----------------------------------------------------------------------------
/** @brief Convert Voltage and Current Strings for Display

The current and voltage values are obtained from the command string breakdown
converted to a QString form suitable for display. The breakdown is
0 - command, 1 - current, 2- voltage.

An entry is saved in the save file if it is open.
*/

void PowerManagementGui::getCurrentVoltage(const QStringList breakdown,
                                           QString* sCurrent, QString* sVoltage)
{
    int size = breakdown.size();
    QString entry = breakdown[0].simplified();
    if (size > 1)
    {
        float fCurrent = breakdown[1].simplified().toFloat()/256;
        *sCurrent = QString("%1").arg(fCurrent,0,'f',2);
        entry = entry.append(",").append(*sCurrent);
    }
    if (size > 2)
    {
        float fVoltage = breakdown[2].simplified().toFloat()/256;
        *sVoltage = QString("%1").arg(fVoltage,0,'f',2);
        entry = entry.append(",").append(*sVoltage);
    }
//    if (! saveFile.isEmpty()) saveLine(entry);
}
//-----------------------------------------------------------------------------
/** @brief Test indicators on the Interface Cards.

Indicators arrive as inversions of state (0 = on, 1 = off), Two bit fields
of which the first bit is overcurrent and the second is undervoltage. Fields
are in ascending order battery1-3, load 1-2, solar module.

@param[in] indicator. enum indicating the indicator signal to be tested.
@returns boolean true if indicator is on.
*/

bool PowerManagementGui::testIndicator(const IndicatorType indicator)
{
    switch (indicator)
    {
    case battery1OverCurrent:
        return ((indicators & 0x01) == 0);
    case battery1UnderVoltage:
        return ((indicators & 0x02) == 0);
    case battery2OverCurrent:
        return ((indicators & 0x04) == 0);
    case battery2UnderVoltage:
        return ((indicators & 0x08) == 0);
    case battery3OverCurrent:
        return ((indicators & 0x10) == 0);
    case battery3UnderVoltage:
        return ((indicators & 0x20) == 0);
    case load1OverCurrent:
        return ((indicators & 0x40) == 0);
    case load1UnderVoltage:
        return ((indicators & 0x80) == 0);
    case load2OverCurrent:
        return ((indicators & 0x100) == 0);
    case load2UnderVoltage:
        return ((indicators & 0x200) == 0);
    case panelOverCurrent:
        return ((indicators & 0x400) == 0);
    case panelUnderVoltage:
        return ((indicators & 0x800) == 0);
    }
    return false;
}

//-----------------------------------------------------------------------------
/** @brief Actions for Switch Settings

These are called when a radio button is pressed on. A message is sent
to the microcontroller to effect a change in switch settings.

Checks for the associated battery enabled, even though, if it were not enabled,
the radio button should have been disabled anyway.
*/

void PowerManagementGui::on_load1Battery1_pressed()
{
    if (PowerManagementMainUi.battery1CheckBox->isChecked())
    {
        socket->write("aS11\n\r");
        PowerManagementMainUi.load1CheckBox->setChecked(true);
    }
}

void PowerManagementGui::on_load1Battery2_pressed()
{
    if (PowerManagementMainUi.battery2CheckBox->isChecked())
    {
        socket->write("aS21\n\r");
        PowerManagementMainUi.load1CheckBox->setChecked(true);
    }
}

void PowerManagementGui::on_load1Battery3_pressed()
{
    if (PowerManagementMainUi.battery3CheckBox->isChecked())
    {
        socket->write("aS31\n\r");
        PowerManagementMainUi.load1CheckBox->setChecked(true);
    }
}

void PowerManagementGui::on_load2Battery1_pressed()
{
    if (PowerManagementMainUi.battery1CheckBox->isChecked())
    {
        socket->write("aS12\n\r");
        PowerManagementMainUi.load2CheckBox->setChecked(true);
    }
}

void PowerManagementGui::on_load2Battery2_pressed()
{
    if (PowerManagementMainUi.battery2CheckBox->isChecked())
    {
        socket->write("aS22\n\r");
        PowerManagementMainUi.load2CheckBox->setChecked(true);
    }
}

void PowerManagementGui::on_load2Battery3_pressed()
{
    if (PowerManagementMainUi.battery3CheckBox->isChecked())
    {
        socket->write("aS32\n\r");
        PowerManagementMainUi.load2CheckBox->setChecked(true);
    }
}

void PowerManagementGui::on_panelBattery1_pressed()
{
    if (PowerManagementMainUi.battery1CheckBox->isChecked())
    {
        socket->write("aS13\n\r");
        PowerManagementMainUi.panelCheckBox->setChecked(true);
    }
}

void PowerManagementGui::on_panelBattery2_pressed()
{
    if (PowerManagementMainUi.battery2CheckBox->isChecked())
    {
        socket->write("aS23\n\r");
        PowerManagementMainUi.panelCheckBox->setChecked(true);
    }
}

void PowerManagementGui::on_panelBattery3_pressed()
{
    if (PowerManagementMainUi.battery3CheckBox->isChecked())
    {
        socket->write("aS33\n\r");
        PowerManagementMainUi.panelCheckBox->setChecked(true);
    }
}

//-----------------------------------------------------------------------------
/** @brief Actions for Load/Panel Enables

These are called when a load/panel checkbox is clicked.

If the load/panel checkbox is unchecked, set all radio buttons off
and send a command to the microcontroller to turn off the relevant switch.

If it is checked, then do nothing as some radio buttons may be disabled
due to disabled battery settings.
*/

void PowerManagementGui::on_load1CheckBox_clicked()
{
    if (! PowerManagementMainUi.load1CheckBox->isChecked())
    {
        socket->write("aS01\n\r");
        PowerManagementMainUi.load1Battery1->setAutoExclusive(false);
        PowerManagementMainUi.load1Battery1->setChecked(false);
        PowerManagementMainUi.load1Battery1->setAutoExclusive(true);
        PowerManagementMainUi.load1Battery2->setAutoExclusive(false);
        PowerManagementMainUi.load1Battery2->setChecked(false);
        PowerManagementMainUi.load1Battery2->setAutoExclusive(true);
        PowerManagementMainUi.load1Battery3->setAutoExclusive(false);
        PowerManagementMainUi.load1Battery3->setChecked(false);
        PowerManagementMainUi.load1Battery3->setAutoExclusive(true);
        PowerManagementMainUi.load1Current->clear();
        PowerManagementMainUi.load1Voltage->clear();
    }
}

void PowerManagementGui::on_load2CheckBox_clicked()
{
    if (! PowerManagementMainUi.load2CheckBox->isChecked())
    {
        socket->write("aS02\n\r");
        PowerManagementMainUi.load2Battery1->setAutoExclusive(false);
        PowerManagementMainUi.load2Battery1->setChecked(false);
        PowerManagementMainUi.load2Battery1->setAutoExclusive(true);
        PowerManagementMainUi.load2Battery2->setAutoExclusive(false);
        PowerManagementMainUi.load2Battery2->setChecked(false);
        PowerManagementMainUi.load2Battery2->setAutoExclusive(true);
        PowerManagementMainUi.load2Battery3->setAutoExclusive(false);
        PowerManagementMainUi.load2Battery3->setChecked(false);
        PowerManagementMainUi.load2Battery3->setAutoExclusive(true);
    }
}

void PowerManagementGui::on_panelCheckBox_clicked()
{
    if (! PowerManagementMainUi.panelCheckBox->isChecked())
    {
        socket->write("aS03\n\r");
        PowerManagementMainUi.panelBattery1->setAutoExclusive(false);
        PowerManagementMainUi.panelBattery1->setChecked(false);
        PowerManagementMainUi.panelBattery1->setAutoExclusive(true);
        PowerManagementMainUi.panelBattery2->setAutoExclusive(false);
        PowerManagementMainUi.panelBattery2->setChecked(false);
        PowerManagementMainUi.panelBattery2->setAutoExclusive(true);
        PowerManagementMainUi.panelBattery3->setAutoExclusive(false);
        PowerManagementMainUi.panelBattery3->setChecked(false);
        PowerManagementMainUi.panelBattery3->setAutoExclusive(true);
    }
}

//-----------------------------------------------------------------------------
/** @brief Actions for Overcurrent Resets

These are called when an overcurrent reset button is clicked.

*/

void PowerManagementGui::on_battery1OverCurrent_clicked()
{
    socket->write("aR0\n\r");
}

void PowerManagementGui::on_battery2OverCurrent_clicked()
{
    socket->write("aR1\n\r");
}

void PowerManagementGui::on_battery3OverCurrent_clicked()
{
    socket->write("aR2\n\r");
}

void PowerManagementGui::on_load1OverCurrent_clicked()
{
    socket->write("aR3\n\r");
}

void PowerManagementGui::on_load2OverCurrent_clicked()
{
    socket->write("aR4\n\r");
}

void PowerManagementGui::on_panelOverCurrent_clicked()
{
    socket->write("aR5\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Actions for Battery Enables

These are called when a battery checkbox is clicked.

If the battery checkbox is checked then enable the associated radio buttons
but don't check any of them.

If it is unchecked then go through and uncheck all associated radio buttons,
if they are checked. Also disable them and send a command to turn off the
relevant switch in the microcontroller.
*/

void PowerManagementGui::on_battery1CheckBox_clicked()
{
    if (PowerManagementMainUi.battery1CheckBox->isChecked())
    {
        PowerManagementMainUi.load1Battery1->setEnabled(true);
        PowerManagementMainUi.load2Battery1->setEnabled(true);
        PowerManagementMainUi.panelBattery1->setEnabled(true);
    }
    else
    {
        if (PowerManagementMainUi.load1Battery1->isChecked())
        {
            socket->write("aS01\n\r");
            PowerManagementMainUi.load1Battery1->setAutoExclusive(false);
            PowerManagementMainUi.load1Battery1->setChecked(false);
            PowerManagementMainUi.load1Battery1->setAutoExclusive(true);
        }
        if (PowerManagementMainUi.load2Battery1->isChecked())
        {
            socket->write("aS02\n\r");
            PowerManagementMainUi.load2Battery1->setAutoExclusive(false);
            PowerManagementMainUi.load2Battery1->setChecked(false);
            PowerManagementMainUi.load2Battery1->setAutoExclusive(true);
        }
        if (PowerManagementMainUi.panelBattery1->isChecked())
        {
            socket->write("aS03\n\r");
            PowerManagementMainUi.panelBattery1->setAutoExclusive(false);
            PowerManagementMainUi.panelBattery1->setChecked(false);
            PowerManagementMainUi.panelBattery1->setAutoExclusive(true);
        }
        PowerManagementMainUi.load1Battery1->setEnabled(false);
        PowerManagementMainUi.load2Battery1->setEnabled(false);
        PowerManagementMainUi.panelBattery1->setEnabled(false);
    }
}

void PowerManagementGui::on_battery2CheckBox_clicked()
{
    if (PowerManagementMainUi.battery2CheckBox->isChecked())
    {
        PowerManagementMainUi.load1Battery2->setEnabled(true);
        PowerManagementMainUi.load2Battery2->setEnabled(true);
        PowerManagementMainUi.panelBattery2->setEnabled(true);
    }
    else
    {
        if (PowerManagementMainUi.load1Battery2->isChecked())
        {
            socket->write("aS01\n\r");
            PowerManagementMainUi.load1Battery2->setAutoExclusive(false);
            PowerManagementMainUi.load1Battery2->setChecked(false);
            PowerManagementMainUi.load1Battery2->setAutoExclusive(true);
        }
        if (PowerManagementMainUi.load2Battery2->isChecked())
        {
            socket->write("aS02\n\r");
            PowerManagementMainUi.load2Battery2->setAutoExclusive(false);
            PowerManagementMainUi.load2Battery2->setChecked(false);
            PowerManagementMainUi.load2Battery2->setAutoExclusive(true);
        }
        if (PowerManagementMainUi.panelBattery2->isChecked())
        {
            socket->write("aS03\n\r");
            PowerManagementMainUi.panelBattery2->setAutoExclusive(false);
            PowerManagementMainUi.panelBattery2->setChecked(false);
            PowerManagementMainUi.panelBattery2->setAutoExclusive(true);
        }
        PowerManagementMainUi.load1Battery2->setEnabled(false);
        PowerManagementMainUi.load2Battery2->setEnabled(false);
        PowerManagementMainUi.panelBattery2->setEnabled(false);
    }
}

void PowerManagementGui::on_battery3CheckBox_clicked()
{
    if (PowerManagementMainUi.battery3CheckBox->isChecked())
    {
        PowerManagementMainUi.load1Battery3->setEnabled(true);
        PowerManagementMainUi.load2Battery3->setEnabled(true);
        PowerManagementMainUi.panelBattery3->setEnabled(true);
    }
    else
    {
        if (PowerManagementMainUi.load1Battery3->isChecked())
        {
            socket->write("aS01\n\r");
            PowerManagementMainUi.load1Battery3->setAutoExclusive(false);
            PowerManagementMainUi.load1Battery3->setChecked(false);
            PowerManagementMainUi.load1Battery3->setAutoExclusive(true);
        }
        if (PowerManagementMainUi.load2Battery3->isChecked())
        {
            socket->write("aS02\n\r");
            PowerManagementMainUi.load2Battery3->setAutoExclusive(false);
            PowerManagementMainUi.load2Battery3->setChecked(false);
            PowerManagementMainUi.load2Battery3->setAutoExclusive(true);
        }
        if (PowerManagementMainUi.panelBattery3->isChecked())
        {
            socket->write("aS03\n\r");
            PowerManagementMainUi.panelBattery3->setAutoExclusive(false);
            PowerManagementMainUi.panelBattery3->setChecked(false);
            PowerManagementMainUi.panelBattery3->setAutoExclusive(true);
        }
        PowerManagementMainUi.load1Battery3->setEnabled(false);
        PowerManagementMainUi.load2Battery3->setEnabled(false);
        PowerManagementMainUi.panelBattery3->setEnabled(false);
    }
}

//-----------------------------------------------------------------------------
/** @brief Actions for SoC Recomputations

These are called when an SoC Reset button is clicked.

*/

void PowerManagementGui::on_battery1SoCReset_clicked()
{
    socket->write("aB1\n\r");
}

void PowerManagementGui::on_battery2SoCReset_clicked()
{
    socket->write("aB2\n\r");
}

void PowerManagementGui::on_battery3SoCReset_clicked()
{
    socket->write("aB3\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Obtain a save file name and path and attempt to open it.

The save filename is created from date and time in the current directory.
This avoids the need for keyboard input.

The files are csv but the ending can be arbitrary to allow compatibility
with the data processing application.
*/

void PowerManagementGui::on_saveFileButton_clicked()
{
//! Make sure there is no file already open.
    if (! saveFile.isEmpty())
    {
        displayErrorMessage("A file is already open - close first");
        return;
    }
    PowerManagementMainUi.errorLabel->clear();
    QString filename = "data-" +
                QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss") +
                ".txt";
    QFileInfo fileInfo(filename);
    saveDirectory = fileInfo.absolutePath();
    saveFile = saveDirectory.filePath(filename);
    outFile = new QFile(saveFile);             // Open file for output
    if (! outFile->open(QIODevice::WriteOnly))
    {
        displayErrorMessage("Could not open the output file");
        return;
    }
}

//-----------------------------------------------------------------------------
/** @brief Save a line to the opened save file.

*/
void PowerManagementGui::saveLine(QString line)
{
//! Check that the save file has been defined and is open.
    if (saveFile.isEmpty())
    {
        displayErrorMessage("Output File not defined");
        return;
    }
    if (! outFile->isOpen())
    {
        displayErrorMessage("Output File not open");
        return;
    }
    QTextStream out(outFile);
    out << line << "\n\r";
}

//-----------------------------------------------------------------------------
/** @brief Close the recording save file.

*/
void PowerManagementGui::on_closeFileButton_clicked()
{
    PowerManagementMainUi.errorLabel->clear();
    if (saveFile.isEmpty())
        displayErrorMessage("File already closed");
    else
    {
        outFile->close();
        delete outFile;
//! Wipe the name to prevent the same file being used.
        saveFile = QString();
    }
}

//-----------------------------------------------------------------------------
/** @brief Initiate AutoTrack Controls.

*/

void PowerManagementGui::on_autoTrackCheckBox_clicked()
{
    if (PowerManagementMainUi.autoTrackCheckBox->isChecked())
    {
        disableRadioButtons(true);
        socket->write("pa+\n\r");
    }
    else
    {
        disableRadioButtons(false);
        socket->write("pa-\n\r");
    }
}

//-----------------------------------------------------------------------------
/** @brief Enable or Disable the Radio buttons.

Used for autotracking.

@param[in] bool disable: if true, disable all radio buttons for autotracking.
*/

void PowerManagementGui::disableRadioButtons(bool disable)
{
    PowerManagementMainUi.load1Battery1->setEnabled(! disable);
    PowerManagementMainUi.load1Battery2->setEnabled(! disable);
    PowerManagementMainUi.load1Battery3->setEnabled(! disable);
    PowerManagementMainUi.load2Battery1->setEnabled(! disable);
    PowerManagementMainUi.load2Battery2->setEnabled(! disable);
    PowerManagementMainUi.load2Battery3->setEnabled(! disable);
    PowerManagementMainUi.panelBattery1->setEnabled(! disable);
    PowerManagementMainUi.panelBattery2->setEnabled(! disable);
    PowerManagementMainUi.panelBattery3->setEnabled(! disable);
}

//-----------------------------------------------------------------------------
/** @brief Successful establishment of serial port setup

@returns TRUE if successful.
*/
bool PowerManagementGui::success()
{
    return true;
}

//-----------------------------------------------------------------------------
/** @brief Close off the window and deallocate resources

This may not be necessary as QT may implement it implicitly.
*/
void PowerManagementGui::closeEvent(QCloseEvent *event)
{
    event->accept();
}

//-----------------------------------------------------------------------------
/** @brief Sleep for a number of centiseconds but keep processing events

*/

void PowerManagementGui::ssleep(int centiseconds)
{
    for (int i=0; i<centiseconds*10; i++)
    {
        qApp->processEvents();
        millisleep(10);
    }
}

//-----------------------------------------------------------------------------
//                  CONFIGURE
//-----------------------------------------------------------------------------
/** @brief Initialise Configurations

*/

void PowerManagementGui::initCalibration()
{
// Set the calibrate progress bar to invisible
    PowerManagementMainUi.calibrateProgressBar->setVisible(false);
    PowerManagementMainUi.calibrateProgressBar->setValue(0);
// Set the battery type combobox entries and default value
    PowerManagementMainUi.battery1TypeCombo->addItem("Wet Cell");
    PowerManagementMainUi.battery1TypeCombo->addItem("Gel Cell");
    PowerManagementMainUi.battery1TypeCombo->addItem("AGM Cell");
    PowerManagementMainUi.battery1TypeCombo->setCurrentIndex(1);
    PowerManagementMainUi.battery2TypeCombo->addItem("Wet Cell");
    PowerManagementMainUi.battery2TypeCombo->addItem("Gel Cell");
    PowerManagementMainUi.battery2TypeCombo->addItem("AGM Cell");
    PowerManagementMainUi.battery2TypeCombo->setCurrentIndex(1);
    PowerManagementMainUi.battery3TypeCombo->addItem("Wet Cell");
    PowerManagementMainUi.battery3TypeCombo->addItem("Gel Cell");
    PowerManagementMainUi.battery3TypeCombo->addItem("AGM Cell");
    PowerManagementMainUi.battery3TypeCombo->setCurrentIndex(1);
// Hide the charging algorithm parameter spinboxes and text.
    PowerManagementMainUi.chargeParameterEdit_1->setVisible(false);
    PowerManagementMainUi.chargeParameterText_1->setVisible(false);
    PowerManagementMainUi.chargeParameterEdit_2->setVisible(false);
    PowerManagementMainUi.chargeParameterText_2->setVisible(false);
    PowerManagementMainUi.chargeParameterEdit_3->setVisible(false);
    PowerManagementMainUi.chargeParameterText_3->setVisible(false);
// Set the resistance display default (with Unicode Omega)
    PowerManagementMainUi.battery1Resistance
            ->setText(QString("0 m").append(QChar(0x03A9)));
    PowerManagementMainUi.battery2Resistance
            ->setText(QString("0 m").append(QChar(0x03A9)));
    PowerManagementMainUi.battery3Resistance
            ->setText(QString("0 m").append(QChar(0x03A9)));
/* Ask for battery parameters to fill display */
    on_queryBatteryButton_clicked();
/* Ask for control settings */
    socket->write("dS\n\r");
/* Ask for charge parameter settings */
    socket->write("dC\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Initiate Calibration

This causes the end device to initiate the calibration sequence. The window
cannot be closed until a result is received or a timeout period is reached.
*/

void PowerManagementGui::on_calibrateButton_clicked()
{
    PowerManagementMainUi.calibrateProgressBar->setVisible(true);
    this->setEnabled(false);
    QApplication::processEvents();
    socket->write("pC\n\r");
    
}

//-----------------------------------------------------------------------------
/** @brief Query Battery Parameters

Resistance, type and capacity of the batteries are requested.
*/

void PowerManagementGui::on_queryBatteryButton_clicked()
{
    socket->write("dB1\n\r");
    socket->write("dB2\n\r");
    socket->write("dB3\n\r");
}
//-----------------------------------------------------------------------------
/** @brief Set Battery Parameters

Resistance, type and capacity of the batteries are set from the GUI values.
*/

void PowerManagementGui::on_setBatteryButton_clicked()
{
    QString typeSet1 = "pT1";
    typeSet1.append(QString("%1").arg(PowerManagementMainUi.battery1TypeCombo
                ->currentIndex(),1));
    typeSet1.append(PowerManagementMainUi.battery1CapacityEdit->text());
    socket->write(typeSet1.append("\n\r").toAscii().constData());
    QString typeSet2 = "pT2";
    typeSet2.append(QString("%1").arg(PowerManagementMainUi.battery2TypeCombo
                ->currentIndex(),1));
    typeSet2.append(PowerManagementMainUi.battery2CapacityEdit->text());
    socket->write(typeSet2.append("\n\r").toAscii().constData());
    QString typeSet3 = "pT3";
    typeSet3.append(QString("%1").arg(PowerManagementMainUi.battery3TypeCombo
                ->currentIndex(),1));
    typeSet3.append(PowerManagementMainUi.battery3CapacityEdit->text());
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

void PowerManagementGui::on_setTrackOptionButton_clicked()
{
    int option = 0;
    if (PowerManagementMainUi.loadChargeCheckBox->isChecked())
    {
        option |= 0x01;
    }
    else
    {
        option &= ~0x01;
    }
    if (PowerManagementMainUi.isolationMaintainCheckBox->isChecked())
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

/* 0 Three Phase Algorithm. */
void PowerManagementGui::on_threePhaseButton_clicked()
{
    QString command = "pS0";
    socket->write(command.append(QString("%1")).append("\n\r")
                         .toAscii().constData());
/* Ask for charge parameter settings */
    socket->write("dC\n\r");
}

/* 1 Intermittent Charge Algorithm. */
void PowerManagementGui::on_icButton_clicked()
{
    QString command = "pS1";
    socket->write(command.append(QString("%1")).append("\n\r")
                         .toAscii().constData());
/* Ask for charge parameter settings */
    socket->write("dC\n\r");
}

/* 2 Interrupted Charge Control Algorithm. */
void PowerManagementGui::on_iccButton_clicked()
{
    QString command = "pS2";
    socket->write(command.append(QString("%1")).append("\n\r")
                         .toAscii().constData());
/* Ask for charge parameter settings */
    socket->write("dC\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Set the Time

The current time is read and transmitted to set time in the remote system.
*/

void PowerManagementGui::on_timeSetButton_clicked()
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

void PowerManagementGui::on_debugMessageCheckbox_clicked()
{
    if (PowerManagementMainUi.debugMessageCheckbox->isChecked())
        socket->write("pd+\n\r");
    else
        socket->write("pd-\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Enable Data Messages

*/

void PowerManagementGui::on_dataMessageCheckbox_clicked()
{
    if (PowerManagementMainUi.dataMessageCheckbox->isChecked())
        socket->write("pM+\n\r");
    else
        socket->write("pM-\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Send Echo Request

*/

void PowerManagementGui::on_echoTestButton_clicked()
{
    socket->write("aE\n\r");
    socket->write("dS\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Reset or Set the Battery 1 Missing Status

*/

void PowerManagementGui::on_resetMissing1Button_clicked()
{
    if (PowerManagementMainUi.resetMissing1Button->text() == "X")
    {
        PowerManagementMainUi.resetMissing1Button->setText("");
        socket->write("pm1-\n\r");
    }
    else
    {
        PowerManagementMainUi.resetMissing1Button->setText("X");
        socket->write("pm1+\n\r");
    }
}

//-----------------------------------------------------------------------------
/** @brief Reset the Battery 2 Missing Status

*/

void PowerManagementGui::on_resetMissing2Button_clicked()
{
    if (PowerManagementMainUi.resetMissing2Button->text() == "X")
    {
        PowerManagementMainUi.resetMissing2Button->setText("");
        socket->write("pm2-\n\r");
    }
    else
    {
        PowerManagementMainUi.resetMissing2Button->setText("X");
        socket->write("pm2+\n\r");
    }
}

//-----------------------------------------------------------------------------
/** @brief Reset the Battery 3 Missing Status

*/

void PowerManagementGui::on_resetMissing3Button_clicked()
{
    if (PowerManagementMainUi.resetMissing3Button->text() == "X")
    {
        PowerManagementMainUi.resetMissing3Button->setText("");
        socket->write("pm3-\n\r");
    }
    else
    {
        PowerManagementMainUi.resetMissing3Button->setText("X");
        socket->write("pm3+\n\r");
    }
}

//-----------------------------------------------------------------------------
/** @brief Process a Message.

After a command is sent, response messages from the remote are passed here for
processing. Appropriate fields on the form are updated.

@todo This interprets response as done in main. See if this can be made
independent of formats.
*/

void PowerManagementGui::configureMessageReceived(const QString response)
{
    bool ok;
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
                PowerManagementMainUi.calibrateProgressBar->setValue(test+1);
            else
            {
                PowerManagementMainUi.quiescentCurrent
                        ->setText(QString("%1 A").arg(quiescentCurrent.
                                  toFloat()/256,0,'f',3));
                this->setEnabled(true);
                PowerManagementMainUi.calibrateProgressBar->setVisible(false);
                PowerManagementMainUi.calibrateProgressBar->setValue(0);
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
                PowerManagementMainUi.battery1Resistance
                    ->setText(batteryResistance);
            else if (battery == '2')
                PowerManagementMainUi.battery2Resistance
                    ->setText(batteryResistance);
            else if (battery == '3')
                PowerManagementMainUi.battery3Resistance
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
                PowerManagementMainUi.battery1TypeCombo
                    ->setCurrentIndex(batteryType);
                PowerManagementMainUi.battery1CapacityEdit
                    ->setText(QString("%1").arg(batteryCapacity,-1));
            }
            else if (battery == '2')
            {
                PowerManagementMainUi.battery2TypeCombo
                    ->setCurrentIndex(batteryType);
                PowerManagementMainUi.battery2CapacityEdit
                    ->setText(QString("%1").arg(batteryCapacity,-1));
            }
            else if (battery == '3')
            {
                PowerManagementMainUi.battery3TypeCombo
                    ->setCurrentIndex(batteryType);
                PowerManagementMainUi.battery3CapacityEdit
                    ->setText(QString("%1").arg(batteryCapacity,-1));
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
                PowerManagementMainUi.battery1AbsorptionVoltage
                    ->setText(absorptionVoltage);
                float battery1Capacity = PowerManagementMainUi.
                            battery1CapacityEdit->text().toFloat(&ok);
                PowerManagementMainUi.battery1AbsorptionCurrent
                    ->setText(QString("%1 A")
                        .arg(battery1Capacity/bulkCurrentScale,0,'f',2));
            }
            else if (battery == '2')
            {
                PowerManagementMainUi.battery2AbsorptionVoltage
                    ->setText(absorptionVoltage);
                float battery2Capacity = PowerManagementMainUi.
                            battery2CapacityEdit->text().toFloat(&ok);
                PowerManagementMainUi.battery2AbsorptionCurrent
                    ->setText(QString("%1 A")
                        .arg(battery2Capacity/bulkCurrentScale,0,'f',2));
            }
            else if (battery == '3')
            {
                PowerManagementMainUi.battery3AbsorptionVoltage
                    ->setText(absorptionVoltage);
                float battery3Capacity = PowerManagementMainUi.
                            battery3CapacityEdit->text().toFloat(&ok);
                PowerManagementMainUi.battery3AbsorptionCurrent
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
                PowerManagementMainUi.battery1FloatVoltage
                    ->setText(floatVoltage);
                float battery1Capacity = PowerManagementMainUi.
                            battery1CapacityEdit->text().toFloat(&ok);
                PowerManagementMainUi.battery1FloatCurrent
                    ->setText(QString("%1 A")
                        .arg(battery1Capacity/floatCurrentScale,0,'f',2));
            }
            else if (battery == '2')
            {
                PowerManagementMainUi.battery2FloatVoltage
                    ->setText(floatVoltage);
                float battery2Capacity = PowerManagementMainUi.
                            battery2CapacityEdit->text().toFloat(&ok);
                PowerManagementMainUi.battery2FloatCurrent
                    ->setText(QString("%1 A")
                        .arg(battery2Capacity/floatCurrentScale,0,'f',2));
            }
            else if (battery == '3')
            {
                PowerManagementMainUi.battery3FloatVoltage
                    ->setText(floatVoltage);
                float battery3Capacity = PowerManagementMainUi.
                            battery3CapacityEdit->text().toFloat(&ok);
                PowerManagementMainUi.battery3FloatCurrent
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
                PowerManagementMainUi.dataMessageCheckbox->setChecked(true);
            else
                PowerManagementMainUi.dataMessageCheckbox->setChecked(false);
            bool debugMessage = ((controlByte & (1<<4)) > 0);
            if (debugMessage)
                PowerManagementMainUi.debugMessageCheckbox->setChecked(true);
            else
                PowerManagementMainUi.debugMessageCheckbox->setChecked(false);
            int chargeAlgorithm = (controlByte >> 5) & 0x03;
            if (chargeAlgorithm == 0)
            {
                PowerManagementMainUi.threePhaseButton->setChecked(true);
                PowerManagementMainUi.chargeParameterEdit_1->setVisible(true);
                PowerManagementMainUi.chargeParameterEdit_1->
                    setToolTip("Lowest duty cycle allowed to prevent it crashing to zero");
                PowerManagementMainUi.chargeParameterText_1->setVisible(true);
                PowerManagementMainUi.chargeParameterText_1->
                    setText("Minimum Duty Cycle (%)");
                PowerManagementMainUi.chargeParameterEdit_2->setVisible(false);
                PowerManagementMainUi.chargeParameterText_2->setVisible(false);
                PowerManagementMainUi.chargeParameterEdit_3->setVisible(false);
                PowerManagementMainUi.chargeParameterText_3->setVisible(false);
            }
            if (chargeAlgorithm == 1)
            {
                PowerManagementMainUi.icButton->setChecked(true);
                PowerManagementMainUi.chargeParameterEdit_1->setVisible(true);
                PowerManagementMainUi.chargeParameterEdit_1->
                    setToolTip("Low voltage threshold ending the charging phase and starting the rest phase, in percent of OCV");
                PowerManagementMainUi.chargeParameterText_1->setVisible(true);
                PowerManagementMainUi.chargeParameterText_1->
                    setText("Rest Phase Voltage %OCV");
                PowerManagementMainUi.chargeParameterEdit_2->setVisible(false);
                PowerManagementMainUi.chargeParameterText_2->setVisible(false);
                PowerManagementMainUi.chargeParameterEdit_3->setVisible(false);
                PowerManagementMainUi.chargeParameterText_3->setVisible(false);
            }
            if (chargeAlgorithm == 2)
            {
                PowerManagementMainUi.iccButton->setChecked(true);
                PowerManagementMainUi.chargeParameterEdit_1->setVisible(true);
                PowerManagementMainUi.chargeParameterEdit_1->
                    setToolTip("Low voltage threshold ending the rest phase and starting the absorption phase, in percent of OCV");
                PowerManagementMainUi.chargeParameterText_1->setVisible(true);
                PowerManagementMainUi.chargeParameterText_1->
                    setText("Rest Phase Voltage %OCV");
                PowerManagementMainUi.chargeParameterEdit_2->setVisible(true);
                PowerManagementMainUi.chargeParameterEdit_2->
                    setToolTip("Length of a slot in a cycle of the absorption phase");
                PowerManagementMainUi.chargeParameterText_2->setVisible(true);
                PowerManagementMainUi.chargeParameterText_2->
                    setText("Slot Length (secs)");
                PowerManagementMainUi.chargeParameterEdit_3->setVisible(true);
                PowerManagementMainUi.chargeParameterEdit_3->
                    setToolTip("Number of slots in a cycle of the absorption phase");
                PowerManagementMainUi.chargeParameterText_3->setVisible(true);
                PowerManagementMainUi.chargeParameterText_3->
                    setText("Number of Slots");
            }
            bool avoidLoad = (((controlByte >> 7) & 0x01) > 0);
            if (avoidLoad)
                PowerManagementMainUi.loadChargeCheckBox->setChecked(true);
            else
                PowerManagementMainUi.loadChargeCheckBox->setChecked(false);
            bool maintainIsolate = (((controlByte >> 8) & 0x01) > 0);
            if (maintainIsolate)
                PowerManagementMainUi.isolationMaintainCheckBox->setChecked(true);
            else
                PowerManagementMainUi.isolationMaintainCheckBox->setChecked(false);
            break;
        }
// Show charger minimum duty cycle parameter
        case '3':
        {
            if (size < 1) break;
            int dutyCycle = breakdown[1].simplified().toInt();
            if (PowerManagementMainUi.threePhaseButton->isChecked())
                PowerManagementMainUi.chargeParameterEdit_1->
                    setText(QString("%1").arg(dutyCycle,-1));
            break;
        }
// Show charger ICC parameters
        case 'P':
        {
            if (size < 2) break;
            int slotLength = breakdown[1].simplified().toInt();
            if (PowerManagementMainUi.iccButton->isChecked())
                PowerManagementMainUi.chargeParameterEdit_2->
                    setText(QString("%1").arg(slotLength,-1));
            int slotNumber = breakdown[2].simplified().toInt();
            if (PowerManagementMainUi.iccButton->isChecked())
                PowerManagementMainUi.chargeParameterEdit_3->
                    setText(QString("%1").arg(slotNumber,-1));
            break;
        }
// Show charger voltage threshold parameter
        case 'V':
        {
            if (size < 1) break;
            int voltage = breakdown[1].simplified().toInt();
            if ((PowerManagementMainUi.icButton->isChecked()) ||
                (PowerManagementMainUi.iccButton->isChecked()))
                PowerManagementMainUi.chargeParameterEdit_1->
                    setText(QString("%1").arg(voltage,-1));
            break;
        }
// Show current time settings from the system
        case 'H':
        {
            if (size < 2) break;
            QDateTime systemTime =
                QDateTime::fromString(breakdown[1].simplified(),Qt::ISODate);
            PowerManagementMainUi.date->setText(systemTime.date().toString("dd.MM.yyyy"));
            PowerManagementMainUi.time->setText(systemTime.time().toString("H.mm.ss"));
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
                    PowerManagementMainUi.resetMissing1Button->
                        setStyleSheet("background-color:lightgreen;");
                    PowerManagementMainUi.resetMissing1Button->setText("");
                }
                else if (healthState == 1)
                {
                    PowerManagementMainUi.resetMissing1Button->
                        setStyleSheet("background-color:orange;");
                    PowerManagementMainUi.resetMissing1Button->setText("");
                }
                else if (healthState == 2)
                {
                    PowerManagementMainUi.resetMissing1Button->
                        setStyleSheet("background-color:white;");
                    PowerManagementMainUi.resetMissing1Button->setText("X");
                }
            }
            else if (battery == '2')
            {
                if (healthState == 0)
                {
                    PowerManagementMainUi.resetMissing2Button->
                        setStyleSheet("background-color:lightgreen;");
                    PowerManagementMainUi.resetMissing2Button->setText("");
                }
                else if (healthState == 1)
                {
                    PowerManagementMainUi.resetMissing2Button->
                        setStyleSheet("background-color:orange;");
                    PowerManagementMainUi.resetMissing2Button->setText("");
                }
                else if (healthState == 2)
                {
                    PowerManagementMainUi.resetMissing2Button->
                        setStyleSheet("background-color:white;");
                    PowerManagementMainUi.resetMissing2Button->setText("X");
                }
            }
            else if (battery == '3')
            {
                if (healthState == 0)
                {
                    PowerManagementMainUi.resetMissing3Button->
                        setStyleSheet("background-color:lightgreen;");
                    PowerManagementMainUi.resetMissing3Button->setText("");
                }
                else if (healthState == 1)
                {
                    PowerManagementMainUi.resetMissing3Button->
                        setStyleSheet("background-color:orange;");
                    PowerManagementMainUi.resetMissing3Button->setText("");
                }
                else if (healthState == 2)
                {
                    PowerManagementMainUi.resetMissing3Button->
                        setStyleSheet("background-color:white;");
                    PowerManagementMainUi.resetMissing3Button->setText("X");
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
/** @brief Error Message

@returns a message when the device didn't respond.
*/

QString PowerManagementGui::error()
{
    return errorMessage;
}

//-----------------------------------------------------------------------------
/** @brief Show an error condition in the Error label.

@param[in] message Message to be displayed.
*/

void PowerManagementGui::displayErrorMessage(const QString message)
{
    PowerManagementMainUi.errorLabel->setText(message);
}

//-----------------------------------------------------------------------------
//                      RECORDING
//-----------------------------------------------------------------------------
/** @brief Initialise Recording

*/

void PowerManagementGui::initRecording()
{
    requestRecordingStatus();
// Ask for the microcontroller SD card free space (process response later)
    getFreeSpace();
    model = new QStandardItemModel(0, 2, this);
    PowerManagementMainUi.fileTableView->setModel(model);
    PowerManagementMainUi.fileTableView->setGridStyle(Qt::NoPen);
    PowerManagementMainUi.fileTableView->setShowGrid(false);
    QHeaderView *verticalHeader = PowerManagementMainUi.fileTableView->verticalHeader();
    verticalHeader->setResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(18);
    row = 0;
// Signal to process a click on a directory item
    connect(PowerManagementMainUi.fileTableView,
                     SIGNAL(clicked(const QModelIndex)),
                     this,SLOT(onListItemClicked(const QModelIndex)));
// Send a command to refresh the directory
    refreshDirectory();
    writeFileHandle = 0xFF;
}
//-----------------------------------------------------------------------------
/** @brief Delete File.

If the delete checkbox is selected, delete the file (if it exists).
*/

void PowerManagementGui::on_deleteButton_clicked()
{
    QString fileName = PowerManagementMainUi.recordFileName->text();
    if ((fileName.length() > 0) &&
        (PowerManagementMainUi.deleteCheckBox->isChecked()))
    {
        socket->write("fX");
        socket->write(fileName.toLocal8Bit().data());
        socket->write("\n\r");
        refreshDirectory();
        getFreeSpace();
    }
}

//-----------------------------------------------------------------------------
/** @brief Open Recording File.

If a write file is not open the specified file is opened for writing. Response
is a status that indicates if the file was opened/created and the recording
started. This is processed later.
*/

void PowerManagementGui::on_recordFileButton_clicked()
{
    QString fileName = PowerManagementMainUi.recordFileName->text();
    if (fileName.length() > 0)
    {
        socket->write("fW");
        socket->write(fileName.toLocal8Bit().data());
        socket->write("\n\r");
        requestRecordingStatus();
        refreshDirectory();
    }
}

//-----------------------------------------------------------------------------
/** @brief Start Recording.

If a write file is not open the specified file is opened for writing and a
command to start recording is sent. Response is a status that indicates if the
file was opened/created and the recording started. This is processed later.
*/

void PowerManagementGui::on_startRecordingButton_clicked()
{
    if (writeFileHandle < 0xFF)
    {
        socket->write("pr+\n\r");
        requestRecordingStatus();
    }
    else PowerManagementMainUi.errorLabel->setText("File not open");
}

//-----------------------------------------------------------------------------
/** @brief Stop Recording.

*/

void PowerManagementGui::on_stopRecordingButton_clicked()
{
    socket->write("pr-\n\r");
    requestRecordingStatus();
}

//-----------------------------------------------------------------------------
/** @brief Close the write file.

If recording is in progress it is stopped and the write file is closed.
*/

void PowerManagementGui::on_closeRecordingFileButton_clicked()
{
    if (writeFileHandle < 0xFF)
    {
        char command[6] = "fC0\n\r";
        command[2] = '0'+writeFileHandle;
        socket->write("pr-\n\r");
        socket->write(command);
        requestRecordingStatus();
    }
    else PowerManagementMainUi.errorLabel->setText("File not open");
    writeFileHandle = 0xFF;
}

//-----------------------------------------------------------------------------
/** @brief Process a Message.

After a command is sent, response messages from the remote are passed here for
processing. Appropriate fields on the form are updated.

@todo This interprets response as done in main. See if this can be made
independent of formats.
*/

void PowerManagementGui::recordMessageReceived(const QString response)
{
    QStringList breakdown = response.split(",");
    QString command = breakdown[0].right(1);
// Error Code
    switch (command[0].toAscii())
    {
// Show Free Space
        case 'F':
        {
            int freeSpace = breakdown[2].toInt()*breakdown[1].toInt()/2048;
            PowerManagementMainUi.diskSpaceAvailable->setText(QString("%1 M")\
                                                    .arg(freeSpace, 0, 10));
            break;
        }
/* Directory listing.
Fill a predefined model with strings from the response breakdown.
The response will be a comma separated list of items preceded by a type
*/
        case 'D':
        {
            model->clear();
            if (breakdown.size() <= 1) break;
            for (int i=1; i<breakdown.size(); i++)
            {
                QChar type = breakdown[i][0];
                bool ok;
                QString fileSize = QString("%1")
                    .arg((float)breakdown[i].mid(1,8).toInt(&ok,16)/1000000,8,'f',3);
                if (type == 'd')
                    fileSize = "";
                if ((type == 'f') || (type == 'd'))
                {
                    QString fileName = breakdown[i].mid(9,breakdown[i].length()-1);
                    QFont font;
                    if (type == 'd') font.setBold(true);
                    QStandardItem *nameItem = new QStandardItem(fileName);
                    QStandardItem *sizeItem = new QStandardItem(fileSize);
                    QList<QStandardItem *> row;
                    nameItem->setFont(font);
                    nameItem->setData(Qt::AlignLeft, Qt::TextAlignmentRole);
                    sizeItem->setData(Qt::AlignRight, Qt::TextAlignmentRole);
                    row.append(nameItem);
                    row.append(sizeItem);
                    nameItem->setData(QVariant(type));
//                    item->setIcon(...);
                    model->appendRow(row);
                }
            }
            break;
        }
// Status of recording and open files.
// The write and read file handles are retrieved from this
        case 's':
        {
            if (breakdown.size() <= 1) break;
            recordingOn = (breakdown[1].toInt() & 0x02) > 0;
            if (recordingOn)  // recording on
                PowerManagementMainUi.startRecordingButton->
                    setStyleSheet("background-color:lightgreen;");
            else
                PowerManagementMainUi.startRecordingButton->
                    setStyleSheet("background-color:lightpink;");
            if (breakdown.size() <= 2) break;
            writeFileHandle = breakdown[2].toInt();
            writeFileOpen = (writeFileHandle < 255);
            if (writeFileOpen)
                PowerManagementMainUi.recordFileButton->
                    setStyleSheet("background-color:lightgreen;");
            else
                PowerManagementMainUi.recordFileButton->
                    setStyleSheet("background-color:lightpink;");
            if (breakdown.size() <= 3) break;
            readFileHandle = breakdown[3].toInt();
            readFileOpen = (readFileHandle < 255);
            if (readFileOpen)
                PowerManagementMainUi.recordFileName->setText(breakdown[3]);
            break;
        }
// Open a file for recording.
        case 'W':
        {
            writeFileHandle = extractValue(response);
            break;
        }
        case 'E':
        {
            QString errorText[19] = {"Hard Disk Error",
                                     "Internal Error",
                                     "Medium Not Ready",
                                     "File not Found",
                                     "Path not Found",
                                     "Invalid Path Format",
                                     "Access denied or directory full",
                                     "File Exists",
                                     "File/directory object invalid",
                                     "Drive write protected",
                                     "Logical drive number invalid",
                                     "Volume has no work area",
                                     "No valid FAT volume",
                                     "Format aborted: parameter error",
                                     "Timeout waiting for access",
                                     "File sharing policy violation",
                                     "LFN working buffer could not be allocated",
                                     "Too many open files",
                                     "Invalid Parameter"};
            int status = breakdown[1].toInt();
            if ((status > 0) && (status < 20))
                PowerManagementMainUi.errorLabel->setText(errorText[status-1]);
            break;
        }
    }
}

//-----------------------------------------------------------------------------
/** @brief Extract an Integer Value from a Response.

*/

int PowerManagementGui::extractValue(const QString &response)
{
    int i = 4;
    QString temp;
    while (response[i] >= '0')
    {
        temp[i-4] = response[i];
        i++;
    }
    return temp.toInt();
}

//-----------------------------------------------------------------------------
/** @brief Slot to process Directory Entry Clicks.

Display filename in edit box, or enter a directory and redisplay.
*/

void PowerManagementGui::onListItemClicked(const QModelIndex & index)
{
    PowerManagementMainUi.recordFileName->clear();
    QStandardItem *item = model->itemFromIndex(index);
    QString fileName = item->text();
    QChar type = item->data().toChar();
    if (type == 'f')
    {
        PowerManagementMainUi.recordFileName->setText(fileName);
        PowerManagementMainUi.readFileName->setText(fileName);
    }
    if (type == 'd')
        socket->write(QString("fD%1\n\r").arg(fileName).toLocal8Bit().data());
}

//-----------------------------------------------------------------------------
/** @brief Remount the storage media

*/

void PowerManagementGui::on_registerButton_clicked()
{
    socket->write("fM/\n\r");
    refreshDirectory();
}

//-----------------------------------------------------------------------------
/** @brief Refresh the Directory.

This requests the directory entry for the top directory only.
*/

void PowerManagementGui::refreshDirectory()
{
    socket->write("fD/\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Ask for Status of Recording.

*/

void PowerManagementGui::requestRecordingStatus()
{
    socket->write("fs\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Ask for Amount of Free Space Remaining on the Medium.

*/

void PowerManagementGui::getFreeSpace()
{
    socket->write("fF\n\r");
}

//-----------------------------------------------------------------------------
/** @brief Activate Keypad

*/

void PowerManagementGui::on_keypadButton_clicked()
{
    if (PowerManagementMainUi.keypad->isEnabled())
        disableKeypad();
    else
        enableKeypad();
}

//-----------------------------------------------------------------------------
/** @brief Enable the Keypad

*/

void PowerManagementGui::enableKeypad()
{
    PowerManagementMainUi.keypad->setEnabled(true);
    PowerManagementMainUi.keypad->setVisible(true);
    PowerManagementMainUi.keypadButton->setEnabled(false);
    PowerManagementMainUi.keypadButton->setVisible(false);
}

//-----------------------------------------------------------------------------
/** @brief Disable the Keypad

*/

void PowerManagementGui::disableKeypad()
{
    PowerManagementMainUi.keypad->setEnabled(false);
    PowerManagementMainUi.keypad->setVisible(false);
    PowerManagementMainUi.keypadButton->setEnabled(true);
    PowerManagementMainUi.keypadButton->setVisible(true);
}

//-----------------------------------------------------------------------------
/** @brief Reimplement LineEdit to respond to a single mouse click

Needed to allow the keypad to appear when the line edit is selected.
*/

bool PowerManagementGui::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        enableKeypad();
        lineEditObject = (QLineEdit*)obj;
        QString objText = lineEditObject->text();
// Fill the recording file name with default name.
        if (obj == PowerManagementMainUi.recordFileName)
        {
            if (objText.isEmpty())
            {
                objText = "D" +
                          QDateTime::currentDateTime().toString("MMdd") +
                          ".TXT";
                lineEditObject->setText(objText);
                lineEditObject->setFocus();
                lineEditObject->setCursorPosition(5);
            }
        }
// All others clear the field for a new value.
        else
        {
            lineEditObject->clear();
        }
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
/** @brief Keypad Data Entry

All methods for 12 buttons.
*/

// Get rid of keypad.
void PowerManagementGui::on_pbok_clicked()
{
    disableKeypad();
}

// Delete the character just after the cursor, except at start position.
void PowerManagementGui::on_pberase_clicked()
{
    lineEditObject->backspace();
}

void PowerManagementGui::on_pbforward_clicked()
{
    lineEditObject->cursorForward(false);
}

void PowerManagementGui::on_pbback_clicked()
{
    lineEditObject->cursorBackward(false);
}

void PowerManagementGui::on_pbpoint_clicked()
{
    lineEditObject->insert(".");
}

void PowerManagementGui::on_pb0_clicked()
{
    lineEditObject->insert("0");
}

void PowerManagementGui::on_pb1_clicked()
{
    lineEditObject->insert("1");
}

void PowerManagementGui::on_pb2_clicked()
{
    lineEditObject->insert("2");
}

void PowerManagementGui::on_pb3_clicked()
{
    lineEditObject->insert("3");
}

void PowerManagementGui::on_pb4_clicked()
{
    lineEditObject->insert("4");
}

void PowerManagementGui::on_pb5_clicked()
{
    lineEditObject->insert("5");
}

void PowerManagementGui::on_pb6_clicked()
{
    lineEditObject->insert("6");
}

void PowerManagementGui::on_pb7_clicked()
{
    lineEditObject->insert("7");
}

void PowerManagementGui::on_pb8_clicked()
{
    lineEditObject->insert("8");
}

void PowerManagementGui::on_pb9_clicked()
{
    lineEditObject->insert("9");
}


