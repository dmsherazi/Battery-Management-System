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
/** @brief Scan the data file for start and end times and record types.

*/

void DataProcessingGui::scanFile(QFile* file)
{
    QTextStream stream(file);
    QDateTime startTime, endTime;
    while (! stream.atEnd())
    {
      	QString line = stream.readLine();
        QStringList breakdown = line.split(",");
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
/** @brief Extract Data.

*/

void DataProcessingGui::on_extractButton_clicked()
{
    file.seek(0);      // rewind file
    int interval = DataProcessingMainUi.intervalSpinBox->value();
    int intervaltype = DataProcessingMainUi.intervalType->currentIndex();
    QTextStream stream(&file);
    QDateTime startTime = DataProcessingMainUi.startTime->dateTime();
    QDateTime endTime = DataProcessingMainUi.endTime->dateTime();
    QDateTime time = startTime;
    while (! stream.atEnd())
    {
      	QString line = stream.readLine();
        QStringList breakdown = line.split(",");
        int length = breakdown.size();
        if (length <= 0) break;
        QString firstField = breakdown[0].simplified();
        if ((firstField == "pH") && (length > 1))
        {
            time = QDateTime::fromString(breakdown[1].simplified(),Qt::ISODate);
        }
        if ((time >= startTime) && (time <= endTime))
        {
            int recordType_1 = DataProcessingMainUi.recordType_1->currentIndex();
            int recordType_2 = DataProcessingMainUi.recordType_2->currentIndex();
            int recordType_3 = DataProcessingMainUi.recordType_3->currentIndex();
            if ((recordType_1 > 0) && (firstField == recordType[recordType_1-1]))
                qDebug() << breakdown[0].simplified() << breakdown[1].simplified();
            if ((recordType_2 > 0) && (firstField == recordType[recordType_2-1]))
                qDebug() << breakdown[0].simplified() << breakdown[1].simplified();
            if ((recordType_3 > 0) && (firstField == recordType[recordType_3-1]))
                qDebug() << breakdown[0].simplified() << breakdown[1].simplified();
        }
    }
}

//-----------------------------------------------------------------------------
/** @brief Open a data file.

*/

void DataProcessingGui::on_openFileButton_clicked()
{
    QString errorMessage;
    QString filename = QFileDialog::getOpenFileName(this,
                                "Data File","./","Text Files (*.txt)");
    if (! filename.isEmpty())
    {
        file.setFileName(filename);
        fileInfo.setFile(file);
        if (file.open(QIODevice::ReadOnly))
        {
            scanFile(&file);
        }
        else
        {
            errorMessage = QString("%1").arg((uint)file.error());
        }
    }
    else errorMessage = "No filename specified";
    if (! errorMessage.isEmpty()) QMessageBox::critical(this,"Open Failure",errorMessage);
}

