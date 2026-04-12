#include "customerbalances.h"
#include "database.h"
#include "printmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFont>
#include <QFrame>

CustomerBalances::CustomerBalances(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("ارصدة الزبائن"));
    setLayoutDirection(Qt::RightToLeft);
    setFixedSize(560, 420);
    setupUI(); applyStyles();
}

void CustomerBalances::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(14);

    // Three filter rows: منطقة معينة / نوع معين / نوع الرصيد
    auto makeFilterRow = [&](QCheckBox *&chk, QPushButton *&btn,
                              QComboBox *&combo, const QString &label,
                              const QString &objName) {
        QHBoxLayout *row = new QHBoxLayout;
        chk = new QCheckBox;
        btn = new QPushButton(label);
        btn->setObjectName(objName);
        btn->setFont(QFont("Tahoma", 11, QFont::Bold));
        btn->setFixedHeight(36);
        btn->setMinimumWidth(130);
        combo = new QComboBox;
        combo->setMinimumWidth(160);
        combo->setVisible(false);   // shown when checkbox ticked
        row->addStretch();
        row->addWidget(chk);
        row->addWidget(btn);
        return row;
    };

    QHBoxLayout *r1 = makeFilterRow(m_regionCheck, m_regionBtn, m_regionCombo,
                                     QString::fromUtf8("لمنطقة معينة"), "redBtn");
    QHBoxLayout *r2 = makeFilterRow(m_typeCheck, m_typeBtn, m_typeCombo,
                                     QString::fromUtf8("لنوع معين"), "redBtn");
    QHBoxLayout *r3 = makeFilterRow(m_balTypeCheck, m_balTypeBtn, m_balTypeCombo,
                                     QString::fromUtf8("نوع الرصيد"), "redBtn");

    // Add combos to their rows
    r1->insertWidget(0, m_regionCombo);
    r2->insertWidget(0, m_typeCombo);
    r3->insertWidget(0, m_balTypeCombo);

    // Populate combos
    m_regionCombo->addItems({QString::fromUtf8("-- الكل --"),
                              QString::fromUtf8("بغداد"), QString::fromUtf8("البصرة"),
                              QString::fromUtf8("الموصل"), QString::fromUtf8("أربيل")});
    m_typeCombo->addItems({QString::fromUtf8("-- الكل --"),
                            QString::fromUtf8("شخص"), QString::fromUtf8("شركة")});
    m_balTypeCombo->addItems({QString::fromUtf8("دائن ومدين"),
                               QString::fromUtf8("مدين فقط"),
                               QString::fromUtf8("دائن فقط")});

    mainLayout->addLayout(r1);
    mainLayout->addLayout(r2);
    mainLayout->addLayout(r3);

    // Date row
    QGroupBox *dateBox = new QGroupBox;
    dateBox->setObjectName("dateBox");
    QHBoxLayout *dateLayout = new QHBoxLayout(dateBox);
    m_toDate = new QDateEdit(QDate::currentDate());
    m_toDate->setDisplayFormat("dd/MM/yyyy");
    m_toDate->setCalendarPopup(true);
    QPushButton *todayBtn = new QPushButton(QString::fromUtf8("لنهاية يوم"));
    todayBtn->setObjectName("dayBtn");
    todayBtn->setFont(QFont("Tahoma", 10));
    dateLayout->addWidget(m_toDate);
    dateLayout->addWidget(todayBtn);
    mainLayout->addWidget(dateBox);

    // Action buttons
    QGroupBox *btnBox = new QGroupBox;
    btnBox->setObjectName("btnBox");
    QHBoxLayout *btnLayout = new QHBoxLayout(btnBox);
    m_printBtn = new QPushButton;
    m_printBtn->setFixedSize(44, 38);
    m_printBtn->setText("🖨");
    m_printBtn->setObjectName("printBtn");
    m_runBtn = new QPushButton("▽");
    m_runBtn->setFixedSize(44, 38);
    m_runBtn->setObjectName("runBtn");
    btnLayout->addWidget(m_printBtn);
    btnLayout->addWidget(m_runBtn);
    mainLayout->addWidget(btnBox);

    // Toggle combos on checkbox change
    connect(m_regionCheck, &QCheckBox::toggled, m_regionCombo, &QComboBox::setVisible);
    connect(m_typeCheck, &QCheckBox::toggled, m_typeCombo, &QComboBox::setVisible);
    connect(m_balTypeCheck, &QCheckBox::toggled, m_balTypeCombo, &QComboBox::setVisible);
    connect(m_runBtn, &QPushButton::clicked, this, &CustomerBalances::runReport);
    connect(m_printBtn, &QPushButton::clicked, this, &CustomerBalances::printReport);
}

void CustomerBalances::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #5c8c7a; }
        #redBtn {
            background: #8b0000;
            color: #FFD700;
            font-weight: bold;
            border: 1px solid #600000;
            border-radius: 4px;
            padding: 4px 16px;
        }
        #redBtn:hover { background: #aa0000; }
        QCheckBox::indicator { width: 18px; height: 18px; }
        #dateBox {
            background: rgba(255,255,255,0.25);
            border: 1px solid rgba(255,255,255,0.4);
            border-radius: 4px;
        }
        #dayBtn {
            background: rgba(255,255,255,0.7);
            border: 1px solid #aaa;
            border-radius: 3px;
            padding: 4px 10px;
            color: #003366;
        }
        #btnBox {
            background: rgba(255,255,255,0.2);
            border: 1px solid rgba(255,255,255,0.4);
            border-radius: 4px;
        }
        #printBtn, #runBtn {
            background: rgba(200,220,255,0.7);
            border: 1px solid #88aacc;
            border-radius: 3px;
            font-size: 14pt;
        }
        QComboBox { background: white; border: 1px solid #aaa; border-radius: 2px; min-height: 28px; }
        QDateEdit { background: white; border: 1px solid #aaa; border-radius: 2px; padding: 2px; }
        QLabel { color: white; }
    )");
}

void CustomerBalances::runReport()
{
    QString region = m_regionCheck->isChecked() ? m_regionCombo->currentText() : QString();
    PrintManager pm;
    QString html = PrintManager::generateCustomerStatementHtml(0, QDate(2000,1,1), m_toDate->date());
    pm.printPreview(this, html, QString::fromUtf8("ارصدة الزبائن"));
}

void CustomerBalances::printReport() { runReport(); }
