/****************************************************************************
** Meta object code from reading C++ file 'QZXing.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qzxing-master/src/QZXing.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QZXing.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QZXing_t {
    QByteArrayData data[70];
    char stringdata0[1049];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QZXing_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QZXing_t qt_meta_stringdata_QZXing = {
    {
QT_MOC_LITERAL(0, 0, 6), // "QZXing"
QT_MOC_LITERAL(1, 7, 15), // "decodingStarted"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 16), // "decodingFinished"
QT_MOC_LITERAL(4, 41, 9), // "succeeded"
QT_MOC_LITERAL(5, 51, 21), // "enabledFormatsChanged"
QT_MOC_LITERAL(6, 73, 8), // "tagFound"
QT_MOC_LITERAL(7, 82, 3), // "tag"
QT_MOC_LITERAL(8, 86, 16), // "tagFoundAdvanced"
QT_MOC_LITERAL(9, 103, 6), // "format"
QT_MOC_LITERAL(10, 110, 7), // "charSet"
QT_MOC_LITERAL(11, 118, 4), // "rect"
QT_MOC_LITERAL(12, 123, 8), // "metadata"
QT_MOC_LITERAL(13, 132, 5), // "error"
QT_MOC_LITERAL(14, 138, 3), // "msg"
QT_MOC_LITERAL(15, 142, 11), // "decodeImage"
QT_MOC_LITERAL(16, 154, 5), // "image"
QT_MOC_LITERAL(17, 160, 8), // "maxWidth"
QT_MOC_LITERAL(18, 169, 9), // "maxHeight"
QT_MOC_LITERAL(19, 179, 20), // "smoothTransformation"
QT_MOC_LITERAL(20, 200, 19), // "decodeImageFromFile"
QT_MOC_LITERAL(21, 220, 13), // "imageFilePath"
QT_MOC_LITERAL(22, 234, 14), // "decodeImageQML"
QT_MOC_LITERAL(23, 249, 4), // "item"
QT_MOC_LITERAL(24, 254, 17), // "decodeSubImageQML"
QT_MOC_LITERAL(25, 272, 7), // "offsetX"
QT_MOC_LITERAL(26, 280, 7), // "offsetY"
QT_MOC_LITERAL(27, 288, 5), // "width"
QT_MOC_LITERAL(28, 294, 6), // "height"
QT_MOC_LITERAL(29, 301, 8), // "imageUrl"
QT_MOC_LITERAL(30, 310, 10), // "encodeData"
QT_MOC_LITERAL(31, 321, 4), // "data"
QT_MOC_LITERAL(32, 326, 19), // "QZXingEncoderConfig"
QT_MOC_LITERAL(33, 346, 13), // "encoderConfig"
QT_MOC_LITERAL(34, 360, 13), // "EncoderFormat"
QT_MOC_LITERAL(35, 374, 13), // "encoderFormat"
QT_MOC_LITERAL(36, 388, 16), // "encoderImageSize"
QT_MOC_LITERAL(37, 405, 26), // "EncodeErrorCorrectionLevel"
QT_MOC_LITERAL(38, 432, 20), // "errorCorrectionLevel"
QT_MOC_LITERAL(39, 453, 6), // "border"
QT_MOC_LITERAL(40, 460, 11), // "transparent"
QT_MOC_LITERAL(41, 472, 28), // "getProcessTimeOfLastDecoding"
QT_MOC_LITERAL(42, 501, 17), // "getEnabledFormats"
QT_MOC_LITERAL(43, 519, 10), // "setDecoder"
QT_MOC_LITERAL(44, 530, 4), // "hint"
QT_MOC_LITERAL(45, 535, 13), // "foundedFormat"
QT_MOC_LITERAL(46, 549, 14), // "processingTime"
QT_MOC_LITERAL(47, 564, 15), // "enabledDecoders"
QT_MOC_LITERAL(48, 580, 9), // "tryHarder"
QT_MOC_LITERAL(49, 590, 17), // "allowedExtensions"
QT_MOC_LITERAL(50, 608, 13), // "DecoderFormat"
QT_MOC_LITERAL(51, 622, 18), // "DecoderFormat_None"
QT_MOC_LITERAL(52, 641, 19), // "DecoderFormat_Aztec"
QT_MOC_LITERAL(53, 661, 21), // "DecoderFormat_CODABAR"
QT_MOC_LITERAL(54, 683, 21), // "DecoderFormat_CODE_39"
QT_MOC_LITERAL(55, 705, 21), // "DecoderFormat_CODE_93"
QT_MOC_LITERAL(56, 727, 22), // "DecoderFormat_CODE_128"
QT_MOC_LITERAL(57, 750, 25), // "DecoderFormat_DATA_MATRIX"
QT_MOC_LITERAL(58, 776, 19), // "DecoderFormat_EAN_8"
QT_MOC_LITERAL(59, 796, 20), // "DecoderFormat_EAN_13"
QT_MOC_LITERAL(60, 817, 17), // "DecoderFormat_ITF"
QT_MOC_LITERAL(61, 835, 22), // "DecoderFormat_MAXICODE"
QT_MOC_LITERAL(62, 858, 21), // "DecoderFormat_PDF_417"
QT_MOC_LITERAL(63, 880, 21), // "DecoderFormat_QR_CODE"
QT_MOC_LITERAL(64, 902, 20), // "DecoderFormat_RSS_14"
QT_MOC_LITERAL(65, 923, 26), // "DecoderFormat_RSS_EXPANDED"
QT_MOC_LITERAL(66, 950, 19), // "DecoderFormat_UPC_A"
QT_MOC_LITERAL(67, 970, 19), // "DecoderFormat_UPC_E"
QT_MOC_LITERAL(68, 990, 31), // "DecoderFormat_UPC_EAN_EXTENSION"
QT_MOC_LITERAL(69, 1022, 26) // "DecoderFormat_CODE_128_GS1"

    },
    "QZXing\0decodingStarted\0\0decodingFinished\0"
    "succeeded\0enabledFormatsChanged\0"
    "tagFound\0tag\0tagFoundAdvanced\0format\0"
    "charSet\0rect\0metadata\0error\0msg\0"
    "decodeImage\0image\0maxWidth\0maxHeight\0"
    "smoothTransformation\0decodeImageFromFile\0"
    "imageFilePath\0decodeImageQML\0item\0"
    "decodeSubImageQML\0offsetX\0offsetY\0"
    "width\0height\0imageUrl\0encodeData\0data\0"
    "QZXingEncoderConfig\0encoderConfig\0"
    "EncoderFormat\0encoderFormat\0"
    "encoderImageSize\0EncodeErrorCorrectionLevel\0"
    "errorCorrectionLevel\0border\0transparent\0"
    "getProcessTimeOfLastDecoding\0"
    "getEnabledFormats\0setDecoder\0hint\0"
    "foundedFormat\0processingTime\0"
    "enabledDecoders\0tryHarder\0allowedExtensions\0"
    "DecoderFormat\0DecoderFormat_None\0"
    "DecoderFormat_Aztec\0DecoderFormat_CODABAR\0"
    "DecoderFormat_CODE_39\0DecoderFormat_CODE_93\0"
    "DecoderFormat_CODE_128\0DecoderFormat_DATA_MATRIX\0"
    "DecoderFormat_EAN_8\0DecoderFormat_EAN_13\0"
    "DecoderFormat_ITF\0DecoderFormat_MAXICODE\0"
    "DecoderFormat_PDF_417\0DecoderFormat_QR_CODE\0"
    "DecoderFormat_RSS_14\0DecoderFormat_RSS_EXPANDED\0"
    "DecoderFormat_UPC_A\0DecoderFormat_UPC_E\0"
    "DecoderFormat_UPC_EAN_EXTENSION\0"
    "DecoderFormat_CODE_128_GS1"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QZXing[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       4,  434, // properties
       1,  450, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  214,    2, 0x06 /* Public */,
       3,    1,  215,    2, 0x06 /* Public */,
       5,    0,  218,    2, 0x06 /* Public */,
       6,    1,  219,    2, 0x06 /* Public */,
       8,    3,  222,    2, 0x06 /* Public */,
       8,    4,  229,    2, 0x06 /* Public */,
       8,    4,  238,    2, 0x06 /* Public */,
      13,    1,  247,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    4,  250,    2, 0x0a /* Public */,
      15,    3,  259,    2, 0x2a /* Public | MethodCloned */,
      15,    2,  266,    2, 0x2a /* Public | MethodCloned */,
      15,    1,  271,    2, 0x2a /* Public | MethodCloned */,
      20,    4,  274,    2, 0x0a /* Public */,
      20,    3,  283,    2, 0x2a /* Public | MethodCloned */,
      20,    2,  290,    2, 0x2a /* Public | MethodCloned */,
      20,    1,  295,    2, 0x2a /* Public | MethodCloned */,
      22,    1,  298,    2, 0x0a /* Public */,
      24,    5,  301,    2, 0x0a /* Public */,
      24,    4,  312,    2, 0x2a /* Public | MethodCloned */,
      24,    3,  321,    2, 0x2a /* Public | MethodCloned */,
      24,    2,  328,    2, 0x2a /* Public | MethodCloned */,
      24,    1,  333,    2, 0x2a /* Public | MethodCloned */,
      22,    1,  336,    2, 0x0a /* Public */,
      24,    5,  339,    2, 0x0a /* Public */,
      24,    4,  350,    2, 0x2a /* Public | MethodCloned */,
      24,    3,  359,    2, 0x2a /* Public | MethodCloned */,
      24,    2,  366,    2, 0x2a /* Public | MethodCloned */,
      24,    1,  371,    2, 0x2a /* Public | MethodCloned */,
      30,    2,  374,    2, 0x0a /* Public */,
      30,    6,  379,    2, 0x0a /* Public */,
      30,    5,  392,    2, 0x2a /* Public | MethodCloned */,
      30,    4,  403,    2, 0x2a /* Public | MethodCloned */,
      30,    3,  412,    2, 0x2a /* Public | MethodCloned */,
      30,    2,  419,    2, 0x2a /* Public | MethodCloned */,
      30,    1,  424,    2, 0x2a /* Public | MethodCloned */,
      41,    0,  427,    2, 0x0a /* Public */,
      42,    0,  428,    2, 0x0a /* Public */,
      43,    1,  429,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      45,    0,  432,    2, 0x02 /* Public */,
      10,    0,  433,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    7,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QRectF,    7,    9,   10,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QVariantMap,    7,    9,   10,   12,
    QMetaType::Void, QMetaType::QString,   14,

 // slots: parameters
    QMetaType::QString, QMetaType::QImage, QMetaType::Int, QMetaType::Int, QMetaType::Bool,   16,   17,   18,   19,
    QMetaType::QString, QMetaType::QImage, QMetaType::Int, QMetaType::Int,   16,   17,   18,
    QMetaType::QString, QMetaType::QImage, QMetaType::Int,   16,   17,
    QMetaType::QString, QMetaType::QImage,   16,
    QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Bool,   21,   17,   18,   19,
    QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,   21,   17,   18,
    QMetaType::QString, QMetaType::QString, QMetaType::Int,   21,   17,
    QMetaType::QString, QMetaType::QString,   21,
    QMetaType::QString, QMetaType::QObjectStar,   23,
    QMetaType::QString, QMetaType::QObjectStar, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   23,   25,   26,   27,   28,
    QMetaType::QString, QMetaType::QObjectStar, QMetaType::Int, QMetaType::Int, QMetaType::Int,   23,   25,   26,   27,
    QMetaType::QString, QMetaType::QObjectStar, QMetaType::Int, QMetaType::Int,   23,   25,   26,
    QMetaType::QString, QMetaType::QObjectStar, QMetaType::Int,   23,   25,
    QMetaType::QString, QMetaType::QObjectStar,   23,
    QMetaType::QString, QMetaType::QUrl,   29,
    QMetaType::QString, QMetaType::QUrl, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   29,   25,   26,   27,   28,
    QMetaType::QString, QMetaType::QUrl, QMetaType::Int, QMetaType::Int, QMetaType::Int,   29,   25,   26,   27,
    QMetaType::QString, QMetaType::QUrl, QMetaType::Int, QMetaType::Int,   29,   25,   26,
    QMetaType::QString, QMetaType::QUrl, QMetaType::Int,   29,   25,
    QMetaType::QString, QMetaType::QUrl,   29,
    QMetaType::QImage, QMetaType::QString, 0x80000000 | 32,   31,   33,
    QMetaType::QImage, QMetaType::QString, 0x80000000 | 34, QMetaType::QSize, 0x80000000 | 37, QMetaType::Bool, QMetaType::Bool,   31,   35,   36,   38,   39,   40,
    QMetaType::QImage, QMetaType::QString, 0x80000000 | 34, QMetaType::QSize, 0x80000000 | 37, QMetaType::Bool,   31,   35,   36,   38,   39,
    QMetaType::QImage, QMetaType::QString, 0x80000000 | 34, QMetaType::QSize, 0x80000000 | 37,   31,   35,   36,   38,
    QMetaType::QImage, QMetaType::QString, 0x80000000 | 34, QMetaType::QSize,   31,   35,   36,
    QMetaType::QImage, QMetaType::QString, 0x80000000 | 34,   31,   35,
    QMetaType::QImage, QMetaType::QString,   31,
    QMetaType::Int,
    QMetaType::UInt,
    QMetaType::Void, QMetaType::UInt,   44,

 // methods: parameters
    QMetaType::QString,
    QMetaType::QString,

 // properties: name, type, flags
      46, QMetaType::Int, 0x00095001,
      47, QMetaType::UInt, 0x00495003,
      48, QMetaType::Bool, 0x00095103,
      49, QMetaType::QVariantList, 0x00095103,

 // properties: notify_signal_id
       0,
       2,
       0,
       0,

 // enums: name, flags, count, data
      50, 0x0,   19,  454,

 // enum data: key, value
      51, uint(QZXing::DecoderFormat_None),
      52, uint(QZXing::DecoderFormat_Aztec),
      53, uint(QZXing::DecoderFormat_CODABAR),
      54, uint(QZXing::DecoderFormat_CODE_39),
      55, uint(QZXing::DecoderFormat_CODE_93),
      56, uint(QZXing::DecoderFormat_CODE_128),
      57, uint(QZXing::DecoderFormat_DATA_MATRIX),
      58, uint(QZXing::DecoderFormat_EAN_8),
      59, uint(QZXing::DecoderFormat_EAN_13),
      60, uint(QZXing::DecoderFormat_ITF),
      61, uint(QZXing::DecoderFormat_MAXICODE),
      62, uint(QZXing::DecoderFormat_PDF_417),
      63, uint(QZXing::DecoderFormat_QR_CODE),
      64, uint(QZXing::DecoderFormat_RSS_14),
      65, uint(QZXing::DecoderFormat_RSS_EXPANDED),
      66, uint(QZXing::DecoderFormat_UPC_A),
      67, uint(QZXing::DecoderFormat_UPC_E),
      68, uint(QZXing::DecoderFormat_UPC_EAN_EXTENSION),
      69, uint(QZXing::DecoderFormat_CODE_128_GS1),

       0        // eod
};

void QZXing::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QZXing *_t = static_cast<QZXing *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->decodingStarted(); break;
        case 1: _t->decodingFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->enabledFormatsChanged(); break;
        case 3: _t->tagFound((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->tagFoundAdvanced((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 5: _t->tagFoundAdvanced((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QRectF(*)>(_a[4]))); break;
        case 6: _t->tagFoundAdvanced((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QVariantMap(*)>(_a[4]))); break;
        case 7: _t->error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: { QString _r = _t->decodeImage((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 9: { QString _r = _t->decodeImage((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 10: { QString _r = _t->decodeImage((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 11: { QString _r = _t->decodeImage((*reinterpret_cast< const QImage(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 12: { QString _r = _t->decodeImageFromFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 13: { QString _r = _t->decodeImageFromFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 14: { QString _r = _t->decodeImageFromFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 15: { QString _r = _t->decodeImageFromFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 16: { QString _r = _t->decodeImageQML((*reinterpret_cast< QObject*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 17: { QString _r = _t->decodeSubImageQML((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])),(*reinterpret_cast< const int(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 18: { QString _r = _t->decodeSubImageQML((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 19: { QString _r = _t->decodeSubImageQML((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 20: { QString _r = _t->decodeSubImageQML((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 21: { QString _r = _t->decodeSubImageQML((*reinterpret_cast< QObject*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 22: { QString _r = _t->decodeImageQML((*reinterpret_cast< const QUrl(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 23: { QString _r = _t->decodeSubImageQML((*reinterpret_cast< const QUrl(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])),(*reinterpret_cast< const int(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 24: { QString _r = _t->decodeSubImageQML((*reinterpret_cast< const QUrl(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 25: { QString _r = _t->decodeSubImageQML((*reinterpret_cast< const QUrl(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 26: { QString _r = _t->decodeSubImageQML((*reinterpret_cast< const QUrl(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 27: { QString _r = _t->decodeSubImageQML((*reinterpret_cast< const QUrl(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 28: { QImage _r = _t->encodeData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QZXingEncoderConfig(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 29: { QImage _r = _t->encodeData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const EncoderFormat(*)>(_a[2])),(*reinterpret_cast< const QSize(*)>(_a[3])),(*reinterpret_cast< const EncodeErrorCorrectionLevel(*)>(_a[4])),(*reinterpret_cast< const bool(*)>(_a[5])),(*reinterpret_cast< const bool(*)>(_a[6])));
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 30: { QImage _r = _t->encodeData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const EncoderFormat(*)>(_a[2])),(*reinterpret_cast< const QSize(*)>(_a[3])),(*reinterpret_cast< const EncodeErrorCorrectionLevel(*)>(_a[4])),(*reinterpret_cast< const bool(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 31: { QImage _r = _t->encodeData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const EncoderFormat(*)>(_a[2])),(*reinterpret_cast< const QSize(*)>(_a[3])),(*reinterpret_cast< const EncodeErrorCorrectionLevel(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 32: { QImage _r = _t->encodeData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const EncoderFormat(*)>(_a[2])),(*reinterpret_cast< const QSize(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 33: { QImage _r = _t->encodeData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const EncoderFormat(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 34: { QImage _r = _t->encodeData((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QImage*>(_a[0]) = std::move(_r); }  break;
        case 35: { int _r = _t->getProcessTimeOfLastDecoding();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 36: { uint _r = _t->getEnabledFormats();
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = std::move(_r); }  break;
        case 37: _t->setDecoder((*reinterpret_cast< const uint(*)>(_a[1]))); break;
        case 38: { QString _r = _t->foundedFormat();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 39: { QString _r = _t->charSet();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (QZXing::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QZXing::decodingStarted)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QZXing::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QZXing::decodingFinished)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QZXing::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QZXing::enabledFormatsChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QZXing::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QZXing::tagFound)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (QZXing::*_t)(const QString & , const QString & , const QString & ) const;
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QZXing::tagFoundAdvanced)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (QZXing::*_t)(const QString & , const QString & , const QString & , const QRectF & ) const;
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QZXing::tagFoundAdvanced)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (QZXing::*_t)(const QString & , const QString & , const QString & , const QVariantMap & ) const;
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QZXing::tagFoundAdvanced)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (QZXing::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QZXing::error)) {
                *result = 7;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        QZXing *_t = static_cast<QZXing *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->getProcessTimeOfLastDecoding(); break;
        case 1: *reinterpret_cast< uint*>(_v) = _t->getEnabledFormats(); break;
        case 2: *reinterpret_cast< bool*>(_v) = _t->getTryHarder(); break;
        case 3: *reinterpret_cast< QVariantList*>(_v) = _t->getAllowedExtensions(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        QZXing *_t = static_cast<QZXing *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setDecoder(*reinterpret_cast< uint*>(_v)); break;
        case 2: _t->setTryHarder(*reinterpret_cast< bool*>(_v)); break;
        case 3: _t->setAllowedExtensions(*reinterpret_cast< QVariantList*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QZXing::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QZXing.data,
      qt_meta_data_QZXing,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QZXing::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QZXing::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QZXing.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QZXing::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 40;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QZXing::decodingStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QZXing::decodingFinished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QZXing::enabledFormatsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QZXing::tagFound(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QZXing::tagFoundAdvanced(const QString & _t1, const QString & _t2, const QString & _t3)const
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(const_cast< QZXing *>(this), &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QZXing::tagFoundAdvanced(const QString & _t1, const QString & _t2, const QString & _t3, const QRectF & _t4)const
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(const_cast< QZXing *>(this), &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QZXing::tagFoundAdvanced(const QString & _t1, const QString & _t2, const QString & _t3, const QVariantMap & _t4)const
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(const_cast< QZXing *>(this), &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QZXing::error(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
