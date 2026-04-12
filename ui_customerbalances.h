/********************************************************************************
** Form generated from reading UI file 'customerbalances.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMERBALANCES_H
#define UI_CUSTOMERBALANCES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CustomerBalances
{
public:
    QVBoxLayout *rootLayout;
    QHBoxLayout *hboxLayout;
    QComboBox *cmbRegion;
    QSpacerItem *spacerItem;
    QCheckBox *chkRegion;
    QPushButton *btnRegion;
    QHBoxLayout *hboxLayout1;
    QComboBox *cmbType;
    QSpacerItem *spacerItem1;
    QCheckBox *chkType;
    QPushButton *btnType;
    QHBoxLayout *hboxLayout2;
    QComboBox *cmbBalType;
    QSpacerItem *spacerItem2;
    QCheckBox *chkBalType;
    QPushButton *btnBalType;
    QGroupBox *grpDate;
    QHBoxLayout *hboxLayout3;
    QDateEdit *dateTo;
    QPushButton *btnToday;
    QGroupBox *grpButtons;
    QHBoxLayout *hboxLayout4;
    QPushButton *btnPrint;
    QPushButton *btnRun;

    void setupUi(QDialog *CustomerBalances)
    {
        if (CustomerBalances->objectName().isEmpty())
            CustomerBalances->setObjectName(QString::fromUtf8("CustomerBalances"));
        CustomerBalances->resize(560, 420);
        CustomerBalances->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(CustomerBalances);
        rootLayout->setSpacing(14);
        rootLayout->setContentsMargins(16, 16, 16, 16);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        cmbRegion = new QComboBox(CustomerBalances);
        cmbRegion->addItem(QString());
        cmbRegion->setObjectName(QString::fromUtf8("cmbRegion"));
        cmbRegion->setMinimumWidth(160);

        hboxLayout->addWidget(cmbRegion);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        chkRegion = new QCheckBox(CustomerBalances);
        chkRegion->setObjectName(QString::fromUtf8("chkRegion"));

        hboxLayout->addWidget(chkRegion);

        btnRegion = new QPushButton(CustomerBalances);
        btnRegion->setObjectName(QString::fromUtf8("btnRegion"));
        btnRegion->setMinimumSize(QSize(130, 36));
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(11);
        font.setBold(true);
        btnRegion->setFont(font);

        hboxLayout->addWidget(btnRegion);


        rootLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        cmbType = new QComboBox(CustomerBalances);
        cmbType->addItem(QString());
        cmbType->addItem(QString());
        cmbType->addItem(QString());
        cmbType->setObjectName(QString::fromUtf8("cmbType"));
        cmbType->setMinimumWidth(160);

        hboxLayout1->addWidget(cmbType);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);

        chkType = new QCheckBox(CustomerBalances);
        chkType->setObjectName(QString::fromUtf8("chkType"));

        hboxLayout1->addWidget(chkType);

        btnType = new QPushButton(CustomerBalances);
        btnType->setObjectName(QString::fromUtf8("btnType"));
        btnType->setMinimumSize(QSize(130, 36));
        btnType->setFont(font);

        hboxLayout1->addWidget(btnType);


        rootLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        cmbBalType = new QComboBox(CustomerBalances);
        cmbBalType->addItem(QString());
        cmbBalType->addItem(QString());
        cmbBalType->addItem(QString());
        cmbBalType->setObjectName(QString::fromUtf8("cmbBalType"));
        cmbBalType->setMinimumWidth(160);

        hboxLayout2->addWidget(cmbBalType);

        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem2);

        chkBalType = new QCheckBox(CustomerBalances);
        chkBalType->setObjectName(QString::fromUtf8("chkBalType"));

        hboxLayout2->addWidget(chkBalType);

        btnBalType = new QPushButton(CustomerBalances);
        btnBalType->setObjectName(QString::fromUtf8("btnBalType"));
        btnBalType->setMinimumSize(QSize(130, 36));
        btnBalType->setFont(font);

        hboxLayout2->addWidget(btnBalType);


        rootLayout->addLayout(hboxLayout2);

        grpDate = new QGroupBox(CustomerBalances);
        grpDate->setObjectName(QString::fromUtf8("grpDate"));
        hboxLayout3 = new QHBoxLayout(grpDate);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        dateTo = new QDateEdit(grpDate);
        dateTo->setObjectName(QString::fromUtf8("dateTo"));
        dateTo->setCalendarPopup(true);

        hboxLayout3->addWidget(dateTo);

        btnToday = new QPushButton(grpDate);
        btnToday->setObjectName(QString::fromUtf8("btnToday"));
        btnToday->setMinimumHeight(32);

        hboxLayout3->addWidget(btnToday);


        rootLayout->addWidget(grpDate);

        grpButtons = new QGroupBox(CustomerBalances);
        grpButtons->setObjectName(QString::fromUtf8("grpButtons"));
        hboxLayout4 = new QHBoxLayout(grpButtons);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        btnPrint = new QPushButton(grpButtons);
        btnPrint->setObjectName(QString::fromUtf8("btnPrint"));
        btnPrint->setMinimumSize(QSize(44, 38));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(14);
        btnPrint->setFont(font1);

        hboxLayout4->addWidget(btnPrint);

        btnRun = new QPushButton(grpButtons);
        btnRun->setObjectName(QString::fromUtf8("btnRun"));
        btnRun->setMinimumSize(QSize(44, 38));

        hboxLayout4->addWidget(btnRun);


        rootLayout->addWidget(grpButtons);


        retranslateUi(CustomerBalances);

        QMetaObject::connectSlotsByName(CustomerBalances);
    } // setupUi

    void retranslateUi(QDialog *CustomerBalances)
    {
        CustomerBalances->setWindowTitle(QCoreApplication::translate("CustomerBalances", "\330\247\330\261\330\265\330\257\330\251 \330\247\331\204\330\262\330\250\330\247\330\246\331\206", nullptr));
        cmbRegion->setItemText(0, QCoreApplication::translate("CustomerBalances", "-- \330\247\331\204\331\203\331\204 --", nullptr));

        btnRegion->setText(QCoreApplication::translate("CustomerBalances", "\331\204\331\205\331\206\330\267\331\202\330\251 \331\205\330\271\331\212\331\206\330\251", nullptr));
        cmbType->setItemText(0, QCoreApplication::translate("CustomerBalances", "-- \330\247\331\204\331\203\331\204 --", nullptr));
        cmbType->setItemText(1, QCoreApplication::translate("CustomerBalances", "\330\264\330\256\330\265", nullptr));
        cmbType->setItemText(2, QCoreApplication::translate("CustomerBalances", "\330\264\330\261\331\203\330\251", nullptr));

        btnType->setText(QCoreApplication::translate("CustomerBalances", "\331\204\331\206\331\210\330\271 \331\205\330\271\331\212\331\206", nullptr));
        cmbBalType->setItemText(0, QCoreApplication::translate("CustomerBalances", "\330\257\330\247\330\246\331\206 \331\210\331\205\330\257\331\212\331\206", nullptr));
        cmbBalType->setItemText(1, QCoreApplication::translate("CustomerBalances", "\331\205\330\257\331\212\331\206 \331\201\331\202\330\267", nullptr));
        cmbBalType->setItemText(2, QCoreApplication::translate("CustomerBalances", "\330\257\330\247\330\246\331\206 \331\201\331\202\330\267", nullptr));

        btnBalType->setText(QCoreApplication::translate("CustomerBalances", "\331\206\331\210\330\271 \330\247\331\204\330\261\330\265\331\212\330\257", nullptr));
        dateTo->setDisplayFormat(QCoreApplication::translate("CustomerBalances", "dd/MM/yyyy", nullptr));
        btnToday->setText(QCoreApplication::translate("CustomerBalances", "\331\204\331\206\331\207\330\247\331\212\330\251 \331\212\331\210\331\205", nullptr));
        btnPrint->setText(QCoreApplication::translate("CustomerBalances", "\360\237\226\250", nullptr));
        btnRun->setText(QCoreApplication::translate("CustomerBalances", "\342\226\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CustomerBalances: public Ui_CustomerBalances {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMERBALANCES_H
