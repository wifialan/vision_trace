/****************************************************************************
** Meta object code from reading C++ file 'ros.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../vision_trace_host/ros.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ros.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Ros_t {
    QByteArrayData data[10];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Ros_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Ros_t qt_meta_stringdata_Ros = {
    {
QT_MOC_LITERAL(0, 0, 3), // "Ros"
QT_MOC_LITERAL(1, 4, 15), // "on_turltebot_up"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 17), // "on_turltebot_down"
QT_MOC_LITERAL(4, 39, 18), // "on_turltebot_right"
QT_MOC_LITERAL(5, 58, 17), // "on_turltebot_left"
QT_MOC_LITERAL(6, 76, 17), // "on_turltebot_turn"
QT_MOC_LITERAL(7, 94, 27), // "on_turltebot_turn_clockwise"
QT_MOC_LITERAL(8, 122, 34), // "on_turltebot_turn_countercloc..."
QT_MOC_LITERAL(9, 157, 17) // "on_turltebot_stop"

    },
    "Ros\0on_turltebot_up\0\0on_turltebot_down\0"
    "on_turltebot_right\0on_turltebot_left\0"
    "on_turltebot_turn\0on_turltebot_turn_clockwise\0"
    "on_turltebot_turn_counterclockwise\0"
    "on_turltebot_stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Ros[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x08 /* Private */,
       3,    2,   59,    2, 0x08 /* Private */,
       4,    2,   64,    2, 0x08 /* Private */,
       5,    2,   69,    2, 0x08 /* Private */,
       6,    2,   74,    2, 0x08 /* Private */,
       7,    2,   79,    2, 0x08 /* Private */,
       8,    2,   84,    2, 0x08 /* Private */,
       9,    0,   89,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void,

       0        // eod
};

void Ros::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Ros *_t = static_cast<Ros *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_turltebot_up((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->on_turltebot_down((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->on_turltebot_right((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->on_turltebot_left((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 4: _t->on_turltebot_turn((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 5: _t->on_turltebot_turn_clockwise((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 6: _t->on_turltebot_turn_counterclockwise((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 7: _t->on_turltebot_stop(); break;
        default: ;
        }
    }
}

const QMetaObject Ros::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Ros.data,
      qt_meta_data_Ros,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Ros::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Ros::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Ros.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Ros::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
