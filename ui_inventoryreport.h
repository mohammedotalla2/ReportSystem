/********************************************************************************
** Form generated from reading UI file 'inventoryreport.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INVENTORYREPORT_H
#define UI_INVENTORYREPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_InventoryReport
{
public:
    QVBoxLayout *rootLayout;
    QHBoxLayout *hboxLayout;
    QListWidget *listGroups;
    QGroupBox *grpGroupBy;
    QVBoxLayout *vboxLayout;
    QRadioButton *rdoGroup;
    QRadioButton *rdoType;
    QRadioButton *rdoGroupType;
    QRadioButton *rdoGeneral;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem;
    QCheckBox *chkHideZero;
    QSpacerItem *spacerItem1;
    QGroupBox *grpDate;
    QHBoxLayout *hboxLayout2;
    QDateEdit *dateTo;
    QPushButton *btnToday;
    QHBoxLayout *hboxLayout3;
    QPushButton *btnPrint;
    QPushButton *btnRun;
    QSpacerItem *spacerItem2;
    QPushButton *btnPrevBalances;

    void setupUi(QDialog *InventoryReport)
    {
        if (InventoryReport->objectName().isEmpty())
            InventoryReport->setObjectName(QString::fromUtf8("InventoryReport"));
        InventoryReport->resize(520, 420);
        InventoryReport->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(InventoryReport);
        rootLayout->setSpacing(10);
        rootLayout->setContentsMargins(14, 14, 14, 14);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        listGroups = new QListWidget(InventoryReport);
        listGroups->setObjectName(QString::fromUtf8("listGroups"));
        listGroups->setMaximumWidth(180);

        hboxLayout->addWidget(listGroups);

        grpGroupBy = new QGroupBox(InventoryReport);
        grpGroupBy->setObjectName(QString::fromUtf8("grpGroupBy"));
        vboxLayout = new QVBoxLayout(grpGroupBy);
        vboxLayout->setSpacing(12);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        rdoGroup = new QRadioButton(grpGroupBy);
        rdoGroup->setObjectName(QString::fromUtf8("rdoGroup"));
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(11);
        rdoGroup->setFont(font);

        vboxLayout->addWidget(rdoGroup);

        rdoType = new QRadioButton(grpGroupBy);
        rdoType->setObjectName(QString::fromUtf8("rdoType"));
        rdoType->setFont(font);

        vboxLayout->addWidget(rdoType);

        rdoGroupType = new QRadioButton(grpGroupBy);
        rdoGroupType->setObjectName(QString::fromUtf8("rdoGroupType"));
        rdoGroupType->setFont(font);

        vboxLayout->addWidget(rdoGroupType);

        rdoGeneral = new QRadioButton(grpGroupBy);
        rdoGeneral->setObjectName(QString::fromUtf8("rdoGeneral"));
        rdoGeneral->setChecked(true);
        rdoGeneral->setFont(font);

        vboxLayout->addWidget(rdoGeneral);


        hboxLayout->addWidget(grpGroupBy);


        rootLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem);

        chkHideZero = new QCheckBox(InventoryReport);
        chkHideZero->setObjectName(QString::fromUtf8("chkHideZero"));
        chkHideZero->setChecked(true);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(10);
        chkHideZero->setFont(font1);

        hboxLayout1->addWidget(chkHideZero);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);


        rootLayout->addLayout(hboxLayout1);

        grpDate = new QGroupBox(InventoryReport);
        grpDate->setObjectName(QString::fromUtf8("grpDate"));
        hboxLayout2 = new QHBoxLayout(grpDate);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        dateTo = new QDateEdit(grpDate);
        dateTo->setObjectName(QString::fromUtf8("dateTo"));
        dateTo->setCalendarPopup(true);

        hboxLayout2->addWidget(dateTo);

        btnToday = new QPushButton(grpDate);
        btnToday->setObjectName(QString::fromUtf8("btnToday"));
        btnToday->setMinimumHeight(32);

        hboxLayout2->addWidget(btnToday);


        rootLayout->addWidget(grpDate);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        btnPrint = new QPushButton(InventoryReport);
        btnPrint->setObjectName(QString::fromUtf8("btnPrint"));
        btnPrint->setMinimumSize(QSize(44, 38));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Tahoma"));
        font2.setPointSize(14);
        btnPrint->setFont(font2);

        hboxLayout3->addWidget(btnPrint);

        btnRun = new QPushButton(InventoryReport);
        btnRun->setObjectName(QString::fromUtf8("btnRun"));
        btnRun->setMinimumSize(QSize(44, 38));

        hboxLayout3->addWidget(btnRun);

        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacerItem2);

        btnPrevBalances = new QPushButton(InventoryReport);
        btnPrevBalances->setObjectName(QString::fromUtf8("btnPrevBalances"));
        btnPrevBalances->setMinimumHeight(38);
        QFont font3;
        font3.setFamily(QString::fromUtf8("Tahoma"));
        font3.setPointSize(10);
        font3.setBold(true);
        btnPrevBalances->setFont(font3);

        hboxLayout3->addWidget(btnPrevBalances);


        rootLayout->addLayout(hboxLayout3);


        retranslateUi(InventoryReport);

        QMetaObject::connectSlotsByName(InventoryReport);
    } // setupUi

    void retranslateUi(QDialog *InventoryReport)
    {
        InventoryReport->setWindowTitle(QCoreApplication::translate("InventoryReport", "\330\254\330\261\330\257 \330\250\330\243\330\261\330\265\330\257\330\251 \330\247\331\204\331\205\331\210\330\247\330\257", nullptr));
        rdoGroup->setText(QCoreApplication::translate("InventoryReport", "\331\205\330\254\331\205\331\210\330\271\330\251", nullptr));
        rdoType->setText(QCoreApplication::translate("InventoryReport", "\331\206\331\200\331\200\331\200\331\210\330\271", nullptr));
        rdoGroupType->setText(QCoreApplication::translate("InventoryReport", "\331\205\330\254\331\205\331\210\330\271\330\251 \331\210\331\206\331\210\330\271", nullptr));
        rdoGeneral->setText(QCoreApplication::translate("InventoryReport", "\330\271\331\200\331\200\331\200\330\247\331\205", nullptr));
        chkHideZero->setText(QCoreApplication::translate("InventoryReport", "\330\243\330\256\331\201\330\247\330\241 \330\247\331\204\331\205\331\210\330\247\330\257 \330\247\331\204\330\252\331\212 \330\261\330\265\331\212\330\257\331\207\330\247 \330\265\331\201\330\261", nullptr));
        dateTo->setDisplayFormat(QCoreApplication::translate("InventoryReport", "dd/MM/yyyy", nullptr));
        btnToday->setText(QCoreApplication::translate("InventoryReport", "\331\204\331\206\331\207\330\247\331\212\330\251 \331\212\331\210\331\205", nullptr));
        btnPrint->setText(QCoreApplication::translate("InventoryReport", "\360\237\226\250", nullptr));
        btnRun->setText(QCoreApplication::translate("InventoryReport", "\342\226\275", nullptr));
        btnPrevBalances->setText(QCoreApplication::translate("InventoryReport", "\330\247\331\204\330\247\330\261\330\265\330\257\330\251 \330\247\331\204\330\263\330\247\331\204\331\212\330\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InventoryReport: public Ui_InventoryReport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INVENTORYREPORT_H
