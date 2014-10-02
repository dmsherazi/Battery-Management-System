PROJECT =       Power Management GUI
TEMPLATE =      app
TARGET          += 
DEPENDPATH      += .
include(../auxiliary/qextserialport-v1.2/src/qextserialport.pri)

OBJECTS_DIR     = obj
MOC_DIR         = moc
UI_HEADERS_DIR  = ui
UI_SOURCES_DIR  = ui
LANGUAGE        = C++
CONFIG          += qt warn_on release
QT              += network

LIBS            += -lqextserialport

RESOURCES       = power-management-gui.qrc
# Input
FORMS           += power-management.ui
HEADERS         += power-management-main.h
HEADERS         += serialport.h
SOURCES         += power-management.cpp
SOURCES         += power-management-main.cpp
SOURCES         += serialport.cpp

