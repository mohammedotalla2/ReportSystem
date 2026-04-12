/********************************************************************************
** Form generated from reading UI file 'customerstatement.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMERSTATEMENT_H
#define UI_CUSTOMERSTATEMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CustomerStatement
{
public:
    QVBoxLayout *rootLayout;
    QGroupBox *grpHeader;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QComboBox *cmbCustomer;
    QPushButton *btnCustomer;
    QHBoxLayout *hboxLayout1;
    QDateEdit *dateFrom;
    QPushButton *btnFrom;
    QSpacerItem *spacerItem;
    QDateEdit *dateTo;
    QPushButton *btnTo;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem1;
    QLabel *lblDollar;
    QSpacerItem *spacerItem2;
    QLabel *lblDinar;
    QSpacerItem *spacerItem3;
    QHBoxLayout *hboxLayout3;
    QPushButton *btnPrint;
    QSpacerItem *spacerItem4;
    QPushButton *btnDetailDollar;
    QPushButton *btnSummaryDollar;
    QSpacerItem *spacerItem5;
    QPushButton *btnDetailDinar;
    QPushButton *btnSummaryDinar;

    void setupUi(QDialog *CustomerStatement)
    {
        if (CustomerStatement->objectName().isEmpty())
            CustomerStatement->setObjectName(QString::fromUtf8("CustomerStatement"));
        CustomerStatement->resize(640, 380);
        CustomerStatement->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(CustomerStatement);
        rootLayout->setSpacing(10);
        rootLayout->setContentsMargins(12, 12, 12, 12);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        grpHeader = new QGroupBox(CustomerStatement);
        grpHeader->setObjectName(QString::fromUtf8("grpHeader"));
        vboxLayout = new QVBoxLayout(grpHeader);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        cmbCustomer = new QComboBox(grpHeader);
        cmbCustomer->setObjectName(QString::fromUtf8("cmbCustomer"));
        cmbCustomer->setEditable(true);
        cmbCustomer->setMinimumWidth(280);

        hboxLayout->addWidget(cmbCustomer);

        btnCustomer = new QPushButton(grpHeader);
        btnCustomer->setObjectName(QString::fromUtf8("btnCustomer"));
        btnCustomer->setMinimumHeight(32);

        hboxLayout->addWidget(btnCustomer);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        dateFrom = new QDateEdit(grpHeader);
        dateFrom->setObjectName(QString::fromUtf8("dateFrom"));
        dateFrom->setCalendarPopup(true);

        hboxLayout1->addWidget(dateFrom);

        btnFrom = new QPushButton(grpHeader);
        btnFrom->setObjectName(QString::fromUtf8("btnFrom"));
        btnFrom->setMinimumHeight(32);

        hboxLayout1->addWidget(btnFrom);

        spacerItem = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem);

        dateTo = new QDateEdit(grpHeader);
        dateTo->setObjectName(QString::fromUtf8("dateTo"));
        dateTo->setCalendarPopup(true);

        hboxLayout1->addWidget(dateTo);

        btnTo = new QPushButton(grpHeader);
        btnTo->setObjectName(QString::fromUtf8("btnTo"));
        btnTo->setMinimumHeight(32);

        hboxLayout1->addWidget(btnTo);


        vboxLayout->addLayout(hboxLayout1);


        rootLayout->addWidget(grpHeader);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem1);

        lblDollar = new QLabel(CustomerStatement);
        lblDollar->setObjectName(QString::fromUtf8("lblDollar"));
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(11);
        lblDollar->setFont(font);

        hboxLayout2->addWidget(lblDollar);

        spacerItem2 = new QSpacerItem(80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem2);

        lblDinar = new QLabel(CustomerStatement);
        lblDinar->setObjectName(QString::fromUtf8("lblDinar"));
        lblDinar->setFont(font);

        hboxLayout2->addWidget(lblDinar);

        spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem3);


        rootLayout->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        btnPrint = new QPushButton(CustomerStatement);
        btnPrint->setObjectName(QString::fromUtf8("btnPrint"));
        btnPrint->setMinimumSize(QSize(44, 38));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(14);
        btnPrint->setFont(font1);

        hboxLayout3->addWidget(btnPrint);

        spacerItem4 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacerItem4);

        btnDetailDollar = new QPushButton(CustomerStatement);
        btnDetailDollar->setObjectName(QString::fromUtf8("btnDetailDollar"));
        btnDetailDollar->setMinimumSize(QSize(80, 38));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Tahoma"));
        font2.setPointSize(11);
        font2.setBold(true);
        btnDetailDollar->setFont(font2);

        hboxLayout3->addWidget(btnDetailDollar);

        btnSummaryDollar = new QPushButton(CustomerStatement);
        btnSummaryDollar->setObjectName(QString::fromUtf8("btnSummaryDollar"));
        btnSummaryDollar->setMinimumSize(QSize(80, 38));
        btnSummaryDollar->setFont(font2);

        hboxLayout3->addWidget(btnSummaryDollar);

        spacerItem5 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacerItem5);

        btnDetailDinar = new QPushButton(CustomerStatement);
        btnDetailDinar->setObjectName(QString::fromUtf8("btnDetailDinar"));
        btnDetailDinar->setMinimumSize(QSize(80, 38));
        btnDetailDinar->setFont(font2);

        hboxLayout3->addWidget(btnDetailDinar);

        btnSummaryDinar = new QPushButton(CustomerStatement);
        btnSummaryDinar->setObjectName(QString::fromUtf8("btnSummaryDinar"));
        btnSummaryDinar->setMinimumSize(QSize(80, 38));
        btnSummaryDinar->setFont(font2);

        hboxLayout3->addWidget(btnSummaryDinar);


        rootLayout->addLayout(hboxLayout3);


        retranslateUi(CustomerStatement);

        QMetaObject::connectSlotsByName(CustomerStatement);
    } // setupUi

    void retranslateUi(QDialog *CustomerStatement)
    {
        CustomerStatement->setWindowTitle(QCoreApplication::translate("CustomerStatement", "\331\203\330\264\331\201 \330\255\330\263\330\247\330\250 \330\262\330\250\331\210\331\206", nullptr));
        btnCustomer->setText(QCoreApplication::translate("CustomerStatement", "\330\247\331\204\330\262\330\250\331\210\331\206", nullptr));
        dateFrom->setDisplayFormat(QCoreApplication::translate("CustomerStatement", "dd/MM/yyyy", nullptr));
        btnFrom->setText(QCoreApplication::translate("CustomerStatement", "\331\204\331\204\331\201\330\252\330\261\330\251 \331\205\331\206", nullptr));
        dateTo->setDisplayFormat(QCoreApplication::translate("CustomerStatement", "dd/MM/yyyy", nullptr));
        btnTo->setText(QCoreApplication::translate("CustomerStatement", "\330\247\331\204\331\211", nullptr));
        lblDollar->setText(QCoreApplication::translate("CustomerStatement", "\330\257\331\210\331\204\330\247\330\261", nullptr));
        lblDinar->setText(QCoreApplication::translate("CustomerStatement", "\330\257\331\212\331\206\330\247\330\261", nullptr));
        btnPrint->setText(QCoreApplication::translate("CustomerStatement", "\360\237\226\250", nullptr));
        btnDetailDollar->setText(QCoreApplication::translate("CustomerStatement", "\331\205\331\201\330\265\331\204", nullptr));
        btnSummaryDollar->setText(QCoreApplication::translate("CustomerStatement", "\330\271\330\247\331\205", nullptr));
        btnDetailDinar->setText(QCoreApplication::translate("CustomerStatement", "\331\205\331\201\330\265\331\204", nullptr));
        btnSummaryDinar->setText(QCoreApplication::translate("CustomerStatement", "\330\271\330\247\331\205", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CustomerStatement: public Ui_CustomerStatement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMERSTATEMENT_H
