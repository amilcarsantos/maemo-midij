/****************************************************************************
** Meta object code from reading C++ file 'MidijMidiObject.h'
**
** Created: Mon 4. Jul 18:35:35 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MidijMidiObject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MidijMidiObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MidijMidiObject[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      64,   35,   16,   16, 0x0a,
      95,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MidijMidiObject[] = {
    "MidijMidiObject\0\0receiveFinished()\0"
    "status,midicontrol,midivalue\0"
    "emulReceive(uchar,uchar,uchar)\0"
    "processPendingReceives()\0"
};

const QMetaObject MidijMidiObject::staticMetaObject = {
    { &MidiObject::staticMetaObject, qt_meta_stringdata_MidijMidiObject,
      qt_meta_data_MidijMidiObject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidijMidiObject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidijMidiObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidijMidiObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidijMidiObject))
        return static_cast<void*>(const_cast< MidijMidiObject*>(this));
    return MidiObject::qt_metacast(_clname);
}

int MidijMidiObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MidiObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: receiveFinished(); break;
        case 1: emulReceive((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< uchar(*)>(_a[3]))); break;
        case 2: processPendingReceives(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MidijMidiObject::receiveFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
