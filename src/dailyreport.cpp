#include "dailyreport.h"
#include "printmanager.h"
#include "database.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFont>
#include <QDate>

DailyReport::DailyReport(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("التقرير اليومي"));
    setLayoutDirection(Qt::RightToLeft);
    setFixedSize(500, 280);
    setupUI(); applyStyles();
}

void DailyReport::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    m_titleLabel = new QLabel(QString::fromUtf8("التقرير اليومي"));
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setFont(QFont("Tahoma", 16, QFont::Bold));

    mainLayout->addWidget(m_titleLabel);

    // User filter
    QHBoxLayout *userRow = new QHBoxLayout;
    m_userCheck = new QCheckBox;
    QPushButton *userBtn = new QPushButton(QString::fromUtf8("المستخدم"));
    userBtn->setObjectName("filterBtn");
    userRow->addStretch();
    userRow->addWidget(m_userCheck);
    userRow->addWidget(userBtn);
    mainLayout->addLayout(userRow);

    // Date range
    QGroupBox *dateBox = new QGroupBox;
    dateBox->setObjectName("dateBox");
    QHBoxLayout *dateLayout = new QHBoxLayout(dateBox);

    m_fromDate = new QDateEdit(QDate::currentDate());
    m_fromDate->setDisplayFormat("dd/MM/yyyy");
    m_fromDate->setCalendarPopup(true);
    m_toDate = new QDateEdit(QDate::currentDate());
    m_toDate->setDisplayFormat("dd/MM/yyyy");
    m_toDate->setCalendarPopup(true);

    QPushButton *fromDayBtn = new QPushButton(QString::fromUtf8("من يوم"));
    fromDayBtn->setObjectName("dayBtn");
    QPushButton *toDayBtn = new QPushButton(QString::fromUtf8("ليوم"));
    toDayBtn->setObjectName("dayBtn");

    dateLayout->addWidget(m_fromDate);
    dateLayout->addWidget(fromDayBtn);
    dateLayout->addSpacing(20);
    dateLayout->addWidget(m_toDate);
    dateLayout->addWidget(toDayBtn);

    mainLayout->addWidget(dateBox);

    // Buttons
    QGroupBox *btnBox = new QGroupBox;
    btnBox->setObjectName("btnBox");
    QHBoxLayout *btnLayout = new QHBoxLayout(btnBox);

    m_printBtn = new QPushButton;
    m_printBtn->setObjectName("printBtn");
    m_printBtn->setFixedSize(44, 38);
    m_printBtn->setFont(QFont("Tahoma", 14));
    m_printBtn->setText("🖨");

    m_summaryBtn = new QPushButton(QString::fromUtf8("مجمل"));
    m_summaryBtn->setObjectName("reportBtn");
    m_summaryBtn->setFont(QFont("Tahoma", 11, QFont::Bold));
    m_summaryBtn->setFixedHeight(38);

    m_detailBtn = new QPushButton(QString::fromUtf8("تفصيلي"));
    m_detailBtn->setObjectName("reportBtn");
    m_detailBtn->setFont(QFont("Tahoma", 11, QFont::Bold));
    m_detailBtn->setFixedHeight(38);

    btnLayout->addWidget(m_printBtn);
    btnLayout->addWidget(m_summaryBtn);
    btnLayout->addWidget(m_detailBtn);

    mainLayout->addWidget(btnBox);

    connect(m_detailBtn, &QPushButton::clicked, this, &DailyReport::runDetailed);
    connect(m_summaryBtn, &QPushButton::clicked, this, &DailyReport::runSummary);
    connect(m_printBtn, &QPushButton::clicked, this, &DailyReport::printReport);
}

void DailyReport::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #c0c8d0; }
        #titleLabel {
            background: #9090c0;
            color: white;
            border: 1px solid #6060a0;
            border-radius: 4px;
            padding: 6px;
        }
        #filterBtn {
            background: #c8e8ff;
            border: 1px solid #88aacc;
            border-radius: 3px;
            padding: 4px 12px;
            color: #003366;
        }
        #dateBox {
            background: transparent;
            border: 1px solid #888;
            border-radius: 4px;
        }
        #dayBtn {
            background: #c8d8e8;
            border: 1px solid #88aacc;
            border-radius: 3px;
            padding: 4px 10px;
        }
        #btnBox { background: transparent; border: 1px solid #888; border-radius: 4px; }
        #printBtn { background: #c8d8e8; border: 1px solid #88aacc; border-radius: 3px; }
        #reportBtn {
            background: #c8d8e8;
            border: 1px solid #88aacc;
            border-radius: 4px;
            color: #003366;
            font-weight: bold;
        }
        #reportBtn:hover { background: #b0c8e0; }
        QDateEdit { background: white; border: 1px solid #99aacc; border-radius: 2px; padding: 2px; }
    )");
}

void DailyReport::runDetailed()
{
    PrintManager pm;
    QString html = PrintManager::generateDailyReportHtml(m_fromDate->date(), m_toDate->date(), true);
    pm.printPreview(this, html, QString::fromUtf8("التقرير اليومي - تفصيلي"));
}

void DailyReport::runSummary()
{
    PrintManager pm;
    QString html = PrintManager::generateDailyReportHtml(m_fromDate->date(), m_toDate->date(), false);
    pm.printPreview(this, html, QString::fromUtf8("التقرير اليومي - مجمل"));
}

void DailyReport::printReport() { runDetailed(); }
