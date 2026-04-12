/********************************************************************************
** Form generated from reading UI file 'latedpayments.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LATEDPAYMENTS_H
#define UI_LATEDPAYMENTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LatedPayments
{
public:
    QVBoxLayout *rootLayout;
    QLabel *lblTitle;
    QLabel *lblRegionTitle;
    QHBoxLayout *hboxLayout;
    QComboBox *cmbRegion;
    QSpacerItem *spacerItem;
    QCheckBox *chkRegion;
    QPushButton *btnRegion;
    QGroupBox *grpDays;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem1;
    QLineEdit *txtDays;
    QPushButton *btnDay;
    QSpacerItem *spacerItem2;
    QGroupBox *grpButtons;
    QHBoxLayout *hboxLayout2;
    QPushButton *btnPrint;
    QPushButton *btnRun;

    void setupUi(QDialog *LatedPayments)
    {
        if (LatedPayments->objectName().isEmpty())
            LatedPayments->setObjectName(QString::fromUtf8("LatedPayments"));
        LatedPayments->resize(520, 340);
        LatedPayments->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(LatedPayments);
        rootLayout->setSpacing(14);
        rootLayout->setContentsMargins(16, 16, 16, 16);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        lblTitle = new QLabel(LatedPayments);
        lblTitle->setObjectName(QString::fromUtf8("lblTitle"));
        lblTitle->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(13);
        font.setBold(true);
        lblTitle->setFont(font);

        rootLayout->addWidget(lblTitle);

        lblRegionTitle = new QLabel(LatedPayments);
        lblRegionTitle->setObjectName(QString::fromUtf8("lblRegionTitle"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(11);
        lblRegionTitle->setFont(font1);

        rootLayout->addWidget(lblRegionTitle);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        cmbRegion = new QComboBox(LatedPayments);
        cmbRegion->addItem(QString());
        cmbRegion->setObjectName(QString::fromUtf8("cmbRegion"));
        cmbRegion->setEditable(true);
        cmbRegion->setMinimumWidth(220);

        hboxLayout->addWidget(cmbRegion);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        chkRegion = new QCheckBox(LatedPayments);
        chkRegion->setObjectName(QString::fromUtf8("chkRegion"));

        hboxLayout->addWidget(chkRegion);

        btnRegion = new QPushButton(LatedPayments);
        btnRegion->setObjectName(QString::fromUtf8("btnRegion"));
        btnRegion->setMinimumHeight(32);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Tahoma"));
        font2.setPointSize(10);
        font2.setBold(true);
        btnRegion->setFont(font2);

        hboxLayout->addWidget(btnRegion);


        rootLayout->addLayout(hboxLayout);

        grpDays = new QGroupBox(LatedPayments);
        grpDays->setObjectName(QString::fromUtf8("grpDays"));
        hboxLayout1 = new QHBoxLayout(grpDays);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);

        txtDays = new QLineEdit(grpDays);
        txtDays->setObjectName(QString::fromUtf8("txtDays"));
        txtDays->setMaximumWidth(80);
        txtDays->setAlignment(Qt::AlignCenter);
        QFont font3;
        font3.setFamily(QString::fromUtf8("Tahoma"));
        font3.setPointSize(12);
        font3.setBold(true);
        txtDays->setFont(font3);

        hboxLayout1->addWidget(txtDays);

        btnDay = new QPushButton(grpDays);
        btnDay->setObjectName(QString::fromUtf8("btnDay"));
        btnDay->setMinimumHeight(32);

        hboxLayout1->addWidget(btnDay);

        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem2);


        rootLayout->addWidget(grpDays);

        grpButtons = new QGroupBox(LatedPayments);
        grpButtons->setObjectName(QString::fromUtf8("grpButtons"));
        hboxLayout2 = new QHBoxLayout(grpButtons);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        btnPrint = new QPushButton(grpButtons);
        btnPrint->setObjectName(QString::fromUtf8("btnPrint"));
        btnPrint->setMinimumSize(QSize(44, 38));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Tahoma"));
        font4.setPointSize(14);
        btnPrint->setFont(font4);

        hboxLayout2->addWidget(btnPrint);

        btnRun = new QPushButton(grpButtons);
        btnRun->setObjectName(QString::fromUtf8("btnRun"));
        btnRun->setMinimumSize(QSize(44, 38));

        hboxLayout2->addWidget(btnRun);


        rootLayout->addWidget(grpButtons);


        retranslateUi(LatedPayments);

        QMetaObject::connectSlotsByName(LatedPayments);
    } // setupUi

    void retranslateUi(QDialog *LatedPayments)
    {
        LatedPayments->setWindowTitle(QCoreApplication::translate("LatedPayments", "\331\203\330\264\331\201 \330\247\331\204\331\205\330\252\330\243\330\256\330\261\331\212\331\206 \330\271\331\206 \330\247\331\204\330\252\330\263\330\257\331\212\330\257", nullptr));
        lblTitle->setText(QCoreApplication::translate("LatedPayments", "\331\203\330\264\331\201 \330\247\331\204\331\205\330\252\330\243\330\256\330\261\331\212\331\206 \330\271\331\206 \330\247\331\204\330\252\330\263\330\257\331\212\330\257", nullptr));
        lblRegionTitle->setText(QCoreApplication::translate("LatedPayments", "\330\247\331\204\331\205\331\206\330\267\331\202\330\251", nullptr));
        cmbRegion->setItemText(0, QCoreApplication::translate("LatedPayments", "-- \330\247\331\204\331\203\331\204 --", nullptr));

        btnRegion->setText(QCoreApplication::translate("LatedPayments", "\331\205\331\206\330\267\331\202\330\251 \330\247\331\204\330\262\330\250\331\210\331\206", nullptr));
        txtDays->setText(QCoreApplication::translate("LatedPayments", "30", nullptr));
        btnDay->setText(QCoreApplication::translate("LatedPayments", "\331\212\331\210\331\205", nullptr));
        btnPrint->setText(QCoreApplication::translate("LatedPayments", "\360\237\226\250", nullptr));
        btnRun->setText(QCoreApplication::translate("LatedPayments", "\342\226\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LatedPayments: public Ui_LatedPayments {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LATEDPAYMENTS_H
