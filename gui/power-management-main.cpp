/*       Power Management Main Window

Here the data stream from the microcontroller is received and saved to a file.
SoC values are displayed directly. Controls can be set to change the switching
between batteries, loads and panel. When a change is made the new values are
transmitted to the microcontroller. Other windows are called up for more
detailed monitoring and for configuration.

A compile option allows use of either serial or TCP comms. For TCP comms,
use a remote machine to convert between serial and TCP/IP. If Linux is used:

socat /dev/ttyUSB0,echo=0,ispeed=115200,ospeed=115200,raw tcp-l:6666,reuseaddr,fork

@date 6 October 2013
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
#include "power-management-record.h"
#include "power-management-monitor.h"
#include "power-management-configure.h"
#include "serialport.h"
#include <QApplication>
#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>
#include <QCloseEvent>
#include <QFileDialog>
#include <QtNetwork>
#include <QTcpSocket>
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
#ifdef SERIAL
    PowerManagementMainUi.connectButton->setEnabled(false);
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
#else
// Query the socket to establish a connection
    connectAddress = "192.168.2.15";
    connectPort = 6666;
    socket = NULL;
    on_connectButton_clicked();
#endif

    saveFile.clear();
    response.clear();

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

/* This should cause the microcontroller to respond with all data */
    socket->write("dS\n\r");
}

PowerManagementGui::~PowerManagementGui()
{
#ifndef SERIAL
    if (socket != NULL) socket->close();
#endif
    if (socket != NULL) delete socket;
}

//-----------------------------------------------------------------------------
/** @brief Handle incoming serial data

This is called when data appears in the serial buffer. Data is pulled in until
a newline occurs, at which point the assembled command QString is processed.

All incoming messages are processed here and passed to other windows as appropriate.
*/

void PowerManagementGui::onDataAvailable()
{
#ifndef SERIAL
    if (! validsocket()) return;
#endif
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
            case 0:
                PowerManagementMainUi.load2CheckBox->setChecked(false);
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
/* Battery 1 Fill and Operational State Indicators */
    if ((size > 0) && (firstField == "dO1"))
    {
        int opState = secondField.toInt() & 0x03;
        int fillState = (secondField.toInt() >> 2) & 0x03;
        int chargingState = (secondField.toInt() >> 4) & 0x03;
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
                setStyleSheet("background-color:orange;");
        }
        else if (fillState == 3)    // Faulty
        {
            PowerManagementMainUi.battery1Fill->
                setStyleSheet("background-color:red;");
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
        else
        {
            PowerManagementMainUi.battery1Charging->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery1Charging->
                setText("F");
        }
    }
/* Battery 2 Fill and Operational State Indicators */
    if ((size > 0) && (firstField == "dO2"))
    {
        int opState = secondField.toInt() & 0x03;
        int fillState = (secondField.toInt() >> 2) & 0x03;
        int chargingState = (secondField.toInt() >> 4) & 0x03;
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
                setStyleSheet("background-color:orange;");
        }
        else if (fillState == 3)    // Faulty
        {
            PowerManagementMainUi.battery1Fill->
                setStyleSheet("background-color:red;");
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
        else
        {
            PowerManagementMainUi.battery2Charging->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery2Charging->
                setText("F");
        }
    }
/* Battery 3 Fill and Operational State Indicators */
    if ((size > 0) && (firstField == "dO3"))
    {
        int opState = secondField.toInt() & 0x03;
        int fillState = (secondField.toInt() >> 2) & 0x03;
        int chargingState = (secondField.toInt() >> 4) & 0x03;
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
                setStyleSheet("background-color:orange;");
        }
        else if (fillState == 3)    // Faulty
        {
            PowerManagementMainUi.battery1Fill->
                setStyleSheet("background-color:red;");
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
        else
        {
            PowerManagementMainUi.battery3Charging->
                setStyleSheet("background-color:lightgreen;");
            PowerManagementMainUi.battery3Charging->
                setText("F");
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
        emit this->recordMessageReceived(response);
    }
/* Messages for the Configure Task start with p or dS */
    if ((size > 0) && (firstField.left(1) == "p"))
    {
        emit this->configureMessageReceived(response);
    }
    if ((size > 0) && (firstField.left(2) == "dD"))
    {
        emit this->configureMessageReceived(response);
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
The entry is sent to the monitor window.
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
    emit this->monitorMessageReceived(entry);
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
/** @brief Obtain a save file name and path and attempt to open it.

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
    QString filename = QFileDialog::getSaveFileName(this,
                        "Acquisition Save Acquired Data",
                        QString(),
                        "Comma Separated Variables (*.csv)",0,0);
    if (filename.isEmpty()) return;
    if (! filename.endsWith(".csv")) filename.append(".csv");
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
/** @brief Close the save file.

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
//! Save the name to prevent the same file being used.
        saveFile = QString();
    }
}
//-----------------------------------------------------------------------------
/** @brief Call up the Recording Window.

*/

void PowerManagementGui::on_recordingButton_clicked()
{
    PowerManagementRecordGui* powerManagementRecordForm =
                    new PowerManagementRecordGui(socket,this);
    powerManagementRecordForm->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(recordMessageReceived(const QString&)),
                    powerManagementRecordForm, SLOT(onMessageReceived(const QString&)));
    powerManagementRecordForm->exec();
}

//-----------------------------------------------------------------------------
/** @brief Call up the Monitor Window.

@Note The monitor window is created without a parent to ensure it can be
placed to the background.
*/

void PowerManagementGui::on_monitorButton_clicked()
{
    PowerManagementMonitorGui* powerManagementMonitorForm =
                    new PowerManagementMonitorGui(socket,NULL);   
    powerManagementMonitorForm->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(monitorMessageReceived(const QString&)),
                  powerManagementMonitorForm, SLOT(onMessageReceived(const QString&)));
    powerManagementMonitorForm->setModal(false);
    powerManagementMonitorForm->show();
}

//-----------------------------------------------------------------------------
/** @brief Call up the Configure Window.

*/

void PowerManagementGui::on_configureButton_clicked()
{
    PowerManagementConfigGui* powerManagementConfigForm =
                    new PowerManagementConfigGui(socket,this);
    powerManagementConfigForm->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, SIGNAL(configureMessageReceived(const QString&)),
                    powerManagementConfigForm, SLOT(onMessageReceived(const QString&)));
    powerManagementConfigForm->exec();
}

//-----------------------------------------------------------------------------
/** @brief Initiate AutoTrack Disable Controls.

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
/** @brief Show an error condition in the Error label.

@param[in] message: Message to be displayed.
*/

void PowerManagementGui::displayErrorMessage(const QString message)
{
    PowerManagementMainUi.errorLabel->setText(message);
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
// TCP specific functions

#ifndef SERIAL
//-----------------------------------------------------------------------------
/** @brief Check if a socket is valid, that is, connected.
*/
bool PowerManagementGui::validsocket()
{
    if (socket == NULL)
    {
        QMessageBox msgBox;
        msgBox.open();
        msgBox.setText("Not yet connected. Click the connect button.");
        ssleep(20);
        msgBox.close();
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
/** @brief Attempt to connect to the remote system.

*/
void PowerManagementGui::on_connectButton_clicked()
{
    if (socket != NULL) delete socket;
// Create the TCP socket to the internet process
    socket = new QTcpSocket(this);
// Setup QT signal/slots for reading and error handling
// The readyRead signal from the QAbstractSocket is linked to the onDataAvailable slot
    connect(socket, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
// Connect to the host
    blockSize = 0;
// Pull in all the information table
    QMessageBox msgBox;
    msgBox.open();
    msgBox.setText("Attempting to connect. Please wait.");
    int count;
    for (count=0; count<100;count++)
    {
        ssleep(1);
        socket->abort();
        socket->connectToHost(connectAddress, connectPort);
        if (socket->waitForConnected(1000)) break;
    }
    if (count >= 100) 
    {
        msgBox.setText("Timeout while attempting to access remote system.");
        ssleep(20);
    }
    msgBox.close();
// Ask for the remote system switch settings (process later)
    if (validsocket()) socket->write("dS\n\r");
}
#endif

