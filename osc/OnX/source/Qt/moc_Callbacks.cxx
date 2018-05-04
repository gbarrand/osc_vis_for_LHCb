/****************************************************************************
** Meta object code from reading C++ file 'Callbacks.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/OnX/Qt/Callbacks.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Callbacks.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtCallback[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      22,   11,   11,   11, 0x0a,
      47,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtCallback[] = {
    "QtCallback\0\0execute()\0execute2(QCallbackData&)\0"
    "execute3(QTreeWidgetItem*)\0"
};

void QtCallback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtCallback *_t = static_cast<QtCallback *>(_o);
        switch (_id) {
        case 0: _t->execute(); break;
        case 1: _t->execute2((*reinterpret_cast< QCallbackData(*)>(_a[1]))); break;
        case 2: _t->execute3((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QtCallback::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QtCallback::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtCallback,
      qt_meta_data_QtCallback, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QtCallback::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QtCallback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QtCallback::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtCallback))
        return static_cast<void*>(const_cast< QtCallback*>(this));
    if (!strcmp(_clname, "OnX::Action"))
        return static_cast< OnX::Action*>(const_cast< QtCallback*>(this));
    return QObject::qt_metacast(_clname);
}

int QtCallback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
static const uint qt_meta_data_OnX__CloseCallback[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OnX__CloseCallback[] = {
    "OnX::CloseCallback\0\0execute()\0"
};

void OnX::CloseCallback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CloseCallback *_t = static_cast<CloseCallback *>(_o);
        switch (_id) {
        case 0: _t->execute(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX::CloseCallback::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX::CloseCallback::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OnX__CloseCallback,
      qt_meta_data_OnX__CloseCallback, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX::CloseCallback::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX::CloseCallback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX::CloseCallback::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX__CloseCallback))
        return static_cast<void*>(const_cast< CloseCallback*>(this));
    if (!strcmp(_clname, "OnX::Action"))
        return static_cast< OnX::Action*>(const_cast< CloseCallback*>(this));
    return QObject::qt_metacast(_clname);
}

int OnX::CloseCallback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
