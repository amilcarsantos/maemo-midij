/****************************************************************************
** Meta object code from reading C++ file 'MidijDrawGraphicsScene.h'
**
** Created: Mon 4. Jul 18:35:03 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MidijDrawGraphicsScene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MidijDrawGraphicsScene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GraphicsSelectionItem[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   23,   22,   22, 0x05,
      57,   48,   22,   22, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_GraphicsSelectionItem[] = {
    "GraphicsSelectionItem\0\0rect\0"
    "rectChanged(QRectF)\0scenePos\0"
    "contextMenu(QPointF)\0"
};

const QMetaObject GraphicsSelectionItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GraphicsSelectionItem,
      qt_meta_data_GraphicsSelectionItem, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GraphicsSelectionItem::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GraphicsSelectionItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GraphicsSelectionItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GraphicsSelectionItem))
        return static_cast<void*>(const_cast< GraphicsSelectionItem*>(this));
    if (!strcmp(_clname, "QGraphicsRectItem"))
        return static_cast< QGraphicsRectItem*>(const_cast< GraphicsSelectionItem*>(this));
    return QObject::qt_metacast(_clname);
}

int GraphicsSelectionItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rectChanged((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 1: contextMenu((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void GraphicsSelectionItem::rectChanged(const QRectF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GraphicsSelectionItem::contextMenu(const QPointF & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_MidijDrawGraphicsScene[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_MidijDrawGraphicsScene[] = {
    "MidijDrawGraphicsScene\0\0"
    "itemClicked(QGraphicsItem*)\0"
};

const QMetaObject MidijDrawGraphicsScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_MidijDrawGraphicsScene,
      qt_meta_data_MidijDrawGraphicsScene, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MidijDrawGraphicsScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MidijDrawGraphicsScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MidijDrawGraphicsScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MidijDrawGraphicsScene))
        return static_cast<void*>(const_cast< MidijDrawGraphicsScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int MidijDrawGraphicsScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: itemClicked((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void MidijDrawGraphicsScene::itemClicked(QGraphicsItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
