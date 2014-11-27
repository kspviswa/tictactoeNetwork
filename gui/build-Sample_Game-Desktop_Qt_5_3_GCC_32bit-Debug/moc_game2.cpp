/****************************************************************************
** Meta object code from reading C++ file 'game2.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Sample_Game/game2.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'game2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Game2_t {
    QByteArrayData data[16];
    char stringdata[208];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Game2_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Game2_t qt_meta_stringdata_Game2 = {
    {
QT_MOC_LITERAL(0, 0, 5),
QT_MOC_LITERAL(1, 6, 21),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 24),
QT_MOC_LITERAL(4, 54, 25),
QT_MOC_LITERAL(5, 80, 6),
QT_MOC_LITERAL(6, 87, 12),
QT_MOC_LITERAL(7, 100, 15),
QT_MOC_LITERAL(8, 116, 6),
QT_MOC_LITERAL(9, 123, 12),
QT_MOC_LITERAL(10, 136, 16),
QT_MOC_LITERAL(11, 153, 12),
QT_MOC_LITERAL(12, 166, 7),
QT_MOC_LITERAL(13, 174, 11),
QT_MOC_LITERAL(14, 186, 14),
QT_MOC_LITERAL(15, 201, 6)
    },
    "Game2\0on_pushButton_clicked\0\0"
    "on_Button_Submit_clicked\0"
    "on_Button_EndGame_clicked\0update\0"
    "Registration\0getButtonStatus\0string\0"
    "QPushButton*\0pPushButton_temp\0"
    "getBoardInfo\0string&\0szBoardInfo\0"
    "setButtonState\0bState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Game2[] = {

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
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    1,   59,    2, 0x08 /* Private */,
      11,    1,   62,    2, 0x08 /* Private */,
      14,    1,   65,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    0x80000000 | 8, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::Bool,   15,

       0        // eod
};

void Game2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Game2 *_t = static_cast<Game2 *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_Button_Submit_clicked(); break;
        case 2: _t->on_Button_EndGame_clicked(); break;
        case 3: _t->update(); break;
        case 4: { bool _r = _t->Registration();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { string _r = _t->getButtonStatus((*reinterpret_cast< QPushButton*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = _r; }  break;
        case 6: _t->getBoardInfo((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 7: _t->setButtonState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QPushButton* >(); break;
            }
            break;
        }
    }
}

const QMetaObject Game2::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Game2.data,
      qt_meta_data_Game2,  qt_static_metacall, 0, 0}
};


const QMetaObject *Game2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Game2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Game2.stringdata))
        return static_cast<void*>(const_cast< Game2*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Game2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
