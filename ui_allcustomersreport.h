/********************************************************************************
** Form generated from reading UI file 'allcustomersreport.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALLCUSTOMERSREPORT_H
#define UI_ALLCUSTOMERSREPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AllCustomersReport
{
public:
    QVBoxLayout *rootLayout;
    QHBoxLayout *hboxLayout;
    QGroupBox *grpRegion;
    QVBoxLayout *vboxLayout;
    QComboBox *cmbRegion;
    QGroupBox *grpFilters;
    QVBoxLayout *vboxLayout1;
    QPushButton *btnByRegion;
    QPushButton *btnByType;
    QPushButton *btnByBoth;
    QPushButton *btnGeneral;
    QHBoxLayout *hboxLayout1;
    QPushButton *btnPrint;
    QPushButton *btnRun;

    void setupUi(QDialog *AllCustomersReport)
    {
        if (AllCustomersReport->objectName().isEmpty())
            AllCustomersReport->setObjectName(QString::fromUtf8("AllCustomersReport"));
        AllCustomersReport->resize(480, 360);
        AllCustomersReport->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(AllCustomersReport);
        rootLayout->setSpacing(12);
        rootLayout->setContentsMargins(16, 16, 16, 16);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        grpRegion = new QGroupBox(AllCustomersReport);
        grpRegion->setObjectName(QString::fromUtf8("grpRegion"));
        vboxLayout = new QVBoxLayout(grpRegion);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        cmbRegion = new QComboBox(grpRegion);
        cmbRegion->addItem(QString());
        cmbRegion->setObjectName(QString::fromUtf8("cmbRegion"));
        cmbRegion->setEditable(true);
        cmbRegion->setMinimumWidth(140);

        vboxLayout->addWidget(cmbRegion);


        hboxLayout->addWidget(grpRegion);

        grpFilters = new QGroupBox(AllCustomersReport);
        grpFilters->setObjectName(QString::fromUtf8("grpFilters"));
        vboxLayout1 = new QVBoxLayout(grpFilters);
        vboxLayout1->setSpacing(8);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        btnByRegion = new QPushButton(grpFilters);
        btnByRegion->setObjectName(QString::fromUtf8("btnByRegion"));
        btnByRegion->setMinimumHeight(36);
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(11);
        font.setBold(true);
        btnByRegion->setFont(font);

        vboxLayout1->addWidget(btnByRegion);

        btnByType = new QPushButton(grpFilters);
        btnByType->setObjectName(QString::fromUtf8("btnByType"));
        btnByType->setMinimumHeight(36);
        btnByType->setFont(font);

        vboxLayout1->addWidget(btnByType);

        btnByBoth = new QPushButton(grpFilters);
        btnByBoth->setObjectName(QString::fromUtf8("btnByBoth"));
        btnByBoth->setMinimumHeight(36);
        btnByBoth->setFont(font);

        vboxLayout1->addWidget(btnByBoth);

        btnGeneral = new QPushButton(grpFilters);
        btnGeneral->setObjectName(QString::fromUtf8("btnGeneral"));
        btnGeneral->setMinimumHeight(36);
        btnGeneral->setFont(font);

        vboxLayout1->addWidget(btnGeneral);


        hboxLayout->addWidget(grpFilters);


        rootLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        btnPrint = new QPushButton(AllCustomersReport);
        btnPrint->setObjectName(QString::fromUtf8("btnPrint"));
        btnPrint->setMinimumSize(QSize(44, 38));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(14);
        btnPrint->setFont(font1);

        hboxLayout1->addWidget(btnPrint);

        btnRun = new QPushButton(AllCustomersReport);
        btnRun->setObjectName(QString::fromUtf8("btnRun"));
        btnRun->setMinimumSize(QSize(44, 38));

        hboxLayout1->addWidget(btnRun);


        rootLayout->addLayout(hboxLayout1);


        retranslateUi(AllCustomersReport);

        QMetaObject::connectSlotsByName(AllCustomersReport);
    } // setupUi

    void retranslateUi(QDialog *AllCustomersReport)
    {
        AllCustomersReport->setWindowTitle(QCoreApplication::translate("AllCustomersReport", "\330\247\330\263\331\205\330\247\330\241 \330\247\331\204\330\262\330\250\330\247\330\246\331\206", nullptr));
        grpRegion->setTitle(QCoreApplication::translate("AllCustomersReport", "\330\247\331\204\331\205\331\206\330\267\331\202\330\251", nullptr));
        cmbRegion->setItemText(0, QCoreApplication::translate("AllCustomersReport", "-- \330\247\331\204\331\203\331\204 --", nullptr));

        btnByRegion->setText(QCoreApplication::translate("AllCustomersReport", "\331\204\331\205\331\206\330\267\331\202\330\251 \331\205\330\271\331\212\331\206\330\251", nullptr));
        btnByType->setText(QCoreApplication::translate("AllCustomersReport", "\331\204\331\206\331\210\330\271 \331\205\330\271\331\212\331\206", nullptr));
        btnByBoth->setText(QCoreApplication::translate("AllCustomersReport", "\331\204\331\205\331\206\330\267\331\202\330\251 \331\210 \331\206\331\210\330\271", nullptr));
        btnGeneral->setText(QCoreApplication::translate("AllCustomersReport", "\330\271\330\247\331\205", nullptr));
        btnPrint->setText(QCoreApplication::translate("AllCustomersReport", "\360\237\226\250", nullptr));
        btnRun->setText(QCoreApplication::translate("AllCustomersReport", "\342\226\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AllCustomersReport: public Ui_AllCustomersReport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALLCUSTOMERSREPORT_H
