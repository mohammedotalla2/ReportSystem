/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *mainHLayout;
    QVBoxLayout *leftVLayout;
    QPushButton *btnDailyReport;
    QPushButton *btnCustomerStatement;
    QPushButton *btnCustomerBalances;
    QPushButton *btnInventory;
    QSpacerItem *spacerItem;
    QPushButton *btnClose;
    QVBoxLayout *centerVLayout;
    QLabel *lblTrial;
    QLabel *lblCompany;
    QGridLayout *btnGrid;
    QPushButton *btnSales;
    QPushButton *btnPurchases;
    QPushButton *btnCashBox;
    QPushButton *btnReports;
    QPushButton *btnSettings;
    QGroupBox *grpProductInfo;
    QGridLayout *productGrid;
    QLabel *lblBarcode;
    QComboBox *cmbBarcode;
    QLabel *lblProductName;
    QLineEdit *txtProductName;
    QLabel *lblQty;
    QLineEdit *txtQty;
    QLabel *lblPriceDinar;
    QLineEdit *txtPriceDinar;
    QLabel *lblPriceUsd;
    QLineEdit *txtPriceUsd;
    QLabel *lblCost;
    QLineEdit *txtCost;
    QLabel *lblCustomer;
    QComboBox *cmbCustomer;
    QLabel *lblBalDollar;
    QLineEdit *txtBalDollar;
    QLabel *lblBalDinar;
    QLineEdit *txtBalDinar;
    QVBoxLayout *rightVLayout;
    QLabel *lblDay;
    QLabel *lblDate;
    QSpacerItem *spacerItem1;
    QLabel *lblTodaySalesTitle;
    QLabel *lblTodaySales;
    QPushButton *btnSalesDetails;
    QSpacerItem *spacerItem2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1366, 720);
        MainWindow->setLayoutDirection(Qt::RightToLeft);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mainHLayout = new QHBoxLayout(centralWidget);
        mainHLayout->setSpacing(0);
        mainHLayout->setContentsMargins(0, 0, 0, 0);
        mainHLayout->setObjectName(QString::fromUtf8("mainHLayout"));
        leftVLayout = new QVBoxLayout();
        leftVLayout->setSpacing(8);
        leftVLayout->setObjectName(QString::fromUtf8("leftVLayout"));
        leftVLayout->setContentsMargins(8, 20, 8, 10);
        btnDailyReport = new QPushButton(centralWidget);
        btnDailyReport->setObjectName(QString::fromUtf8("btnDailyReport"));
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(10);
        btnDailyReport->setFont(font);

        leftVLayout->addWidget(btnDailyReport);

        btnCustomerStatement = new QPushButton(centralWidget);
        btnCustomerStatement->setObjectName(QString::fromUtf8("btnCustomerStatement"));
        btnCustomerStatement->setFont(font);

        leftVLayout->addWidget(btnCustomerStatement);

        btnCustomerBalances = new QPushButton(centralWidget);
        btnCustomerBalances->setObjectName(QString::fromUtf8("btnCustomerBalances"));
        btnCustomerBalances->setFont(font);

        leftVLayout->addWidget(btnCustomerBalances);

        btnInventory = new QPushButton(centralWidget);
        btnInventory->setObjectName(QString::fromUtf8("btnInventory"));
        btnInventory->setFont(font);

        leftVLayout->addWidget(btnInventory);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        leftVLayout->addItem(spacerItem);

        btnClose = new QPushButton(centralWidget);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(10);
        font1.setBold(true);
        btnClose->setFont(font1);

        leftVLayout->addWidget(btnClose);


        mainHLayout->addLayout(leftVLayout);

        centerVLayout = new QVBoxLayout();
        centerVLayout->setSpacing(8);
        centerVLayout->setContentsMargins(10, 10, 10, 10);
        centerVLayout->setObjectName(QString::fromUtf8("centerVLayout"));
        lblTrial = new QLabel(centralWidget);
        lblTrial->setObjectName(QString::fromUtf8("lblTrial"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Tahoma"));
        font2.setPointSize(18);
        font2.setBold(true);
        lblTrial->setFont(font2);
        lblTrial->setAlignment(Qt::AlignCenter);

        centerVLayout->addWidget(lblTrial);

        lblCompany = new QLabel(centralWidget);
        lblCompany->setObjectName(QString::fromUtf8("lblCompany"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Tahoma"));
        font3.setPointSize(12);
        font3.setBold(true);
        lblCompany->setFont(font3);
        lblCompany->setAlignment(Qt::AlignCenter);

        centerVLayout->addWidget(lblCompany);

        btnGrid = new QGridLayout();
        btnGrid->setSpacing(10);
        btnGrid->setObjectName(QString::fromUtf8("btnGrid"));
        btnSales = new QPushButton(centralWidget);
        btnSales->setObjectName(QString::fromUtf8("btnSales"));
        btnSales->setMinimumSize(QSize(160, 90));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Tahoma"));
        font4.setPointSize(16);
        font4.setBold(true);
        btnSales->setFont(font4);

        btnGrid->addWidget(btnSales, 0, 0, 1, 1);

        btnPurchases = new QPushButton(centralWidget);
        btnPurchases->setObjectName(QString::fromUtf8("btnPurchases"));
        btnPurchases->setMinimumSize(QSize(160, 90));
        btnPurchases->setFont(font4);

        btnGrid->addWidget(btnPurchases, 0, 1, 1, 1);

        btnCashBox = new QPushButton(centralWidget);
        btnCashBox->setObjectName(QString::fromUtf8("btnCashBox"));
        btnCashBox->setMinimumSize(QSize(160, 90));
        btnCashBox->setFont(font4);

        btnGrid->addWidget(btnCashBox, 1, 0, 1, 1);

        btnReports = new QPushButton(centralWidget);
        btnReports->setObjectName(QString::fromUtf8("btnReports"));
        btnReports->setMinimumSize(QSize(160, 90));
        btnReports->setFont(font4);

        btnGrid->addWidget(btnReports, 1, 1, 1, 1);

        btnSettings = new QPushButton(centralWidget);
        btnSettings->setObjectName(QString::fromUtf8("btnSettings"));
        btnSettings->setMinimumSize(QSize(160, 90));
        btnSettings->setFont(font4);

        btnGrid->addWidget(btnSettings, 1, 2, 1, 1);


        centerVLayout->addLayout(btnGrid);

        grpProductInfo = new QGroupBox(centralWidget);
        grpProductInfo->setObjectName(QString::fromUtf8("grpProductInfo"));
        productGrid = new QGridLayout(grpProductInfo);
        productGrid->setSpacing(4);
        productGrid->setObjectName(QString::fromUtf8("productGrid"));
        lblBarcode = new QLabel(grpProductInfo);
        lblBarcode->setObjectName(QString::fromUtf8("lblBarcode"));

        productGrid->addWidget(lblBarcode, 0, 3, 1, 1);

        cmbBarcode = new QComboBox(grpProductInfo);
        cmbBarcode->setObjectName(QString::fromUtf8("cmbBarcode"));
        cmbBarcode->setEditable(true);

        productGrid->addWidget(cmbBarcode, 1, 3, 1, 1);

        lblProductName = new QLabel(grpProductInfo);
        lblProductName->setObjectName(QString::fromUtf8("lblProductName"));

        productGrid->addWidget(lblProductName, 0, 2, 1, 1);

        txtProductName = new QLineEdit(grpProductInfo);
        txtProductName->setObjectName(QString::fromUtf8("txtProductName"));
        txtProductName->setReadOnly(true);

        productGrid->addWidget(txtProductName, 1, 2, 1, 1);

        lblQty = new QLabel(grpProductInfo);
        lblQty->setObjectName(QString::fromUtf8("lblQty"));

        productGrid->addWidget(lblQty, 2, 3, 1, 1);

        txtQty = new QLineEdit(grpProductInfo);
        txtQty->setObjectName(QString::fromUtf8("txtQty"));
        txtQty->setReadOnly(true);

        productGrid->addWidget(txtQty, 3, 3, 1, 1);

        lblPriceDinar = new QLabel(grpProductInfo);
        lblPriceDinar->setObjectName(QString::fromUtf8("lblPriceDinar"));

        productGrid->addWidget(lblPriceDinar, 2, 2, 1, 1);

        txtPriceDinar = new QLineEdit(grpProductInfo);
        txtPriceDinar->setObjectName(QString::fromUtf8("txtPriceDinar"));
        txtPriceDinar->setReadOnly(true);

        productGrid->addWidget(txtPriceDinar, 3, 2, 1, 1);

        lblPriceUsd = new QLabel(grpProductInfo);
        lblPriceUsd->setObjectName(QString::fromUtf8("lblPriceUsd"));

        productGrid->addWidget(lblPriceUsd, 2, 1, 1, 1);

        txtPriceUsd = new QLineEdit(grpProductInfo);
        txtPriceUsd->setObjectName(QString::fromUtf8("txtPriceUsd"));
        txtPriceUsd->setReadOnly(true);

        productGrid->addWidget(txtPriceUsd, 3, 1, 1, 1);

        lblCost = new QLabel(grpProductInfo);
        lblCost->setObjectName(QString::fromUtf8("lblCost"));

        productGrid->addWidget(lblCost, 4, 2, 1, 1);

        txtCost = new QLineEdit(grpProductInfo);
        txtCost->setObjectName(QString::fromUtf8("txtCost"));
        txtCost->setReadOnly(true);

        productGrid->addWidget(txtCost, 5, 2, 1, 1);

        lblCustomer = new QLabel(grpProductInfo);
        lblCustomer->setObjectName(QString::fromUtf8("lblCustomer"));

        productGrid->addWidget(lblCustomer, 6, 3, 1, 1);

        cmbCustomer = new QComboBox(grpProductInfo);
        cmbCustomer->setObjectName(QString::fromUtf8("cmbCustomer"));
        cmbCustomer->setEditable(true);

        productGrid->addWidget(cmbCustomer, 7, 2, 1, 2);

        lblBalDollar = new QLabel(grpProductInfo);
        lblBalDollar->setObjectName(QString::fromUtf8("lblBalDollar"));

        productGrid->addWidget(lblBalDollar, 6, 1, 1, 1);

        txtBalDollar = new QLineEdit(grpProductInfo);
        txtBalDollar->setObjectName(QString::fromUtf8("txtBalDollar"));
        txtBalDollar->setReadOnly(true);

        productGrid->addWidget(txtBalDollar, 7, 1, 1, 1);

        lblBalDinar = new QLabel(grpProductInfo);
        lblBalDinar->setObjectName(QString::fromUtf8("lblBalDinar"));

        productGrid->addWidget(lblBalDinar, 6, 2, 1, 1);

        txtBalDinar = new QLineEdit(grpProductInfo);
        txtBalDinar->setObjectName(QString::fromUtf8("txtBalDinar"));
        txtBalDinar->setReadOnly(true);

        productGrid->addWidget(txtBalDinar, 7, 0, 1, 1);


        centerVLayout->addWidget(grpProductInfo);


        mainHLayout->addLayout(centerVLayout);

        rightVLayout = new QVBoxLayout();
        rightVLayout->setSpacing(8);
        rightVLayout->setContentsMargins(8, 8, 8, 8);
        rightVLayout->setObjectName(QString::fromUtf8("rightVLayout"));
        lblDay = new QLabel(centralWidget);
        lblDay->setObjectName(QString::fromUtf8("lblDay"));
        lblDay->setFont(font3);
        lblDay->setAlignment(Qt::AlignCenter);

        rightVLayout->addWidget(lblDay);

        lblDate = new QLabel(centralWidget);
        lblDate->setObjectName(QString::fromUtf8("lblDate"));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Tahoma"));
        font5.setPointSize(11);
        lblDate->setFont(font5);
        lblDate->setAlignment(Qt::AlignCenter);

        rightVLayout->addWidget(lblDate);

        spacerItem1 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        rightVLayout->addItem(spacerItem1);

        lblTodaySalesTitle = new QLabel(centralWidget);
        lblTodaySalesTitle->setObjectName(QString::fromUtf8("lblTodaySalesTitle"));
        QFont font6;
        font6.setFamily(QString::fromUtf8("Tahoma"));
        font6.setPointSize(9);
        lblTodaySalesTitle->setFont(font6);
        lblTodaySalesTitle->setAlignment(Qt::AlignCenter);

        rightVLayout->addWidget(lblTodaySalesTitle);

        lblTodaySales = new QLabel(centralWidget);
        lblTodaySales->setObjectName(QString::fromUtf8("lblTodaySales"));
        QFont font7;
        font7.setFamily(QString::fromUtf8("Tahoma"));
        font7.setPointSize(14);
        font7.setBold(true);
        lblTodaySales->setFont(font7);
        lblTodaySales->setAlignment(Qt::AlignCenter);

        rightVLayout->addWidget(lblTodaySales);

        btnSalesDetails = new QPushButton(centralWidget);
        btnSalesDetails->setObjectName(QString::fromUtf8("btnSalesDetails"));
        btnSalesDetails->setFont(font6);

        rightVLayout->addWidget(btnSalesDetails);

        spacerItem2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        rightVLayout->addItem(spacerItem2);


        mainHLayout->addLayout(rightVLayout);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\331\206\330\270\330\247\331\205 \330\247\331\204\331\205\330\250\331\212\330\271\330\247\330\252 \331\210\330\247\331\204\331\205\330\264\330\252\330\261\331\212\330\247\330\252", nullptr));
        btnDailyReport->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\252\331\202\330\261\331\212\330\261 \330\247\331\204\331\212\331\210\331\205\331\212", nullptr));
        btnCustomerStatement->setText(QCoreApplication::translate("MainWindow", "\331\203\330\264\331\201 \330\255\330\263\330\247\330\250 \330\262\330\250\331\210\331\206", nullptr));
        btnCustomerBalances->setText(QCoreApplication::translate("MainWindow", "\330\247\330\261\330\265\330\257\330\251 \330\247\331\204\330\262\330\250\330\247\330\246\331\206", nullptr));
        btnInventory->setText(QCoreApplication::translate("MainWindow", "\330\254\330\261\330\257 \330\247\330\261\330\265\330\257\330\251 \330\247\331\204\331\205\331\210\330\247\330\257", nullptr));
        btnClose->setText(QCoreApplication::translate("MainWindow", "\330\247\330\272\331\204\330\247\331\202 \330\247\331\204\331\206\330\270\330\247\331\205", nullptr));
        lblTrial->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\331\206\330\263\330\256\330\251 \331\204\331\204\330\252\330\254\330\261\330\250\330\251", nullptr));
        lblCompany->setText(QCoreApplication::translate("MainWindow", "\331\206\330\270\330\247\331\205 \330\247\331\204\330\255\330\263\330\247\330\250\330\247\330\252", nullptr));
        btnSales->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\331\205\330\250\331\212\331\200\330\271\331\200\330\247\330\252", nullptr));
        btnPurchases->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\331\205\330\264\330\252\330\261\331\212\330\247\330\252", nullptr));
        btnCashBox->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\265\331\206\331\200\330\257\331\210\331\202", nullptr));
        btnReports->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\252\331\202\330\247\330\261\331\212\330\261", nullptr));
        btnSettings->setText(QCoreApplication::translate("MainWindow", "\330\243\330\271\330\257\330\247\330\257 \330\247\331\204\331\206\330\270\330\247\331\205", nullptr));
        lblBarcode->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\250\330\247\330\261\331\203\331\210\330\257 \330\252\331\206\330\247\330\252", nullptr));
        lblProductName->setText(QCoreApplication::translate("MainWindow", "\330\247\330\263\331\205 \330\247\331\204\331\205\330\247\330\257\330\251", nullptr));
        lblQty->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\271\330\257\330\257", nullptr));
        lblPriceDinar->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\263\330\271\330\261 \330\257", nullptr));
        lblPriceUsd->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\263\330\271\330\261 $", nullptr));
        lblCost->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\331\203\331\204\331\201\330\251", nullptr));
        lblCustomer->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\262\330\250\331\210\331\206", nullptr));
        lblBalDollar->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\261\330\265\331\212\330\257 $", nullptr));
        lblBalDinar->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\261\330\265\331\212\330\257 \330\257\331\212\331\206\330\247\330\261", nullptr));
        lblDay->setText(QCoreApplication::translate("MainWindow", "\330\247\331\204\330\243\330\261\330\250\330\271\330\247\330\241", nullptr));
        lblDate->setText(QCoreApplication::translate("MainWindow", "08/04/2026", nullptr));
        lblTodaySalesTitle->setText(QCoreApplication::translate("MainWindow", "\331\205\330\263\330\252\330\255\331\202\331\212 \331\205\330\250\331\212\330\271\330\247\330\252 \330\247\331\204\331\212\331\210\331\205", nullptr));
        lblTodaySales->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        btnSalesDetails->setText(QCoreApplication::translate("MainWindow", "\330\252\331\201\330\247\330\265\331\212\331\204", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
