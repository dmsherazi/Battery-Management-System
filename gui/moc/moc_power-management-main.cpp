/****************************************************************************
** Meta object code from reading C++ file 'power-management-main.h'
**
** Created: Sun Mar 23 08:36:58 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../power-management-main.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'power-management-main.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PowerManagementGui[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      34,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   20,   19,   19, 0x05,
      61,   20,   19,   19, 0x05,
      92,   20,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     126,   19,   19,   19, 0x08,
     144,   19,   19,   19, 0x08,
     171,   19,   19,   19, 0x08,
     198,   19,   19,   19, 0x08,
     225,   19,   19,   19, 0x08,
     252,   19,   19,   19, 0x08,
     279,   19,   19,   19, 0x08,
     306,   19,   19,   19, 0x08,
     333,   19,   19,   19, 0x08,
     360,   19,   19,   19, 0x08,
     387,   19,   19,   19, 0x08,
     414,   19,   19,   19, 0x08,
     441,   19,   19,   19, 0x08,
     468,   19,   19,   19, 0x08,
     501,   19,   19,   19, 0x08,
     534,   19,   19,   19, 0x08,
     567,   19,   19,   19, 0x08,
     597,   19,   19,   19, 0x08,
     627,   19,   19,   19, 0x08,
     657,   19,   19,   19, 0x08,
     687,   19,   19,   19, 0x08,
     717,   19,   19,   19, 0x08,
     762,  752,  747,   19, 0x08,
     791,   19,   19,   19, 0x08,
     819,   19,   19,   19, 0x08,
     848,   19,   19,   19, 0x08,
     877,   19,   19,   19, 0x08,
     904,   19,   19,   19, 0x08,
     933,   19,   19,   19, 0x08,
     964,   19,   19,   19, 0x08,
     996,  989,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PowerManagementGui[] = {
    "PowerManagementGui\0\0response\0"
    "monitorMessageReceived(QString)\0"
    "recordMessageReceived(QString)\0"
    "configureMessageReceived(QString)\0"
    "onDataAvailable()\0on_load1Battery1_pressed()\0"
    "on_load1Battery2_pressed()\0"
    "on_load1Battery3_pressed()\0"
    "on_load2Battery1_pressed()\0"
    "on_load2Battery2_pressed()\0"
    "on_load2Battery3_pressed()\0"
    "on_panelBattery1_pressed()\0"
    "on_panelBattery2_pressed()\0"
    "on_panelBattery3_pressed()\0"
    "on_load1CheckBox_clicked()\0"
    "on_load2CheckBox_clicked()\0"
    "on_panelCheckBox_clicked()\0"
    "on_battery1OverCurrent_clicked()\0"
    "on_battery2OverCurrent_clicked()\0"
    "on_battery3OverCurrent_clicked()\0"
    "on_load1OverCurrent_clicked()\0"
    "on_load2OverCurrent_clicked()\0"
    "on_panelOverCurrent_clicked()\0"
    "on_battery1CheckBox_clicked()\0"
    "on_battery2CheckBox_clicked()\0"
    "on_battery3CheckBox_clicked()\0bool\0"
    "indicator\0testIndicator(IndicatorType)\0"
    "on_saveFileButton_clicked()\0"
    "on_closeFileButton_clicked()\0"
    "on_recordingButton_clicked()\0"
    "on_monitorButton_clicked()\0"
    "on_configureButton_clicked()\0"
    "on_autoTrackCheckBox_clicked()\0"
    "closeEvent(QCloseEvent*)\0enable\0"
    "disableRadioButtons(bool)\0"
};

void PowerManagementGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PowerManagementGui *_t = static_cast<PowerManagementGui *>(_o);
        switch (_id) {
        case 0: _t->monitorMessageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->recordMessageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->configureMessageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onDataAvailable(); break;
        case 4: _t->on_load1Battery1_pressed(); break;
        case 5: _t->on_load1Battery2_pressed(); break;
        case 6: _t->on_load1Battery3_pressed(); break;
        case 7: _t->on_load2Battery1_pressed(); break;
        case 8: _t->on_load2Battery2_pressed(); break;
        case 9: _t->on_load2Battery3_pressed(); break;
        case 10: _t->on_panelBattery1_pressed(); break;
        case 11: _t->on_panelBattery2_pressed(); break;
        case 12: _t->on_panelBattery3_pressed(); break;
        case 13: _t->on_load1CheckBox_clicked(); break;
        case 14: _t->on_load2CheckBox_clicked(); break;
        case 15: _t->on_panelCheckBox_clicked(); break;
        case 16: _t->on_battery1OverCurrent_clicked(); break;
        case 17: _t->on_battery2OverCurrent_clicked(); break;
        case 18: _t->on_battery3OverCurrent_clicked(); break;
        case 19: _t->on_load1OverCurrent_clicked(); break;
        case 20: _t->on_load2OverCurrent_clicked(); break;
        case 21: _t->on_panelOverCurrent_clicked(); break;
        case 22: _t->on_battery1CheckBox_clicked(); break;
        case 23: _t->on_battery2CheckBox_clicked(); break;
        case 24: _t->on_battery3CheckBox_clicked(); break;
        case 25: { bool _r = _t->testIndicator((*reinterpret_cast< const IndicatorType(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 26: _t->on_saveFileButton_clicked(); break;
        case 27: _t->on_closeFileButton_clicked(); break;
        case 28: _t->on_recordingButton_clicked(); break;
        case 29: _t->on_monitorButton_clicked(); break;
        case 30: _t->on_configureButton_clicked(); break;
        case 31: _t->on_autoTrackCheckBox_clicked(); break;
        case 32: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 33: _t->disableRadioButtons((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PowerManagementGui::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PowerManagementGui::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PowerManagementGui,
      qt_meta_data_PowerManagementGui, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PowerManagementGui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PowerManagementGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PowerManagementGui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PowerManagementGui))
        return static_cast<void*>(const_cast< PowerManagementGui*>(this));
    return QDialog::qt_metacast(_clname);
}

int PowerManagementGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    }
    return _id;
}

// SIGNAL 0
void PowerManagementGui::monitorMessageReceived(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PowerManagementGui::recordMessageReceived(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PowerManagementGui::configureMessageReceived(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
