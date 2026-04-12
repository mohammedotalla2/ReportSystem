/********************************************************************************
** Form generated from reading UI file 'dailyreport.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DAILYREPORT_H
#define UI_DAILYREPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DailyReport
{
public:
    QVBoxLayout *rootLayout;
    QLabel *lblTitle;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QCheckBox *chkUser;
    QPushButton *btnUser;
    QGroupBox *grpDates;
    QHBoxLayout *datesLayout;
    QDateEdit *dateFrom;
    QPushButton *btnFromDay;
    QSpacerItem *spacerItem1;
    QDateEdit *dateTo;
    QPushButton *btnToDay;
    QGroupBox *grpButtons;
    QHBoxLayout *buttonsLayout;
    QPushButton *btnPrint;
    QPushButton *btnSummary;
    QPushButton *btnDetailed;

    void setupUi(QDialog *DailyReport)
    {
        if (DailyReport->objectName().isEmpty())
            DailyReport->setObjectName(QString::fromUtf8("DailyReport"));
        DailyReport->resize(500, 280);
        DailyReport->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(DailyReport);
        rootLayout->setSpacing(12);
        rootLayout->setContentsMargins(16, 16, 16, 16);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        lblTitle = new QLabel(DailyReport);
        lblTitle->setObjectName(QString::fromUtf8("lblTitle"));
        lblTitle->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(16);
        font.setBold(true);
        lblTitle->setFont(font);

        rootLayout->addWidget(lblTitle);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        chkUser = new QCheckBox(DailyReport);
        chkUser->setObjectName(QString::fromUtf8("chkUser"));

        hboxLayout->addWidget(chkUser);

        btnUser = new QPushButton(DailyReport);
        btnUser->setObjectName(QString::fromUtf8("btnUser"));
        btnUser->setMinimumHeight(32);

        hboxLayout->addWidget(btnUser);


        rootLayout->addLayout(hboxLayout);

        grpDates = new QGroupBox(DailyReport);
        grpDates->setObjectName(QString::fromUtf8("grpDates"));
        datesLayout = new QHBoxLayout(grpDates);
        datesLayout->setObjectName(QString::fromUtf8("datesLayout"));
        dateFrom = new QDateEdit(grpDates);
        dateFrom->setObjectName(QString::fromUtf8("dateFrom"));
        dateFrom->setCalendarPopup(true);

        datesLayout->addWidget(dateFrom);

        btnFromDay = new QPushButton(grpDates);
        btnFromDay->setObjectName(QString::fromUtf8("btnFromDay"));
        btnFromDay->setMinimumHeight(32);

        datesLayout->addWidget(btnFromDay);

        spacerItem1 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        datesLayout->addItem(spacerItem1);

        dateTo = new QDateEdit(grpDates);
        dateTo->setObjectName(QString::fromUtf8("dateTo"));
        dateTo->setCalendarPopup(true);

        datesLayout->addWidget(dateTo);

        btnToDay = new QPushButton(grpDates);
        btnToDay->setObjectName(QString::fromUtf8("btnToDay"));
        btnToDay->setMinimumHeight(32);

        datesLayout->addWidget(btnToDay);


        rootLayout->addWidget(grpDates);

        grpButtons = new QGroupBox(DailyReport);
        grpButtons->setObjectName(QString::fromUtf8("grpButtons"));
        buttonsLayout = new QHBoxLayout(grpButtons);
        buttonsLayout->setObjectName(QString::fromUtf8("buttonsLayout"));
        btnPrint = new QPushButton(grpButtons);
        btnPrint->setObjectName(QString::fromUtf8("btnPrint"));
        btnPrint->setMinimumSize(QSize(44, 38));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(14);
        btnPrint->setFont(font1);

        buttonsLayout->addWidget(btnPrint);

        btnSummary = new QPushButton(grpButtons);
        btnSummary->setObjectName(QString::fromUtf8("btnSummary"));
        btnSummary->setMinimumHeight(38);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Tahoma"));
        font2.setPointSize(11);
        font2.setBold(true);
        btnSummary->setFont(font2);

        buttonsLayout->addWidget(btnSummary);

        btnDetailed = new QPushButton(grpButtons);
        btnDetailed->setObjectName(QString::fromUtf8("btnDetailed"));
        btnDetailed->setMinimumHeight(38);
        btnDetailed->setFont(font2);

        buttonsLayout->addWidget(btnDetailed);


        rootLayout->addWidget(grpButtons);


        retranslateUi(DailyReport);

        QMetaObject::connectSlotsByName(DailyReport);
    } // setupUi

    void retranslateUi(QDialog *DailyReport)
    {
        DailyReport->setWindowTitle(QCoreApplication::translate("DailyReport", "\330\247\331\204\330\252\331\202\330\261\331\212\330\261 \330\247\331\204\331\212\331\210\331\205\331\212", nullptr));
        lblTitle->setText(QCoreApplication::translate("DailyReport", "\330\247\331\204\330\252\331\202\330\261\331\212\330\261 \330\247\331\204\331\212\331\210\331\205\331\212", nullptr));
        btnUser->setText(QCoreApplication::translate("DailyReport", "\330\247\331\204\331\205\330\263\330\252\330\256\330\257\331\205", nullptr));
        dateFrom->setDisplayFormat(QCoreApplication::translate("DailyReport", "dd/MM/yyyy", nullptr));
        btnFromDay->setText(QCoreApplication::translate("DailyReport", "\331\205\331\206 \331\212\331\210\331\205", nullptr));
        dateTo->setDisplayFormat(QCoreApplication::translate("DailyReport", "dd/MM/yyyy", nullptr));
        btnToDay->setText(QCoreApplication::translate("DailyReport", "\331\204\331\212\331\210\331\205", nullptr));
        btnPrint->setText(QCoreApplication::translate("DailyReport", "\360\237\226\250", nullptr));
        btnSummary->setText(QCoreApplication::translate("DailyReport", "\331\205\330\254\331\205\331\204", nullptr));
        btnDetailed->setText(QCoreApplication::translate("DailyReport", "\330\252\331\201\330\265\331\212\331\204\331\212", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DailyReport: public Ui_DailyReport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DAILYREPORT_H
