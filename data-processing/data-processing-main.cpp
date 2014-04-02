/*       Power Management Data Processing Main Window

@date 22 March 2014
*/

/****************************************************************************
 *   Copyright (C) 2013 by Ken Sarkies                                      *
 *   ksarkies@trinity.asn.au                                                *
 *                                                                          *
 *   This file is part of Power Management                                  *
 *                                                                          *
 *   Power Management is free software; you can redistribute it and/or      *
 *   modify it under the terms of the GNU General Public License as         *
 *   published by the Free Software Foundation; either version 2 of the     *
 *   License, or (at your option) any later version.                        *
 *                                                                          *
 *   Power Management is distributed in the hope that it will be useful,    *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with Power Management if not, write to the                       *
 *   Free Software Foundation, Inc.,                                        *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.              *
 ***************************************************************************/


#include "data-processing-main.h"
#include <QApplication>
#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QTextEdit>
#include <QCloseEvent>
#include <QFileDialog>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

//-----------------------------------------------------------------------------
/** Power Management Data Processing Main Window Constructor

@param[in] parent Parent widget.
*/

DataProcessingGui::DataProcessingGui()
{
// Build the User Interface display from the Ui class in ui_mainwindowform.h
    DataProcessingMainUi.setupUi(this);
// Build the record type list
    recordType << "pH"  << "dT"  << "dD" << "ds";
    recordType << "dB1" << "dB2" << "dB3";
    recordType << "dC1" << "dC2" << "dC3";
    recordType << "dO1" << "dO2" << "dO3";
    recordType << "dL1" << "dL2" << "dM1";
    recordText << "Time" << "Temperature" << "Controls" << "Switch Setting";
    recordText << "Battery 1" << "Battery 2" << "Battery 3";
    recordText << "Charge State 1" << "Charge State 2" << "Charge State 3";
    recordText << "Charge Phase 1" << "Charge Phase 2" << "Charge Phase 3";
    recordText << "Load 1" << "Load 2" << "Panel";
    DataProcessingMainUi.recordType_1->addItem("None");
    DataProcessingMainUi.recordType_2->addItem("None");
    DataProcessingMainUi.recordType_3->addItem("None");
    for (int n=0; n<recordType.size(); n++)
    {
        DataProcessingMainUi.recordType_1->addItem(recordText[n]);
        DataProcessingMainUi.recordType_2->addItem(recordText[n]);
        DataProcessingMainUi.recordType_3->addItem(recordText[n]);
    }
    DataProcessingMainUi.intervalSpinBox->setMinimum(1);
    DataProcessingMainUi.intervalType->addItem("Average");
    DataProcessingMainUi.intervalType->addItem("Maximum");
    DataProcessingMainUi.intervalType->addItem("Sample");
}

DataProcessingGui::~DataProcessingGui()
{
}

//-----------------------------------------------------------------------------
/** @brief Successful establishment of Window

@returns TRUE if successful.
*/
bool DataProcessingGui::success()
{
    return true;
}

//-----------------------------------------------------------------------------
/** @brief Extract All to CSV.

All data is extracted to a csv file with one record per time interval. Format
suitable for spreadsheet analysis.
*/

void DataProcessingGui::on_dumpAllButton_clicked()
{
    int battery1Voltage = -1;
    int battery1Current = 0;
    int battery1Capacity = -1;
    QString battery1StateText;
    QString battery1FillText;
    QString battery1ChargeText;
    int battery2Voltage = -1;
    int battery2Current = 0;
    int battery2Capacity = -1;
    QString battery2StateText;
    QString battery2FillText;
    QString battery2ChargeText;
    int battery3Voltage = -1;
    int battery3Current = 0;
    int battery3Capacity = -1;
    QString battery3StateText;
    QString battery3FillText;
    QString battery3ChargeText;
    int load1Voltage = -1;
    int load1Current = 0;
    int load2Voltage = -1;
    int load2Current = 0;
    int panel1Voltage = -1;
    int panel1Current = 0;
    int temperature = -1;
    QString controls = "     ";
    QString switches;
    int decision = -1;
    int debug1a = -1;
    int debug2a = -1;
    int debug3a = -1;
    int debug1b = -1;
    int debug2b = -1;
    int debug3b = -1;
    if (! inFile->isOpen()) return;
    if (! openSaveFile()) return;
    inFile->seek(0);      // rewind file
    QTextStream inStream(inFile);
    bool blockStart = false;
    QTextStream outStream(outFile);
    outStream << "Time,";
    outStream << "B1 I," << "B1 V," << "B1 Cap," << "B1 Op," << "B1 State," << "B1 Charge,";
    outStream << "B2 I," << "B2 V," << "B2 Cap," << "B2 Op," << "B2 State," << "B2 Charge,";
    outStream << "B3 I," << "B3 V," << "B3 Cap," << "B3 Op," << "B3 State," << "B3 Charge,";
    outStream << "L1 I," << "L1 V," << "L2 I," << "L2 V," << "M1 I," << "M1 V,";
    outStream << "Temp," << "Controls," << "Switches," << "Decisions,";
    outStream << "Debug 1a," << "Debug 1b," << "Debug 2a," << "Debug 2b," << "Debug 3a," << "Debug 3b";
    outStream << "\n\r";
    while (! inStream.atEnd())
    {
      	QString lineIn = inStream.readLine();
        QStringList breakdown = lineIn.split(",");
        int size = breakdown.size();
        if (size <= 0) break;
        QString firstText = breakdown[0].simplified();
        QString secondText;
        QString thirdText;
        int secondField = -1;
        if (size > 1)
        {
            secondText = breakdown[1].simplified();
            secondField = secondText.toInt();
        }
        int thirdField = -1;
        if (size > 2)
        {
            thirdText = breakdown[2].simplified();
            thirdField = thirdText.toInt();
        }
        if (size > 1)
        {
// Find and extract the time record
            if (firstText == "pH")
            {
                if (blockStart)
                {
                    outStream << timeRecord << ",";
                    outStream << (float)battery1Voltage/256 << ",";
                    outStream << (float)battery1Current/256 << ",";
                    outStream << (float)battery1Capacity/256 << ",";
                    outStream << battery1StateText << ",";
                    outStream << battery1FillText << ",";
                    outStream << battery1ChargeText << ",";
                    outStream << (float)battery2Voltage/256 << ",";
                    outStream << (float)battery2Current/256 << ",";
                    outStream << (float)battery2Capacity/256 << ",";
                    outStream << battery2StateText << ",";
                    outStream << battery2FillText << ",";
                    outStream << battery2ChargeText << ",";
                    outStream << (float)battery3Voltage/256 << ",";
                    outStream << (float)battery3Current/256 << ",";
                    outStream << (float)battery3Capacity/256 << ",";
                    outStream << battery3StateText << ",";
                    outStream << battery3FillText << ",";
                    outStream << battery3ChargeText << ",";
                    outStream << (float)load1Voltage/256 << ",";
                    outStream << (float)load1Current/256 << ",";
                    outStream << (float)load2Voltage/256 << ",";
                    outStream << (float)load2Current/256 << ",";
                    outStream << (float)panel1Voltage/256 << ",";
                    outStream << (float)panel1Current/256 << ",";
                    outStream << (float)temperature/256 << ",";
                    outStream << controls << ",";
                    outStream << switches << ",";
                    outStream << decision << ",";
                    outStream << debug1a << ",";
                    outStream << debug1b << ",";
                    outStream << debug2a << ",";
                    outStream << debug2b << ",";
                    outStream << debug3a << ",";
                    outStream << debug3b;
                    outStream << "\n\r";
                }
                timeRecord = breakdown[1].simplified();
                blockStart = true;
            }
            if (firstText == "dB1")
            {
                battery1Voltage = secondField;
                battery1Current = thirdField;
            }
            if (firstText == "dB2")
            {
                battery2Voltage = secondField;
                battery2Current = thirdField;
            }
            if (firstText == "dB3")
            {
                battery3Voltage = secondField;
                battery3Current = thirdField;
            }
            if (firstText == "dC1")
            {
                battery1Capacity = secondField;
            }
            if (firstText == "dC2")
            {
                battery2Capacity = secondField;
            }
            if (firstText == "dC3")
            {
                battery3Capacity = secondField;
            }
            if (firstText == "dO1")
            {
                uint battery1State = (secondField & 0x03);
                if (battery1State == 0) battery1StateText = "Loaded";
                else if (battery1State == 1) battery1StateText = "Charge";
                else if (battery1State == 2) battery1StateText = "Isolate";
                else if (battery1State == 3) battery1StateText = "Missing";
                else battery1StateText = "?";
                uint battery1Fill = (secondField >> 2) & 0x03;
                if (battery1Fill == 0) battery1FillText = "Normal";
                else if (battery1Fill == 1) battery1FillText = "Low";
                else if (battery1Fill == 2) battery1FillText = "Critical";
                else if (battery1Fill == 3) battery1FillText = "Faulty";
                else battery1FillText = "?";
                uint battery1Charge = (secondField >> 4) & 0x03;
                if (battery1Charge == 0) battery1ChargeText = "Bulk";
                else if (battery1Charge == 1) battery1ChargeText = "Absorp";
                else if (battery1Charge == 2) battery1ChargeText = "Float";
                else battery1ChargeText = "?";
            }
            if (firstText == "dO2")
            {
                uint battery2State = (secondField & 0x03);
                if (battery2State == 0) battery2StateText = "Loaded";
                else if (battery2State == 1) battery2StateText = "Charge";
                else if (battery2State == 2) battery2StateText = "Isolate";
                else if (battery2State == 3) battery2StateText = "Missing";
                else battery2StateText = "?";
                uint battery2Fill = (secondField >> 2) & 0x03;
                if (battery2Fill == 0) battery2FillText = "Normal";
                else if (battery2Fill == 1) battery2FillText = "Low";
                else if (battery2Fill == 2) battery2FillText = "Critical";
                else battery2FillText = "?";
                uint battery2Charge = (secondField >> 4) & 0x03;
                if (battery2Charge == 0) battery2ChargeText = "Bulk";
                else if (battery2Charge == 1) battery2ChargeText = "Absorp";
                else if (battery2Charge == 2) battery2ChargeText = "Float";
                else battery2ChargeText = "?";
            }
            if (firstText == "dO3")
            {
                uint battery3State = (secondField & 0x03);
                if (battery3State == 0) battery3StateText = "Loaded";
                else if (battery3State == 1) battery3StateText = "Charge";
                else if (battery3State == 2) battery3StateText = "Isolate";
                else if (battery3State == 3) battery3StateText = "Missing";
                else battery3StateText = "?";
                uint battery3Fill = (secondField >> 2) & 0x03;
                if (battery3Fill == 0) battery3FillText = "Normal";
                else if (battery3Fill == 1) battery3FillText = "Low";
                else if (battery3Fill == 2) battery3FillText = "Critical";
                else battery3FillText = "?";
                uint battery3Charge = (secondField >> 4) & 0x03;
                if (battery3Charge == 0) battery3ChargeText = "Bulk";
                else if (battery3Charge == 1) battery3ChargeText = "Absorp";
                else if (battery3Charge == 2) battery3ChargeText = "Float";
                else battery3ChargeText = "?";
            }
            if (firstText == "dL1")
            {
                load1Voltage = secondField;
                load1Current = thirdField;
            }
            if (firstText == "dL2")
            {
                load2Voltage = secondField;
                load2Current = thirdField;
            }
            if (firstText == "dM1")
            {
                panel1Voltage = secondField;
                panel1Current = thirdField;
            }
            if (firstText == "dT")
            {
                temperature = secondField;
            }
// A = autotrack, R = recording, X = switch avoidance, M = send measurements,
// D = debug
            if (firstText == "dD")
            {
                if ((secondField & (1 << 0)) > 0) controls[0] = 'A';
                if ((secondField & (1 << 1)) > 0) controls[1] = 'R';
                if ((secondField & (1 << 2)) > 0) controls[2] = 'X';
                if ((secondField & (1 << 3)) > 0) controls[3] = 'M';
                if ((secondField & (1 << 4)) > 0) controls[4] = 'D';
            }
// Switch control bits - three 2-bit fields: battery number for each of
// load1, load2 and panel.
            if (firstText == "ds")
            {
                switches.clear();
                uint load1Battery = (secondField >> 0) & 0x03;
                QString load1BatteryText;
                load1BatteryText.setNum(load1Battery);
                if (load1Battery > 0) switches.append(" ").append(load1BatteryText);
                else switches.append("  ");
                uint load2Battery = (secondField >> 2) & 0x03;
                QString load2BatteryText;
                load2BatteryText.setNum(load2Battery);
                if (load2Battery > 0) switches.append(" ").append(load2BatteryText);
                else switches.append("  ");
                uint panelBattery = (secondField >> 4) & 0x03;
                QString panelBatteryText;
                panelBatteryText.setNum(panelBattery);
                if (panelBattery > 0) switches.append(" ").append(panelBatteryText);
                else switches.append("  ");
            }
            if (firstText == "dd")
            {
                decision = secondField;
            }
            if (firstText == "D1")
            {
                debug1a = secondField;
                if (size > 2) debug1b = thirdField;
            }
            if (firstText == "D2")
            {
                debug2a = secondField;
                if (size > 2) debug2b = thirdField;
            }
            if (firstText == "D3")
            {
                debug3a = secondField;
                if (size > 2) debug3b = thirdField;
            }
        }
    }    
    if (saveFile.isEmpty())
        displayErrorMessage("File already closed");
    else
    {
        outFile->close();
        delete outFile;
//! Clear the name to prevent the same file being used.
        saveFile = QString();
    }
}

//-----------------------------------------------------------------------------
/** @brief Extract Data.

Up to three data sets specified are extracted and written to a file for plotting.
An interval is specified over which data may be taken as the first sample, the
maximum or the average. The time over which the extraction occurs can be
specified.
*/

void DataProcessingGui::on_extractButton_clicked()
{
    if (! inFile->isOpen()) return;
    if (! openSaveFile()) return;
    inFile->seek(0);      // rewind file
    int interval = DataProcessingMainUi.intervalSpinBox->value();
    int intervaltype = DataProcessingMainUi.intervalType->currentIndex();
    QTextStream inStream(inFile);
    QDateTime startTime = DataProcessingMainUi.startTime->dateTime();
    QDateTime endTime = DataProcessingMainUi.endTime->dateTime();
    QDateTime time;
    while (! inStream.atEnd())
    {
      	QString lineIn = inStream.readLine();
        QStringList breakdown = lineIn.split(",");
        int size = breakdown.size();
        if (size <= 0) break;
        QString firstText = breakdown[0].simplified();
        int secondField = 0;
        int thirdField = 0;
// Extract the time record for time range comparison
        if (size > 1)
        {
            if ((firstText == "pH") && (size > 1))
            {
                time = QDateTime::fromString(breakdown[1].simplified(),Qt::ISODate);
            }
            else
            {
                secondField = breakdown[1].simplified().toInt();
            }
        }
        if (size > 2) thirdField = breakdown[2].simplified().toInt();
// Extract records
        if ((time >= startTime) && (time <= endTime))
        {
            int recordType_1 = DataProcessingMainUi.recordType_1->currentIndex();
            int recordType_2 = DataProcessingMainUi.recordType_2->currentIndex();
            int recordType_3 = DataProcessingMainUi.recordType_3->currentIndex();
// Find the relevant records and extract their fields
            if ((recordType_1 > 0) && (firstText == recordType[recordType_1-1]))
            {
                qDebug() << breakdown[0].simplified() << breakdown[1].simplified();
            }
            if ((recordType_2 > 0) && (firstText == recordType[recordType_2-1]))
                qDebug() << breakdown[0].simplified() << breakdown[1].simplified();
            if ((recordType_3 > 0) && (firstText == recordType[recordType_3-1]))
                qDebug() << breakdown[0].simplified() << breakdown[1].simplified();
        }
    }
    if (saveFile.isEmpty())
        displayErrorMessage("File already closed");
    else
    {
        outFile->close();
        delete outFile;
//! Clear the name to prevent the same file being used.
        saveFile = QString();
    }
}

//-----------------------------------------------------------------------------
/** @brief Open a data file for Writing.

This is called from other action functions. The file is requested in a file
dialogue and opened. The function aborts if the file exists.

@returns true if file successfully created and opened.
*/

bool DataProcessingGui::openSaveFile()
{
    if (! saveFile.isEmpty())
    {
        displayErrorMessage("A save file is already open - close first");
        return false;
    }
    QString filename = QFileDialog::getSaveFileName(this,
                        "Save csv Data",
                        QString(),
                        "Comma Separated Variables (*.csv)",0,0);
    if (filename.isEmpty()) return false;
    if (! filename.endsWith(".csv")) filename.append(".csv");
    QFileInfo fileInfo(filename);
    saveDirectory = fileInfo.absolutePath();
    saveFile = saveDirectory.filePath(filename);
    outFile = new QFile(saveFile);             // Open file for output
    if (! outFile->open(QIODevice::WriteOnly))
    {
        displayErrorMessage("Could not open the output file");
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
/** @brief Open a data file for Reading.

*/

void DataProcessingGui::on_openReadFileButton_clicked()
{
    QString errorMessage;
    QFileInfo fileInfo;
    QString filename = QFileDialog::getOpenFileName(this,
                                "Data File","./","Text Files (*.txt)");
    if (filename.isEmpty())
    {
        displayErrorMessage("No filename specified");
        return;
    }
    inFile = new QFile(filename);
    fileInfo.setFile(filename);
    if (inFile->open(QIODevice::ReadOnly))
    {
        scanFile(inFile);
    }
    else
    {
        displayErrorMessage(QString("%1").arg((uint)inFile->error()));
    }
}

//-----------------------------------------------------------------------------
/** @brief Scan the data file for start and end times and record types.

*/

void DataProcessingGui::scanFile(QFile* inFile)
{
    if (! inFile->isOpen()) return;
    QTextStream inStream(inFile);
    QDateTime startTime, endTime;
    while (! inStream.atEnd())
    {
      	QString lineIn = inStream.readLine();
        QStringList breakdown = lineIn.split(",");
        int length = breakdown.size();
        if (length <= 0) break;
        QString firstText = breakdown[0].simplified();
        if ((firstText == "pH") && (length > 1))
        {
            QDateTime time = QDateTime::fromString(breakdown[1].simplified(),Qt::ISODate);
            if (startTime.isNull()) startTime = time;
            endTime = time;
        }
    }
    if (! startTime.isNull()) DataProcessingMainUi.startTime->setDateTime(startTime);
    if (! endTime.isNull()) DataProcessingMainUi.endTime->setDateTime(endTime);
}

//-----------------------------------------------------------------------------
/** @brief Print an error message.

*/

void DataProcessingGui::displayErrorMessage(QString message)
{
}

