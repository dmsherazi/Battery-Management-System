Battery Management System GUI
-----------------------------

The GUI provides a means of displaying performance and status information in
numerical and graphical form, setting up data recording on the remote system
and configuring the system.

It can connect via serial (directly or using a serial to USB adapter), or by
TCP/IP when connected through an intermediate machine that maps the serial
interface to a TCP/IP interface. This makes it possible to monitor the system
remotely over Internet.

To compile this program, ensure that QT4 is installed and also that
qextserialport has been downloaded and unzipped into a suitable directory. It
doesn't need to be compiled or installed, as the GUI will incorporate it
directly into the compile and link procedure.

The file power-management.pro must be modified to point to the directory
holding qextserialport.

More information is available on:

http://www.jiggerjuice.info/electronics/projects/solarbms/solarbms-gui.html

(c) K. Sarkies 01/09/2014

TODO

1. File - add file info (date).
2. Download feature (may cause saturation of the comms interface).


