/****************************************************************************
** Meta object code from reading C++ file 'MidiMappingGui.h'
**
** Created: Mon 4. Jul 18:36:02 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../midij/gui/MidiMappingGui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MidiMappingGui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MidiMappingGui[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      85,   57,   15,   15, 0x0a,
     119,   16,   15,   15, 0x0a,

 // methods: signature, parameters, type, tag, flags
     147,   15,   15,   15, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_MidiMappingGui[] = {
    "MidiMappingGui\0\0data,length\0"
    "midiSysexMsgOut(uchar*,uint)\0"
    "statusByte,midino,midivalue\0"
    "midiShortMsgIn(uchar,uchar,uchar)\0"
    "midiSysexMsgIn(uchar*,uint)\0"
    "ctrlAllNotesOff()\0"
};

const QMetaObject MidiMappingGui::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MidiMappingGui,
      qt_meta_data_MidiMappingGui, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidiMappingGui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidiMappingGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidiMappingGui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidiMappingGui))
        return static_cast<void*>(const_cast< MidiMappingGui*>(this));
    return QObject::qt_metacast(_clname);
}

int MidiMappingGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: midiSysexMsgOut((*reinterpret_cast< uchar*(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 1: midiShortMsgIn((*reinterpret_cast< uchar(*)>(_a[1])),(*reinterpret_cast< uchar(*)>(_a[2])),(*reinterpret_cast< uchar(*)>(_a[3]))); break;
        case 2: midiSysexMsgIn((*reinterpret_cast< uchar*(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 3: ctrlAllNotesOff(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void MidiMappingGui::midiSysexMsgOut(uchar * _t1, uint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
