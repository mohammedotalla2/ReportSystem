/****************************************************************************
** Meta object code from reading C++ file 'saleswindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/saleswindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'saleswindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SalesWindow_t {
    QByteArrayData data[17];
    char stringdata0[219];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SalesWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SalesWindow_t qt_meta_stringdata_SalesWindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SalesWindow"
QT_MOC_LITERAL(1, 12, 17), // "onProductSelected"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "index"
QT_MOC_LITERAL(4, 37, 18), // "onCustomerSelected"
QT_MOC_LITERAL(5, 56, 12), // "onQtyChanged"
QT_MOC_LITERAL(6, 69, 7), // "addItem"
QT_MOC_LITERAL(7, 77, 10), // "deleteItem"
QT_MOC_LITERAL(8, 88, 11), // "saveInvoice"
QT_MOC_LITERAL(9, 100, 12), // "printInvoice"
QT_MOC_LITERAL(10, 113, 12), // "printThermal"
QT_MOC_LITERAL(11, 126, 9), // "exportPDF"
QT_MOC_LITERAL(12, 136, 20), // "printPreparationList"
QT_MOC_LITERAL(13, 157, 21), // "showCustomerStatement"
QT_MOC_LITERAL(14, 179, 13), // "navigateFirst"
QT_MOC_LITERAL(15, 193, 12), // "navigateLast"
QT_MOC_LITERAL(16, 206, 12) // "updateTotals"

    },
    "SalesWindow\0onProductSelected\0\0index\0"
    "onCustomerSelected\0onQtyChanged\0addItem\0"
    "deleteItem\0saveInvoice\0printInvoice\0"
    "printThermal\0exportPDF\0printPreparationList\0"
    "showCustomerStatement\0navigateFirst\0"
    "navigateLast\0updateTotals"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SalesWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x08 /* Private */,
       4,    1,   87,    2, 0x08 /* Private */,
       5,    0,   90,    2, 0x08 /* Private */,
       6,    0,   91,    2, 0x08 /* Private */,
       7,    0,   92,    2, 0x08 /* Private */,
       8,    0,   93,    2, 0x08 /* Private */,
       9,    0,   94,    2, 0x08 /* Private */,
      10,    0,   95,    2, 0x08 /* Private */,
      11,    0,   96,    2, 0x08 /* Private */,
      12,    0,   97,    2, 0x08 /* Private */,
      13,    0,   98,    2, 0x08 /* Private */,
      14,    0,   99,    2, 0x08 /* Private */,
      15,    0,  100,    2, 0x08 /* Private */,
      16,    0,  101,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
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
    QMetaType::Void,

       0        // eod
};

void SalesWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SalesWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onProductSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onCustomerSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onQtyChanged(); break;
        case 3: _t->addItem(); break;
        case 4: _t->deleteItem(); break;
        case 5: _t->saveInvoice(); break;
        case 6: _t->printInvoice(); break;
        case 7: _t->printThermal(); break;
        case 8: _t->exportPDF(); break;
        case 9: _t->printPreparationList(); break;
        case 10: _t->showCustomerStatement(); break;
        case 11: _t->navigateFirst(); break;
        case 12: _t->navigateLast(); break;
        case 13: _t->updateTotals(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SalesWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_SalesWindow.data,
    qt_meta_data_SalesWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SalesWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SalesWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SalesWindow.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SalesWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
