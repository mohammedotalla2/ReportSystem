/********************************************************************************
** Form generated from reading UI file 'saleswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SALESWINDOW_H
#define UI_SALESWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
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

class Ui_SalesWindow
{
public:
    QVBoxLayout *rootLayout;
    QWidget *headerWidget;
    QGridLayout *headerGrid;
    QLabel *lblInvoiceNo;
    QSpinBox *spnInvoiceNo;
    QLabel *lblDate;
    QDateEdit *dateYear;
    QDateEdit *dateMonth;
    QDateEdit *dateDay;
    QLabel *lblDayName;
    QLabel *lblCustomer;
    QComboBox *cmbCustomer;
    QLabel *lblPayType;
    QComboBox *cmbPayType;
    QLabel *lblReceiver;
    QLineEdit *txtReceiver;
    QLabel *lblExchRate;
    QLineEdit *txtExchRate;
    QLabel *lblCurrency;
    QComboBox *cmbCurrency;
    QLabel *lblPricingType;
    QComboBox *cmbPricingType;
    QLineEdit *txtNotes;
    QHBoxLayout *midLayout;
    QWidget *tableArea;
    QVBoxLayout *tableVLayout;
    QHBoxLayout *itemEntryLayout;
    QLabel *lblRowNum;
    QLabel *lblRowNumVal;
    QComboBox *cmbBarcode;
    QComboBox *cmbProduct;
    QComboBox *cmbUnit;
    QLabel *label;
    QLineEdit *txtQty;
    QLabel *label1;
    QLineEdit *txtUnitPrice;
    QLabel *label2;
    QLineEdit *txtTotal;
    QLabel *lblStock;
    QPushButton *btnAddItem;
    QPushButton *btnEditItem;
    QPushButton *btnDeleteItem;
    QTableWidget *tblItems;
    QHBoxLayout *hboxLayout;
    QLabel *label3;
    QLabel *lblStockBal;
    QSpacerItem *spacerItem;
    QLabel *label4;
    QLabel *lblPaidDollar;
    QLabel *label5;
    QLabel *lblPaidDinar;
    QHBoxLayout *hboxLayout1;
    QLabel *label6;
    QLineEdit *txtDiscount;
    QLabel *label7;
    QLabel *lblEquivDollar;
    QLabel *label8;
    QLabel *lblCarriage;
    QSpacerItem *spacerItem1;
    QPushButton *btnAddToWait;
    QPushButton *btnFromWait;
    QHBoxLayout *hboxLayout2;
    QLabel *label9;
    QLabel *lblCustBalDollar;
    QLabel *label10;
    QLabel *lblCustBalDinar;
    QSpacerItem *spacerItem2;
    QLabel *label11;
    QLabel *lblTotalDollar;
    QLabel *label12;
    QLabel *lblTotalDinar;
    QHBoxLayout *hboxLayout3;
    QLabel *label13;
    QLabel *lblCurrentBalDollar;
    QLabel *label14;
    QLabel *lblCurrentBalDinar;
    QSpacerItem *spacerItem3;
    QGridLayout *gridLayout;
    QLabel *label15;
    QLineEdit *txtAddress;
    QLabel *label16;
    QLineEdit *txtPhone;
    QLabel *label17;
    QComboBox *cmbDriver;
    QLabel *label18;
    QLineEdit *txtCarNo;
    QLabel *label19;
    QLabel *lblOperator;
    QLabel *label20;
    QLabel *lblTime;
    QWidget *productPanel;
    QVBoxLayout *productPanelLayout;
    QLabel *lblGroupTitle;
    QComboBox *cmbGroup;
    QLabel *lblPartName;
    QLineEdit *txtPartName;
    QTableWidget *tblProductList;
    QWidget *toolbar;
    QHBoxLayout *toolbarLayout;
    QPushButton *btnSave;
    QPushButton *btnSearch;
    QPushButton *btnDelete;
    QPushButton *btnPrint;
    QLabel *label21;
    QSpinBox *spnCopies;
    QLabel *label22;
    QCheckBox *chkPrinted;
    QPushButton *btnThermal;
    QPushButton *btnPrepList;
    QPushButton *btnCustStatement;
    QPushButton *btnProductMove;
    QPushButton *btnPdf;
    QSpacerItem *spacerItem4;
    QPushButton *btnFirst;
    QLabel *lblNav;
    QLabel *label23;
    QLabel *lblTotal;
    QPushButton *btnLast;
    QPushButton *btnRecv;
    QPushButton *btnView;
    QPushButton *btnFwd;
    QPushButton *btnSign;

    void setupUi(QDialog *SalesWindow)
    {
        if (SalesWindow->objectName().isEmpty())
            SalesWindow->setObjectName(QString::fromUtf8("SalesWindow"));
        SalesWindow->resize(1350, 800);
        SalesWindow->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(SalesWindow);
        rootLayout->setSpacing(4);
        rootLayout->setContentsMargins(4, 4, 4, 4);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        headerWidget = new QWidget(SalesWindow);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerGrid = new QGridLayout(headerWidget);
        headerGrid->setSpacing(4);
        headerGrid->setContentsMargins(6, 6, 6, 6);
        headerGrid->setObjectName(QString::fromUtf8("headerGrid"));
        headerGrid->setContentsMargins(0, 0, 0, 0);
        lblInvoiceNo = new QLabel(headerWidget);
        lblInvoiceNo->setObjectName(QString::fromUtf8("lblInvoiceNo"));

        headerGrid->addWidget(lblInvoiceNo, 0, 11, 1, 1);

        spnInvoiceNo = new QSpinBox(headerWidget);
        spnInvoiceNo->setObjectName(QString::fromUtf8("spnInvoiceNo"));
        spnInvoiceNo->setMaximum(9999999);

        headerGrid->addWidget(spnInvoiceNo, 0, 10, 1, 1);

        lblDate = new QLabel(headerWidget);
        lblDate->setObjectName(QString::fromUtf8("lblDate"));

        headerGrid->addWidget(lblDate, 0, 9, 1, 1);

        dateYear = new QDateEdit(headerWidget);
        dateYear->setObjectName(QString::fromUtf8("dateYear"));
        dateYear->setCalendarPopup(false);

        headerGrid->addWidget(dateYear, 0, 8, 1, 1);

        dateMonth = new QDateEdit(headerWidget);
        dateMonth->setObjectName(QString::fromUtf8("dateMonth"));

        headerGrid->addWidget(dateMonth, 0, 7, 1, 1);

        dateDay = new QDateEdit(headerWidget);
        dateDay->setObjectName(QString::fromUtf8("dateDay"));

        headerGrid->addWidget(dateDay, 0, 6, 1, 1);

        lblDayName = new QLabel(headerWidget);
        lblDayName->setObjectName(QString::fromUtf8("lblDayName"));
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(10);
        lblDayName->setFont(font);

        headerGrid->addWidget(lblDayName, 0, 5, 1, 1);

        lblCustomer = new QLabel(headerWidget);
        lblCustomer->setObjectName(QString::fromUtf8("lblCustomer"));

        headerGrid->addWidget(lblCustomer, 0, 4, 1, 1);

        cmbCustomer = new QComboBox(headerWidget);
        cmbCustomer->setObjectName(QString::fromUtf8("cmbCustomer"));
        cmbCustomer->setEditable(true);
        cmbCustomer->setMinimumWidth(200);

        headerGrid->addWidget(cmbCustomer, 0, 2, 1, 2);

        lblPayType = new QLabel(headerWidget);
        lblPayType->setObjectName(QString::fromUtf8("lblPayType"));

        headerGrid->addWidget(lblPayType, 0, 1, 1, 1);

        cmbPayType = new QComboBox(headerWidget);
        cmbPayType->addItem(QString());
        cmbPayType->addItem(QString());
        cmbPayType->addItem(QString());
        cmbPayType->setObjectName(QString::fromUtf8("cmbPayType"));

        headerGrid->addWidget(cmbPayType, 0, 0, 1, 1);

        lblReceiver = new QLabel(headerWidget);
        lblReceiver->setObjectName(QString::fromUtf8("lblReceiver"));

        headerGrid->addWidget(lblReceiver, 1, 7, 1, 1);

        txtReceiver = new QLineEdit(headerWidget);
        txtReceiver->setObjectName(QString::fromUtf8("txtReceiver"));

        headerGrid->addWidget(txtReceiver, 1, 6, 1, 1);

        lblExchRate = new QLabel(headerWidget);
        lblExchRate->setObjectName(QString::fromUtf8("lblExchRate"));

        headerGrid->addWidget(lblExchRate, 1, 5, 1, 1);

        txtExchRate = new QLineEdit(headerWidget);
        txtExchRate->setObjectName(QString::fromUtf8("txtExchRate"));
        txtExchRate->setMaximumWidth(80);

        headerGrid->addWidget(txtExchRate, 1, 4, 1, 1);

        lblCurrency = new QLabel(headerWidget);
        lblCurrency->setObjectName(QString::fromUtf8("lblCurrency"));

        headerGrid->addWidget(lblCurrency, 1, 3, 1, 1);

        cmbCurrency = new QComboBox(headerWidget);
        cmbCurrency->addItem(QString());
        cmbCurrency->addItem(QString());
        cmbCurrency->setObjectName(QString::fromUtf8("cmbCurrency"));

        headerGrid->addWidget(cmbCurrency, 1, 2, 1, 1);

        lblPricingType = new QLabel(headerWidget);
        lblPricingType->setObjectName(QString::fromUtf8("lblPricingType"));

        headerGrid->addWidget(lblPricingType, 1, 1, 1, 1);

        cmbPricingType = new QComboBox(headerWidget);
        cmbPricingType->addItem(QString());
        cmbPricingType->addItem(QString());
        cmbPricingType->setObjectName(QString::fromUtf8("cmbPricingType"));

        headerGrid->addWidget(cmbPricingType, 1, 0, 1, 1);

        txtNotes = new QLineEdit(headerWidget);
        txtNotes->setObjectName(QString::fromUtf8("txtNotes"));

        headerGrid->addWidget(txtNotes, 2, 0, 1, 12);


        rootLayout->addWidget(headerWidget);

        midLayout = new QHBoxLayout();
        midLayout->setSpacing(4);
        midLayout->setObjectName(QString::fromUtf8("midLayout"));
        tableArea = new QWidget(SalesWindow);
        tableArea->setObjectName(QString::fromUtf8("tableArea"));
        tableVLayout = new QVBoxLayout(tableArea);
        tableVLayout->setSpacing(2);
        tableVLayout->setContentsMargins(2, 2, 2, 2);
        tableVLayout->setObjectName(QString::fromUtf8("tableVLayout"));
        tableVLayout->setContentsMargins(0, 0, 0, 0);
        itemEntryLayout = new QHBoxLayout();
        itemEntryLayout->setObjectName(QString::fromUtf8("itemEntryLayout"));
        lblRowNum = new QLabel(tableArea);
        lblRowNum->setObjectName(QString::fromUtf8("lblRowNum"));

        itemEntryLayout->addWidget(lblRowNum);

        lblRowNumVal = new QLabel(tableArea);
        lblRowNumVal->setObjectName(QString::fromUtf8("lblRowNumVal"));

        itemEntryLayout->addWidget(lblRowNumVal);

        cmbBarcode = new QComboBox(tableArea);
        cmbBarcode->setObjectName(QString::fromUtf8("cmbBarcode"));
        cmbBarcode->setEditable(true);
        cmbBarcode->setMinimumWidth(110);

        itemEntryLayout->addWidget(cmbBarcode);

        cmbProduct = new QComboBox(tableArea);
        cmbProduct->setObjectName(QString::fromUtf8("cmbProduct"));
        cmbProduct->setEditable(true);
        cmbProduct->setMinimumWidth(200);

        itemEntryLayout->addWidget(cmbProduct);

        cmbUnit = new QComboBox(tableArea);
        cmbUnit->addItem(QString());
        cmbUnit->addItem(QString());
        cmbUnit->setObjectName(QString::fromUtf8("cmbUnit"));
        cmbUnit->setMaximumWidth(80);

        itemEntryLayout->addWidget(cmbUnit);

        label = new QLabel(tableArea);
        label->setObjectName(QString::fromUtf8("label"));

        itemEntryLayout->addWidget(label);

        txtQty = new QLineEdit(tableArea);
        txtQty->setObjectName(QString::fromUtf8("txtQty"));
        txtQty->setMaximumWidth(60);

        itemEntryLayout->addWidget(txtQty);

        label1 = new QLabel(tableArea);
        label1->setObjectName(QString::fromUtf8("label1"));

        itemEntryLayout->addWidget(label1);

        txtUnitPrice = new QLineEdit(tableArea);
        txtUnitPrice->setObjectName(QString::fromUtf8("txtUnitPrice"));
        txtUnitPrice->setMaximumWidth(90);

        itemEntryLayout->addWidget(txtUnitPrice);

        label2 = new QLabel(tableArea);
        label2->setObjectName(QString::fromUtf8("label2"));

        itemEntryLayout->addWidget(label2);

        txtTotal = new QLineEdit(tableArea);
        txtTotal->setObjectName(QString::fromUtf8("txtTotal"));
        txtTotal->setReadOnly(true);
        txtTotal->setMaximumWidth(90);

        itemEntryLayout->addWidget(txtTotal);

        lblStock = new QLabel(tableArea);
        lblStock->setObjectName(QString::fromUtf8("lblStock"));

        itemEntryLayout->addWidget(lblStock);

        btnAddItem = new QPushButton(tableArea);
        btnAddItem->setObjectName(QString::fromUtf8("btnAddItem"));
        btnAddItem->setMaximumWidth(34);
        btnAddItem->setMaximumHeight(28);

        itemEntryLayout->addWidget(btnAddItem);

        btnEditItem = new QPushButton(tableArea);
        btnEditItem->setObjectName(QString::fromUtf8("btnEditItem"));
        btnEditItem->setMaximumWidth(34);
        btnEditItem->setMaximumHeight(28);

        itemEntryLayout->addWidget(btnEditItem);

        btnDeleteItem = new QPushButton(tableArea);
        btnDeleteItem->setObjectName(QString::fromUtf8("btnDeleteItem"));
        btnDeleteItem->setMaximumWidth(34);
        btnDeleteItem->setMaximumHeight(28);

        itemEntryLayout->addWidget(btnDeleteItem);


        tableVLayout->addLayout(itemEntryLayout);

        tblItems = new QTableWidget(tableArea);
        if (tblItems->columnCount() < 8)
            tblItems->setColumnCount(8);
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
        tblItems->setObjectName(QString::fromUtf8("tblItems"));
        tblItems->setMinimumHeight(240);
        tblItems->setSelectionBehavior(QAbstractItemView::SelectRows);
        tblItems->setEditTriggers(QAbstractItemView::NoEditTriggers);

        tableVLayout->addWidget(tblItems);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label3 = new QLabel(tableArea);
        label3->setObjectName(QString::fromUtf8("label3"));

        hboxLayout->addWidget(label3);

        lblStockBal = new QLabel(tableArea);
        lblStockBal->setObjectName(QString::fromUtf8("lblStockBal"));

        hboxLayout->addWidget(lblStockBal);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        label4 = new QLabel(tableArea);
        label4->setObjectName(QString::fromUtf8("label4"));

        hboxLayout->addWidget(label4);

        lblPaidDollar = new QLabel(tableArea);
        lblPaidDollar->setObjectName(QString::fromUtf8("lblPaidDollar"));

        hboxLayout->addWidget(lblPaidDollar);

        label5 = new QLabel(tableArea);
        label5->setObjectName(QString::fromUtf8("label5"));

        hboxLayout->addWidget(label5);

        lblPaidDinar = new QLabel(tableArea);
        lblPaidDinar->setObjectName(QString::fromUtf8("lblPaidDinar"));

        hboxLayout->addWidget(lblPaidDinar);


        tableVLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label6 = new QLabel(tableArea);
        label6->setObjectName(QString::fromUtf8("label6"));

        hboxLayout1->addWidget(label6);

        txtDiscount = new QLineEdit(tableArea);
        txtDiscount->setObjectName(QString::fromUtf8("txtDiscount"));
        txtDiscount->setMaximumWidth(80);

        hboxLayout1->addWidget(txtDiscount);

        label7 = new QLabel(tableArea);
        label7->setObjectName(QString::fromUtf8("label7"));

        hboxLayout1->addWidget(label7);

        lblEquivDollar = new QLabel(tableArea);
        lblEquivDollar->setObjectName(QString::fromUtf8("lblEquivDollar"));

        hboxLayout1->addWidget(lblEquivDollar);

        label8 = new QLabel(tableArea);
        label8->setObjectName(QString::fromUtf8("label8"));

        hboxLayout1->addWidget(label8);

        lblCarriage = new QLabel(tableArea);
        lblCarriage->setObjectName(QString::fromUtf8("lblCarriage"));

        hboxLayout1->addWidget(lblCarriage);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);

        btnAddToWait = new QPushButton(tableArea);
        btnAddToWait->setObjectName(QString::fromUtf8("btnAddToWait"));

        hboxLayout1->addWidget(btnAddToWait);

        btnFromWait = new QPushButton(tableArea);
        btnFromWait->setObjectName(QString::fromUtf8("btnFromWait"));

        hboxLayout1->addWidget(btnFromWait);


        tableVLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        label9 = new QLabel(tableArea);
        label9->setObjectName(QString::fromUtf8("label9"));

        hboxLayout2->addWidget(label9);

        lblCustBalDollar = new QLabel(tableArea);
        lblCustBalDollar->setObjectName(QString::fromUtf8("lblCustBalDollar"));

        hboxLayout2->addWidget(lblCustBalDollar);

        label10 = new QLabel(tableArea);
        label10->setObjectName(QString::fromUtf8("label10"));

        hboxLayout2->addWidget(label10);

        lblCustBalDinar = new QLabel(tableArea);
        lblCustBalDinar->setObjectName(QString::fromUtf8("lblCustBalDinar"));

        hboxLayout2->addWidget(lblCustBalDinar);

        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem2);

        label11 = new QLabel(tableArea);
        label11->setObjectName(QString::fromUtf8("label11"));

        hboxLayout2->addWidget(label11);

        lblTotalDollar = new QLabel(tableArea);
        lblTotalDollar->setObjectName(QString::fromUtf8("lblTotalDollar"));

        hboxLayout2->addWidget(lblTotalDollar);

        label12 = new QLabel(tableArea);
        label12->setObjectName(QString::fromUtf8("label12"));

        hboxLayout2->addWidget(label12);

        lblTotalDinar = new QLabel(tableArea);
        lblTotalDinar->setObjectName(QString::fromUtf8("lblTotalDinar"));

        hboxLayout2->addWidget(lblTotalDinar);


        tableVLayout->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        label13 = new QLabel(tableArea);
        label13->setObjectName(QString::fromUtf8("label13"));

        hboxLayout3->addWidget(label13);

        lblCurrentBalDollar = new QLabel(tableArea);
        lblCurrentBalDollar->setObjectName(QString::fromUtf8("lblCurrentBalDollar"));

        hboxLayout3->addWidget(lblCurrentBalDollar);

        label14 = new QLabel(tableArea);
        label14->setObjectName(QString::fromUtf8("label14"));

        hboxLayout3->addWidget(label14);

        lblCurrentBalDinar = new QLabel(tableArea);
        lblCurrentBalDinar->setObjectName(QString::fromUtf8("lblCurrentBalDinar"));

        hboxLayout3->addWidget(lblCurrentBalDinar);

        spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacerItem3);


        tableVLayout->addLayout(hboxLayout3);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label15 = new QLabel(tableArea);
        label15->setObjectName(QString::fromUtf8("label15"));

        gridLayout->addWidget(label15, 0, 0, 1, 1);

        txtAddress = new QLineEdit(tableArea);
        txtAddress->setObjectName(QString::fromUtf8("txtAddress"));
        txtAddress->setReadOnly(true);

        gridLayout->addWidget(txtAddress, 0, 1, 1, 1);

        label16 = new QLabel(tableArea);
        label16->setObjectName(QString::fromUtf8("label16"));

        gridLayout->addWidget(label16, 0, 2, 1, 1);

        txtPhone = new QLineEdit(tableArea);
        txtPhone->setObjectName(QString::fromUtf8("txtPhone"));
        txtPhone->setReadOnly(true);

        gridLayout->addWidget(txtPhone, 0, 3, 1, 1);

        label17 = new QLabel(tableArea);
        label17->setObjectName(QString::fromUtf8("label17"));

        gridLayout->addWidget(label17, 1, 0, 1, 1);

        cmbDriver = new QComboBox(tableArea);
        cmbDriver->setObjectName(QString::fromUtf8("cmbDriver"));
        cmbDriver->setEditable(true);

        gridLayout->addWidget(cmbDriver, 1, 1, 1, 1);

        label18 = new QLabel(tableArea);
        label18->setObjectName(QString::fromUtf8("label18"));

        gridLayout->addWidget(label18, 1, 2, 1, 1);

        txtCarNo = new QLineEdit(tableArea);
        txtCarNo->setObjectName(QString::fromUtf8("txtCarNo"));

        gridLayout->addWidget(txtCarNo, 1, 3, 1, 1);

        label19 = new QLabel(tableArea);
        label19->setObjectName(QString::fromUtf8("label19"));

        gridLayout->addWidget(label19, 1, 4, 1, 1);

        lblOperator = new QLabel(tableArea);
        lblOperator->setObjectName(QString::fromUtf8("lblOperator"));

        gridLayout->addWidget(lblOperator, 1, 5, 1, 1);

        label20 = new QLabel(tableArea);
        label20->setObjectName(QString::fromUtf8("label20"));

        gridLayout->addWidget(label20, 1, 6, 1, 1);

        lblTime = new QLabel(tableArea);
        lblTime->setObjectName(QString::fromUtf8("lblTime"));

        gridLayout->addWidget(lblTime, 1, 7, 1, 1);


        tableVLayout->addLayout(gridLayout);


        midLayout->addWidget(tableArea);

        productPanel = new QWidget(SalesWindow);
        productPanel->setObjectName(QString::fromUtf8("productPanel"));
        productPanel->setMaximumWidth(225);
        productPanel->setMinimumWidth(225);
        productPanelLayout = new QVBoxLayout(productPanel);
        productPanelLayout->setSpacing(4);
        productPanelLayout->setContentsMargins(4, 4, 4, 4);
        productPanelLayout->setObjectName(QString::fromUtf8("productPanelLayout"));
        productPanelLayout->setContentsMargins(0, 0, 0, 0);
        lblGroupTitle = new QLabel(productPanel);
        lblGroupTitle->setObjectName(QString::fromUtf8("lblGroupTitle"));

        productPanelLayout->addWidget(lblGroupTitle);

        cmbGroup = new QComboBox(productPanel);
        cmbGroup->addItem(QString());
        cmbGroup->setObjectName(QString::fromUtf8("cmbGroup"));
        cmbGroup->setEditable(true);

        productPanelLayout->addWidget(cmbGroup);

        lblPartName = new QLabel(productPanel);
        lblPartName->setObjectName(QString::fromUtf8("lblPartName"));

        productPanelLayout->addWidget(lblPartName);

        txtPartName = new QLineEdit(productPanel);
        txtPartName->setObjectName(QString::fromUtf8("txtPartName"));

        productPanelLayout->addWidget(txtPartName);

        tblProductList = new QTableWidget(productPanel);
        if (tblProductList->columnCount() < 2)
            tblProductList->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tblProductList->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tblProductList->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        tblProductList->setObjectName(QString::fromUtf8("tblProductList"));
        tblProductList->setSelectionBehavior(QAbstractItemView::SelectRows);

        productPanelLayout->addWidget(tblProductList);


        midLayout->addWidget(productPanel);


        rootLayout->addLayout(midLayout);

        toolbar = new QWidget(SalesWindow);
        toolbar->setObjectName(QString::fromUtf8("toolbar"));
        toolbarLayout = new QHBoxLayout(toolbar);
        toolbarLayout->setSpacing(4);
        toolbarLayout->setContentsMargins(4, 4, 4, 4);
        toolbarLayout->setObjectName(QString::fromUtf8("toolbarLayout"));
        toolbarLayout->setContentsMargins(0, 0, 0, 0);
        btnSave = new QPushButton(toolbar);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setMaximumWidth(40);
        btnSave->setMinimumHeight(36);

        toolbarLayout->addWidget(btnSave);

        btnSearch = new QPushButton(toolbar);
        btnSearch->setObjectName(QString::fromUtf8("btnSearch"));
        btnSearch->setMaximumWidth(40);
        btnSearch->setMinimumHeight(36);

        toolbarLayout->addWidget(btnSearch);

        btnDelete = new QPushButton(toolbar);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));
        btnDelete->setMaximumWidth(40);
        btnDelete->setMinimumHeight(36);

        toolbarLayout->addWidget(btnDelete);

        btnPrint = new QPushButton(toolbar);
        btnPrint->setObjectName(QString::fromUtf8("btnPrint"));
        btnPrint->setMaximumWidth(40);
        btnPrint->setMinimumHeight(36);

        toolbarLayout->addWidget(btnPrint);

        label21 = new QLabel(toolbar);
        label21->setObjectName(QString::fromUtf8("label21"));

        toolbarLayout->addWidget(label21);

        spnCopies = new QSpinBox(toolbar);
        spnCopies->setObjectName(QString::fromUtf8("spnCopies"));
        spnCopies->setMinimum(1);
        spnCopies->setMaximum(99);
        spnCopies->setValue(1);
        spnCopies->setMaximumWidth(50);

        toolbarLayout->addWidget(spnCopies);

        label22 = new QLabel(toolbar);
        label22->setObjectName(QString::fromUtf8("label22"));

        toolbarLayout->addWidget(label22);

        chkPrinted = new QCheckBox(toolbar);
        chkPrinted->setObjectName(QString::fromUtf8("chkPrinted"));

        toolbarLayout->addWidget(chkPrinted);

        btnThermal = new QPushButton(toolbar);
        btnThermal->setObjectName(QString::fromUtf8("btnThermal"));
        btnThermal->setMinimumHeight(36);

        toolbarLayout->addWidget(btnThermal);

        btnPrepList = new QPushButton(toolbar);
        btnPrepList->setObjectName(QString::fromUtf8("btnPrepList"));
        btnPrepList->setMinimumHeight(36);

        toolbarLayout->addWidget(btnPrepList);

        btnCustStatement = new QPushButton(toolbar);
        btnCustStatement->setObjectName(QString::fromUtf8("btnCustStatement"));
        btnCustStatement->setMinimumHeight(36);

        toolbarLayout->addWidget(btnCustStatement);

        btnProductMove = new QPushButton(toolbar);
        btnProductMove->setObjectName(QString::fromUtf8("btnProductMove"));
        btnProductMove->setMinimumHeight(36);

        toolbarLayout->addWidget(btnProductMove);

        btnPdf = new QPushButton(toolbar);
        btnPdf->setObjectName(QString::fromUtf8("btnPdf"));
        btnPdf->setMinimumHeight(36);

        toolbarLayout->addWidget(btnPdf);

        spacerItem4 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolbarLayout->addItem(spacerItem4);

        btnFirst = new QPushButton(toolbar);
        btnFirst->setObjectName(QString::fromUtf8("btnFirst"));
        btnFirst->setMaximumWidth(40);
        btnFirst->setMinimumHeight(36);

        toolbarLayout->addWidget(btnFirst);

        lblNav = new QLabel(toolbar);
        lblNav->setObjectName(QString::fromUtf8("lblNav"));
        lblNav->setAlignment(Qt::AlignCenter);
        lblNav->setMinimumWidth(45);

        toolbarLayout->addWidget(lblNav);

        label23 = new QLabel(toolbar);
        label23->setObjectName(QString::fromUtf8("label23"));

        toolbarLayout->addWidget(label23);

        lblTotal = new QLabel(toolbar);
        lblTotal->setObjectName(QString::fromUtf8("lblTotal"));
        lblTotal->setMinimumWidth(45);

        toolbarLayout->addWidget(lblTotal);

        btnLast = new QPushButton(toolbar);
        btnLast->setObjectName(QString::fromUtf8("btnLast"));
        btnLast->setMaximumWidth(40);
        btnLast->setMinimumHeight(36);

        toolbarLayout->addWidget(btnLast);

        btnRecv = new QPushButton(toolbar);
        btnRecv->setObjectName(QString::fromUtf8("btnRecv"));
        btnRecv->setMaximumWidth(40);
        btnRecv->setMinimumHeight(36);

        toolbarLayout->addWidget(btnRecv);

        btnView = new QPushButton(toolbar);
        btnView->setObjectName(QString::fromUtf8("btnView"));
        btnView->setMaximumWidth(40);
        btnView->setMinimumHeight(36);

        toolbarLayout->addWidget(btnView);

        btnFwd = new QPushButton(toolbar);
        btnFwd->setObjectName(QString::fromUtf8("btnFwd"));
        btnFwd->setMaximumWidth(40);
        btnFwd->setMinimumHeight(36);

        toolbarLayout->addWidget(btnFwd);

        btnSign = new QPushButton(toolbar);
        btnSign->setObjectName(QString::fromUtf8("btnSign"));
        btnSign->setMaximumWidth(40);
        btnSign->setMinimumHeight(36);

        toolbarLayout->addWidget(btnSign);


        rootLayout->addWidget(toolbar);


        retranslateUi(SalesWindow);

        QMetaObject::connectSlotsByName(SalesWindow);
    } // setupUi

    void retranslateUi(QDialog *SalesWindow)
    {
        SalesWindow->setWindowTitle(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\205\330\250\331\212\330\271\330\247\330\252", nullptr));
        lblInvoiceNo->setText(QCoreApplication::translate("SalesWindow", "\330\261\331\202\331\205 \330\247\331\204\331\202\330\247\330\246\331\205\330\251", nullptr));
        lblDate->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\252\330\247\330\261\331\212\330\256", nullptr));
        dateYear->setDisplayFormat(QCoreApplication::translate("SalesWindow", "yyyy", nullptr));
        dateMonth->setDisplayFormat(QCoreApplication::translate("SalesWindow", "M", nullptr));
        dateDay->setDisplayFormat(QCoreApplication::translate("SalesWindow", "d", nullptr));
        lblDayName->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\253\331\204\330\247\330\253\330\247\330\241", nullptr));
        lblCustomer->setText(QCoreApplication::translate("SalesWindow", "\330\247\330\263\331\205 \330\247\331\204\330\262\330\250\331\210\331\206", nullptr));
        lblPayType->setText(QCoreApplication::translate("SalesWindow", "\331\206\331\210\330\271 \330\247\331\204\330\257\331\201\330\271", nullptr));
        cmbPayType->setItemText(0, QCoreApplication::translate("SalesWindow", "\331\206\331\202\330\257\331\212", nullptr));
        cmbPayType->setItemText(1, QCoreApplication::translate("SalesWindow", "\330\242\330\254\331\204", nullptr));
        cmbPayType->setItemText(2, QCoreApplication::translate("SalesWindow", "\330\254\331\205\331\204\330\251", nullptr));

        lblReceiver->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\205\330\263\330\252\331\204\331\205", nullptr));
        lblExchRate->setText(QCoreApplication::translate("SalesWindow", "\330\263\330\271\330\261 \330\265\330\261\331\201 $", nullptr));
        lblCurrency->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\271\331\205\331\204\330\251", nullptr));
        cmbCurrency->setItemText(0, QCoreApplication::translate("SalesWindow", "$", nullptr));
        cmbCurrency->setItemText(1, QCoreApplication::translate("SalesWindow", "\330\257\331\212\331\206\330\247\330\261", nullptr));

        lblPricingType->setText(QCoreApplication::translate("SalesWindow", "\331\206\331\210\330\271 \330\247\331\204\330\252\330\263\330\271\331\212\330\261", nullptr));
        cmbPricingType->setItemText(0, QCoreApplication::translate("SalesWindow", "\331\205\331\201\330\261\330\257", nullptr));
        cmbPricingType->setItemText(1, QCoreApplication::translate("SalesWindow", "\330\254\331\205\331\204\330\251", nullptr));

        txtNotes->setPlaceholderText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\205\331\204\330\247\330\255\330\270\330\247\330\252", nullptr));
        lblRowNum->setText(QCoreApplication::translate("SalesWindow", "\330\252", nullptr));
        lblRowNumVal->setText(QCoreApplication::translate("SalesWindow", "1", nullptr));
        cmbUnit->setItemText(0, QCoreApplication::translate("SalesWindow", "\331\203\330\261\330\252\331\210\331\206\330\251", nullptr));
        cmbUnit->setItemText(1, QCoreApplication::translate("SalesWindow", "\331\202\330\267\330\271\330\251", nullptr));

        label->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\203\331\205\331\212\330\251", nullptr));
        txtQty->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label1->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\263\330\271\330\261", nullptr));
        txtUnitPrice->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label2->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\205\330\254\331\205\331\210\330\271", nullptr));
        txtTotal->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        lblStock->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        btnAddItem->setText(QCoreApplication::translate("SalesWindow", "\342\234\224", nullptr));
        btnEditItem->setText(QCoreApplication::translate("SalesWindow", "\342\234\216", nullptr));
        btnDeleteItem->setText(QCoreApplication::translate("SalesWindow", "\342\234\226", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblItems->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("SalesWindow", "\330\252", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblItems->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\250\330\247\330\261\331\203\331\210\330\257", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblItems->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("SalesWindow", "\330\261\331\205\330\262 \330\247\331\204\331\205\330\247\330\257\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblItems->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("SalesWindow", "\330\247\330\263\331\205 \330\247\331\204\331\205\330\247\330\257\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblItems->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\203\331\205\331\212\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tblItems->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("SalesWindow", "\330\263\330\271\330\261 \330\247\331\204\331\205\331\201\330\261\330\257", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tblItems->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\205\330\250\331\204\330\272 \330\247\331\204\331\203\331\204\331\212", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tblItems->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\205\331\204\330\247\330\255\330\270\330\247\330\252", nullptr));
        label3->setText(QCoreApplication::translate("SalesWindow", "\330\261\330\265\331\212\330\257 \331\205\330\247\330\257\330\251", nullptr));
        lblStockBal->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label4->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\210\330\247\330\265\331\204 $", nullptr));
        lblPaidDollar->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label5->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\210\330\247\330\265\331\204 \330\257", nullptr));
        lblPaidDinar->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label6->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\256\330\265\331\205 \330\257\331\212\331\206\330\247\330\261", nullptr));
        txtDiscount->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label7->setText(QCoreApplication::translate("SalesWindow", "\331\205\330\247\331\212\330\271\330\247\330\257\331\204 $", nullptr));
        lblEquivDollar->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label8->setText(QCoreApplication::translate("SalesWindow", "\330\255\331\205\330\247\331\204\331\212\330\251", nullptr));
        lblCarriage->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        btnAddToWait->setText(QCoreApplication::translate("SalesWindow", "\330\245\330\266\330\247\331\201\330\251 \331\204\331\204\330\247\331\206\330\252\330\270\330\247\330\261", nullptr));
        btnFromWait->setText(QCoreApplication::translate("SalesWindow", "\330\247\330\263\330\252\330\261\330\254\330\247\330\271 \331\205\331\206 \330\247\331\204\330\247\331\206\330\252\330\270\330\247\330\261", nullptr));
        label9->setText(QCoreApplication::translate("SalesWindow", "\330\261\330\265\331\212\330\257 \330\247\331\204\330\262\330\250\331\210\331\206 $", nullptr));
        lblCustBalDollar->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label10->setText(QCoreApplication::translate("SalesWindow", "\330\261\330\265\331\212\330\257 \330\247\331\204\330\262\330\250\331\210\331\206 \330\257", nullptr));
        lblCustBalDinar->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label11->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\245\330\254\331\205\330\247\331\204\331\212 $", nullptr));
        lblTotalDollar->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label12->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\245\330\254\331\205\330\247\331\204\331\212 \330\257", nullptr));
        lblTotalDinar->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label13->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\261\330\265\331\212\330\257 \330\247\331\204\330\255\330\247\331\204\331\212 $", nullptr));
        lblCurrentBalDollar->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label14->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\261\330\265\331\212\330\257 \330\247\331\204\330\255\330\247\331\204\331\212 \330\257", nullptr));
        lblCurrentBalDinar->setText(QCoreApplication::translate("SalesWindow", "0", nullptr));
        label15->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\271\331\206\331\210\330\247\331\206", nullptr));
        label16->setText(QCoreApplication::translate("SalesWindow", "\331\207\330\247\330\252\331\201 \330\247\331\204\330\262\330\250\331\210\331\206", nullptr));
        label17->setText(QCoreApplication::translate("SalesWindow", "\330\247\330\263\331\205 \330\247\331\204\330\263\330\247\330\246\331\202", nullptr));
        label18->setText(QCoreApplication::translate("SalesWindow", "\330\261\331\202\331\205 \330\247\331\204\330\263\331\212\330\247\330\261\330\251", nullptr));
        label19->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\205\331\206\330\270\331\205", nullptr));
        lblOperator->setText(QCoreApplication::translate("SalesWindow", "1", nullptr));
        label20->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\331\210\331\202\330\252", nullptr));
        lblTime->setText(QCoreApplication::translate("SalesWindow", "12:00 PM", nullptr));
        lblGroupTitle->setText(QCoreApplication::translate("SalesWindow", "\330\255\330\263\330\250 \330\247\331\204\331\205\330\254\331\205\331\210\330\271\330\251", nullptr));
        cmbGroup->setItemText(0, QCoreApplication::translate("SalesWindow", "\331\203\331\204 \330\247\331\204\330\247\330\265\331\206\330\247\331\201", nullptr));

        lblPartName->setText(QCoreApplication::translate("SalesWindow", "\330\254\330\262\330\241 \331\205\331\206 \330\247\331\204\330\247\330\263\331\205", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tblProductList->horizontalHeaderItem(0);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("SalesWindow", "\330\247\330\256\330\252\331\212\330\247\330\261", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tblProductList->horizontalHeaderItem(1);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("SalesWindow", "\330\247\330\263\331\205 \330\247\331\204\331\205\330\247\330\257\330\251", nullptr));
        btnSave->setText(QCoreApplication::translate("SalesWindow", "\360\237\222\276", nullptr));
#if QT_CONFIG(tooltip)
        btnSave->setToolTip(QCoreApplication::translate("SalesWindow", "\330\255\331\201\330\270", nullptr));
#endif // QT_CONFIG(tooltip)
        btnSearch->setText(QCoreApplication::translate("SalesWindow", "\360\237\224\215", nullptr));
#if QT_CONFIG(tooltip)
        btnSearch->setToolTip(QCoreApplication::translate("SalesWindow", "\330\250\330\255\330\253", nullptr));
#endif // QT_CONFIG(tooltip)
        btnDelete->setText(QCoreApplication::translate("SalesWindow", "\342\234\226", nullptr));
#if QT_CONFIG(tooltip)
        btnDelete->setToolTip(QCoreApplication::translate("SalesWindow", "\330\255\330\260\331\201", nullptr));
#endif // QT_CONFIG(tooltip)
        btnPrint->setText(QCoreApplication::translate("SalesWindow", "\360\237\226\250", nullptr));
#if QT_CONFIG(tooltip)
        btnPrint->setToolTip(QCoreApplication::translate("SalesWindow", "\330\267\330\250\330\247\330\271\330\251", nullptr));
#endif // QT_CONFIG(tooltip)
        label21->setText(QCoreApplication::translate("SalesWindow", "\330\271\330\257\330\257 \330\247\331\204\331\206\330\263\330\256", nullptr));
        label22->setText(QCoreApplication::translate("SalesWindow", "\330\267\330\250\330\271\330\252", nullptr));
        btnThermal->setText(QCoreApplication::translate("SalesWindow", "\330\247\331\204\330\267\330\247\330\250\330\271\330\251 \330\247\331\204\330\264\330\261\331\212\330\267\331\212\330\251", nullptr));
        btnPrepList->setText(QCoreApplication::translate("SalesWindow", "\331\202\330\247\330\246\331\205\330\251 \330\252\330\254\331\207\331\212\330\262", nullptr));
        btnCustStatement->setText(QCoreApplication::translate("SalesWindow", "\331\203\330\264\331\201 \330\255\330\263\330\247\330\250", nullptr));
        btnProductMove->setText(QCoreApplication::translate("SalesWindow", "\330\255\330\261\331\203\330\251 \331\205\330\247\330\257\330\251", nullptr));
        btnPdf->setText(QCoreApplication::translate("SalesWindow", "PDF", nullptr));
        btnFirst->setText(QCoreApplication::translate("SalesWindow", "\342\217\256", nullptr));
        lblNav->setText(QCoreApplication::translate("SalesWindow", "1", nullptr));
        label23->setText(QCoreApplication::translate("SalesWindow", "\331\205\331\206", nullptr));
        lblTotal->setText(QCoreApplication::translate("SalesWindow", "1", nullptr));
        btnLast->setText(QCoreApplication::translate("SalesWindow", "\342\217\255", nullptr));
        btnRecv->setText(QCoreApplication::translate("SalesWindow", "\360\237\244\262", nullptr));
        btnView->setText(QCoreApplication::translate("SalesWindow", "\360\237\221\201", nullptr));
        btnFwd->setText(QCoreApplication::translate("SalesWindow", "\342\236\241", nullptr));
        btnSign->setText(QCoreApplication::translate("SalesWindow", "\342\234\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SalesWindow: public Ui_SalesWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SALESWINDOW_H
