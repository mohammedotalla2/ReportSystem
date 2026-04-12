/********************************************************************************
** Form generated from reading UI file 'purchasewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PURCHASEWINDOW_H
#define UI_PURCHASEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PurchaseWindow
{
public:
    QVBoxLayout *rootLayout;
    QWidget *headerWidget;
    QGridLayout *headerGrid;
    QLabel *label;
    QSpinBox *spnInvoiceNo;
    QLabel *label1;
    QDateEdit *dateYear;
    QDateEdit *dateMonth;
    QDateEdit *dateDay;
    QLabel *label2;
    QComboBox *cmbPurchaseType;
    QLabel *label3;
    QComboBox *cmbSupplier;
    QLabel *label4;
    QComboBox *cmbPayType;
    QLabel *label5;
    QComboBox *cmbCurrency;
    QLabel *label6;
    QLineEdit *txtInvoiceRef;
    QLabel *label7;
    QLineEdit *txtExchRate;
    QLineEdit *txtNotes;
    QHBoxLayout *itemEntryLayout;
    QComboBox *cmbBarcode;
    QComboBox *cmbProduct;
    QLabel *label8;
    QLineEdit *txtQty;
    QLabel *label9;
    QLineEdit *txtCostPrice;
    QLabel *label10;
    QLineEdit *txtWholesaleDollar;
    QLabel *label11;
    QLineEdit *txtRetailDollar;
    QLabel *label12;
    QLineEdit *txtWholesaleDinar;
    QLabel *label13;
    QLineEdit *txtRetailDinar;
    QPushButton *btnAdd;
    QPushButton *btnEdit;
    QPushButton *btnDel;
    QLabel *lblStock;
    QTableWidget *tblItems;
    QHBoxLayout *hboxLayout;
    QLabel *lblSupplierBal;
    QSpacerItem *spacerItem;
    QLabel *label14;
    QLineEdit *txtGrandDollar;
    QLabel *label15;
    QLineEdit *txtGrandDinar;
    QLabel *label16;
    QLineEdit *txtDiscount;
    QWidget *toolbar;
    QHBoxLayout *toolbarLayout;
    QPushButton *btnSave;
    QPushButton *btnSearch;
    QPushButton *btnDelete;
    QPushButton *btnPrint;
    QPushButton *btnPdf;
    QLabel *label17;
    QSpinBox *spnOrganiser;
    QLabel *label18;
    QLabel *lblTime;
    QSpacerItem *spacerItem1;
    QLabel *lblExpRatio;
    QPushButton *btnTotalExpenses;
    QSpacerItem *spacerItem2;
    QPushButton *btnFirst;
    QLabel *lblNav;
    QLabel *label19;
    QLabel *lblTotal;
    QPushButton *btnLast;
    QPushButton *btnRecv;
    QPushButton *btnView;
    QPushButton *btnFwd;
    QPushButton *btnSign;

    void setupUi(QDialog *PurchaseWindow)
    {
        if (PurchaseWindow->objectName().isEmpty())
            PurchaseWindow->setObjectName(QString::fromUtf8("PurchaseWindow"));
        PurchaseWindow->resize(1350, 800);
        PurchaseWindow->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(PurchaseWindow);
        rootLayout->setSpacing(4);
        rootLayout->setContentsMargins(4, 4, 4, 4);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        headerWidget = new QWidget(PurchaseWindow);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerGrid = new QGridLayout(headerWidget);
        headerGrid->setSpacing(4);
        headerGrid->setContentsMargins(6, 6, 6, 6);
        headerGrid->setObjectName(QString::fromUtf8("headerGrid"));
        headerGrid->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(headerWidget);
        label->setObjectName(QString::fromUtf8("label"));

        headerGrid->addWidget(label, 0, 12, 1, 1);

        spnInvoiceNo = new QSpinBox(headerWidget);
        spnInvoiceNo->setObjectName(QString::fromUtf8("spnInvoiceNo"));
        spnInvoiceNo->setMaximum(9999999);

        headerGrid->addWidget(spnInvoiceNo, 0, 11, 1, 1);

        label1 = new QLabel(headerWidget);
        label1->setObjectName(QString::fromUtf8("label1"));

        headerGrid->addWidget(label1, 0, 10, 1, 1);

        dateYear = new QDateEdit(headerWidget);
        dateYear->setObjectName(QString::fromUtf8("dateYear"));

        headerGrid->addWidget(dateYear, 0, 9, 1, 1);

        dateMonth = new QDateEdit(headerWidget);
        dateMonth->setObjectName(QString::fromUtf8("dateMonth"));

        headerGrid->addWidget(dateMonth, 0, 8, 1, 1);

        dateDay = new QDateEdit(headerWidget);
        dateDay->setObjectName(QString::fromUtf8("dateDay"));

        headerGrid->addWidget(dateDay, 0, 7, 1, 1);

        label2 = new QLabel(headerWidget);
        label2->setObjectName(QString::fromUtf8("label2"));

        headerGrid->addWidget(label2, 0, 6, 1, 1);

        cmbPurchaseType = new QComboBox(headerWidget);
        cmbPurchaseType->addItem(QString());
        cmbPurchaseType->addItem(QString());
        cmbPurchaseType->setObjectName(QString::fromUtf8("cmbPurchaseType"));

        headerGrid->addWidget(cmbPurchaseType, 0, 5, 1, 1);

        label3 = new QLabel(headerWidget);
        label3->setObjectName(QString::fromUtf8("label3"));

        headerGrid->addWidget(label3, 0, 4, 1, 1);

        cmbSupplier = new QComboBox(headerWidget);
        cmbSupplier->setObjectName(QString::fromUtf8("cmbSupplier"));
        cmbSupplier->setEditable(true);
        cmbSupplier->setMinimumWidth(180);

        headerGrid->addWidget(cmbSupplier, 0, 3, 1, 1);

        label4 = new QLabel(headerWidget);
        label4->setObjectName(QString::fromUtf8("label4"));

        headerGrid->addWidget(label4, 0, 2, 1, 1);

        cmbPayType = new QComboBox(headerWidget);
        cmbPayType->addItem(QString());
        cmbPayType->addItem(QString());
        cmbPayType->setObjectName(QString::fromUtf8("cmbPayType"));

        headerGrid->addWidget(cmbPayType, 0, 1, 1, 1);

        label5 = new QLabel(headerWidget);
        label5->setObjectName(QString::fromUtf8("label5"));

        headerGrid->addWidget(label5, 1, 12, 1, 1);

        cmbCurrency = new QComboBox(headerWidget);
        cmbCurrency->addItem(QString());
        cmbCurrency->addItem(QString());
        cmbCurrency->setObjectName(QString::fromUtf8("cmbCurrency"));

        headerGrid->addWidget(cmbCurrency, 1, 11, 1, 1);

        label6 = new QLabel(headerWidget);
        label6->setObjectName(QString::fromUtf8("label6"));

        headerGrid->addWidget(label6, 1, 9, 1, 1);

        txtInvoiceRef = new QLineEdit(headerWidget);
        txtInvoiceRef->setObjectName(QString::fromUtf8("txtInvoiceRef"));

        headerGrid->addWidget(txtInvoiceRef, 1, 8, 1, 1);

        label7 = new QLabel(headerWidget);
        label7->setObjectName(QString::fromUtf8("label7"));

        headerGrid->addWidget(label7, 1, 6, 1, 1);

        txtExchRate = new QLineEdit(headerWidget);
        txtExchRate->setObjectName(QString::fromUtf8("txtExchRate"));
        txtExchRate->setMaximumWidth(80);

        headerGrid->addWidget(txtExchRate, 1, 5, 1, 1);

        txtNotes = new QLineEdit(headerWidget);
        txtNotes->setObjectName(QString::fromUtf8("txtNotes"));

        headerGrid->addWidget(txtNotes, 2, 0, 1, 13);


        rootLayout->addWidget(headerWidget);

        itemEntryLayout = new QHBoxLayout();
        itemEntryLayout->setObjectName(QString::fromUtf8("itemEntryLayout"));
        cmbBarcode = new QComboBox(PurchaseWindow);
        cmbBarcode->setObjectName(QString::fromUtf8("cmbBarcode"));
        cmbBarcode->setEditable(true);
        cmbBarcode->setMinimumWidth(110);

        itemEntryLayout->addWidget(cmbBarcode);

        cmbProduct = new QComboBox(PurchaseWindow);
        cmbProduct->setObjectName(QString::fromUtf8("cmbProduct"));
        cmbProduct->setEditable(true);
        cmbProduct->setMinimumWidth(180);

        itemEntryLayout->addWidget(cmbProduct);

        label8 = new QLabel(PurchaseWindow);
        label8->setObjectName(QString::fromUtf8("label8"));

        itemEntryLayout->addWidget(label8);

        txtQty = new QLineEdit(PurchaseWindow);
        txtQty->setObjectName(QString::fromUtf8("txtQty"));
        txtQty->setMaximumWidth(60);

        itemEntryLayout->addWidget(txtQty);

        label9 = new QLabel(PurchaseWindow);
        label9->setObjectName(QString::fromUtf8("label9"));

        itemEntryLayout->addWidget(label9);

        txtCostPrice = new QLineEdit(PurchaseWindow);
        txtCostPrice->setObjectName(QString::fromUtf8("txtCostPrice"));
        txtCostPrice->setMaximumWidth(80);

        itemEntryLayout->addWidget(txtCostPrice);

        label10 = new QLabel(PurchaseWindow);
        label10->setObjectName(QString::fromUtf8("label10"));

        itemEntryLayout->addWidget(label10);

        txtWholesaleDollar = new QLineEdit(PurchaseWindow);
        txtWholesaleDollar->setObjectName(QString::fromUtf8("txtWholesaleDollar"));
        txtWholesaleDollar->setMaximumWidth(80);

        itemEntryLayout->addWidget(txtWholesaleDollar);

        label11 = new QLabel(PurchaseWindow);
        label11->setObjectName(QString::fromUtf8("label11"));

        itemEntryLayout->addWidget(label11);

        txtRetailDollar = new QLineEdit(PurchaseWindow);
        txtRetailDollar->setObjectName(QString::fromUtf8("txtRetailDollar"));
        txtRetailDollar->setMaximumWidth(80);

        itemEntryLayout->addWidget(txtRetailDollar);

        label12 = new QLabel(PurchaseWindow);
        label12->setObjectName(QString::fromUtf8("label12"));

        itemEntryLayout->addWidget(label12);

        txtWholesaleDinar = new QLineEdit(PurchaseWindow);
        txtWholesaleDinar->setObjectName(QString::fromUtf8("txtWholesaleDinar"));
        txtWholesaleDinar->setMaximumWidth(80);

        itemEntryLayout->addWidget(txtWholesaleDinar);

        label13 = new QLabel(PurchaseWindow);
        label13->setObjectName(QString::fromUtf8("label13"));

        itemEntryLayout->addWidget(label13);

        txtRetailDinar = new QLineEdit(PurchaseWindow);
        txtRetailDinar->setObjectName(QString::fromUtf8("txtRetailDinar"));
        txtRetailDinar->setMaximumWidth(80);

        itemEntryLayout->addWidget(txtRetailDinar);

        btnAdd = new QPushButton(PurchaseWindow);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));
        btnAdd->setMaximumWidth(36);
        btnAdd->setMaximumHeight(30);

        itemEntryLayout->addWidget(btnAdd);

        btnEdit = new QPushButton(PurchaseWindow);
        btnEdit->setObjectName(QString::fromUtf8("btnEdit"));
        btnEdit->setMaximumWidth(36);
        btnEdit->setMaximumHeight(30);

        itemEntryLayout->addWidget(btnEdit);

        btnDel = new QPushButton(PurchaseWindow);
        btnDel->setObjectName(QString::fromUtf8("btnDel"));
        btnDel->setMaximumWidth(36);
        btnDel->setMaximumHeight(30);

        itemEntryLayout->addWidget(btnDel);

        lblStock = new QLabel(PurchaseWindow);
        lblStock->setObjectName(QString::fromUtf8("lblStock"));

        itemEntryLayout->addWidget(lblStock);


        rootLayout->addLayout(itemEntryLayout);

        tblItems = new QTableWidget(PurchaseWindow);
        if (tblItems->columnCount() < 9)
            tblItems->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblItems->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblItems->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tblItems->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tblItems->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tblItems->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tblItems->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tblItems->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tblItems->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tblItems->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        tblItems->setObjectName(QString::fromUtf8("tblItems"));
        tblItems->setMinimumHeight(300);
        tblItems->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblItems->setEditTriggers(QAbstractItemView::NoEditTriggers);

        rootLayout->addWidget(tblItems);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lblSupplierBal = new QLabel(PurchaseWindow);
        lblSupplierBal->setObjectName(QString::fromUtf8("lblSupplierBal"));

        hboxLayout->addWidget(lblSupplierBal);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        label14 = new QLabel(PurchaseWindow);
        label14->setObjectName(QString::fromUtf8("label14"));

        hboxLayout->addWidget(label14);

        txtGrandDollar = new QLineEdit(PurchaseWindow);
        txtGrandDollar->setObjectName(QString::fromUtf8("txtGrandDollar"));
        txtGrandDollar->setReadOnly(true);
        txtGrandDollar->setMaximumWidth(100);

        hboxLayout->addWidget(txtGrandDollar);

        label15 = new QLabel(PurchaseWindow);
        label15->setObjectName(QString::fromUtf8("label15"));

        hboxLayout->addWidget(label15);

        txtGrandDinar = new QLineEdit(PurchaseWindow);
        txtGrandDinar->setObjectName(QString::fromUtf8("txtGrandDinar"));
        txtGrandDinar->setReadOnly(true);
        txtGrandDinar->setMaximumWidth(120);

        hboxLayout->addWidget(txtGrandDinar);

        label16 = new QLabel(PurchaseWindow);
        label16->setObjectName(QString::fromUtf8("label16"));

        hboxLayout->addWidget(label16);

        txtDiscount = new QLineEdit(PurchaseWindow);
        txtDiscount->setObjectName(QString::fromUtf8("txtDiscount"));
        txtDiscount->setMaximumWidth(80);

        hboxLayout->addWidget(txtDiscount);


        rootLayout->addLayout(hboxLayout);

        toolbar = new QWidget(PurchaseWindow);
        toolbar->setObjectName(QString::fromUtf8("toolbar"));
        toolbarLayout = new QHBoxLayout(toolbar);
        toolbarLayout->setSpacing(4);
        toolbarLayout->setContentsMargins(6, 6, 6, 6);
        toolbarLayout->setObjectName(QString::fromUtf8("toolbarLayout"));
        toolbarLayout->setContentsMargins(0, 0, 0, 0);
        btnSave = new QPushButton(toolbar);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setMaximumWidth(42);
        btnSave->setMinimumHeight(36);

        toolbarLayout->addWidget(btnSave);

        btnSearch = new QPushButton(toolbar);
        btnSearch->setObjectName(QString::fromUtf8("btnSearch"));
        btnSearch->setMaximumWidth(42);
        btnSearch->setMinimumHeight(36);

        toolbarLayout->addWidget(btnSearch);

        btnDelete = new QPushButton(toolbar);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));
        btnDelete->setMaximumWidth(42);
        btnDelete->setMinimumHeight(36);

        toolbarLayout->addWidget(btnDelete);

        btnPrint = new QPushButton(toolbar);
        btnPrint->setObjectName(QString::fromUtf8("btnPrint"));
        btnPrint->setMaximumWidth(42);
        btnPrint->setMinimumHeight(36);

        toolbarLayout->addWidget(btnPrint);

        btnPdf = new QPushButton(toolbar);
        btnPdf->setObjectName(QString::fromUtf8("btnPdf"));
        btnPdf->setMinimumHeight(36);

        toolbarLayout->addWidget(btnPdf);

        label17 = new QLabel(toolbar);
        label17->setObjectName(QString::fromUtf8("label17"));

        toolbarLayout->addWidget(label17);

        spnOrganiser = new QSpinBox(toolbar);
        spnOrganiser->setObjectName(QString::fromUtf8("spnOrganiser"));
        spnOrganiser->setValue(1);
        spnOrganiser->setMaximumWidth(50);

        toolbarLayout->addWidget(spnOrganiser);

        label18 = new QLabel(toolbar);
        label18->setObjectName(QString::fromUtf8("label18"));

        toolbarLayout->addWidget(label18);

        lblTime = new QLabel(toolbar);
        lblTime->setObjectName(QString::fromUtf8("lblTime"));

        toolbarLayout->addWidget(lblTime);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolbarLayout->addItem(spacerItem1);

        lblExpRatio = new QLabel(toolbar);
        lblExpRatio->setObjectName(QString::fromUtf8("lblExpRatio"));

        toolbarLayout->addWidget(lblExpRatio);

        btnTotalExpenses = new QPushButton(toolbar);
        btnTotalExpenses->setObjectName(QString::fromUtf8("btnTotalExpenses"));
        btnTotalExpenses->setMinimumHeight(36);

        toolbarLayout->addWidget(btnTotalExpenses);

        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolbarLayout->addItem(spacerItem2);

        btnFirst = new QPushButton(toolbar);
        btnFirst->setObjectName(QString::fromUtf8("btnFirst"));
        btnFirst->setMaximumWidth(42);
        btnFirst->setMinimumHeight(36);

        toolbarLayout->addWidget(btnFirst);

        lblNav = new QLabel(toolbar);
        lblNav->setObjectName(QString::fromUtf8("lblNav"));
        lblNav->setMinimumWidth(50);
        lblNav->setAlignment(Qt::AlignCenter);

        toolbarLayout->addWidget(lblNav);

        label19 = new QLabel(toolbar);
        label19->setObjectName(QString::fromUtf8("label19"));

        toolbarLayout->addWidget(label19);

        lblTotal = new QLabel(toolbar);
        lblTotal->setObjectName(QString::fromUtf8("lblTotal"));
        lblTotal->setMinimumWidth(50);

        toolbarLayout->addWidget(lblTotal);

        btnLast = new QPushButton(toolbar);
        btnLast->setObjectName(QString::fromUtf8("btnLast"));
        btnLast->setMaximumWidth(42);
        btnLast->setMinimumHeight(36);

        toolbarLayout->addWidget(btnLast);

        btnRecv = new QPushButton(toolbar);
        btnRecv->setObjectName(QString::fromUtf8("btnRecv"));
        btnRecv->setMaximumWidth(42);
        btnRecv->setMinimumHeight(36);

        toolbarLayout->addWidget(btnRecv);

        btnView = new QPushButton(toolbar);
        btnView->setObjectName(QString::fromUtf8("btnView"));
        btnView->setMaximumWidth(42);
        btnView->setMinimumHeight(36);

        toolbarLayout->addWidget(btnView);

        btnFwd = new QPushButton(toolbar);
        btnFwd->setObjectName(QString::fromUtf8("btnFwd"));
        btnFwd->setMaximumWidth(42);
        btnFwd->setMinimumHeight(36);

        toolbarLayout->addWidget(btnFwd);

        btnSign = new QPushButton(toolbar);
        btnSign->setObjectName(QString::fromUtf8("btnSign"));
        btnSign->setMaximumWidth(42);
        btnSign->setMinimumHeight(36);

        toolbarLayout->addWidget(btnSign);


        rootLayout->addWidget(toolbar);


        retranslateUi(PurchaseWindow);

        QMetaObject::connectSlotsByName(PurchaseWindow);
    } // setupUi

    void retranslateUi(QDialog *PurchaseWindow)
    {
        PurchaseWindow->setWindowTitle(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\331\205\330\264\330\252\330\261\331\212\330\247\330\252", nullptr));
        label->setText(QCoreApplication::translate("PurchaseWindow", "\330\261\331\202\331\205 \330\247\331\204\331\202\330\247\330\246\331\205\330\251", nullptr));
        label1->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\330\252\330\247\330\261\331\212\330\256", nullptr));
        dateYear->setDisplayFormat(QCoreApplication::translate("PurchaseWindow", "yyyy", nullptr));
        dateMonth->setDisplayFormat(QCoreApplication::translate("PurchaseWindow", "M", nullptr));
        dateDay->setDisplayFormat(QCoreApplication::translate("PurchaseWindow", "d", nullptr));
        label2->setText(QCoreApplication::translate("PurchaseWindow", "\331\206\331\210\330\271 \330\247\331\204\330\264\330\261\330\247\330\241", nullptr));
        cmbPurchaseType->setItemText(0, QCoreApplication::translate("PurchaseWindow", "\331\205\330\255\331\204\331\212", nullptr));
        cmbPurchaseType->setItemText(1, QCoreApplication::translate("PurchaseWindow", "\330\247\330\263\330\252\331\212\330\261\330\247\330\257", nullptr));

        label3->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\330\263\331\205 \330\247\331\204\331\205\330\254\331\207\330\262", nullptr));
        label4->setText(QCoreApplication::translate("PurchaseWindow", "\331\206\331\210\330\271 \330\247\331\204\330\257\331\201\330\271", nullptr));
        cmbPayType->setItemText(0, QCoreApplication::translate("PurchaseWindow", "\331\206\331\202\330\257\331\212", nullptr));
        cmbPayType->setItemText(1, QCoreApplication::translate("PurchaseWindow", "\330\242\330\254\331\204", nullptr));

        label5->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\330\271\331\205\331\204\330\251", nullptr));
        cmbCurrency->setItemText(0, QCoreApplication::translate("PurchaseWindow", "$", nullptr));
        cmbCurrency->setItemText(1, QCoreApplication::translate("PurchaseWindow", "\330\257\331\212\331\206\330\247\330\261", nullptr));

        label6->setText(QCoreApplication::translate("PurchaseWindow", "\330\263\331\206\330\257 \330\247\331\204\330\265\330\261\331\201", nullptr));
        label7->setText(QCoreApplication::translate("PurchaseWindow", "\330\263\330\271\330\261 \330\265\330\261\331\201 $", nullptr));
        txtNotes->setPlaceholderText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\331\205\331\204\330\247\330\255\330\270\330\247\330\252", nullptr));
        label8->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\331\203\331\205\331\212\330\251", nullptr));
        txtQty->setText(QCoreApplication::translate("PurchaseWindow", "0", nullptr));
        label9->setText(QCoreApplication::translate("PurchaseWindow", "\331\203\331\204\331\201\330\251 $", nullptr));
        txtCostPrice->setText(QCoreApplication::translate("PurchaseWindow", "0", nullptr));
        label10->setText(QCoreApplication::translate("PurchaseWindow", "\330\254\331\205\331\204\330\251 $", nullptr));
        txtWholesaleDollar->setText(QCoreApplication::translate("PurchaseWindow", "0", nullptr));
        label11->setText(QCoreApplication::translate("PurchaseWindow", "\331\205\331\201\330\261\330\257 $", nullptr));
        txtRetailDollar->setText(QCoreApplication::translate("PurchaseWindow", "0", nullptr));
        label12->setText(QCoreApplication::translate("PurchaseWindow", "\330\254\331\205\331\204\330\251 \330\257", nullptr));
        txtWholesaleDinar->setText(QCoreApplication::translate("PurchaseWindow", "0", nullptr));
        label13->setText(QCoreApplication::translate("PurchaseWindow", "\331\205\331\201\330\261\330\257 \330\257", nullptr));
        txtRetailDinar->setText(QCoreApplication::translate("PurchaseWindow", "0", nullptr));
        btnAdd->setText(QCoreApplication::translate("PurchaseWindow", "\342\234\224", nullptr));
        btnEdit->setText(QCoreApplication::translate("PurchaseWindow", "\342\234\216", nullptr));
        btnDel->setText(QCoreApplication::translate("PurchaseWindow", "\342\234\226", nullptr));
        lblStock->setText(QCoreApplication::translate("PurchaseWindow", "\330\261\330\265\331\212\330\257: 0", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblItems->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\330\250\330\247\330\261\331\203\331\210\330\257", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblItems->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("PurchaseWindow", "\330\261\331\205\330\262 \330\247\331\204\331\205\330\247\330\257\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblItems->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\330\263\331\205 \330\247\331\204\331\205\331\200\330\247\330\257\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblItems->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\331\203\331\205\331\212\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblItems->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\330\263\330\271\330\261", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tblItems->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\331\205\330\250\331\204\330\272 \330\257\331\212\331\206\330\247\330\261", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tblItems->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\331\203\331\204\331\201\330\251 $", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tblItems->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("PurchaseWindow", "\331\205\330\250\331\204\330\272 \330\247\331\204\331\203\331\204\331\201\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tblItems->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("PurchaseWindow", "\330\263\330\271\330\261 \330\247\331\204\330\250\331\212\330\271 $\331\205\331\201\330\261\330\257", nullptr));
        lblSupplierBal->setText(QCoreApplication::translate("PurchaseWindow", "\330\261\330\265\331\212\330\257\330\247\331\204\331\205\330\254\331\207\330\262: 0", nullptr));
        label14->setText(QCoreApplication::translate("PurchaseWindow", "$", nullptr));
        txtGrandDollar->setText(QCoreApplication::translate("PurchaseWindow", "0", nullptr));
        label15->setText(QCoreApplication::translate("PurchaseWindow", "\330\257\331\212\331\206\330\247\330\261", nullptr));
        txtGrandDinar->setText(QCoreApplication::translate("PurchaseWindow", "0", nullptr));
        label16->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\330\256\330\265\331\205 \330\257\331\212\331\206\330\247\330\261", nullptr));
        txtDiscount->setText(QCoreApplication::translate("PurchaseWindow", "0", nullptr));
        btnSave->setText(QCoreApplication::translate("PurchaseWindow", "\360\237\222\276", nullptr));
        btnSearch->setText(QCoreApplication::translate("PurchaseWindow", "\360\237\224\215", nullptr));
        btnDelete->setText(QCoreApplication::translate("PurchaseWindow", "\342\234\226", nullptr));
        btnPrint->setText(QCoreApplication::translate("PurchaseWindow", "\360\237\226\250", nullptr));
        btnPdf->setText(QCoreApplication::translate("PurchaseWindow", "PDF", nullptr));
        label17->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\331\205\331\206\330\270\331\205", nullptr));
        label18->setText(QCoreApplication::translate("PurchaseWindow", "\330\247\331\204\331\210\331\202\330\252", nullptr));
        lblTime->setText(QCoreApplication::translate("PurchaseWindow", "12:00 PM", nullptr));
        lblExpRatio->setText(QCoreApplication::translate("PurchaseWindow", "\331\206\330\263\330\250\330\251 \330\247\331\204\331\205\330\265\330\247\330\261\331\212\331\201", nullptr));
        btnTotalExpenses->setText(QCoreApplication::translate("PurchaseWindow", "\331\205\330\254\331\205\331\210\330\271 \330\247\331\204\331\205\330\265\330\247\330\261\331\212\331\201", nullptr));
        btnFirst->setText(QCoreApplication::translate("PurchaseWindow", "\342\217\256", nullptr));
        lblNav->setText(QCoreApplication::translate("PurchaseWindow", "1", nullptr));
        label19->setText(QCoreApplication::translate("PurchaseWindow", "\331\205\331\206", nullptr));
        lblTotal->setText(QCoreApplication::translate("PurchaseWindow", "1", nullptr));
        btnLast->setText(QCoreApplication::translate("PurchaseWindow", "\342\217\255", nullptr));
        btnRecv->setText(QCoreApplication::translate("PurchaseWindow", "\360\237\244\262", nullptr));
        btnView->setText(QCoreApplication::translate("PurchaseWindow", "\360\237\221\201", nullptr));
        btnFwd->setText(QCoreApplication::translate("PurchaseWindow", "\342\236\241", nullptr));
        btnSign->setText(QCoreApplication::translate("PurchaseWindow", "\342\234\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PurchaseWindow: public Ui_PurchaseWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PURCHASEWINDOW_H
