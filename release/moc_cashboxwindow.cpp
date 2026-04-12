/****************************************************************************
** Meta object code from reading C++ file 'cashboxwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/cashboxwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cashboxwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CashBoxWindow_t {
    QByteArrayData data[18];
    char stringdata0[253];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CashBoxWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CashBoxWindow_t qt_meta_stringdata_CashBoxWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CashBoxWindow"
QT_MOC_LITERAL(1, 14, 13), // "onTypeChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 20), // "onMainAccountChanged"
QT_MOC_LITERAL(4, 50, 5), // "index"
QT_MOC_LITERAL(5, 56, 19), // "calculateEquivalent"
QT_MOC_LITERAL(6, 76, 15), // "saveTransaction"
QT_MOC_LITERAL(7, 92, 14), // "newTransaction"
QT_MOC_LITERAL(8, 107, 17), // "deleteTransaction"
QT_MOC_LITERAL(9, 125, 17), // "searchTransaction"
QT_MOC_LITERAL(10, 143, 12), // "printReceipt"
QT_MOC_LITERAL(11, 156, 9), // "exportPDF"
QT_MOC_LITERAL(12, 166, 13), // "navigateFirst"
QT_MOC_LITERAL(13, 180, 12), // "navigatePrev"
QT_MOC_LITERAL(14, 193, 12), // "navigateNext"
QT_MOC_LITERAL(15, 206, 12), // "navigateLast"
QT_MOC_LITERAL(16, 219, 19), // "receiveFromCustomer"
QT_MOC_LITERAL(17, 239, 13) // "payToCustomer"

    },
    "CashBoxWindow\0onTypeChanged\0\0"
    "onMainAccountChanged\0index\0"
    "calculateEquivalent\0saveTransaction\0"
    "newTransaction\0deleteTransaction\0"
    "searchTransaction\0printReceipt\0exportPDF\0"
    "navigateFirst\0navigatePrev\0navigateNext\0"
    "navigateLast\0receiveFromCustomer\0"
    "payToCustomer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CashBoxWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    1,   90,    2, 0x08 /* Private */,
       5,    0,   93,    2, 0x08 /* Private */,
       6,    0,   94,    2, 0x08 /* Private */,
       7,    0,   95,    2, 0x08 /* Private */,
       8,    0,   96,    2, 0x08 /* Private */,
       9,    0,   97,    2, 0x08 /* Private */,
      10,    0,   98,    2, 0x08 /* Private */,
      11,    0,   99,    2, 0x08 /* Private */,
      12,    0,  100,    2, 0x08 /* Private */,
      13,    0,  101,    2, 0x08 /* Private */,
      14,    0,  102,    2, 0x08 /* Private */,
      15,    0,  103,    2, 0x08 /* Private */,
      16,    0,  104,    2, 0x08 /* Private */,
      17,    0,  105,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
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
    QMetaType::Void,

       0        // eod
};

void CashBoxWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CashBoxWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onTypeChanged(); break;
        case 1: _t->onMainAccountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->calculateEquivalent(); break;
        case 3: _t->saveTransaction(); break;
        case 4: _t->newTransaction(); break;
        case 5: _t->deleteTransaction(); break;
        case 6: _t->searchTransaction(); break;
        case 7: _t->printReceipt(); break;
        case 8: _t->exportPDF(); break;
        case 9: _t->navigateFirst(); break;
        case 10: _t->navigatePrev(); break;
        case 11: _t->navigateNext(); break;
        case 12: _t->navigateLast(); break;
        case 13: _t->receiveFromCustomer(); break;
        case 14: _t->payToCustomer(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CashBoxWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CashBoxWindow.data,
    qt_meta_data_CashBoxWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CashBoxWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CashBoxWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CashBoxWindow.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int CashBoxWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
