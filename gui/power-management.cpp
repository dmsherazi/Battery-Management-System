/**
@mainpage Power Management GUI
@version 1.0
@author Ken Sarkies (www.jiggerjuice.net)
@date 6 October 2013

This is the PC user interface to the Solar-Battery Power Management System.

Call with power-management [options]

-P   port (/dev/ttyUSB0 default)
-b   baudrate (from 2400, 4800, 9600, 19200, 38400 default, 57600, 115200)
-a   TCP address (192.168.2.14 default)
-p   TCP port (6666 default)

The first two are used only when compiled for serial comms, and the latter when
compiled for TCP/IP.

@note
Compiler: gcc (Ubuntu 4.6.3-1ubuntu5) 4.6.3
@note
Uses: Qt version 4.8.1
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
    QString serialDevice = DEFAULT_SERIAL_PORT;
    uint initialBaudrate = DEFAULT_BAUDRATE;
    QString tcpAddress = DEFAULT_TCP_ADDRESS;
    uint tcpPort = DEFAULT_TCP_PORT;
    int baudParm;
    int c;
    opterr = 0;
#ifdef SERIAL
    while ((c = getopt (argc, argv, "P:b:")) != -1)
#else
    while ((c = getopt (argc, argv, "a:p:")) != -1)
#endif
    {
        switch (c)
        {
#ifdef SERIAL
// Serial Port Device
        case 'P':
            serialDevice = optarg;
            break;
// Serial baudrate
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
#else
// TCP address
        case 'a':
            tcpAddress = optarg;
// TCP port number
        case 'p':
            tcpPort = atoi(optarg);
#endif
// Unknown
        case '?':
#ifdef SERIAL
            if ((optopt == 'P') || (optopt == 'b'))
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
#else
            if ((optopt == 'a') || (optopt == 'p'))
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
#endif
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
            default: return false;
        }
    }
    QString inDevice;
    uint parameter;
#ifdef SERIAL
    inDevice = serialDevice;
    parameter = initialBaudrate;
#else
    inDevice = tcpAddress;
    parameter = tcpPort;
#endif

    QApplication application(argc,argv);
    PowerManagementGui powerManagementGui(inDevice,parameter);
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
