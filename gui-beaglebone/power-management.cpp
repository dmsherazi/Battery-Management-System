/**
@mainpage Power Management GUI for TouchScreen
@version 1.0
@author Ken Sarkies (www.jiggerjuice.net)
@date 6 October 2013

This is the touchscreen user interface to the Solar-Battery Power Management
System.on BeagleBone.

@note
Compiler: gcc (Ubuntu 4.6.3-1ubuntu5) 4.6.3
@note
Uses: Qt version 4.8.1
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

#include "power-management-main.h"
#include <QApplication>
#include <QMessageBox>

//-----------------------------------------------------------------------------
/** @brief Power Management GUI Main Program

*/

int main(int argc,char ** argv)
{
    QApplication application(argc,argv);
    application.setOverrideCursor(Qt::BlankCursor);
    PowerManagementGui powerManagementGui;
    if (powerManagementGui.success())
    {
        powerManagementGui.setWindowFlags(Qt::X11BypassWindowManagerHint);
        powerManagementGui.show();
        return application.exec();
    }
    else
        QMessageBox::critical(0,"Unable to connect to remote system",
              QString("%1").arg(powerManagementGui.error()));
    return false;
}
