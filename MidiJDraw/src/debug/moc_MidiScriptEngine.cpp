/****************************************************************************
** Meta object code from reading C++ file 'MidiScriptEngine.h'
**
** Created: Mon 4. Jul 18:36:43 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../midij/mixxx/script/MidiScriptEngine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MidiScriptEngine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MidiScriptEngine[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   32,   17,   17, 0x0a,

 // methods: signature, parameters, type, tag, flags
      81,   70,   63,   17, 0x02,
     127,  107,   17,   17, 0x02,
     196,  165,  160,   17, 0x02,
     261,  241,  160,   17, 0x22,
     301,   70,   17,   17, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_MidiScriptEngine[] = {
    "MidiScriptEngine\0\0initialized()\0value\0"
    "slotValueChanged(double)\0double\0"
    "group,name\0getValue(QString,QString)\0"
    "group,name,newValue\0"
    "setValue(QString,QString,double)\0bool\0"
    "group,name,function,disconnect\0"
    "connectControl(QString,QString,QString,bool)\0"
    "group,name,function\0"
    "connectControl(QString,QString,QString)\0"
    "trigger(QString,QString)\0"
};

const QMetaObject MidiScriptEngine::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_MidiScriptEngine,
      qt_meta_data_MidiScriptEngine, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidiScriptEngine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidiScriptEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidiScriptEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidiScriptEngine))
        return static_cast<void*>(const_cast< MidiScriptEngine*>(this));
    return QThread::qt_metacast(_clname);
}

int MidiScriptEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: initialized(); break;
        case 1: slotValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: { double _r = getValue((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 3: setValue((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 4: { bool _r = connectControl((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = connectControl((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: trigger((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MidiScriptEngine::initialized()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
