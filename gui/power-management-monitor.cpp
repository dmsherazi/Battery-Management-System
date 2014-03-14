/*       Power Management Monitor Window

Incoming data is displayed in time graphical form.

Two plots are provided with choices of all six interfaces and voltage or
current to display. only one curve per plot is provided. Sliders are provided
to scale and offset the curves. Data is stored in arrays for 500 ticks
(usually seconds but depends on the frequency of incoming messages). The
x-axis is nominally 100 ticks.

At the beginning the curve is built up until it reaches the plot end. then
it jumps back to allow later data to be displayed.

@todo A slider to offset the x-axis. Once offset the jump effect needs to be
disabled. The view must remain stable until the slider is changed, then it
must jump to the correct position relative to the last data plotted, and
according to the slider setting. This will take into account old plot data
being deleted as time goes on.

@todo A widget to set the x-axis scale between 1 and 5 time ticks per axis tick.

Parts of QWT 6.1.0 examples "realtime" were adapted for this code.
*/
/****************************************************************************
 *   Copyright (C) 2013 by Ken Sarkies                                      *
 *   ksarkies@internode.on.net                                              *
 *                                                                          *
 *   This file is part of power-management                                  *
 *                                                                          *
 *   serial-response-tool is free software; you can redistribute it and/or  *
 *   modify it under the terms of the GNU General Public License as         *
 *   published by the Free Software Foundation; either version 2 of the     *
 *   License, or (at your option) any later version.                        *
 *                                                                          *
 *   power-management is distributed in the hope that it will be useful,    *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with serial-response-tool if not, write to the                   *
 *   Free Software Foundation, Inc.,                                        *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.              *
 ***************************************************************************/

#include "power-management-main.h"
#include "power-management-monitor.h"
#include "serialport.h"
#include <QApplication>
#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QCloseEvent>
#include <QDebug>
#include <QtNetwork>
#include <QTcpSocket>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_directpainter.h>
#include <qwt_painter.h>
#include <qwt_series_data.h>
#include <qpaintengine.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

//-----------------------------------------------------------------------------
/** @brief Curve Data Subclass of QwtArraySeriesData

In QwtSeriesData the bounding rectangle of each type of data series is
computed through qwtBoundingRect, by iterating over the entire series.
d_boundingRect is a class member of QwtSeriesData intended for caching the
resulting rectangle.

d_samples is a QVector of the samples.
*/

class CurveData: public QwtArraySeriesData<QPointF>
{
public:
    CurveData()
    {
    }

    virtual QRectF boundingRect() const
    {
        if ( d_boundingRect.width() < 0.0 ) // that is, cleared or not defined
            d_boundingRect = qwtBoundingRect( *this );

        return d_boundingRect;
    }

    inline void append( const QPointF &point )
    {
        d_samples += point;
    }

    void clear()
    {
        d_samples.clear();
        d_samples.squeeze();
        d_boundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
    }
};

//-----------------------------------------------------------------------------
/** Monitor GUI Constructor

@param[in] p TCP Socket object pointer
@param[in] parent Parent widget.
*/

#ifdef SERIAL
PowerManagementMonitorGui::PowerManagementMonitorGui(SerialPort* p, QWidget* parent)
                                                    : QDialog(parent)
{
    socket = p;
#else
PowerManagementMonitorGui::PowerManagementMonitorGui(QTcpSocket* tcpSocket, QWidget* parent)
                                                    : QDialog(parent)
{
    socket = tcpSocket;
#endif
    PowerManagementMonitorUi.setupUi(this);
    PowerManagementMonitorUi.qwtPlot1->setFrameStyle(QFrame::NoFrame);
    PowerManagementMonitorUi.qwtPlot1->setLineWidth(0);
    PowerManagementMonitorUi.qwtPlot1->plotLayout()->setAlignCanvasToScales(true);

    PowerManagementMonitorUi.qwtPlot2->setFrameStyle(QFrame::NoFrame);
    PowerManagementMonitorUi.qwtPlot2->setLineWidth(0);
    PowerManagementMonitorUi.qwtPlot2->plotLayout()->setAlignCanvasToScales(true);

    QwtPlotGrid *grid1 = new QwtPlotGrid;
    grid1->setMajorPen(Qt::gray, 0, Qt::DotLine);
    grid1->attach(PowerManagementMonitorUi.qwtPlot1);
    QwtPlotGrid *grid2 = new QwtPlotGrid;
    grid2->setMajorPen(Qt::gray, 0, Qt::DotLine);
    grid2->attach(PowerManagementMonitorUi.qwtPlot2);

    PowerManagementMonitorUi.qwtPlot1->setCanvasBackground(QColor(Qt::white));
    PowerManagementMonitorUi.qwtPlot2->setCanvasBackground(QColor(Qt::white));

    xRangeMax = 100;
    xRangeMin = 0;

    yScaleBase1 = 10;
    yOffsetBase1 = 10;
    yScale1 = yScaleBase1;
    yOffset1 = yOffsetBase1;
    yRangeMax1 = yOffset1+yScale1/2;
    yRangeMin1 = yOffset1-yScale1/2;
    PowerManagementMonitorUi.qwtPlot1->setAxisScale(QwtPlot::xBottom, xRangeMin, xRangeMax);
    PowerManagementMonitorUi.qwtPlot1->setAxisScale(QwtPlot::yLeft, yRangeMin1, yRangeMax1);
    PowerManagementMonitorUi.offsetSlider1->setSliderPosition(50);
    PowerManagementMonitorUi.scaleSlider1->setSliderPosition(3);
    PowerManagementMonitorUi.qwtPlot1->replot();

    yScaleBase2 = 5;
    yOffsetBase2 = 12.5;
    yScale2 = yScaleBase2;
    yOffset2 = yOffsetBase2;
    yRangeMax2 = yOffset2+yScale2/2;
    yRangeMin2 = yOffset2-yScale2/2;
    PowerManagementMonitorUi.qwtPlot2->setAxisScale(QwtPlot::xBottom, xRangeMin, xRangeMax);
    PowerManagementMonitorUi.qwtPlot2->setAxisScale(QwtPlot::yLeft, yRangeMin2, yRangeMax2);
    PowerManagementMonitorUi.offsetSlider2->setSliderPosition(50);
    PowerManagementMonitorUi.scaleSlider2->setSliderPosition(3);
    PowerManagementMonitorUi.rangeLabelVertical2->setText("Volts");
    PowerManagementMonitorUi.qwtPlot2->replot();

    d_directPainter1 = new QwtPlotDirectPainter(this);
    d_directPainter2 = new QwtPlotDirectPainter(this);

    if (QwtPainter::isX11GraphicsSystem())
    {
#if QT_VERSION < 0x050000
        PowerManagementMonitorUi.qwtPlot1->
            canvas()->setAttribute(Qt::WA_PaintOutsidePaintEvent, true);
        PowerManagementMonitorUi.qwtPlot2->
            canvas()->setAttribute(Qt::WA_PaintOutsidePaintEvent, true);
#endif
        PowerManagementMonitorUi.qwtPlot1->
            canvas()->setAttribute(Qt::WA_PaintOnScreen, true);
        PowerManagementMonitorUi.qwtPlot2->
            canvas()->setAttribute(Qt::WA_PaintOnScreen, true);
    }

    d_curve1 = new QwtPlotCurve("Monitor");
    d_curve1->setData(new CurveData());
    d_curve1->setStyle(QwtPlotCurve::Lines);
    d_curve1->setPen(Qt::black);
    d_curve1->attach(PowerManagementMonitorUi.qwtPlot1);

    d_curve2 = new QwtPlotCurve("Monitor");
    d_curve2->setData(new CurveData());
    d_curve2->setStyle(QwtPlotCurve::Lines);
    d_curve2->setPen(Qt::black);
    d_curve2->attach(PowerManagementMonitorUi.qwtPlot2);

    PowerManagementMonitorUi.qwtPlot1->setAutoReplot(false);
    PowerManagementMonitorUi.qwtPlot2->setAutoReplot(false);

    xindex = 0;
    plotStartIndex = 0;
    plotEnd = 0;
    xoffset = 0;
    lastIndex = 0;
    xSamples = 1;

    PowerManagementMonitorUi.sourceComboBox1->addItem("Battery 1 Current");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Battery 1 Voltage");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Battery 2 Current");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Battery 2 Voltage");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Battery 3 Current");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Battery 3 Voltage");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Load 1 Current");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Load 1 Voltage");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Load 2 Current");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Load 2 Voltage");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Panel Current");
    PowerManagementMonitorUi.sourceComboBox1->addItem("Panel Voltage");
    PowerManagementMonitorUi.sourceComboBox1->setCurrentIndex(0);

    PowerManagementMonitorUi.sourceComboBox2->addItem("Battery 1 Current");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Battery 1 Voltage");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Battery 2 Current");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Battery 2 Voltage");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Battery 3 Current");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Battery 3 Voltage");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Load 1 Current");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Load 1 Voltage");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Load 2 Current");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Load 2 Voltage");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Panel Current");
    PowerManagementMonitorUi.sourceComboBox2->addItem("Panel Voltage");
    PowerManagementMonitorUi.sourceComboBox2->setCurrentIndex(1);

    source1 = yDataB1Current;
    source2 = yDataB1Voltage;
}

PowerManagementMonitorGui::~PowerManagementMonitorGui()
{
    delete d_curve1;
    delete d_curve2;
}

//-----------------------------------------------------------------------------
/** @brief Change the Display Graph Data Series

*/
void PowerManagementMonitorGui::on_sourceComboBox1_currentIndexChanged(int index)
{
    switch (index)
    {
        case 0:
            source1 = yDataB1Current;
            break;
        case 1:
            source1 = yDataB1Voltage;
            break;
        case 2:
            source1 = yDataB2Current;
            break;
        case 3:
            source1 = yDataB2Voltage;
            break;
        case 4:
            source1 = yDataB3Current;
            break;
        case 5:
            source1 = yDataB3Voltage;
            break;
        case 6:
            source1 = yDataL1Current;
            break;
        case 7:
            source1 = yDataL1Voltage;
            break;
        case 8:
            source1 = yDataL2Current;
            break;
        case 9:
            source1 = yDataL2Voltage;
            break;
        case 10:
            source1 = yDataM1Current;
            break;
        case 11:
            source1 = yDataM1Voltage;
            break;
    }
/* Set the vertical ranges for current or voltage.
Assumes the entries alternate between current/voltage. */
    if ((index % 2) == 0)
    {
        yScaleBase1 = 10;
        yOffsetBase1 = 0;
        PowerManagementMonitorUi.rangeLabelVertical1->setText("Amps");
    }
    else /* Voltage */
    {
        yScaleBase1 = 5;
        yOffsetBase1 = 12.5;
        PowerManagementMonitorUi.rangeLabelVertical1->setText("Volts");
    }
    yScale1 = yScaleBase1;
    yOffset1 = yOffsetBase1;
    yRangeMax1 = yOffset1+yScale1/2;
    yRangeMin1 = yOffset1-yScale1/2;
    PowerManagementMonitorUi.qwtPlot1->setAxisScale(QwtPlot::yLeft, yRangeMin1, yRangeMax1);
    PowerManagementMonitorUi.offsetSlider1->setSliderPosition(50);
    PowerManagementMonitorUi.scaleSlider1->setSliderPosition(3);

/* Back fill the plot with existing data */
    on_sampleSpinBox_valueChanged(PowerManagementMonitorUi.sampleSpinBox->value());
}

void PowerManagementMonitorGui::on_sourceComboBox2_currentIndexChanged(int index)
{
    switch (index)
    {
        case 0:
            source2 = yDataB1Current;
            break;
        case 1:
            source2 = yDataB1Voltage;
            break;
        case 2:
            source2 = yDataB2Current;
            break;
        case 3:
            source2 = yDataB2Voltage;
            break;
        case 4:
            source2 = yDataB3Current;
            break;
        case 5:
            source2 = yDataB3Voltage;
            break;
        case 6:
            source2 = yDataL1Current;
            break;
        case 7:
            source2 = yDataL1Voltage;
            break;
        case 8:
            source2 = yDataL2Current;
            break;
        case 9:
            source2 = yDataL2Voltage;
            break;
        case 10:
            source2 = yDataM1Current;
            break;
        case 11:
            source2 = yDataM1Voltage;
            break;
    }
/* Set the vertical ranges for current or voltage.
Assumes the entries alternate between current/voltage. */
    if ((index % 2) == 0)
    {
        yScaleBase2 = 10;
        yOffsetBase2 = 0;
        PowerManagementMonitorUi.rangeLabelVertical2->setText("Amps");
    }
    else /* Voltage */
    {
        yScaleBase2 = 5;
        yOffsetBase2 = 12.5;
        PowerManagementMonitorUi.rangeLabelVertical2->setText("Volts");
    }
    yScale2 = yScaleBase2;
    yOffset2 = yOffsetBase2;
    yRangeMax2 = yOffset2+yScale2/2;
    yRangeMin2 = yOffset2-yScale2/2;
    PowerManagementMonitorUi.qwtPlot2->setAxisScale(QwtPlot::yLeft, yRangeMin2, yRangeMax2);
    PowerManagementMonitorUi.offsetSlider2->setSliderPosition(50);
    PowerManagementMonitorUi.scaleSlider2->setSliderPosition(3);

/* Back fill the plot with existing data */
    on_sampleSpinBox_valueChanged(PowerManagementMonitorUi.sampleSpinBox->value());
}

//-----------------------------------------------------------------------------
/** @brief Process a Message.

After a command is sent, response messages from the remote are passed here for
processing. Appropriate fields on the form are updated.

@param QString entry: A csv string with identifier and FP current and voltage.
*/

void PowerManagementMonitorGui::onMessageReceived(const QString &entry)
{
/* Update plots.
This makes an assumption that all quantities will be sent each time tick,
and that the last in the set is module 1. Collect all data first then plot
the selected series. Some data will come through that is ignored. */
    QStringList breakdown = entry.split(",");
    float current = 0;
    if (breakdown.size() > 1) current = breakdown[1].simplified().toFloat();
    float voltage = 0;
    if (breakdown.size() > 2) voltage = breakdown[2].simplified().toFloat();
/* xindex is proportional to time and continuously increases. To access the
arrays, index is the modulo of xindex and wraps around the array bounds
to form a circular buffer. NUMBER_POINTS is the array size. */
    int index = xindex % NUMBER_POINTS;
/* Fill the data arrays */
    if (breakdown[0].simplified() == "dB1")
    {
        yDataB1Current[index] = current;
        yDataB1Voltage[index] = voltage;
    }
    else if (breakdown[0].simplified() == "dB2")
    {
        yDataB2Current[index] = current;
        yDataB2Voltage[index] = voltage;
    }
    else if (breakdown[0].simplified() == "dB3")
    {
        yDataB3Current[index] = current;
        yDataB3Voltage[index] = voltage;
    }
    else if (breakdown[0].simplified() == "dL1")
    {
        yDataL1Current[index] = current;
        yDataL1Voltage[index] = voltage;
    }
    else if (breakdown[0].simplified() == "dL2")
    {
        yDataL2Current[index] = current;
        yDataL2Voltage[index] = voltage;
    }
    else if (breakdown[0].simplified() == "dM1")
    {
        yDataM1Current[index] = current;
        yDataM1Voltage[index] = voltage;

        xData[index] = (float)xindex;
/* Process Plots with new incoming data, using the incremental plot feature. */
/* Only every xSamples item. */
        if ((index % xSamples) == 0)
        {
/* The plot runs from zero time index to the end of the plot.
It then jumps back by a single discrete time step to allow room for more data.
This only happens when the xoffset is zero and is being followed in real time. */
            int plotLength = VISIBLE_POINTS*xSamples;
            if ((plotEnd == plotLength) && (xoffset == 0))
            {
                on_xoffsetSlider_valueChanged(JUMP);
            }
/* Incrementally add to the end of the plots if they are still growing. */
            plotEnd += xSamples;
            if (plotEnd >= plotLength) plotEnd = plotLength;
            CurveData *data1 = static_cast<CurveData *> (d_curve1->data());
            data1->append(QPointF(xData[index], source1[index]));
            d_directPainter1->drawSeries(d_curve1,data1->size()-2,data1->size()-1);
            CurveData *data2 = static_cast<CurveData *> (d_curve2->data());
            data2->append(QPointF(xData[index], source2[index]));
            d_directPainter2->drawSeries(d_curve2,data2->size()-2,data2->size()-1);
        }
        xindex++;
    }
}

//-----------------------------------------------------------------------------
/** @brief Replot the Display Graphs

This computes the index of the plot start in the data array, wipes the plot
and replots from the old data.

Global xSamples: the x-axis scale factor.

@param int 
*/
void PowerManagementMonitorGui::replot(int plotStartIndex, int plotEnd)
{
    int plotLength = VISIBLE_POINTS*xSamples;
    int dataStartIndex = plotStartIndex % NUMBER_POINTS;
    xRangeMin = (float)plotStartIndex;
    xRangeMax = xRangeMin+plotLength;
    PowerManagementMonitorUi.qwtPlot1->
        setAxisScale(QwtPlot::xBottom, xRangeMin, xRangeMax);
    PowerManagementMonitorUi.qwtPlot2->
        setAxisScale(QwtPlot::xBottom, xRangeMin, xRangeMax);
    CurveData *data1 = static_cast<CurveData *> (d_curve1->data());
    data1->clear();
    CurveData *data2 = static_cast<CurveData *> (d_curve2->data());
    data2->clear();
    QPolygonF points1, points2;
/* Back fill the plot with old data */
    for (int i=0; i<plotEnd; i+=xSamples)
    {
        int index = (i+dataStartIndex) % NUMBER_POINTS;
        points1 << QPointF(xData[index],source1[index]);
        points2 << QPointF(xData[index],source2[index]);
    }
    d_curve1->setSamples(points1);
    PowerManagementMonitorUi.qwtPlot1->replot();
    PowerManagementMonitorUi.qwtPlot1->repaint();
    d_curve2->setSamples(points2);
    PowerManagementMonitorUi.qwtPlot2->replot();
    PowerManagementMonitorUi.qwtPlot2->repaint();
}

//-----------------------------------------------------------------------------
/** @brief Change the Display Graph Vertical Offset

*/
void PowerManagementMonitorGui::on_offsetSlider1_valueChanged(int value)
{
    yOffset1 = yOffsetBase1+yScale1*(value-50)/100;
    yRangeMax1 = yOffset1+yScale1/2;
    yRangeMin1 = yOffset1-yScale1/2;
    PowerManagementMonitorUi.qwtPlot1->
            setAxisScale(QwtPlot::yLeft, yRangeMin1, yRangeMax1);
    PowerManagementMonitorUi.qwtPlot1->replot();
}

void PowerManagementMonitorGui::on_offsetSlider2_valueChanged(int value)
{
    yOffset2 = yOffsetBase2+yScale2*(value-50)/100;
    yRangeMax2 = yOffset2+yScale2/2;
    yRangeMin2 = yOffset2-yScale2/2;
    PowerManagementMonitorUi.qwtPlot2->
            setAxisScale(QwtPlot::yLeft, yRangeMin2, yRangeMax2);
    PowerManagementMonitorUi.qwtPlot2->replot();
}

//-----------------------------------------------------------------------------
/** @brief Change the Display Graph Horizontal Time Offset

Work back through the array to find the first point to plot. Replot the graph
from that point on. It will remain steady until xoffset becomes zero.

xindex is the time index of the latest data point.
plotEndIndex is the time index at the next discrete time step.
plotStartIndex is the time index where the plot starts.
(These indexes are translated to actual indexes in the data circular buffer)
plotEnd is the point on the plot where the data plot stops.
plotLength is the length of the plot in xSamples time steps.

value ranges from 0 to 20 in steps of 1.
*/
void PowerManagementMonitorGui::on_xoffsetSlider_valueChanged(int offset)
{
    int plotLength = VISIBLE_POINTS*xSamples;
/* If the last offset was zero, start from normal operation at xindex which
is the time index for the latest data point to arrive.
lastIndex is the point where we last viewed the realtime graph. When offset,
this remains unchanged until a return to realtime. */
    if (xoffset == 0) lastIndex = xindex;
    xoffset = (JUMP-offset)*JUMP;
/* The number of data points to move the plot back is the offset plus a bit
to bring the curve to the grid line. */
    int stepback = lastIndex % JUMP + xoffset;
/* If the offset is now zero, reset back to normal operation at xindex */
    if (xoffset == 0)
    {
        lastIndex = xindex;
        stepback = 0;
    }
/* If the last index is shorter than the plot length, set end to the index */
    int plotEndIndex = lastIndex;
    if (lastIndex > plotLength)
        plotEndIndex = lastIndex + JUMP - lastIndex % JUMP;
    int plotStartIndex = plotEndIndex - plotLength;
/* Deal with very short plots */
    if (plotStartIndex < 0) plotStartIndex = 0;
/* Step back the full way if there is room */
    else if (plotStartIndex > stepback)
    {
        plotStartIndex -= stepback;
        plotEndIndex -= stepback;
    }
/* Otherwise start at zero and plot from there */
    else if (plotStartIndex > 0)
    {
        plotStartIndex = 0;
        plotEndIndex -= plotStartIndex;
    }
/* Don't move back to times before the buffered data starts */
    if (plotStartIndex < xindex - NUMBER_POINTS)
    {
        plotStartIndex = xindex - NUMBER_POINTS;
        plotEndIndex = plotStartIndex + plotLength;
    }
    plotEnd = lastIndex - plotStartIndex;
    if (plotEnd > plotLength) plotEnd = plotLength;
    replot(plotStartIndex, plotEnd);
}

//-----------------------------------------------------------------------------
/** @brief Change the Display Graph Sample Period

Period can be changed between 1 and 5.
*/
void PowerManagementMonitorGui::on_sampleSpinBox_valueChanged(int value)
{
    xSamples = value;
    int plotLength = VISIBLE_POINTS*xSamples;
/* Refresh the plot for no xoffset similarly to that done for normal plots. */
    if (xoffset == 0)
    {
        int plotEndIndex = xindex;
        if (xindex > plotLength)
            plotEndIndex = xindex + JUMP - xindex % JUMP;
        plotStartIndex = plotEndIndex - plotLength;
        if (plotStartIndex < 0) plotStartIndex = 0;
        plotEnd = xindex - plotStartIndex;
        if (plotEnd > plotLength) plotEnd = plotLength;
        replot(plotStartIndex, plotEnd);
    }
/* For shifted plots need to refresh around the shifted point. */
    else
    {
        on_xoffsetSlider_valueChanged(PowerManagementMonitorUi.xoffsetSlider->value());
    }
}

//-----------------------------------------------------------------------------
/** @brief Change the Display Graph Vertical Scale

The scales are changed exponentially about the offset. The slider has 6
gradations.
*/
void PowerManagementMonitorGui::on_scaleSlider1_valueChanged(int value)
{
    float rescale;
    if (value < 3) rescale = 1/(float)(1<<(3-value));
    else rescale = (float)(1<<(value-3));
    yScale1 = yScaleBase1*rescale;
    yRangeMax1 = yOffset1+yScale1/2;
    yRangeMin1 = yOffset1-yScale1/2;
    PowerManagementMonitorUi.qwtPlot1->setAxisScale(QwtPlot::yLeft, yRangeMin1, yRangeMax1);
    PowerManagementMonitorUi.qwtPlot1->replot();
}

void PowerManagementMonitorGui::on_scaleSlider2_valueChanged(int value)
{
    float rescale;
    if (value < 3) rescale = 1/(float)(1<<(3-value));
    else rescale = (float)(1<<(value-3));
    yScale2 = yScaleBase2*rescale;
    yRangeMax2 = yOffset2+yScale2/2;
    yRangeMin2 = yOffset2-yScale2/2;
    PowerManagementMonitorUi.qwtPlot2->setAxisScale(QwtPlot::yLeft, yRangeMin2, yRangeMax2);
    PowerManagementMonitorUi.qwtPlot2->replot();
}

//-----------------------------------------------------------------------------
/** @brief Close Window

*/

void PowerManagementMonitorGui::on_closeButton_clicked()
{
    this->close();
}

