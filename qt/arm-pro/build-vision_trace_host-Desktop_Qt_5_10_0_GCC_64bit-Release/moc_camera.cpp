/****************************************************************************
** Meta object code from reading C++ file 'camera.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../vision_trace_host/camera.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camera.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Camera_t {
    QByteArrayData data[24];
    char stringdata0[406];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Camera_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Camera_t qt_meta_stringdata_Camera = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Camera"
QT_MOC_LITERAL(1, 7, 10), // "show_frame"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 12), // "show_frame_2"
QT_MOC_LITERAL(4, 32, 20), // "show_tutlebot_status"
QT_MOC_LITERAL(5, 53, 12), // "turltebot_up"
QT_MOC_LITERAL(6, 66, 14), // "turltebot_down"
QT_MOC_LITERAL(7, 81, 15), // "turltebot_right"
QT_MOC_LITERAL(8, 97, 14), // "turltebot_left"
QT_MOC_LITERAL(9, 112, 14), // "turltebot_turn"
QT_MOC_LITERAL(10, 127, 14), // "turltebot_stop"
QT_MOC_LITERAL(11, 142, 12), // "show_command"
QT_MOC_LITERAL(12, 155, 15), // "send_info_to_pc"
QT_MOC_LITERAL(13, 171, 16), // "update_path_node"
QT_MOC_LITERAL(14, 188, 22), // "update_path_start_node"
QT_MOC_LITERAL(15, 211, 13), // "on_next_frame"
QT_MOC_LITERAL(16, 225, 17), // "on_read_path_plan"
QT_MOC_LITERAL(17, 243, 14), // "on_update_path"
QT_MOC_LITERAL(18, 258, 18), // "on_timer_turn_tail"
QT_MOC_LITERAL(19, 277, 26), // "on_timer_through_crossroad"
QT_MOC_LITERAL(20, 304, 21), // "on_timer_crossroad_qr"
QT_MOC_LITERAL(21, 326, 25), // "on_timer_path_status_slow"
QT_MOC_LITERAL(22, 352, 25), // "on_timer_is_straight_road"
QT_MOC_LITERAL(23, 378, 27) // "on_send_path_info_to_camera"

    },
    "Camera\0show_frame\0\0show_frame_2\0"
    "show_tutlebot_status\0turltebot_up\0"
    "turltebot_down\0turltebot_right\0"
    "turltebot_left\0turltebot_turn\0"
    "turltebot_stop\0show_command\0send_info_to_pc\0"
    "update_path_node\0update_path_start_node\0"
    "on_next_frame\0on_read_path_plan\0"
    "on_update_path\0on_timer_turn_tail\0"
    "on_timer_through_crossroad\0"
    "on_timer_crossroad_qr\0on_timer_path_status_slow\0"
    "on_timer_is_straight_road\0"
    "on_send_path_info_to_camera"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Camera[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  124,    2, 0x06 /* Public */,
       3,    1,  127,    2, 0x06 /* Public */,
       4,    1,  130,    2, 0x06 /* Public */,
       5,    2,  133,    2, 0x06 /* Public */,
       6,    2,  138,    2, 0x06 /* Public */,
       7,    2,  143,    2, 0x06 /* Public */,
       8,    2,  148,    2, 0x06 /* Public */,
       9,    2,  153,    2, 0x06 /* Public */,
      10,    0,  158,    2, 0x06 /* Public */,
      11,    1,  159,    2, 0x06 /* Public */,
      12,    1,  162,    2, 0x06 /* Public */,
      13,    1,  165,    2, 0x06 /* Public */,
      14,    1,  168,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,  171,    2, 0x08 /* Private */,
      16,    0,  172,    2, 0x08 /* Private */,
      17,    0,  173,    2, 0x08 /* Private */,
      18,    0,  174,    2, 0x08 /* Private */,
      19,    0,  175,    2, 0x08 /* Private */,
      20,    0,  176,    2, 0x08 /* Private */,
      21,    0,  177,    2, 0x08 /* Private */,
      22,    0,  178,    2, 0x08 /* Private */,
      23,    1,  179,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    2,
    QMetaType::Void, QMetaType::QImage,    2,
    QMetaType::Void, QMetaType::SChar,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    2,

       0        // eod
};

void Camera::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Camera *_t = static_cast<Camera *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->show_frame((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->show_frame_2((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->show_tutlebot_status((*reinterpret_cast< qint8(*)>(_a[1]))); break;
        case 3: _t->turltebot_up((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 4: _t->turltebot_down((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 5: _t->turltebot_right((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 6: _t->turltebot_left((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 7: _t->turltebot_turn((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 8: _t->turltebot_stop(); break;
        case 9: _t->show_command((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 10: _t->send_info_to_pc((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 11: _t->update_path_node((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 12: _t->update_path_start_node((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 13: _t->on_next_frame(); break;
        case 14: _t->on_read_path_plan(); break;
        case 15: _t->on_update_path(); break;
        case 16: _t->on_timer_turn_tail(); break;
        case 17: _t->on_timer_through_crossroad(); break;
        case 18: _t->on_timer_crossroad_qr(); break;
        case 19: _t->on_timer_path_status_slow(); break;
        case 20: _t->on_timer_is_straight_road(); break;
        case 21: _t->on_send_path_info_to_camera((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Camera::*_t)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::show_frame)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::show_frame_2)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(qint8 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::show_tutlebot_status)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::turltebot_up)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::turltebot_down)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::turltebot_right)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::turltebot_left)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::turltebot_turn)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (Camera::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::turltebot_stop)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::show_command)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::send_info_to_pc)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::update_path_node)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::update_path_start_node)) {
                *result = 12;
                return;
            }
        }
    }
}

const QMetaObject Camera::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Camera.data,
      qt_meta_data_Camera,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Camera::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Camera::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Camera.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Camera::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void Camera::show_frame(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Camera::show_frame_2(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Camera::show_tutlebot_status(qint8 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Camera::turltebot_up(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Camera::turltebot_down(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Camera::turltebot_right(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Camera::turltebot_left(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Camera::turltebot_turn(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Camera::turltebot_stop()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void Camera::show_command(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Camera::send_info_to_pc(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Camera::update_path_node(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Camera::update_path_start_node(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
