/*          Power Management GUI Main Window Header

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

#ifndef POWER_MANAGEMENT_MAIN_H
#define POWER_MANAGEMENT_MAIN_H

// Parameters of the voltage and current amplifiers
#define R9 20
#define R7 10
#define R4 10
#define R5 2.2
#define Vref 2.5
#define Ioffset 3.3/2
#define Iscale 10

#define Voffset R9*Vref/R5
#define Vscale (1+R4/R5)/(1+R9/R7)

#include "ui_power-management.h"
#include "power-management.h"
#include "serialport.h"
#include <QDir>
#include <QFile>
#include <QTime>
#include <QListWidgetItem>
#include <QDialog>
#include <QCloseEvent>
#include <QStandardItemModel>

typedef enum {battery1UnderVoltage, battery2UnderVoltage, battery3UnderVoltage, 
              battery1OverCurrent, battery2OverCurrent, battery3OverCurrent,
              load1UnderVoltage, load2UnderVoltage, panelUnderVoltage, 
              load1OverCurrent, load2OverCurrent, panelOverCurrent, }
              IndicatorType;

#define millisleep(a) usleep(a*1000)

//-----------------------------------------------------------------------------
/** @brief Power Management Main Window.

*/

class PowerManagementGui : public QDialog
{
    Q_OBJECT
public:
    PowerManagementGui(QWidget* parent = 0);
    ~PowerManagementGui();
    bool success();
    QString error();
protected:
private slots:
    void tabChanged(int index);
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
    void on_load1PushButton_clicked();
    void on_load2PushButton_clicked();
    void on_panelPushButton_clicked();
    void on_battery1PushButton_clicked();
    void on_battery2PushButton_clicked();
    void on_battery3PushButton_clicked();
    void on_battery1OverCurrent_clicked();
    void on_battery2OverCurrent_clicked();
    void on_battery3OverCurrent_clicked();
    void on_load1OverCurrent_clicked();
    void on_load2OverCurrent_clicked();
    void on_panelOverCurrent_clicked();
    void on_battery1SoCReset_clicked();
    void on_battery2SoCReset_clicked();
    void on_battery3SoCReset_clicked();
    bool testIndicator(const IndicatorType indicator);
    void on_autoTrackPushButton_clicked();
    void disableRadioButtons(bool enable);
    void on_shutdownButton_clicked();
// Configuration
    void on_timeSetButton_clicked();
    void on_debugMessageCheckbox_clicked();
    void on_dataMessageCheckbox_clicked();
    void on_echoTestButton_clicked();
    void on_queryBatteryButton_clicked();
    void on_resetMissing1Button_clicked();
    void on_resetMissing2Button_clicked();
    void on_resetMissing3Button_clicked();
    void on_forceZeroCurrent1_clicked();
    void on_forceZeroCurrent2_clicked();
    void on_forceZeroCurrent3_clicked();
    void initCalibration();
    void on_connectButton_clicked();
    void on_calibrateButton_clicked();
    void on_setTrackOptionButton_clicked();
    void on_absorptionMuteCheckbox_clicked();
    void configureMessageReceived(const QString text);
// Recording
    void on_newFileButton_clicked();
    void onListItemClicked(const QModelIndex & index);
    void on_recordFileButton_clicked();
    void on_startRecordingButton_clicked();
    void on_stopRecordingButton_clicked();
    void on_closeRecordingFileButton_clicked();
    void on_registerButton_clicked();
    void recordMessageReceived(const QString text);
    void on_refreshListButton_clicked();
private:
// User Interface object instance
    Ui::PowerManagementDialog PowerManagementMainUi;
    uint baudrate;
    bool synchronized;
    QString errorMessage;
    QString response;
    SerialPort* socket;           //!< Serial port object pointer
    quint16 blockSize;
    QTime tick;
    int load1Current;
    int load1Voltage;
    unsigned int indicators;
    void initGui();
    void processResponse(const QString response);
    void getCurrentVoltage(const QStringList breakdown,QString* sVoltage,
                           QString* sCurrent);
    void displayErrorMessage(const QString message);
    void ssleep(int seconds);
    char timeTick;
    QString quiescentCurrent;
    int extractValue(const QString &response);
    bool writeFileOpen;
    void initRecording();
    void refreshDirectory();
    void getFreeSpace();
    void requestRecordingStatus();
    int writeFileHandle;
    int readFileHandle;
    bool recordingOn;
    bool recordingInitialised;
    bool configInitialised;
    QStandardItemModel *model;
    int rowCount;
    QLineEdit* lineEditObject;
};

#endif
