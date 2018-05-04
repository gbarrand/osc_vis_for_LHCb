/****************************************************************************
** Meta object code from reading C++ file 'QTk.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/OnX/Qt/QTk.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QTk.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OnX_QApplication[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_OnX_QApplication[] = {
    "OnX_QApplication\0"
};

void OnX_QApplication::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX_QApplication::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX_QApplication::staticMetaObject = {
    { &QApplication::staticMetaObject, qt_meta_stringdata_OnX_QApplication,
      qt_meta_data_OnX_QApplication, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX_QApplication::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX_QApplication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX_QApplication::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX_QApplication))
        return static_cast<void*>(const_cast< OnX_QApplication*>(this));
    return QApplication::qt_metacast(_clname);
}

int OnX_QApplication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_OnX_QOpenGLArea[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      39,   16,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_OnX_QOpenGLArea[] = {
    "OnX_QOpenGLArea\0\0paint(QCallbackData&)\0"
    "eventHandler(QCallbackData&)\0"
};

void OnX_QOpenGLArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OnX_QOpenGLArea *_t = static_cast<OnX_QOpenGLArea *>(_o);
        switch (_id) {
        case 0: _t->paint((*reinterpret_cast< QCallbackData(*)>(_a[1]))); break;
        case 1: _t->eventHandler((*reinterpret_cast< QCallbackData(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData OnX_QOpenGLArea::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX_QOpenGLArea::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_OnX_QOpenGLArea,
      qt_meta_data_OnX_QOpenGLArea, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX_QOpenGLArea::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX_QOpenGLArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX_QOpenGLArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX_QOpenGLArea))
        return static_cast<void*>(const_cast< OnX_QOpenGLArea*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int OnX_QOpenGLArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
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
void OnX_QOpenGLArea::paint(QCallbackData & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OnX_QOpenGLArea::eventHandler(QCallbackData & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_OnX_QSplitter[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_OnX_QSplitter[] = {
    "OnX_QSplitter\0"
};

void OnX_QSplitter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX_QSplitter::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX_QSplitter::staticMetaObject = {
    { &QSplitter::staticMetaObject, qt_meta_stringdata_OnX_QSplitter,
      qt_meta_data_OnX_QSplitter, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX_QSplitter::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX_QSplitter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX_QSplitter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX_QSplitter))
        return static_cast<void*>(const_cast< OnX_QSplitter*>(this));
    return QSplitter::qt_metacast(_clname);
}

int OnX_QSplitter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSplitter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_OnX_QColorDialog[] = {

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
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      23,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_OnX_QColorDialog[] = {
    "OnX_QColorDialog\0\0ok()\0color_selected()\0"
};

void OnX_QColorDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OnX_QColorDialog *_t = static_cast<OnX_QColorDialog *>(_o);
        switch (_id) {
        case 0: _t->ok(); break;
        case 1: _t->color_selected(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX_QColorDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX_QColorDialog::staticMetaObject = {
    { &OnX_QColorDialog_Base::staticMetaObject, qt_meta_stringdata_OnX_QColorDialog,
      qt_meta_data_OnX_QColorDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX_QColorDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX_QColorDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX_QColorDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX_QColorDialog))
        return static_cast<void*>(const_cast< OnX_QColorDialog*>(this));
    return OnX_QColorDialog_Base::qt_metacast(_clname);
}

int OnX_QColorDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OnX_QColorDialog_Base::qt_metacall(_c, _id, _a);
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
void OnX_QColorDialog::ok()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_OnX_QFileDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_OnX_QFileDialog[] = {
    "OnX_QFileDialog\0\0ok()\0"
};

void OnX_QFileDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OnX_QFileDialog *_t = static_cast<OnX_QFileDialog *>(_o);
        switch (_id) {
        case 0: _t->ok(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX_QFileDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX_QFileDialog::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OnX_QFileDialog,
      qt_meta_data_OnX_QFileDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX_QFileDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX_QFileDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX_QFileDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX_QFileDialog))
        return static_cast<void*>(const_cast< OnX_QFileDialog*>(this));
    return QWidget::qt_metacast(_clname);
}

int OnX_QFileDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void OnX_QFileDialog::ok()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_OnX_QMenu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_OnX_QMenu[] = {
    "OnX_QMenu\0"
};

void OnX_QMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX_QMenu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX_QMenu::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OnX_QMenu,
      qt_meta_data_OnX_QMenu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX_QMenu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX_QMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX_QMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX_QMenu))
        return static_cast<void*>(const_cast< OnX_QMenu*>(this));
    return QWidget::qt_metacast(_clname);
}

int OnX_QMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_OnX_QMenuItem[] = {

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
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      27,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OnX_QMenuItem[] = {
    "OnX_QMenuItem\0\0activated()\0execute()\0"
};

void OnX_QMenuItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OnX_QMenuItem *_t = static_cast<OnX_QMenuItem *>(_o);
        switch (_id) {
        case 0: _t->activated(); break;
        case 1: _t->execute(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX_QMenuItem::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX_QMenuItem::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OnX_QMenuItem,
      qt_meta_data_OnX_QMenuItem, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX_QMenuItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX_QMenuItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX_QMenuItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX_QMenuItem))
        return static_cast<void*>(const_cast< OnX_QMenuItem*>(this));
    return QWidget::qt_metacast(_clname);
}

int OnX_QMenuItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void OnX_QMenuItem::activated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_OnX_QToggleMenuItem[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_OnX_QToggleMenuItem[] = {
    "OnX_QToggleMenuItem\0"
};

void OnX_QToggleMenuItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX_QToggleMenuItem::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX_QToggleMenuItem::staticMetaObject = {
    { &OnX_QMenuItem::staticMetaObject, qt_meta_stringdata_OnX_QToggleMenuItem,
      qt_meta_data_OnX_QToggleMenuItem, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX_QToggleMenuItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX_QToggleMenuItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX_QToggleMenuItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX_QToggleMenuItem))
        return static_cast<void*>(const_cast< OnX_QToggleMenuItem*>(this));
    return OnX_QMenuItem::qt_metacast(_clname);
}

int OnX_QToggleMenuItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = OnX_QMenuItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_OnX_QMenuSeparator[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_OnX_QMenuSeparator[] = {
    "OnX_QMenuSeparator\0"
};

void OnX_QMenuSeparator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX_QMenuSeparator::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX_QMenuSeparator::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OnX_QMenuSeparator,
      qt_meta_data_OnX_QMenuSeparator, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX_QMenuSeparator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX_QMenuSeparator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX_QMenuSeparator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX_QMenuSeparator))
        return static_cast<void*>(const_cast< OnX_QMenuSeparator*>(this));
    return QWidget::qt_metacast(_clname);
}

int OnX_QMenuSeparator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_OnX_QMainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_OnX_QMainWindow[] = {
    "OnX_QMainWindow\0\0close_window()\0"
};

void OnX_QMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        OnX_QMainWindow *_t = static_cast<OnX_QMainWindow *>(_o);
        switch (_id) {
        case 0: _t->close_window(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData OnX_QMainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject OnX_QMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_OnX_QMainWindow,
      qt_meta_data_OnX_QMainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &OnX_QMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *OnX_QMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *OnX_QMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OnX_QMainWindow))
        return static_cast<void*>(const_cast< OnX_QMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int OnX_QMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void OnX_QMainWindow::close_window()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
