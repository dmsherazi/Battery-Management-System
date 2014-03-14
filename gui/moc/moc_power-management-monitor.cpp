/****************************************************************************
** Meta object code from reading C++ file 'power-management-monitor.h'
**
** Created: Fri Feb 21 12:30:54 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../power-management-monitor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'power-management-monitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PowerManagementMonitorGui[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      33,   27,   26,   26, 0x08,
      66,   60,   26,   26, 0x08,
     116,  110,   26,   26, 0x08,
     151,  110,   26,   26, 0x08,
     185,   60,   26,   26, 0x08,
     229,  110,   26,   26, 0x08,
     264,  110,   26,   26, 0x08,
     298,  110,   26,   26, 0x08,
     333,  110,   26,   26, 0x08,
     368,   26,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PowerManagementMonitorGui[] = {
    "PowerManagementMonitorGui\0\0entry\0"
    "onMessageReceived(QString)\0index\0"
    "on_sourceComboBox1_currentIndexChanged(int)\0"
    "value\0on_offsetSlider1_valueChanged(int)\0"
    "on_scaleSlider1_valueChanged(int)\0"
    "on_sourceComboBox2_currentIndexChanged(int)\0"
    "on_offsetSlider2_valueChanged(int)\0"
    "on_scaleSlider2_valueChanged(int)\0"
    "on_xoffsetSlider_valueChanged(int)\0"
    "on_sampleSpinBox_valueChanged(int)\0"
    "on_closeButton_clicked()\0"
};

void PowerManagementMonitorGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PowerManagementMonitorGui *_t = static_cast<PowerManagementMonitorGui *>(_o);
        switch (_id) {
        case 0: _t->onMessageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->on_sourceComboBox1_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_offsetSlider1_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_scaleSlider1_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_sourceComboBox2_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_offsetSlider2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_scaleSlider2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_xoffsetSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_sampleSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_closeButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PowerManagementMonitorGui::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PowerManagementMonitorGui::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PowerManagementMonitorGui,
      qt_meta_data_PowerManagementMonitorGui, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PowerManagementMonitorGui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PowerManagementMonitorGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PowerManagementMonitorGui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PowerManagementMonitorGui))
        return static_cast<void*>(const_cast< PowerManagementMonitorGui*>(this));
    return QDialog::qt_metacast(_clname);
}

int PowerManagementMonitorGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
