/****************************************************************************
** Meta object code from reading C++ file 'GraphicsMidiControlItem.h'
**
** Created: Mon 4. Jul 18:36:11 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../midij/gui/GraphicsMidiControlItem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GraphicsMidiControlItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MidiOutputController[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      21, 0x0,    5,   18,

 // enum data: key, value
      33, uint(MidiOutputController::Button),
      40, uint(MidiOutputController::SliderAbsolute),
      55, uint(MidiOutputController::SliderRelative),
      70, uint(MidiOutputController::WheelAbsolute),
      84, uint(MidiOutputController::WheelRelative),

       0        // eod
};

static const char qt_meta_stringdata_MidiOutputController[] = {
    "MidiOutputController\0ControlType\0"
    "Button\0SliderAbsolute\0SliderRelative\0"
    "WheelAbsolute\0WheelRelative\0"
};

const QMetaObject MidiOutputController::staticMetaObject = {
    { 0, qt_meta_stringdata_MidiOutputController,
      qt_meta_data_MidiOutputController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidiOutputController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION
static const uint qt_meta_data_GraphicsMidiControlItem[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       1,   24, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      53,   25,   24,   24, 0x05,
      88,   24,   24,   24, 0x05,

 // properties: name, type, flags
     110,  105, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_GraphicsMidiControlItem[] = {
    "GraphicsMidiControlItem\0\0"
    "statusByte,midino,midivalue\0"
    "midiShortMsgOut(uchar,uchar,uchar)\0"
    "visibleChanged()\0bool\0visible\0"
};

const QMetaObject GraphicsMidiControlItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GraphicsMidiControlItem,
      qt_meta_data_GraphicsMidiControlItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GraphicsMidiControlItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GraphicsMidiControlItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GraphicsMidiControlItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GraphicsMidiControlItem))
        return static_cast<void*>(const_cast< GraphicsMidiControlItem*>(this));
    if (!strcmp(_clname, "QGraphicsPixmapItem"))
        return static_cast< QGraphicsPixmapItem*>(const_cast< GraphicsMidiControlItem*>(this));
    return QObject::qt_metacast(_clname);
}

int GraphicsMidiControlItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: midiShortMsgOut((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< uchar(*)>(_a[3]))); break;
        case 1: visibleChanged(); break;
        default: ;
        }
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = isVisible(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setVisible(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void GraphicsMidiControlItem::midiShortMsgOut(uchar _t1, uchar _t2, uchar _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GraphicsMidiControlItem::visibleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
