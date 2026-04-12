#include "inventoryreport.h"
#include "database.h"
#include "printmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFont>

InventoryReport::InventoryReport(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("جرد بأرصدة المواد"));
    setLayoutDirection(Qt::RightToLeft);
    setFixedSize(520, 420);
    setupUI(); applyStyles();
}

void InventoryReport::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(10);

    QHBoxLayout *topRow = new QHBoxLayout;

    // Left: group list
    m_groupList = new QListWidget;
    m_groupList->setObjectName("groupList");
    m_groupList->setMaximumWidth(180);
    topRow->addWidget(m_groupList);

    // Right: radio buttons
    QGroupBox *radioBox = new QGroupBox;
    radioBox->setObjectName("radioBox");
    QVBoxLayout *radioLayout = new QVBoxLayout(radioBox);
    radioLayout->setSpacing(12);

    m_groupRadio = new QRadioButton(QString::fromUtf8("مجموعة"));
    m_typeRadio = new QRadioButton(QString::fromUtf8("نـــوع"));
    m_groupTypeRadio = new QRadioButton(QString::fromUtf8("مجموعة ونوع"));
    m_generalRadio = new QRadioButton(QString::fromUtf8("عـــام"));
    m_generalRadio->setChecked(true);

    for (auto *r : {m_groupRadio, m_typeRadio, m_groupTypeRadio, m_generalRadio}) {
        r->setFont(QFont("Tahoma", 11));
        radioLayout->addWidget(r);
    }
    topRow->addWidget(radioBox);
    mainLayout->addLayout(topRow);

    // Hide zero check
    QHBoxLayout *checkRow = new QHBoxLayout;
    m_hideZeroCheck = new QCheckBox(QString::fromUtf8("أخفاء المواد التي رصيدها صفر"));
    m_hideZeroCheck->setChecked(true);
    m_hideZeroCheck->setFont(QFont("Tahoma", 10));
    checkRow->addStretch();
    checkRow->addWidget(m_hideZeroCheck);
    checkRow->addStretch();
    mainLayout->addLayout(checkRow);

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

    // Buttons row
    QHBoxLayout *btnRow = new QHBoxLayout;
    m_printBtn = new QPushButton;
    m_printBtn->setFixedSize(44, 38);
    m_printBtn->setText("🖨");
    m_printBtn->setObjectName("printBtn");

    m_runBtn = new QPushButton("▽");
    m_runBtn->setFixedSize(44, 38);
    m_runBtn->setObjectName("runBtn");

    m_prevBalBtn = new QPushButton(QString::fromUtf8("الارصدة السالية"));
    m_prevBalBtn->setObjectName("prevBalBtn");
    m_prevBalBtn->setFont(QFont("Tahoma", 10, QFont::Bold));
    m_prevBalBtn->setFixedHeight(38);

    btnRow->addWidget(m_printBtn);
    btnRow->addWidget(m_runBtn);
    btnRow->addStretch();
    btnRow->addWidget(m_prevBalBtn);
    mainLayout->addLayout(btnRow);

    connect(m_runBtn, &QPushButton::clicked, this, &InventoryReport::runReport);
    connect(m_printBtn, &QPushButton::clicked, this, &InventoryReport::printReport);
    connect(m_prevBalBtn, &QPushButton::clicked, this, &InventoryReport::showPrevBalances);
}

void InventoryReport::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #c0c4cc; }
        #groupList {
            background: white;
            border: 1px solid #99aacc;
            border-radius: 3px;
        }
        #radioBox {
            background: rgba(200,210,230,0.6);
            border: 1px solid #99aacc;
            border-radius: 4px;
        }
        QRadioButton { color: #003366; font-size: 11pt; }
        QCheckBox {
            color: #003366;
            font-weight: bold;
            background: white;
            padding: 4px 8px;
            border: 1px solid #99aacc;
            border-radius: 3px;
        }
        #dateBox { background: transparent; border: 1px solid #99aacc; border-radius: 4px; }
        #dayBtn {
            background: #c8d8e8;
            border: 1px solid #88aacc;
            border-radius: 3px;
            padding: 4px 10px;
        }
        #printBtn, #runBtn {
            background: #c8d8e8;
            border: 1px solid #88aacc;
            border-radius: 3px;
            font-size: 14pt;
        }
        #prevBalBtn {
            background: #c8d8e8;
            border: 1px solid #88aacc;
            border-radius: 3px;
            color: #003366;
            padding: 0 12px;
        }
        QDateEdit { background: white; border: 1px solid #aaa; border-radius: 2px; padding: 2px; }
    )");
}

void InventoryReport::runReport()
{
    QString groupBy = "general";
    if (m_groupRadio->isChecked()) groupBy = "group";
    else if (m_typeRadio->isChecked()) groupBy = "type";
    PrintManager pm;
    QString html = PrintManager::generateInventoryReportHtml(m_toDate->date(), groupBy);
    pm.printPreview(this, html, QString::fromUtf8("جرد أرصدة المواد"));
}

void InventoryReport::printReport() { runReport(); }
void InventoryReport::showPrevBalances() { runReport(); }
