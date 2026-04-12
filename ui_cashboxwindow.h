/********************************************************************************
** Form generated from reading UI file 'cashboxwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CASHBOXWINDOW_H
#define UI_CASHBOXWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CashBoxWindow
{
public:
    QVBoxLayout *rootLayout;
    QLabel *lblTitle;
    QWidget *headerWidget;
    QGridLayout *headerGrid;
    QLabel *label;
    QComboBox *cmbType;
    QLabel *label1;
    QLineEdit *txtDocNo;
    QLabel *label2;
    QSpinBox *spnOrganiser;
    QLabel *label3;
    QLabel *lblTime;
    QLabel *label4;
    QDateEdit *dateDay;
    QDateEdit *dateMonth;
    QDateEdit *dateYear;
    QLabel *label5;
    QComboBox *cmbMainAccount;
    QLabel *label6;
    QComboBox *cmbSubAccount;
    QLabel *label7;
    QLineEdit *txtPrevCustDollar;
    QLabel *label8;
    QLineEdit *txtPrevBalDollar;
    QLabel *label9;
    QLineEdit *txtPrevCustDinar;
    QLabel *label10;
    QLineEdit *txtPrevBalDinar;
    QLabel *label11;
    QLineEdit *txtAmountDollar;
    QLabel *label12;
    QLineEdit *txtEquivDinar;
    QLabel *label13;
    QLineEdit *txtAmountDinar;
    QLabel *label14;
    QLineEdit *txtEquivDollar;
    QPushButton *btnCalcEquiv;
    QLabel *label15;
    QComboBox *cmbCashbox;
    QLabel *label16;
    QLineEdit *txtExchRate;
    QLabel *label17;
    QComboBox *cmbPurchaseList;
    QLabel *label18;
    QLineEdit *txtNotes;
    QLabel *label19;
    QLineEdit *txtOrganiserName;
    QSpacerItem *spacerItem;
    QWidget *toolbar;
    QHBoxLayout *toolbarLayout;
    QPushButton *btnSave;
    QPushButton *btnEdit;
    QPushButton *btnUndo;
    QPushButton *btnSearch;
    QPushButton *btnDelete;
    QPushButton *btnPrint;
    QPushButton *btnPdf;
    QSpacerItem *spacerItem1;
    QLabel *lblNav;
    QLabel *label20;
    QLabel *lblTotal;
    QSpacerItem *spacerItem2;
    QPushButton *btnReceive;
    QPushButton *btnPay;
    QPushButton *btnFirst;
    QPushButton *btnPrev;
    QPushButton *btnNext;
    QPushButton *btnLast;

    void setupUi(QDialog *CashBoxWindow)
    {
        if (CashBoxWindow->objectName().isEmpty())
            CashBoxWindow->setObjectName(QString::fromUtf8("CashBoxWindow"));
        CashBoxWindow->resize(1150, 720);
        CashBoxWindow->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(CashBoxWindow);
#ifndef Q_OS_MAC
        rootLayout->setSpacing(6);
#endif
        rootLayout->setContentsMargins(6, 6, 6, 6);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        lblTitle = new QLabel(CashBoxWindow);
        lblTitle->setObjectName(QString::fromUtf8("lblTitle"));
        lblTitle->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(16);
        font.setBold(true);
        lblTitle->setFont(font);

        rootLayout->addWidget(lblTitle);

        headerWidget = new QWidget(CashBoxWindow);
        headerWidget->setObjectName(QString::fromUtf8("headerWidget"));
        headerGrid = new QGridLayout(headerWidget);
#ifndef Q_OS_MAC
        headerGrid->setSpacing(6);
#endif
        headerGrid->setContentsMargins(8, 8, 8, 8);
        headerGrid->setObjectName(QString::fromUtf8("headerGrid"));
        headerGrid->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(headerWidget);
        label->setObjectName(QString::fromUtf8("label"));

        headerGrid->addWidget(label, 0, 11, 1, 1);

        cmbType = new QComboBox(headerWidget);
        cmbType->addItem(QString());
        cmbType->addItem(QString());
        cmbType->addItem(QString());
        cmbType->addItem(QString());
        cmbType->addItem(QString());
        cmbType->setObjectName(QString::fromUtf8("cmbType"));

        headerGrid->addWidget(cmbType, 0, 9, 1, 2);

        label1 = new QLabel(headerWidget);
        label1->setObjectName(QString::fromUtf8("label1"));

        headerGrid->addWidget(label1, 0, 8, 1, 1);

        txtDocNo = new QLineEdit(headerWidget);
        txtDocNo->setObjectName(QString::fromUtf8("txtDocNo"));

        headerGrid->addWidget(txtDocNo, 0, 7, 1, 1);

        label2 = new QLabel(headerWidget);
        label2->setObjectName(QString::fromUtf8("label2"));

        headerGrid->addWidget(label2, 0, 5, 1, 1);

        spnOrganiser = new QSpinBox(headerWidget);
        spnOrganiser->setObjectName(QString::fromUtf8("spnOrganiser"));
        spnOrganiser->setValue(1);
        spnOrganiser->setMaximumWidth(60);

        headerGrid->addWidget(spnOrganiser, 0, 4, 1, 1);

        label3 = new QLabel(headerWidget);
        label3->setObjectName(QString::fromUtf8("label3"));

        headerGrid->addWidget(label3, 0, 2, 1, 1);

        lblTime = new QLabel(headerWidget);
        lblTime->setObjectName(QString::fromUtf8("lblTime"));

        headerGrid->addWidget(lblTime, 0, 1, 1, 1);

        label4 = new QLabel(headerWidget);
        label4->setObjectName(QString::fromUtf8("label4"));

        headerGrid->addWidget(label4, 1, 11, 1, 1);

        dateDay = new QDateEdit(headerWidget);
        dateDay->setObjectName(QString::fromUtf8("dateDay"));

        headerGrid->addWidget(dateDay, 1, 10, 1, 1);

        dateMonth = new QDateEdit(headerWidget);
        dateMonth->setObjectName(QString::fromUtf8("dateMonth"));

        headerGrid->addWidget(dateMonth, 1, 9, 1, 1);

        dateYear = new QDateEdit(headerWidget);
        dateYear->setObjectName(QString::fromUtf8("dateYear"));

        headerGrid->addWidget(dateYear, 1, 8, 1, 1);

        label5 = new QLabel(headerWidget);
        label5->setObjectName(QString::fromUtf8("label5"));

        headerGrid->addWidget(label5, 2, 11, 1, 1);

        cmbMainAccount = new QComboBox(headerWidget);
        cmbMainAccount->addItem(QString());
        cmbMainAccount->addItem(QString());
        cmbMainAccount->addItem(QString());
        cmbMainAccount->addItem(QString());
        cmbMainAccount->addItem(QString());
        cmbMainAccount->addItem(QString());
        cmbMainAccount->setObjectName(QString::fromUtf8("cmbMainAccount"));
        cmbMainAccount->setEditable(true);

        headerGrid->addWidget(cmbMainAccount, 2, 9, 1, 2);

        label6 = new QLabel(headerWidget);
        label6->setObjectName(QString::fromUtf8("label6"));

        headerGrid->addWidget(label6, 2, 5, 1, 1);

        cmbSubAccount = new QComboBox(headerWidget);
        cmbSubAccount->setObjectName(QString::fromUtf8("cmbSubAccount"));
        cmbSubAccount->setEditable(true);

        headerGrid->addWidget(cmbSubAccount, 2, 3, 1, 2);

        label7 = new QLabel(headerWidget);
        label7->setObjectName(QString::fromUtf8("label7"));

        headerGrid->addWidget(label7, 3, 11, 1, 1);

        txtPrevCustDollar = new QLineEdit(headerWidget);
        txtPrevCustDollar->setObjectName(QString::fromUtf8("txtPrevCustDollar"));
        txtPrevCustDollar->setReadOnly(true);

        headerGrid->addWidget(txtPrevCustDollar, 3, 10, 1, 1);

        label8 = new QLabel(headerWidget);
        label8->setObjectName(QString::fromUtf8("label8"));

        headerGrid->addWidget(label8, 3, 6, 1, 1);

        txtPrevBalDollar = new QLineEdit(headerWidget);
        txtPrevBalDollar->setObjectName(QString::fromUtf8("txtPrevBalDollar"));
        txtPrevBalDollar->setReadOnly(true);

        headerGrid->addWidget(txtPrevBalDollar, 3, 5, 1, 1);

        label9 = new QLabel(headerWidget);
        label9->setObjectName(QString::fromUtf8("label9"));

        headerGrid->addWidget(label9, 4, 11, 1, 1);

        txtPrevCustDinar = new QLineEdit(headerWidget);
        txtPrevCustDinar->setObjectName(QString::fromUtf8("txtPrevCustDinar"));
        txtPrevCustDinar->setReadOnly(true);

        headerGrid->addWidget(txtPrevCustDinar, 4, 10, 1, 1);

        label10 = new QLabel(headerWidget);
        label10->setObjectName(QString::fromUtf8("label10"));

        headerGrid->addWidget(label10, 4, 6, 1, 1);

        txtPrevBalDinar = new QLineEdit(headerWidget);
        txtPrevBalDinar->setObjectName(QString::fromUtf8("txtPrevBalDinar"));
        txtPrevBalDinar->setReadOnly(true);

        headerGrid->addWidget(txtPrevBalDinar, 4, 5, 1, 1);

        label11 = new QLabel(headerWidget);
        label11->setObjectName(QString::fromUtf8("label11"));

        headerGrid->addWidget(label11, 5, 11, 1, 1);

        txtAmountDollar = new QLineEdit(headerWidget);
        txtAmountDollar->setObjectName(QString::fromUtf8("txtAmountDollar"));

        headerGrid->addWidget(txtAmountDollar, 5, 10, 1, 1);

        label12 = new QLabel(headerWidget);
        label12->setObjectName(QString::fromUtf8("label12"));

        headerGrid->addWidget(label12, 5, 6, 1, 1);

        txtEquivDinar = new QLineEdit(headerWidget);
        txtEquivDinar->setObjectName(QString::fromUtf8("txtEquivDinar"));
        txtEquivDinar->setReadOnly(true);

        headerGrid->addWidget(txtEquivDinar, 5, 5, 1, 1);

        label13 = new QLabel(headerWidget);
        label13->setObjectName(QString::fromUtf8("label13"));

        headerGrid->addWidget(label13, 6, 11, 1, 1);

        txtAmountDinar = new QLineEdit(headerWidget);
        txtAmountDinar->setObjectName(QString::fromUtf8("txtAmountDinar"));

        headerGrid->addWidget(txtAmountDinar, 6, 10, 1, 1);

        label14 = new QLabel(headerWidget);
        label14->setObjectName(QString::fromUtf8("label14"));

        headerGrid->addWidget(label14, 6, 6, 1, 1);

        txtEquivDollar = new QLineEdit(headerWidget);
        txtEquivDollar->setObjectName(QString::fromUtf8("txtEquivDollar"));
        txtEquivDollar->setReadOnly(true);

        headerGrid->addWidget(txtEquivDollar, 6, 5, 1, 1);

        btnCalcEquiv = new QPushButton(headerWidget);
        btnCalcEquiv->setObjectName(QString::fromUtf8("btnCalcEquiv"));

        headerGrid->addWidget(btnCalcEquiv, 6, 3, 1, 1);

        label15 = new QLabel(headerWidget);
        label15->setObjectName(QString::fromUtf8("label15"));

        headerGrid->addWidget(label15, 7, 11, 1, 1);

        cmbCashbox = new QComboBox(headerWidget);
        cmbCashbox->addItem(QString());
        cmbCashbox->addItem(QString());
        cmbCashbox->addItem(QString());
        cmbCashbox->setObjectName(QString::fromUtf8("cmbCashbox"));
        cmbCashbox->setEditable(true);

        headerGrid->addWidget(cmbCashbox, 7, 9, 1, 2);

        label16 = new QLabel(headerWidget);
        label16->setObjectName(QString::fromUtf8("label16"));

        headerGrid->addWidget(label16, 7, 6, 1, 1);

        txtExchRate = new QLineEdit(headerWidget);
        txtExchRate->setObjectName(QString::fromUtf8("txtExchRate"));

        headerGrid->addWidget(txtExchRate, 7, 5, 1, 1);

        label17 = new QLabel(headerWidget);
        label17->setObjectName(QString::fromUtf8("label17"));

        headerGrid->addWidget(label17, 8, 11, 1, 1);

        cmbPurchaseList = new QComboBox(headerWidget);
        cmbPurchaseList->setObjectName(QString::fromUtf8("cmbPurchaseList"));
        cmbPurchaseList->setEditable(true);

        headerGrid->addWidget(cmbPurchaseList, 8, 9, 1, 2);

        label18 = new QLabel(headerWidget);
        label18->setObjectName(QString::fromUtf8("label18"));

        headerGrid->addWidget(label18, 9, 11, 1, 1);

        txtNotes = new QLineEdit(headerWidget);
        txtNotes->setObjectName(QString::fromUtf8("txtNotes"));

        headerGrid->addWidget(txtNotes, 9, 0, 1, 11);

        label19 = new QLabel(headerWidget);
        label19->setObjectName(QString::fromUtf8("label19"));

        headerGrid->addWidget(label19, 10, 11, 1, 1);

        txtOrganiserName = new QLineEdit(headerWidget);
        txtOrganiserName->setObjectName(QString::fromUtf8("txtOrganiserName"));

        headerGrid->addWidget(txtOrganiserName, 10, 5, 1, 6);


        rootLayout->addWidget(headerWidget);

        spacerItem = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        rootLayout->addItem(spacerItem);

        toolbar = new QWidget(CashBoxWindow);
        toolbar->setObjectName(QString::fromUtf8("toolbar"));
        toolbarLayout = new QHBoxLayout(toolbar);
        toolbarLayout->setSpacing(4);
        toolbarLayout->setContentsMargins(6, 6, 6, 6);
        toolbarLayout->setObjectName(QString::fromUtf8("toolbarLayout"));
        toolbarLayout->setContentsMargins(0, 0, 0, 0);
        btnSave = new QPushButton(toolbar);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setMaximumWidth(42);
        btnSave->setMinimumHeight(38);

        toolbarLayout->addWidget(btnSave);

        btnEdit = new QPushButton(toolbar);
        btnEdit->setObjectName(QString::fromUtf8("btnEdit"));
        btnEdit->setMaximumWidth(42);
        btnEdit->setMinimumHeight(38);

        toolbarLayout->addWidget(btnEdit);

        btnUndo = new QPushButton(toolbar);
        btnUndo->setObjectName(QString::fromUtf8("btnUndo"));
        btnUndo->setMaximumWidth(42);
        btnUndo->setMinimumHeight(38);

        toolbarLayout->addWidget(btnUndo);

        btnSearch = new QPushButton(toolbar);
        btnSearch->setObjectName(QString::fromUtf8("btnSearch"));
        btnSearch->setMaximumWidth(42);
        btnSearch->setMinimumHeight(38);

        toolbarLayout->addWidget(btnSearch);

        btnDelete = new QPushButton(toolbar);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));
        btnDelete->setMaximumWidth(42);
        btnDelete->setMinimumHeight(38);

        toolbarLayout->addWidget(btnDelete);

        btnPrint = new QPushButton(toolbar);
        btnPrint->setObjectName(QString::fromUtf8("btnPrint"));
        btnPrint->setMaximumWidth(42);
        btnPrint->setMinimumHeight(38);

        toolbarLayout->addWidget(btnPrint);

        btnPdf = new QPushButton(toolbar);
        btnPdf->setObjectName(QString::fromUtf8("btnPdf"));
        btnPdf->setMinimumHeight(38);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(10);
        font1.setBold(true);
        btnPdf->setFont(font1);

        toolbarLayout->addWidget(btnPdf);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolbarLayout->addItem(spacerItem1);

        lblNav = new QLabel(toolbar);
        lblNav->setObjectName(QString::fromUtf8("lblNav"));
        lblNav->setMinimumWidth(50);
        lblNav->setAlignment(Qt::AlignCenter);

        toolbarLayout->addWidget(lblNav);

        label20 = new QLabel(toolbar);
        label20->setObjectName(QString::fromUtf8("label20"));

        toolbarLayout->addWidget(label20);

        lblTotal = new QLabel(toolbar);
        lblTotal->setObjectName(QString::fromUtf8("lblTotal"));
        lblTotal->setMinimumWidth(50);

        toolbarLayout->addWidget(lblTotal);

        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        toolbarLayout->addItem(spacerItem2);

        btnReceive = new QPushButton(toolbar);
        btnReceive->setObjectName(QString::fromUtf8("btnReceive"));
        btnReceive->setMaximumWidth(42);
        btnReceive->setMinimumHeight(38);

        toolbarLayout->addWidget(btnReceive);

        btnPay = new QPushButton(toolbar);
        btnPay->setObjectName(QString::fromUtf8("btnPay"));
        btnPay->setMaximumWidth(42);
        btnPay->setMinimumHeight(38);

        toolbarLayout->addWidget(btnPay);

        btnFirst = new QPushButton(toolbar);
        btnFirst->setObjectName(QString::fromUtf8("btnFirst"));
        btnFirst->setMaximumWidth(42);
        btnFirst->setMinimumHeight(38);

        toolbarLayout->addWidget(btnFirst);

        btnPrev = new QPushButton(toolbar);
        btnPrev->setObjectName(QString::fromUtf8("btnPrev"));
        btnPrev->setMaximumWidth(42);
        btnPrev->setMinimumHeight(38);

        toolbarLayout->addWidget(btnPrev);

        btnNext = new QPushButton(toolbar);
        btnNext->setObjectName(QString::fromUtf8("btnNext"));
        btnNext->setMaximumWidth(42);
        btnNext->setMinimumHeight(38);

        toolbarLayout->addWidget(btnNext);

        btnLast = new QPushButton(toolbar);
        btnLast->setObjectName(QString::fromUtf8("btnLast"));
        btnLast->setMaximumWidth(42);
        btnLast->setMinimumHeight(38);

        toolbarLayout->addWidget(btnLast);


        rootLayout->addWidget(toolbar);


        retranslateUi(CashBoxWindow);

        QMetaObject::connectSlotsByName(CashBoxWindow);
    } // setupUi

    void retranslateUi(QDialog *CashBoxWindow)
    {
        CashBoxWindow->setWindowTitle(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\330\265\331\206\330\257\331\210\331\202", nullptr));
        lblTitle->setText(QCoreApplication::translate("CashBoxWindow", "\330\243\330\257\330\247\331\205\330\251 \330\255\330\261\331\203\330\247\330\252 \330\247\331\204\330\265\331\206\330\257\331\210\331\202", nullptr));
        label->setText(QCoreApplication::translate("CashBoxWindow", "\331\206\331\210\330\271 \330\247\331\204\330\255\330\261\331\203\330\251", nullptr));
        cmbType->setItemText(0, QCoreApplication::translate("CashBoxWindow", "\330\265\330\261\331\201 \331\205\330\265\330\247\330\261\331\212\331\201 \330\271\330\247\331\205\330\251", nullptr));
        cmbType->setItemText(1, QCoreApplication::translate("CashBoxWindow", "\330\265\330\261\331\201 \330\260\331\205\331\205 \330\271\331\205\331\204\330\247\330\241", nullptr));
        cmbType->setItemText(2, QCoreApplication::translate("CashBoxWindow", "\331\202\330\250\330\266 \331\205\331\206 \330\271\331\205\331\204\330\247\330\241", nullptr));
        cmbType->setItemText(3, QCoreApplication::translate("CashBoxWindow", "\330\245\331\212\330\257\330\247\330\271 \331\201\331\212 \330\247\331\204\330\265\331\206\330\257\331\210\331\202", nullptr));
        cmbType->setItemText(4, QCoreApplication::translate("CashBoxWindow", "\330\263\330\255\330\250 \331\205\331\206 \330\247\331\204\330\265\331\206\330\257\331\210\331\202", nullptr));

        label1->setText(QCoreApplication::translate("CashBoxWindow", "\330\261\331\202\331\205 \330\247\331\204\330\263\331\206\330\257", nullptr));
        label2->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\331\205\331\206\330\270\331\205", nullptr));
        label3->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\331\210\331\202\330\252", nullptr));
        lblTime->setText(QCoreApplication::translate("CashBoxWindow", "11:40 AM", nullptr));
        label4->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\330\252\330\247\330\261\331\212\330\256", nullptr));
        dateDay->setDisplayFormat(QCoreApplication::translate("CashBoxWindow", "d", nullptr));
        dateMonth->setDisplayFormat(QCoreApplication::translate("CashBoxWindow", "M", nullptr));
        dateYear->setDisplayFormat(QCoreApplication::translate("CashBoxWindow", "yyyy", nullptr));
        label5->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\330\255\330\263\330\247\330\250 \330\247\331\204\330\261\330\246\331\212\330\263\331\212", nullptr));
        cmbMainAccount->setItemText(0, QCoreApplication::translate("CashBoxWindow", "\331\205\330\265\330\247\330\261\331\212\331\201 \330\271\330\247\331\205\330\251", nullptr));
        cmbMainAccount->setItemText(1, QCoreApplication::translate("CashBoxWindow", "\330\260\331\205\331\205 \330\271\331\205\331\204\330\247\330\241", nullptr));
        cmbMainAccount->setItemText(2, QCoreApplication::translate("CashBoxWindow", "\330\245\331\212\330\254\330\247\330\261", nullptr));
        cmbMainAccount->setItemText(3, QCoreApplication::translate("CashBoxWindow", "\330\261\331\210\330\247\330\252\330\250", nullptr));
        cmbMainAccount->setItemText(4, QCoreApplication::translate("CashBoxWindow", "\331\205\330\263\330\252\331\204\330\262\331\205\330\247\330\252", nullptr));
        cmbMainAccount->setItemText(5, QCoreApplication::translate("CashBoxWindow", "\330\243\330\256\330\261\331\211", nullptr));

        label6->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\330\255\330\263\330\247\330\250 \330\247\331\204\331\201\330\261\330\271\331\212", nullptr));
        label7->setText(QCoreApplication::translate("CashBoxWindow", "\330\261.\330\263\330\247\330\250\331\202 \331\204\331\204\330\262\330\250\331\210\331\206 $", nullptr));
        txtPrevCustDollar->setText(QCoreApplication::translate("CashBoxWindow", "0", nullptr));
        label8->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\330\261\330\265\331\212\330\257 \330\247\331\204\331\205\330\252\330\250\331\202\331\212 $", nullptr));
        txtPrevBalDollar->setText(QCoreApplication::translate("CashBoxWindow", "0", nullptr));
        label9->setText(QCoreApplication::translate("CashBoxWindow", "\330\261.\330\263\330\247\330\250\331\202 \330\257\331\212\331\206\330\247\330\261", nullptr));
        txtPrevCustDinar->setText(QCoreApplication::translate("CashBoxWindow", "0", nullptr));
        label10->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\330\261\330\265\331\212\330\257 \330\247\331\204\331\205\330\252\330\250\331\202\331\212 \330\257\331\212\331\206\330\247\330\261", nullptr));
        txtPrevBalDinar->setText(QCoreApplication::translate("CashBoxWindow", "0", nullptr));
        label11->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\331\205\330\250\331\204\330\272 \330\250\330\247\331\204\330\257\331\210\331\204\330\247\330\261", nullptr));
        txtAmountDollar->setText(QCoreApplication::translate("CashBoxWindow", "0", nullptr));
        label12->setText(QCoreApplication::translate("CashBoxWindow", "\331\205\330\247\331\212\330\271\330\247\330\257\331\204 \330\250\330\247\331\204\330\257\331\212\331\206\330\247\330\261", nullptr));
        txtEquivDinar->setText(QCoreApplication::translate("CashBoxWindow", "0", nullptr));
        label13->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\331\205\330\250\331\204\330\272 \330\250\330\247\331\204\330\257\331\212\331\206\330\247\330\261", nullptr));
        txtAmountDinar->setText(QCoreApplication::translate("CashBoxWindow", "0", nullptr));
        label14->setText(QCoreApplication::translate("CashBoxWindow", "\331\205\330\247\331\212\330\271\330\247\330\257\331\204 \330\250\330\247\331\204\330\257\331\210\331\204\330\247\330\261", nullptr));
        txtEquivDollar->setText(QCoreApplication::translate("CashBoxWindow", "0", nullptr));
        btnCalcEquiv->setText(QCoreApplication::translate("CashBoxWindow", "\331\205\330\247\331\212\330\271\330\247\330\257\331\204", nullptr));
        label15->setText(QCoreApplication::translate("CashBoxWindow", "\330\254\331\207\330\251 \330\247\331\204\330\265\330\261\331\201", nullptr));
        cmbCashbox->setItemText(0, QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\330\265\331\206\330\257\331\210\331\202 \330\247\331\204\330\261\330\246\331\212\330\263\331\212", nullptr));
        cmbCashbox->setItemText(1, QCoreApplication::translate("CashBoxWindow", "\330\265\331\206\330\257\331\210\331\202 2", nullptr));
        cmbCashbox->setItemText(2, QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\330\250\331\206\331\203", nullptr));

        label16->setText(QCoreApplication::translate("CashBoxWindow", "\330\263\330\271\330\261 \330\265\330\261\331\201 $", nullptr));
        txtExchRate->setText(QCoreApplication::translate("CashBoxWindow", "1450", nullptr));
        label17->setText(QCoreApplication::translate("CashBoxWindow", "\331\202\330\247\330\246\331\205\330\251 \330\247\331\204\330\264\330\261\330\247\330\241", nullptr));
        label18->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\331\205\331\204\330\247\330\255\330\270\330\247\330\252", nullptr));
        label19->setText(QCoreApplication::translate("CashBoxWindow", "\330\247\331\204\331\205\331\206\330\270\331\205", nullptr));
        btnSave->setText(QCoreApplication::translate("CashBoxWindow", "\360\237\222\276", nullptr));
#if QT_CONFIG(tooltip)
        btnSave->setToolTip(QCoreApplication::translate("CashBoxWindow", "\330\255\331\201\330\270", nullptr));
#endif // QT_CONFIG(tooltip)
        btnEdit->setText(QCoreApplication::translate("CashBoxWindow", "\342\234\216", nullptr));
#if QT_CONFIG(tooltip)
        btnEdit->setToolTip(QCoreApplication::translate("CashBoxWindow", "\330\252\330\271\330\257\331\212\331\204", nullptr));
#endif // QT_CONFIG(tooltip)
        btnUndo->setText(QCoreApplication::translate("CashBoxWindow", "\342\206\251", nullptr));
#if QT_CONFIG(tooltip)
        btnUndo->setToolTip(QCoreApplication::translate("CashBoxWindow", "\330\252\330\261\330\247\330\254\330\271", nullptr));
#endif // QT_CONFIG(tooltip)
        btnSearch->setText(QCoreApplication::translate("CashBoxWindow", "\360\237\224\215", nullptr));
#if QT_CONFIG(tooltip)
        btnSearch->setToolTip(QCoreApplication::translate("CashBoxWindow", "\330\250\330\255\330\253", nullptr));
#endif // QT_CONFIG(tooltip)
        btnDelete->setText(QCoreApplication::translate("CashBoxWindow", "\342\234\226", nullptr));
#if QT_CONFIG(tooltip)
        btnDelete->setToolTip(QCoreApplication::translate("CashBoxWindow", "\330\255\330\260\331\201", nullptr));
#endif // QT_CONFIG(tooltip)
        btnPrint->setText(QCoreApplication::translate("CashBoxWindow", "\360\237\226\250", nullptr));
#if QT_CONFIG(tooltip)
        btnPrint->setToolTip(QCoreApplication::translate("CashBoxWindow", "\330\267\330\250\330\247\330\271\330\251", nullptr));
#endif // QT_CONFIG(tooltip)
        btnPdf->setText(QCoreApplication::translate("CashBoxWindow", "PDF", nullptr));
        lblNav->setText(QCoreApplication::translate("CashBoxWindow", "0", nullptr));
        label20->setText(QCoreApplication::translate("CashBoxWindow", "\331\205\331\206", nullptr));
        lblTotal->setText(QCoreApplication::translate("CashBoxWindow", "1", nullptr));
        btnReceive->setText(QCoreApplication::translate("CashBoxWindow", "\360\237\244\262", nullptr));
#if QT_CONFIG(tooltip)
        btnReceive->setToolTip(QCoreApplication::translate("CashBoxWindow", "\331\202\330\250\330\266", nullptr));
#endif // QT_CONFIG(tooltip)
        btnPay->setText(QCoreApplication::translate("CashBoxWindow", "\360\237\244\235", nullptr));
#if QT_CONFIG(tooltip)
        btnPay->setToolTip(QCoreApplication::translate("CashBoxWindow", "\330\265\330\261\331\201", nullptr));
#endif // QT_CONFIG(tooltip)
        btnFirst->setText(QCoreApplication::translate("CashBoxWindow", "\342\217\256", nullptr));
        btnPrev->setText(QCoreApplication::translate("CashBoxWindow", "\342\227\200", nullptr));
        btnNext->setText(QCoreApplication::translate("CashBoxWindow", "\342\226\266", nullptr));
        btnLast->setText(QCoreApplication::translate("CashBoxWindow", "\342\217\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CashBoxWindow: public Ui_CashBoxWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CASHBOXWINDOW_H
