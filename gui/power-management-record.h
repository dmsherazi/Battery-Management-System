/*          Power Management GUI Recording Window Header

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

#ifndef POWER_MANAGEMENT_RECORD_H
#define POWER_MANAGEMENT_RECORD_H
#define _TTY_POSIX_         // Need to tell qextserialport we are in POSIX

#include "power-management.h"
#include "ui_power-management-record.h"
#include "serialport.h"
#include <QDialog>
#include <QStandardItemModel>
#include <QtNetwork>
#include <QTcpSocket>

//-----------------------------------------------------------------------------
/** @brief Power Management Recording Window.

*/

class PowerManagementRecordGui : public QDialog
{
    Q_OBJECT
public:
#ifdef SERIAL
    PowerManagementRecordGui(SerialPort* socket, QWidget* parent = 0);
#else
    PowerManagementRecordGui(QTcpSocket* socket, QWidget* parent = 0);
#endif
    ~PowerManagementRecordGui();
private slots:
    void on_deleteButton_clicked();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_closeFileButton_clicked();
    void on_recordFileButton_clicked();
    void onMessageReceived(const QString &text);
    void onListItemClicked(const QModelIndex & index);
    void on_registerButton_clicked();
    void on_closeButton_clicked();
private:
// User Interface object instance
    Ui::PowerManagementRecordDialog PowerManagementRecordUi;
#ifdef SERIAL
    SerialPort *socket;           //!< Serial port object pointer
#else
    QTcpSocket *socket;
#endif
    int extractValue(const QString &response);
    void requestRecordingStatus();
    void refreshDirectory();
    void getFreeSpace();
    int writeFileHandle;
    int readFileHandle;
    bool recordingOn;
    bool writeFileOpen;
    bool readFileOpen;
    QStandardItemModel *model;
    int row;
    bool directoryEnded;
    bool nextDirectoryEntry;

};

#endif
