/**
@mainpage Power Management Data Processing Tool
@version 1.0
@author Ken Sarkies (www.jiggerjuice.net)
@date 22 march 2014

This is tool for processing and displaying data gathered by the Solar-Battery
Power Management System.

@note
Compiler: gcc (Ubuntu 4.6.3-1ubuntu5) 4.6.3
@note
Uses: Qt version 4.8.1
*/
/****************************************************************************
 *   Copyright (C) 2014 by Ken Sarkies                                      *
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

//-----------------------------------------------------------------------------
/** @brief Power Management Data Processing Main Program

*/

int main(int argc,char ** argv)
{
    QApplication application(argc,argv);
    DataProcessingGui dataProcessingGui;
    if (dataProcessingGui.success())
    {
        dataProcessingGui.show();
        return application.exec();
    }
}
