Battery Management System GUI for BeagleBone Touch Screen
---------------------------------------------------------

This is a cut-down version of the GUI for PC intended for use with a BeagleBone
touch screen for field monitoring and configuration of the BMS.

It only connects via serial (directly or using a serial to USB adapter).

To compile this program, ensure that QT4 is installed on the BeagleBone and
also that qextserialport has been downloaded and unzipped into a suitable
directory. It doesn't need to be compiled or installed, as the GUI will
incorporate it directly into the compile and link procedure.

The file power-management.pro must be modified to point to the directory
holding qextserialport.

More information is available on:

http://www.jiggerjuice.info/electronics/projects/solarbms/solarbms-gui.html

(c) K. Sarkies 29/09/2014

