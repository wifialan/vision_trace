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
    QByteArrayData data[28];
    char stringdata0[490];
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
QT_MOC_LITERAL(10, 127, 24), // "turltebot_turn_clockwise"
QT_MOC_LITERAL(11, 152, 31), // "turltebot_turn_counterclockwise"
QT_MOC_LITERAL(12, 184, 14), // "turltebot_stop"
QT_MOC_LITERAL(13, 199, 19), // "lanuch_turltebot_go"
QT_MOC_LITERAL(14, 219, 12), // "show_command"
QT_MOC_LITERAL(15, 232, 15), // "send_info_to_pc"
QT_MOC_LITERAL(16, 248, 16), // "update_path_node"
QT_MOC_LITERAL(17, 265, 22), // "update_path_start_node"
QT_MOC_LITERAL(18, 288, 26), // "update_trultebot_direction"
QT_MOC_LITERAL(19, 315, 13), // "on_next_frame"
QT_MOC_LITERAL(20, 329, 17), // "on_read_path_plan"
QT_MOC_LITERAL(21, 347, 14), // "on_update_path"
QT_MOC_LITERAL(22, 362, 18), // "on_timer_turn_tail"
QT_MOC_LITERAL(23, 381, 26), // "on_timer_through_crossroad"
QT_MOC_LITERAL(24, 408, 21), // "on_timer_crossroad_qr"
QT_MOC_LITERAL(25, 430, 27), // "on_send_path_info_to_camera"
QT_MOC_LITERAL(26, 458, 9), // "path_info"
QT_MOC_LITERAL(27, 468, 21) // "on_send_road_boundary"

    },
    "Camera\0show_frame\0\0show_frame_2\0"
    "show_tutlebot_status\0turltebot_up\0"
    "turltebot_down\0turltebot_right\0"
    "turltebot_left\0turltebot_turn\0"
    "turltebot_turn_clockwise\0"
    "turltebot_turn_counterclockwise\0"
    "turltebot_stop\0lanuch_turltebot_go\0"
    "show_command\0send_info_to_pc\0"
    "update_path_node\0update_path_start_node\0"
    "update_trultebot_direction\0on_next_frame\0"
    "on_read_path_plan\0on_update_path\0"
    "on_timer_turn_tail\0on_timer_through_crossroad\0"
    "on_timer_crossroad_qr\0on_send_path_info_to_camera\0"
    "path_info\0on_send_road_boundary"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Camera[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      17,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  139,    2, 0x06 /* Public */,
       3,    1,  142,    2, 0x06 /* Public */,
       4,    1,  145,    2, 0x06 /* Public */,
       5,    2,  148,    2, 0x06 /* Public */,
       6,    2,  153,    2, 0x06 /* Public */,
       7,    2,  158,    2, 0x06 /* Public */,
       8,    2,  163,    2, 0x06 /* Public */,
       9,    2,  168,    2, 0x06 /* Public */,
      10,    2,  173,    2, 0x06 /* Public */,
      11,    2,  178,    2, 0x06 /* Public */,
      12,    0,  183,    2, 0x06 /* Public */,
      13,    0,  184,    2, 0x06 /* Public */,
      14,    1,  185,    2, 0x06 /* Public */,
      15,    1,  188,    2, 0x06 /* Public */,
      16,    1,  191,    2, 0x06 /* Public */,
      17,    1,  194,    2, 0x06 /* Public */,
      18,    1,  197,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    0,  200,    2, 0x08 /* Private */,
      20,    0,  201,    2, 0x08 /* Private */,
      21,    0,  202,    2, 0x08 /* Private */,
      22,    0,  203,    2, 0x08 /* Private */,
      23,    0,  204,    2, 0x08 /* Private */,
      24,    0,  205,    2, 0x08 /* Private */,
      25,    1,  206,    2, 0x08 /* Private */,
      27,    2,  209,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    2,
    QMetaType::Void, QMetaType::QImage,    2,
    QMetaType::Void, QMetaType::Short,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   26,
    QMetaType::Void, QMetaType::UShort, QMetaType::UShort,    2,    2,

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
        case 2: _t->show_tutlebot_status((*reinterpret_cast< qint16(*)>(_a[1]))); break;
        case 3: _t->turltebot_up((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 4: _t->turltebot_down((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 5: _t->turltebot_right((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 6: _t->turltebot_left((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 7: _t->turltebot_turn((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 8: _t->turltebot_turn_clockwise((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 9: _t->turltebot_turn_counterclockwise((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 10: _t->turltebot_stop(); break;
        case 11: _t->lanuch_turltebot_go(); break;
        case 12: _t->show_command((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 13: _t->send_info_to_pc((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 14: _t->update_path_node((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 15: _t->update_path_start_node((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 16: _t->update_trultebot_direction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->on_next_frame(); break;
        case 18: _t->on_read_path_plan(); break;
        case 19: _t->on_update_path(); break;
        case 20: _t->on_timer_turn_tail(); break;
        case 21: _t->on_timer_through_crossroad(); break;
        case 22: _t->on_timer_crossroad_qr(); break;
        case 23: _t->on_send_path_info_to_camera((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 24: _t->on_send_road_boundary((*reinterpret_cast< quint16(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
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
            typedef void (Camera::*_t)(qint16 );
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
            typedef void (Camera::*_t)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::turltebot_turn_clockwise)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::turltebot_turn_counterclockwise)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (Camera::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::turltebot_stop)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (Camera::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::lanuch_turltebot_go)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::show_command)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::send_info_to_pc)) {
                *result = 13;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::update_path_node)) {
                *result = 14;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::update_path_start_node)) {
                *result = 15;
                return;
            }
        }
        {
            typedef void (Camera::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Camera::update_trultebot_direction)) {
                *result = 16;
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
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 25;
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
void Camera::show_tutlebot_status(qint16 _t1)
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
void Camera::turltebot_turn_clockwise(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Camera::turltebot_turn_counterclockwise(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Camera::turltebot_stop()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void Camera::lanuch_turltebot_go()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void Camera::show_command(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void Camera::send_info_to_pc(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void Camera::update_path_node(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void Camera::update_path_start_node(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void Camera::update_trultebot_direction(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
