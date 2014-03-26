/****************************************************************************
** Meta object code from reading C++ file 'power-management-configure.h'
**
** Created: Wed Mar 26 18:49:51 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../power-management-configure.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'power-management-configure.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PowerManagementConfigGui[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      31,   26,   25,   25, 0x08,
      58,   25,   25,   25, 0x08,
      87,   25,   25,   25, 0x08,
     119,   25,   25,   25, 0x08,
     146,   25,   25,   25, 0x08,
     171,   25,   25,   25, 0x08,
     209,  201,   25,   25, 0x08,
     238,   25,   25,   25, 0x08,
     267,   25,   25,   25, 0x08,
     301,   25,   25,   25, 0x08,
     334,   25,   25,   25, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PowerManagementConfigGui[] = {
    "PowerManagementConfigGui\0\0text\0"
    "onMessageReceived(QString)\0"
    "on_calibrateButton_clicked()\0"
    "on_queryBatteryButton_clicked()\0"
    "on_timeSetButton_clicked()\0"
    "on_closeButton_clicked()\0"
    "on_setBatteryButton_clicked()\0message\0"
    "displayErrorMessage(QString)\0"
    "on_setOptionButton_clicked()\0"
    "on_debugMessageCheckbox_clicked()\0"
    "on_dataMessageCheckbox_clicked()\0"
    "on_echoTestButton_clicked()\0"
};

void PowerManagementConfigGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PowerManagementConfigGui *_t = static_cast<PowerManagementConfigGui *>(_o);
        switch (_id) {
        case 0: _t->onMessageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->on_calibrateButton_clicked(); break;
        case 2: _t->on_queryBatteryButton_clicked(); break;
        case 3: _t->on_timeSetButton_clicked(); break;
        case 4: _t->on_closeButton_clicked(); break;
        case 5: _t->on_setBatteryButton_clicked(); break;
        case 6: _t->displayErrorMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->on_setOptionButton_clicked(); break;
        case 8: _t->on_debugMessageCheckbox_clicked(); break;
        case 9: _t->on_dataMessageCheckbox_clicked(); break;
        case 10: _t->on_echoTestButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PowerManagementConfigGui::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PowerManagementConfigGui::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PowerManagementConfigGui,
      qt_meta_data_PowerManagementConfigGui, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PowerManagementConfigGui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PowerManagementConfigGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PowerManagementConfigGui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PowerManagementConfigGui))
        return static_cast<void*>(const_cast< PowerManagementConfigGui*>(this));
    return QDialog::qt_metacast(_clname);
}

int PowerManagementConfigGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
