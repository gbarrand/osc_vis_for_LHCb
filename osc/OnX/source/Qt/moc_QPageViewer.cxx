/****************************************************************************
** Meta object code from reading C++ file 'QPageViewer.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/OnX/Qt/QPageViewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QPageViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OnX__QPageViewer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   52,   52,   52, 0x05,
      53,   52,   52,   52, 0x05,
      77,   52,   52,   52, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_OnX__QPageViewer[] = {
    "OnX::QPageViewer\0rectangularCollect(QCallbackData&)\0"
    "\0collect(QCallbackData&)\0popup(QCallbackData&)\0"
};

void OnX::QPageViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPageViewer *_t = static_cast<QPageViewer *>(_o);
        switch (_id) {
        case 0: _t->rectangularCollect((*reinterpret_cast< QCallbackData(*)>(_a[1]))); break;
        case 1: _t->collect((*reinterpret_cast< QCallbackData(*)>(_a[1]))); break;
        case 2: _t->popup((*reinterpret_cast< QCallbackData(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OnX::QPageViewer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX::QPageViewer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OnX__QPageViewer,
      qt_meta_data_OnX__QPageViewer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX::QPageViewer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX::QPageViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX::QPageViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX__QPageViewer))
        return static_cast<void*>(const_cast< QPageViewer*>(this));
    return QWidget::qt_metacast(_clname);
}

int OnX::QPageViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void OnX::QPageViewer::rectangularCollect(QCallbackData & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OnX::QPageViewer::collect(QCallbackData & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OnX::QPageViewer::popup(QCallbackData & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
static const uint qt_meta_data_OnX__QPageViewerPopupCallback[] = {

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
      30,   40,   40,   40, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OnX__QPageViewerPopupCallback[] = {
    "OnX::QPageViewerPopupCallback\0execute()\0"
    "\0"
};

void OnX::QPageViewerPopupCallback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QPageViewerPopupCallback *_t = static_cast<QPageViewerPopupCallback *>(_o);
        switch (_id) {
        case 0: _t->execute(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX::QPageViewerPopupCallback::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX::QPageViewerPopupCallback::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_OnX__QPageViewerPopupCallback,
      qt_meta_data_OnX__QPageViewerPopupCallback, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX::QPageViewerPopupCallback::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX::QPageViewerPopupCallback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX::QPageViewerPopupCallback::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX__QPageViewerPopupCallback))
        return static_cast<void*>(const_cast< QPageViewerPopupCallback*>(this));
    return QObject::qt_metacast(_clname);
}

int OnX::QPageViewerPopupCallback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
