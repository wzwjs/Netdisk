/****************************************************************************
** Meta object code from reading C++ file 'networkdisk.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../networkdisk.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkdisk.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Networkdisk_t {
    const uint offsetsAndSize[14];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_Networkdisk_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_Networkdisk_t qt_meta_stringdata_Networkdisk = {
    {
QT_MOC_LITERAL(0, 11), // "Networkdisk"
QT_MOC_LITERAL(12, 11), // "showConnect"
QT_MOC_LITERAL(24, 0), // ""
QT_MOC_LITERAL(25, 7), // "recvMsg"
QT_MOC_LITERAL(33, 19), // "on_login_pb_clicked"
QT_MOC_LITERAL(53, 20), // "on_regist_pb_clicked"
QT_MOC_LITERAL(74, 20) // "on_cancel_pb_clicked"

    },
    "Networkdisk\0showConnect\0\0recvMsg\0"
    "on_login_pb_clicked\0on_regist_pb_clicked\0"
    "on_cancel_pb_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Networkdisk[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x0a,    1 /* Public */,
       3,    0,   45,    2, 0x0a,    2 /* Public */,
       4,    0,   46,    2, 0x08,    3 /* Private */,
       5,    0,   47,    2, 0x08,    4 /* Private */,
       6,    0,   48,    2, 0x08,    5 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Networkdisk::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Networkdisk *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->showConnect(); break;
        case 1: _t->recvMsg(); break;
        case 2: _t->on_login_pb_clicked(); break;
        case 3: _t->on_regist_pb_clicked(); break;
        case 4: _t->on_cancel_pb_clicked(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject Networkdisk::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Networkdisk.offsetsAndSize,
    qt_meta_data_Networkdisk,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Networkdisk_t
, QtPrivate::TypeAndForceComplete<Networkdisk, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *Networkdisk::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Networkdisk::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Networkdisk.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Networkdisk::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
