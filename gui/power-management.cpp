/**
@mainpage Power Management GUI
@version 1.0
@author Ken Sarkies (www.jiggerjuice.net)
@date 6 October 2013

This is the PC user interface to the Solar-Battery Power Management System.

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

#include <unistd.h>
#include "power-management-main.h"
#include <QApplication>
#include <QMessageBox>

//-----------------------------------------------------------------------------
/** @brief Power Management GUI Main Program

*/

int main(int argc,char ** argv)
{
/* Interpret any command line options */
    QString inPort = SERIAL_PORT;
    int c;
    uint initialBaudrate = 4;
    int baudParm;
    opterr = 0;
    while ((c = getopt (argc, argv, "P:b:")) != -1)
    {
        switch (c)
        {
        case 'P':
            inPort = optarg;
            break;
        case 'b':
            baudParm = atoi(optarg);
            switch (baudParm)
            {
            case 2400: initialBaudrate=0;break;
            case 4800: initialBaudrate=1;break;
            case 9600: initialBaudrate=2;break;
            case 19200: initialBaudrate=3;break;
            case 38400: initialBaudrate=4;break;
            case 57600: initialBaudrate=5;break;
            case 115200: initialBaudrate=6;break;
            default:
                fprintf (stderr, "Invalid Baudrate %i.\n", baudParm);
                return false;
            }
            break;
        case '?':
            if (optopt == 'P')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
            default: return false;
        }
    }

    QApplication application(argc,argv);
    PowerManagementGui powerManagementGui(inPort,initialBaudrate);
    if (powerManagementGui.success())
    {
        powerManagementGui.show();
        return application.exec();
    }
    else
        QMessageBox::critical(0,"Unable to connect to remote system",
              QString("%1").arg(powerManagementGui.error()));
    return false;
}
