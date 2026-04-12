#include "overallbalancereport.h"
#include "database.h"
#include "printmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFont>
#include <QLabel>

OverallBalanceReport::OverallBalanceReport(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("الرصيد الإجمالي للزبائن دائن ومدين"));
    setLayoutDirection(Qt::RightToLeft);
    setFixedSize(560, 420);
    setupUI(); applyStyles();
}

void OverallBalanceReport::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(14);

    auto makeRow = [&](QCheckBox *&chk, QPushButton *&btn, QComboBox *&combo,
                        const QString &label) -> QHBoxLayout* {
        QHBoxLayout *row = new QHBoxLayout;
        chk = new QCheckBox;
        btn = new QPushButton(label);
        btn->setObjectName("redBtn");
        btn->setFont(QFont("Tahoma", 11, QFont::Bold));
        btn->setFixedHeight(36);
        btn->setMinimumWidth(160);
        combo = new QComboBox;
        combo->setMinimumWidth(160);
        combo->setVisible(false);
        row->addWidget(combo);
        row->addStretch();
        row->addWidget(chk);
        row->addWidget(btn);
        connect(chk, &QCheckBox::toggled, combo, &QComboBox::setVisible);
        return row;
    };

    mainLayout->addLayout(makeRow(m_regionCheck, m_regionBtn, m_regionCombo,
                                   QString::fromUtf8("لمنطقة معينة")));
    m_regionCombo->addItems({QString::fromUtf8("بغداد"), QString::fromUtf8("البصرة"),
                              QString::fromUtf8("الموصل")});

    mainLayout->addLayout(makeRow(m_typeCheck, m_typeBtn, m_typeCombo,
                                   QString::fromUtf8("لنوع معين")));
    m_typeCombo->addItems({QString::fromUtf8("شخص"), QString::fromUtf8("شركة")});

    mainLayout->addLayout(makeRow(m_balTypeCheck, m_balTypeBtn, m_balTypeCombo,
                                   QString::fromUtf8("نوع الرصيد")));
    m_balTypeCombo->addItems({QString::fromUtf8("دائن ومدين"),
                               QString::fromUtf8("مدين"), QString::fromUtf8("دائن")});

    // Date row
    QGroupBox *dateBox = new QGroupBox;
    dateBox->setObjectName("dateBox");
    QHBoxLayout *dateLayout = new QHBoxLayout(dateBox);
    m_toDate = new QDateEdit(QDate::currentDate());
    m_toDate->setDisplayFormat("dd/MM/yyyy");
    m_toDate->setCalendarPopup(true);
    QPushButton *todayBtn = new QPushButton(QString::fromUtf8("لنهاية يوم"));
    todayBtn->setObjectName("dayBtn");
    dateLayout->addWidget(m_toDate);
    dateLayout->addWidget(todayBtn);
    mainLayout->addWidget(dateBox);

    // Buttons
    QGroupBox *btnBox = new QGroupBox;
    btnBox->setObjectName("btnBox");
    QHBoxLayout *btnLayout = new QHBoxLayout(btnBox);
    m_printBtn = new QPushButton; m_printBtn->setFixedSize(44,38);
    m_printBtn->setText("🖨"); m_printBtn->setObjectName("printBtn");
    m_runBtn = new QPushButton("▽"); m_runBtn->setFixedSize(44,38);
    m_runBtn->setObjectName("runBtn");
    btnLayout->addWidget(m_printBtn); btnLayout->addWidget(m_runBtn);
    mainLayout->addWidget(btnBox);

    connect(m_runBtn, &QPushButton::clicked, this, &OverallBalanceReport::runReport);
    connect(m_printBtn, &QPushButton::clicked, this, &OverallBalanceReport::printReport);
}

void OverallBalanceReport::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #5c8c7a; }
        #redBtn {
            background: #8b0000; color: #FFD700;
            font-weight: bold; border: 1px solid #600000; border-radius: 4px; padding: 4px 16px;
        }
        #redBtn:hover { background: #aa0000; }
        QCheckBox::indicator { width: 18px; height: 18px; background: white; border: 1px solid #999; }
        QCheckBox::indicator:checked { background: white; border: 2px solid #003366; }
        #dateBox { background: rgba(255,255,255,0.2); border: 1px solid rgba(255,255,255,0.4); border-radius: 4px; }
        #dayBtn { background: rgba(255,255,255,0.7); border: 1px solid #aaa; border-radius: 3px; padding: 4px 10px; color: #003366; }
        #btnBox { background: rgba(255,255,255,0.2); border: 1px solid rgba(255,255,255,0.4); border-radius: 4px; }
        #printBtn, #runBtn { background: rgba(200,220,255,0.7); border: 1px solid #88aacc; border-radius: 3px; font-size: 14pt; }
        QComboBox { background: white; border: 1px solid #aaa; border-radius: 2px; min-height: 28px; }
        QDateEdit { background: white; border: 1px solid #aaa; border-radius: 2px; padding: 2px; }
    )");
}

void OverallBalanceReport::runReport()
{
    QString region = m_regionCheck->isChecked() ? m_regionCombo->currentText() : QString();
    QString type = m_typeCheck->isChecked() ? m_typeCombo->currentText() : QString();
    PrintManager pm;
    QString html = PrintManager::generateOverallBalanceHtml(region, type);
    pm.printPreview(this, html, QString::fromUtf8("الرصيد الإجمالي للزبائن"));
}

void OverallBalanceReport::printReport() { runReport(); }
