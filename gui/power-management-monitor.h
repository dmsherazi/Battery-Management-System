/*          Power Management GUI Monitor Window Header

@date 29 October 2013
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

#ifndef POWER_MANAGEMENT_MONITOR_H
#define POWER_MANAGEMENT_MONITOR_H
#define _TTY_POSIX_         // Need to tell qextserialport we are in POSIX

#include "power-management.h"
#include "ui_power-management-monitor.h"
#include "serialport.h"
#include <qwt_plot.h>
#include <QDialog>
#include <QtNetwork>
#include <QTcpSocket>

#define NUMBER_POINTS   600
#define VISIBLE_POINTS  100
#define JUMP             20

class QwtPlotCurve;
class QwtPlotDirectPainter;

//-----------------------------------------------------------------------------
/** @brief Power Management Monitor Window.

*/

class PowerManagementMonitorGui : public QDialog
{
    Q_OBJECT
public:
#ifdef SERIAL
    PowerManagementMonitorGui(SerialPort* socket, QWidget* parent = 0);
#else
    PowerManagementMonitorGui(QTcpSocket* socket, QWidget* parent = 0);
#endif
    ~PowerManagementMonitorGui();
private slots:
    void onMessageReceived(const QString &entry);
    void on_sourceComboBox1_currentIndexChanged(int index);
    void on_offsetSlider1_valueChanged(int value);
    void on_scaleSlider1_valueChanged(int value);
    void on_sourceComboBox2_currentIndexChanged(int index);
    void on_offsetSlider2_valueChanged(int value);
    void on_scaleSlider2_valueChanged(int value);
    void on_xoffsetSlider_valueChanged(int value);
    void on_sampleSpinBox_valueChanged(int value);
    void on_closeButton_clicked();
private:
// User Interface object instance
    Ui::PowerManagementMonitorDialog PowerManagementMonitorUi;
#ifdef SERIAL
    SerialPort *socket;           //!< Serial port object pointer
#else
    QTcpSocket *socket;
#endif
    void replot(int plotStartIndex, int plotEnd);
    QwtPlotCurve *d_curve1, *d_curve2;
    QwtPlotDirectPainter *d_directPainter1, *d_directPainter2;
    int xoffset;
    int xSamples;
    float xRangeMax;
    float xRangeMin;
    float xData[NUMBER_POINTS];
    float yDataB1Current[NUMBER_POINTS];
    float yDataB1Voltage[NUMBER_POINTS];
    float yDataB2Current[NUMBER_POINTS];
    float yDataB2Voltage[NUMBER_POINTS];
    float yDataB3Current[NUMBER_POINTS];
    float yDataB3Voltage[NUMBER_POINTS];
    float yDataL1Current[NUMBER_POINTS];
    float yDataL1Voltage[NUMBER_POINTS];
    float yDataL2Current[NUMBER_POINTS];
    float yDataL2Voltage[NUMBER_POINTS];
    float yDataM1Current[NUMBER_POINTS];
    float yDataM1Voltage[NUMBER_POINTS];
    long long xindex;
    long long lastIndex;
    int plotStartIndex, plotEnd;

    float yScaleBase1;
    float yScale1;
    float yOffsetBase1;
    float yOffset1;
    float yRangeMax1;
    float yRangeMin1;
    float* source1;

    float yScaleBase2;
    float yScale2;
    float yOffsetBase2;
    float yOffset2;
    float yRangeMax2;
    float yRangeMin2;
    float* source2;
};

#endif
