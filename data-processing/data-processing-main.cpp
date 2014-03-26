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
/** @brief Extract Data.

Up to three data sets specified are extracted and written to a file for plotting.
An interval is specified over which data may be taken as the first sample, the
maximum or the average. The time over which the extraction occurs can be
specified.
*/

void DataProcessingGui::on_extractButton_clicked()
{
    if (! inFile->isOpen()) return;
    inFile->seek(0);      // rewind file
    if (! saveFile.isEmpty())
    {
        displayErrorMessage("A save file is already open - close first");
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this,
                        "Save csv Data",
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
    int interval = DataProcessingMainUi.intervalSpinBox->value();
    int intervaltype = DataProcessingMainUi.intervalType->currentIndex();
    QTextStream stream(inFile);
    QDateTime startTime = DataProcessingMainUi.startTime->dateTime();
    QDateTime endTime = DataProcessingMainUi.endTime->dateTime();
    QDateTime time;
    while (! stream.atEnd())
    {
      	QString lineIn = stream.readLine();
        QStringList breakdown = lineIn.split(",");
        int size = breakdown.size();
        if (size <= 0) break;
        QString firstField = breakdown[0].simplified();
        int secondField = 0;
        int thirdField = 0;
// Extract the time record for time range comparison
        if (size > 1)
        {
            if ((firstField == "pH") && (size > 1))
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
            if ((recordType_1 > 0) && (firstField == recordType[recordType_1-1]))
            {
                qDebug() << breakdown[0].simplified() << breakdown[1].simplified();
            }
            if ((recordType_2 > 0) && (firstField == recordType[recordType_2-1]))
                qDebug() << breakdown[0].simplified() << breakdown[1].simplified();
            if ((recordType_3 > 0) && (firstField == recordType[recordType_3-1]))
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
/** @brief Open a data file.

*/

void DataProcessingGui::on_openFileButton_clicked()
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
    QTextStream stream(inFile);
    QDateTime startTime, endTime;
    while (! stream.atEnd())
    {
      	QString lineIn = stream.readLine();
        QStringList breakdown = lineIn.split(",");
        int length = breakdown.size();
        if (length <= 0) break;
        QString firstField = breakdown[0].simplified();
        if ((firstField == "pH") && (length > 1))
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

