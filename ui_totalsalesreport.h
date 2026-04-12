/********************************************************************************
** Form generated from reading UI file 'totalsalesreport.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOTALSALESREPORT_H
#define UI_TOTALSALESREPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TotalSalesReport
{
public:
    QVBoxLayout *rootLayout;
    QLabel *lblTitle;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QCheckBox *chkGroup;
    QPushButton *btnGroup;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem1;
    QCheckBox *chkProduct;
    QPushButton *btnProduct;
    QHBoxLayout *hboxLayout2;
    QLineEdit *txtCustomerName;
    QComboBox *cmbCustomer;
    QLabel *label;
    QSpacerItem *spacerItem2;
    QCheckBox *chkCustomer;
    QPushButton *btnCustomer;
    QHBoxLayout *hboxLayout3;
    QSpacerItem *spacerItem3;
    QCheckBox *chkRegion;
    QPushButton *btnRegion;
    QGroupBox *grpDates;
    QHBoxLayout *hboxLayout4;
    QDateEdit *dateFrom;
    QPushButton *btnFrom;
    QSpacerItem *spacerItem4;
    QDateEdit *dateTo;
    QPushButton *btnTo;
    QGroupBox *grpButtons;
    QHBoxLayout *hboxLayout5;
    QPushButton *btnPrint;
    QPushButton *btnRun;

    void setupUi(QDialog *TotalSalesReport)
    {
        if (TotalSalesReport->objectName().isEmpty())
            TotalSalesReport->setObjectName(QString::fromUtf8("TotalSalesReport"));
        TotalSalesReport->resize(640, 560);
        TotalSalesReport->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(TotalSalesReport);
        rootLayout->setSpacing(12);
        rootLayout->setContentsMargins(14, 14, 14, 14);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        lblTitle = new QLabel(TotalSalesReport);
        lblTitle->setObjectName(QString::fromUtf8("lblTitle"));
        lblTitle->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(14);
        font.setBold(true);
        lblTitle->setFont(font);

        rootLayout->addWidget(lblTitle);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        chkGroup = new QCheckBox(TotalSalesReport);
        chkGroup->setObjectName(QString::fromUtf8("chkGroup"));

        hboxLayout->addWidget(chkGroup);

        btnGroup = new QPushButton(TotalSalesReport);
        btnGroup->setObjectName(QString::fromUtf8("btnGroup"));
        btnGroup->setMinimumSize(QSize(140, 36));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(11);
        font1.setBold(true);
        btnGroup->setFont(font1);

        hboxLayout->addWidget(btnGroup);


        rootLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);

        chkProduct = new QCheckBox(TotalSalesReport);
        chkProduct->setObjectName(QString::fromUtf8("chkProduct"));

        hboxLayout1->addWidget(chkProduct);

        btnProduct = new QPushButton(TotalSalesReport);
        btnProduct->setObjectName(QString::fromUtf8("btnProduct"));
        btnProduct->setMinimumSize(QSize(140, 36));
        btnProduct->setFont(font1);

        hboxLayout1->addWidget(btnProduct);


        rootLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        txtCustomerName = new QLineEdit(TotalSalesReport);
        txtCustomerName->setObjectName(QString::fromUtf8("txtCustomerName"));
        txtCustomerName->setMinimumWidth(160);

        hboxLayout2->addWidget(txtCustomerName);

        cmbCustomer = new QComboBox(TotalSalesReport);
        cmbCustomer->setObjectName(QString::fromUtf8("cmbCustomer"));
        cmbCustomer->setMaximumWidth(60);

        hboxLayout2->addWidget(cmbCustomer);

        label = new QLabel(TotalSalesReport);
        label->setObjectName(QString::fromUtf8("label"));

        hboxLayout2->addWidget(label);

        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem2);

        chkCustomer = new QCheckBox(TotalSalesReport);
        chkCustomer->setObjectName(QString::fromUtf8("chkCustomer"));
        chkCustomer->setChecked(true);

        hboxLayout2->addWidget(chkCustomer);

        btnCustomer = new QPushButton(TotalSalesReport);
        btnCustomer->setObjectName(QString::fromUtf8("btnCustomer"));
        btnCustomer->setMinimumSize(QSize(140, 36));
        btnCustomer->setFont(font1);

        hboxLayout2->addWidget(btnCustomer);


        rootLayout->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacerItem3);

        chkRegion = new QCheckBox(TotalSalesReport);
        chkRegion->setObjectName(QString::fromUtf8("chkRegion"));

        hboxLayout3->addWidget(chkRegion);

        btnRegion = new QPushButton(TotalSalesReport);
        btnRegion->setObjectName(QString::fromUtf8("btnRegion"));
        btnRegion->setMinimumSize(QSize(140, 36));
        btnRegion->setFont(font1);

        hboxLayout3->addWidget(btnRegion);


        rootLayout->addLayout(hboxLayout3);

        grpDates = new QGroupBox(TotalSalesReport);
        grpDates->setObjectName(QString::fromUtf8("grpDates"));
        hboxLayout4 = new QHBoxLayout(grpDates);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        dateFrom = new QDateEdit(grpDates);
        dateFrom->setObjectName(QString::fromUtf8("dateFrom"));
        dateFrom->setCalendarPopup(true);

        hboxLayout4->addWidget(dateFrom);

        btnFrom = new QPushButton(grpDates);
        btnFrom->setObjectName(QString::fromUtf8("btnFrom"));
        btnFrom->setMinimumHeight(32);

        hboxLayout4->addWidget(btnFrom);

        spacerItem4 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout4->addItem(spacerItem4);

        dateTo = new QDateEdit(grpDates);
        dateTo->setObjectName(QString::fromUtf8("dateTo"));
        dateTo->setCalendarPopup(true);

        hboxLayout4->addWidget(dateTo);

        btnTo = new QPushButton(grpDates);
        btnTo->setObjectName(QString::fromUtf8("btnTo"));
        btnTo->setMinimumHeight(32);

        hboxLayout4->addWidget(btnTo);


        rootLayout->addWidget(grpDates);

        grpButtons = new QGroupBox(TotalSalesReport);
        grpButtons->setObjectName(QString::fromUtf8("grpButtons"));
        hboxLayout5 = new QHBoxLayout(grpButtons);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        btnPrint = new QPushButton(grpButtons);
        btnPrint->setObjectName(QString::fromUtf8("btnPrint"));
        btnPrint->setMinimumSize(QSize(44, 38));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Tahoma"));
        font2.setPointSize(14);
        btnPrint->setFont(font2);

        hboxLayout5->addWidget(btnPrint);

        btnRun = new QPushButton(grpButtons);
        btnRun->setObjectName(QString::fromUtf8("btnRun"));
        btnRun->setMinimumSize(QSize(44, 38));

        hboxLayout5->addWidget(btnRun);


        rootLayout->addWidget(grpButtons);


        retranslateUi(TotalSalesReport);

        QMetaObject::connectSlotsByName(TotalSalesReport);
    } // setupUi

    void retranslateUi(QDialog *TotalSalesReport)
    {
        TotalSalesReport->setWindowTitle(QCoreApplication::translate("TotalSalesReport", "\331\205\330\254\331\205\331\210\330\271 \330\247\331\204\331\205\330\250\331\212\330\271\330\247\330\252 \330\255\330\263\330\250 \330\247\331\204\330\262\330\250\330\247\330\246\331\206", nullptr));
        lblTitle->setText(QCoreApplication::translate("TotalSalesReport", "\331\205\330\254\331\205\331\210\330\271 \330\247\331\204\331\205\330\250\331\212\330\271\330\247\330\252 \330\255\330\263\330\250 \330\247\331\204\330\262\330\250\330\247\330\246\331\206", nullptr));
        btnGroup->setText(QCoreApplication::translate("TotalSalesReport", "\331\205\330\254\331\205\331\210\330\271\330\251 \330\247\331\204\331\205\330\247\330\257\330\251", nullptr));
        btnProduct->setText(QCoreApplication::translate("TotalSalesReport", "\330\247\331\204\331\205\330\247\330\257\330\251", nullptr));
        label->setText(QCoreApplication::translate("TotalSalesReport", "\330\243\330\263\331\205 \330\247\331\204\330\262\330\250\331\210\331\206", nullptr));
        btnCustomer->setText(QCoreApplication::translate("TotalSalesReport", "\330\247\331\204\330\262\330\250\331\210\331\206", nullptr));
        btnRegion->setText(QCoreApplication::translate("TotalSalesReport", "\330\247\331\204\331\205\331\206\330\267\331\202\330\251", nullptr));
        dateFrom->setDisplayFormat(QCoreApplication::translate("TotalSalesReport", "dd/MM/yyyy", nullptr));
        btnFrom->setText(QCoreApplication::translate("TotalSalesReport", "\331\204\331\204\331\201\330\252\330\261\330\251 \331\205\331\206", nullptr));
        dateTo->setDisplayFormat(QCoreApplication::translate("TotalSalesReport", "dd/MM/yyyy", nullptr));
        btnTo->setText(QCoreApplication::translate("TotalSalesReport", "\330\247\331\204\331\211", nullptr));
        btnPrint->setText(QCoreApplication::translate("TotalSalesReport", "\360\237\226\250", nullptr));
        btnRun->setText(QCoreApplication::translate("TotalSalesReport", "\342\226\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TotalSalesReport: public Ui_TotalSalesReport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOTALSALESREPORT_H
