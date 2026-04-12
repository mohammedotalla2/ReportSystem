/********************************************************************************
** Form generated from reading UI file 'reportswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPORTSWINDOW_H
#define UI_REPORTSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReportsWindow
{
public:
    QVBoxLayout *rootLayout;
    QListWidget *listReports;
    QWidget *categoryWidget;
    QHBoxLayout *categoryLayout;
    QPushButton *btnCatCustomers;
    QPushButton *btnCatMaterials;
    QPushButton *btnCatCashBox;
    QPushButton *btnCatProfits;
    QPushButton *btnCatDeleted;
    QHBoxLayout *hboxLayout;
    QPushButton *btnClose;
    QSpacerItem *spacerItem;

    void setupUi(QDialog *ReportsWindow)
    {
        if (ReportsWindow->objectName().isEmpty())
            ReportsWindow->setObjectName(QString::fromUtf8("ReportsWindow"));
        ReportsWindow->resize(520, 580);
        ReportsWindow->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(ReportsWindow);
#ifndef Q_OS_MAC
        rootLayout->setSpacing(6);
#endif
        rootLayout->setContentsMargins(8, 8, 8, 8);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        listReports = new QListWidget(ReportsWindow);
        listReports->setObjectName(QString::fromUtf8("listReports"));
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(11);
        listReports->setFont(font);
        listReports->setLayoutDirection(Qt::RightToLeft);

        rootLayout->addWidget(listReports);

        categoryWidget = new QWidget(ReportsWindow);
        categoryWidget->setObjectName(QString::fromUtf8("categoryWidget"));
        categoryLayout = new QHBoxLayout(categoryWidget);
#ifndef Q_OS_MAC
        categoryLayout->setSpacing(6);
#endif
        categoryLayout->setContentsMargins(4, 4, 4, 4);
        categoryLayout->setObjectName(QString::fromUtf8("categoryLayout"));
        categoryLayout->setContentsMargins(0, 0, 0, 0);
        btnCatCustomers = new QPushButton(categoryWidget);
        btnCatCustomers->setObjectName(QString::fromUtf8("btnCatCustomers"));
        btnCatCustomers->setMinimumHeight(36);
        btnCatCustomers->setCheckable(true);
        btnCatCustomers->setChecked(true);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(11);
        font1.setBold(true);
        btnCatCustomers->setFont(font1);

        categoryLayout->addWidget(btnCatCustomers);

        btnCatMaterials = new QPushButton(categoryWidget);
        btnCatMaterials->setObjectName(QString::fromUtf8("btnCatMaterials"));
        btnCatMaterials->setMinimumHeight(36);
        btnCatMaterials->setCheckable(true);
        btnCatMaterials->setFont(font1);

        categoryLayout->addWidget(btnCatMaterials);

        btnCatCashBox = new QPushButton(categoryWidget);
        btnCatCashBox->setObjectName(QString::fromUtf8("btnCatCashBox"));
        btnCatCashBox->setMinimumHeight(36);
        btnCatCashBox->setCheckable(true);
        btnCatCashBox->setFont(font1);

        categoryLayout->addWidget(btnCatCashBox);

        btnCatProfits = new QPushButton(categoryWidget);
        btnCatProfits->setObjectName(QString::fromUtf8("btnCatProfits"));
        btnCatProfits->setMinimumHeight(36);
        btnCatProfits->setCheckable(true);
        btnCatProfits->setFont(font1);

        categoryLayout->addWidget(btnCatProfits);

        btnCatDeleted = new QPushButton(categoryWidget);
        btnCatDeleted->setObjectName(QString::fromUtf8("btnCatDeleted"));
        btnCatDeleted->setMinimumHeight(36);
        btnCatDeleted->setCheckable(true);
        btnCatDeleted->setFont(font1);

        categoryLayout->addWidget(btnCatDeleted);


        rootLayout->addWidget(categoryWidget);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        btnClose = new QPushButton(ReportsWindow);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setMaximumWidth(44);
        btnClose->setMinimumHeight(36);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Tahoma"));
        font2.setPointSize(14);
        btnClose->setFont(font2);

        hboxLayout->addWidget(btnClose);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);


        rootLayout->addLayout(hboxLayout);


        retranslateUi(ReportsWindow);

        QMetaObject::connectSlotsByName(ReportsWindow);
    } // setupUi

    void retranslateUi(QDialog *ReportsWindow)
    {
        ReportsWindow->setWindowTitle(QCoreApplication::translate("ReportsWindow", "\330\247\331\204\330\252\331\202\330\247\330\261\331\212\330\261", nullptr));
        btnCatCustomers->setText(QCoreApplication::translate("ReportsWindow", "\330\247\331\204\330\262\330\250\330\247\330\246\331\206", nullptr));
        btnCatMaterials->setText(QCoreApplication::translate("ReportsWindow", "\330\247\331\204\331\205\331\210\330\247\330\257", nullptr));
        btnCatCashBox->setText(QCoreApplication::translate("ReportsWindow", "\330\247\331\204\330\265\331\206\330\257\331\210\331\202", nullptr));
        btnCatProfits->setText(QCoreApplication::translate("ReportsWindow", "\330\247\331\204\330\247\330\261\330\250\330\247\330\255", nullptr));
        btnCatDeleted->setText(QCoreApplication::translate("ReportsWindow", "\330\247\331\204\331\205\330\255\330\260\331\210\331\201\330\247\330\252", nullptr));
        btnClose->setText(QCoreApplication::translate("ReportsWindow", "\342\234\226", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReportsWindow: public Ui_ReportsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTSWINDOW_H
