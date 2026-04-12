#include "latedpayments.h"
#include "database.h"
#include "printmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFont>
#include <QLabel>

LatedPayments::LatedPayments(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("كشف المتأخرين عن التسديد"));
    setLayoutDirection(Qt::RightToLeft);
    setFixedSize(520, 340);
    setupUI(); applyStyles();
}

void LatedPayments::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(14);

    // Title
    QLabel *titleLabel = new QLabel(QString::fromUtf8("كشف المتأخرين عن التسديد"));
    titleLabel->setObjectName("titleLabel");
    titleLabel->setFont(QFont("Tahoma", 13, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Region filter
    QLabel *regionLbl = new QLabel(QString::fromUtf8("المنطقة"));
    regionLbl->setFont(QFont("Tahoma", 11));
    regionLbl->setAlignment(Qt::AlignRight);
    mainLayout->addWidget(regionLbl);

    QHBoxLayout *regionRow = new QHBoxLayout;
    m_regionCombo = new QComboBox;
    m_regionCombo->setEditable(true);
    m_regionCombo->setMinimumWidth(220);
    m_regionCombo->addItems({QString::fromUtf8("-- الكل --"),
                               QString::fromUtf8("بغداد"), QString::fromUtf8("البصرة"),
                               QString::fromUtf8("الموصل")});

    m_regionCheck = new QCheckBox;
    m_regionBtn = new QPushButton(QString::fromUtf8("منطقة الزبون"));
    m_regionBtn->setObjectName("regionBtn");
    m_regionBtn->setFont(QFont("Tahoma", 10, QFont::Bold));
    m_regionBtn->setFixedHeight(32);

    regionRow->addWidget(m_regionCombo);
    regionRow->addStretch();
    regionRow->addWidget(m_regionCheck);
    regionRow->addWidget(m_regionBtn);
    mainLayout->addLayout(regionRow);

    // Days
    QGroupBox *daysBox = new QGroupBox;
    daysBox->setObjectName("daysBox");
    QHBoxLayout *daysLayout = new QHBoxLayout(daysBox);

    m_daysEdit = new QLineEdit("30");
    m_daysEdit->setFixedWidth(80);
    m_daysEdit->setFont(QFont("Tahoma", 12, QFont::Bold));
    m_daysEdit->setAlignment(Qt::AlignCenter);

    m_daysBtn = new QPushButton(QString::fromUtf8("يوم"));
    m_daysBtn->setObjectName("dayBtn");
    m_daysBtn->setFont(QFont("Tahoma", 10));
    m_daysBtn->setFixedHeight(32);

    daysLayout->addStretch();
    daysLayout->addWidget(m_daysEdit);
    daysLayout->addWidget(m_daysBtn);
    daysLayout->addStretch();
    mainLayout->addWidget(daysBox);

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

    connect(m_runBtn, &QPushButton::clicked, this, &LatedPayments::runReport);
    connect(m_printBtn, &QPushButton::clicked, this, &LatedPayments::printReport);
    connect(m_regionCheck, &QCheckBox::toggled, m_regionCombo, &QWidget::setEnabled);
}

void LatedPayments::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #c0c8d0; }
        #titleLabel {
            background: #d0d8e8;
            color: #003366;
            border: 1px solid #88aacc;
            border-radius: 4px;
            padding: 6px;
        }
        #regionBtn {
            background: #c8d8e8;
            border: 1px solid #88aacc;
            border-radius: 3px;
            color: #003366;
            padding: 4px 10px;
        }
        QCheckBox::indicator { width: 18px; height: 18px; background: white; border: 1px solid #999; }
        QCheckBox::indicator:checked { background: #003366; }
        #daysBox { background: rgba(200,210,230,0.5); border: 1px solid #99aacc; border-radius: 4px; }
        #dayBtn {
            background: #c8d8e8;
            border: 1px solid #88aacc;
            border-radius: 3px;
            padding: 4px 14px;
        }
        #btnBox { background: rgba(200,210,230,0.5); border: 1px solid #99aacc; border-radius: 4px; }
        #printBtn, #runBtn {
            background: #c8d8e8;
            border: 1px solid #88aacc;
            border-radius: 3px;
            font-size: 14pt;
        }
        QComboBox { background: white; border: 1px solid #aaa; border-radius: 2px; min-height: 28px; }
        QLineEdit { background: white; border: 1px solid #99aacc; border-radius: 2px; }
        QLabel { color: #003366; }
    )");
}

void LatedPayments::runReport()
{
    int days = m_daysEdit->text().toInt();
    QString region = m_regionCheck->isChecked() ? m_regionCombo->currentText() : QString();
    PrintManager pm;
    QString html = PrintManager::generateLatePaymentsHtml(days, region);
    pm.printPreview(this, html, QString::fromUtf8("كشف المتأخرين عن التسديد"));
}

void LatedPayments::printReport() { runReport(); }
