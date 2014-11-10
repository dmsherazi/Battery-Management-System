/*          Power Management GUI Configure Window Header

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

#ifndef POWER_MANAGEMENT_CONFIGURE_H
#define POWER_MANAGEMENT_CONFIGURE_H
#define _TTY_POSIX_         // Need to tell qextserialport we are in POSIX

#include "power-management.h"
#include "ui_power-management-configure.h"
#include "serialport.h"
#include <QDialog>
#include <QtNetwork>
#include <QTcpSocket>

//-----------------------------------------------------------------------------
/** @brief Power Management Configure Window.

*/

class PowerManagementConfigGui : public QDialog
{
    Q_OBJECT
public:
#ifdef SERIAL
    PowerManagementConfigGui(SerialPort*, QWidget* parent = 0);
#else
    PowerManagementConfigGui(QTcpSocket* socket, QWidget* parent = 0);
#endif
    ~PowerManagementConfigGui();
    QString error();
private slots:
    void onMessageReceived(const QString &text);
    void on_calibrateButton_clicked();
    void on_queryBatteryButton_clicked();
    void on_timeSetButton_clicked();
    void on_closeButton_clicked();
    void on_setBatteryButton_clicked();
    void displayErrorMessage(const QString message);
    void on_setTrackOptionButton_clicked();
    void on_threePhaseButton_clicked();
    void on_pulseButton_clicked();
    void on_debugMessageCheckbox_clicked();
    void on_dataMessageCheckbox_clicked();
    void on_echoTestButton_clicked();
    void on_resetMissing1Button_clicked();
    void on_resetMissing2Button_clicked();
    void on_resetMissing3Button_clicked();
private:
// User Interface object instance
    Ui::PowerManagementConfigDialog PowerManagementConfigUi;
#ifdef SERIAL
    SerialPort *socket;           //!< Serial port object pointer
#else
    QTcpSocket *socket;
#endif
    QString errorMessage;
    QString response;           // String to build a line of characters
    QString quiescentCurrent;
};

#endif
