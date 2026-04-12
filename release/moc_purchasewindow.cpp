/****************************************************************************
** Meta object code from reading C++ file 'purchasewindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/purchasewindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'purchasewindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PurchaseWindow_t {
    QByteArrayData data[15];
    char stringdata0[173];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PurchaseWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PurchaseWindow_t qt_meta_stringdata_PurchaseWindow = {
    {
QT_MOC_LITERAL(0, 0, 14), // "PurchaseWindow"
QT_MOC_LITERAL(1, 15, 17), // "onProductSelected"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "index"
QT_MOC_LITERAL(4, 40, 7), // "addItem"
QT_MOC_LITERAL(5, 48, 10), // "deleteItem"
QT_MOC_LITERAL(6, 59, 11), // "saveInvoice"
QT_MOC_LITERAL(7, 71, 9), // "clearForm"
QT_MOC_LITERAL(8, 81, 12), // "printInvoice"
QT_MOC_LITERAL(9, 94, 9), // "exportPDF"
QT_MOC_LITERAL(10, 104, 15), // "calculateTotals"
QT_MOC_LITERAL(11, 120, 13), // "navigateFirst"
QT_MOC_LITERAL(12, 134, 12), // "navigatePrev"
QT_MOC_LITERAL(13, 147, 12), // "navigateNext"
QT_MOC_LITERAL(14, 160, 12) // "navigateLast"

    },
    "PurchaseWindow\0onProductSelected\0\0"
    "index\0addItem\0deleteItem\0saveInvoice\0"
    "clearForm\0printInvoice\0exportPDF\0"
    "calculateTotals\0navigateFirst\0"
    "navigatePrev\0navigateNext\0navigateLast"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PurchaseWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x08 /* Private */,
       4,    0,   77,    2, 0x08 /* Private */,
       5,    0,   78,    2, 0x08 /* Private */,
       6,    0,   79,    2, 0x08 /* Private */,
       7,    0,   80,    2, 0x08 /* Private */,
       8,    0,   81,    2, 0x08 /* Private */,
       9,    0,   82,    2, 0x08 /* Private */,
      10,    0,   83,    2, 0x08 /* Private */,
      11,    0,   84,    2, 0x08 /* Private */,
      12,    0,   85,    2, 0x08 /* Private */,
      13,    0,   86,    2, 0x08 /* Private */,
      14,    0,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PurchaseWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PurchaseWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onProductSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->addItem(); break;
        case 2: _t->deleteItem(); break;
        case 3: _t->saveInvoice(); break;
        case 4: _t->clearForm(); break;
        case 5: _t->printInvoice(); break;
        case 6: _t->exportPDF(); break;
        case 7: _t->calculateTotals(); break;
        case 8: _t->navigateFirst(); break;
        case 9: _t->navigatePrev(); break;
        case 10: _t->navigateNext(); break;
        case 11: _t->navigateLast(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PurchaseWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_PurchaseWindow.data,
    qt_meta_data_PurchaseWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PurchaseWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PurchaseWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PurchaseWindow.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int PurchaseWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
