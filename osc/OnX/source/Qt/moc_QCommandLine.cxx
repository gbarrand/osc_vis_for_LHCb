/****************************************************************************
** Meta object code from reading C++ file 'QCommandLine.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/OnX/Qt/QCommandLine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QCommandLine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OnX__QCommandLine[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   29,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   29,   29,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OnX__QCommandLine[] = {
    "OnX::QCommandLine\0complete()\0\0execute()\0"
};

void OnX::QCommandLine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QCommandLine *_t = static_cast<QCommandLine *>(_o);
        switch (_id) {
        case 0: _t->complete(); break;
        case 1: _t->execute(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX::QCommandLine::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX::QCommandLine::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_OnX__QCommandLine,
      qt_meta_data_OnX__QCommandLine, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX::QCommandLine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX::QCommandLine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX::QCommandLine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX__QCommandLine))
        return static_cast<void*>(const_cast< QCommandLine*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int OnX::QCommandLine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void OnX::QCommandLine::complete()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
