/****************************************************************************
** Meta object code from reading C++ file 'MidijDraw.h'
**
** Created: Wed 4. Jan 19:26:55 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MidijDraw.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MidijDraw.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MidiJDraw[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      27,   10,   10,   10, 0x0a,
      83,   70,   10,   10, 0x0a,
     130,  116,   10,   10, 0x0a,
     185,  168,   10,   10, 0x0a,
     246,   10,   10,   10, 0x0a,
     287,  282,   10,   10, 0x0a,
     316,  312,   10,   10, 0x0a,
     341,   10,   10,   10, 0x0a,
     359,   10,   10,   10, 0x0a,
     368,   10,   10,   10, 0x0a,
     387,  378,   10,   10, 0x0a,
     422,  414,   10,   10, 0x0a,
     458,  449,   10,   10, 0x0a,
     487,   10,   10,   10, 0x0a,
     497,   10,   10,   10, 0x0a,
     506,   10,   10,   10, 0x0a,
     521,   10,   10,   10, 0x0a,
     533,   10,   10,   10, 0x0a,
     546,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MidiJDraw[] = {
    "MidiJDraw\0\0selectProject()\0"
    "propertyBrowserItemChanged(QtBrowserItem*)\0"
    "property,val\0intValueChanged(QtProperty*,int)\0"
    "property,area\0areaValueChanged(QtProperty*,QString)\0"
    "current,previous\0"
    "controlBrowserItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)\0"
    "graphicsItemClicked(QGraphicsItem*)\0"
    "rect\0sceneRectChanged(QRectF)\0pos\0"
    "itemContextMenu(QPointF)\0callStateEditor()\0"
    "zoomIn()\0zoomOut()\0sysexMsg\0"
    "midiSysexSelected(QString)\0midiMsg\0"
    "midiStateSelected(QString)\0modified\0"
    "xmlModificationChanged(bool)\0saveXml()\0"
    "reload()\0sceneSavePng()\0runScript()\0"
    "stopScript()\0debuggerResumed()\0"
};

const QMetaObject MidiJDraw::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MidiJDraw,
      qt_meta_data_MidiJDraw, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidiJDraw::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidiJDraw::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidiJDraw::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidiJDraw))
        return static_cast<void*>(const_cast< MidiJDraw*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MidiJDraw::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectProject(); break;
        case 1: propertyBrowserItemChanged((*reinterpret_cast< QtBrowserItem*(*)>(_a[1]))); break;
        case 2: intValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: areaValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: controlBrowserItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 5: graphicsItemClicked((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        case 6: sceneRectChanged((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 7: itemContextMenu((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 8: callStateEditor(); break;
        case 9: zoomIn(); break;
        case 10: zoomOut(); break;
        case 11: midiSysexSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: midiStateSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: xmlModificationChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: saveXml(); break;
        case 15: reload(); break;
        case 16: sceneSavePng(); break;
        case 17: runScript(); break;
        case 18: stopScript(); break;
        case 19: debuggerResumed(); break;
        default: ;
        }
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
