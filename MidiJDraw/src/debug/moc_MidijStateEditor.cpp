/****************************************************************************
** Meta object code from reading C++ file 'MidijStateEditor.h'
**
** Created: Mon 4. Jul 18:35:18 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MidijStateEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MidijStateEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MidijStateEditor[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      35,   17,   17,   17, 0x0a,
      44,   17,   17,   17, 0x0a,
      54,   17,   17,   17, 0x0a,
      72,   17,   17,   17, 0x0a,
      96,   17,   17,   17, 0x0a,
     121,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MidijStateEditor[] = {
    "MidijStateEditor\0\0stateSelect(int)\0"
    "zoomIn()\0zoomOut()\0gridVisible(bool)\0"
    "gridSizeSelect(QString)\0"
    "gridExtraSelect(QString)\0"
    "sceneRectChanged(QRectF)\0"
};

const QMetaObject MidijStateEditor::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MidijStateEditor,
      qt_meta_data_MidijStateEditor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidijStateEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidijStateEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidijStateEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidijStateEditor))
        return static_cast<void*>(const_cast< MidijStateEditor*>(this));
    return QDialog::qt_metacast(_clname);
}

int MidijStateEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stateSelect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: zoomIn(); break;
        case 2: zoomOut(); break;
        case 3: gridVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: gridSizeSelect((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: gridExtraSelect((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: sceneRectChanged((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
