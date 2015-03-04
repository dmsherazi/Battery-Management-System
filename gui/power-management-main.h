/*          Power Management GUI Main Window Header

@date 6 October 2013
*/

/****************************************************************************
 *   Copyright (C) 2013 by Ken Sarkies                                      *
 *   ksarkies@internode.on.net                                              *
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

#ifndef POWER_MANAGEMENT_MAIN_H
#define POWER_MANAGEMENT_MAIN_H

#include "ui_power-management-main.h"
#include "power-management.h"
#include "serialport.h"
#include <QTcpSocket>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QListWidgetItem>
#include <QDialog>
#include <QCloseEvent>

typedef enum {battery1UnderVoltage, battery2UnderVoltage, battery3UnderVoltage, 
              battery1OverCurrent, battery2OverCurrent, battery3OverCurrent,
              load1UnderVoltage, load2UnderVoltage, panelUnderVoltage, 
              load1OverCurrent, load2OverCurrent, panelOverCurrent, }
              IndicatorType;

#define DEFAULT_SERIAL_PORT "/dev/ttyUSB0"
#define DEFAULT_BAUDRATE    4
#define DEFAULT_TCP_ADDRESS "192.168.2.14"
#define DEFAULT_TCP_PORT    6666

#define millisleep(a) usleep(a*1000)

//-----------------------------------------------------------------------------
/** @brief Power Management Main Window.

*/

class PowerManagementGui : public QDialog
{
    Q_OBJECT
public:
    PowerManagementGui(QString device, uint parameter, QWidget* parent = 0);
    ~PowerManagementGui();
    bool success();
    QString error();
private slots:
    void on_connectButton_clicked();
    void onDataAvailable();
    void on_load1Battery1_pressed();
    void on_load1Battery2_pressed();
    void on_load1Battery3_pressed();
    void on_load2Battery1_pressed();
    void on_load2Battery2_pressed();
    void on_load2Battery3_pressed();
    void on_panelBattery1_pressed();
    void on_panelBattery2_pressed();
    void on_panelBattery3_pressed();
    void on_load1CheckBox_clicked();
    void on_load2CheckBox_clicked();
    void on_panelCheckBox_clicked();
    void on_battery1OverCurrent_clicked();
    void on_battery2OverCurrent_clicked();
    void on_battery3OverCurrent_clicked();
    void on_load1OverCurrent_clicked();
    void on_load2OverCurrent_clicked();
    void on_panelOverCurrent_clicked();
    void on_battery1CheckBox_clicked();
    void on_battery2CheckBox_clicked();
    void on_battery3CheckBox_clicked();
    void on_battery1SoCReset_clicked();
    void on_battery2SoCReset_clicked();
    void on_battery3SoCReset_clicked();
    bool testIndicator(const IndicatorType indicator);
    void on_saveFileButton_clicked();
    void on_closeFileButton_clicked();
    void on_recordingButton_clicked();
    void on_monitorButton_clicked();
    void on_configureButton_clicked();
    void on_autoTrackCheckBox_clicked();
    void closeEvent(QCloseEvent*);
    void disableRadioButtons(bool enable);
signals:
    void monitorMessageReceived(const QString response);
    void recordMessageReceived(const QString response);
    void configureMessageReceived(const QString response);
private:
// User Interface object instance
    Ui::PowerManagementMainDialog PowerManagementMainUi;
// Common code
    void initMainWindow(Ui::PowerManagementMainDialog);
    void setSourceComboBox();
// Methods
    void processResponse(const QString response);
    void getCurrentVoltage(const QStringList breakdown,QString* sVoltage, QString* sCurrent);
    void displayErrorMessage(const QString message);
    void saveLine(QString line);    // Save line to a file
    void ssleep(int seconds);
// Variables
    QString serialDevice;
    uint baudrate;
    bool synchronized;
    QString connectAddress;
    quint16 connectPort;
    QString errorMessage;
    QString response;
#ifdef SERIAL
    SerialPort* socket;           //!< Serial port object pointer
#else
    QTcpSocket *socket;
#endif
    bool validsocket();
    quint16 blockSize;
    QTime tick;
    QDir saveDirectory;
    QString saveFile;
    QFile* outFile;
    int load1Current;
    int load1Voltage;
    unsigned int indicators;
    char timeTick;
};

#endif
