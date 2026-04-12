#include "totalsalesreport.h"
#include "database.h"
#include "printmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFont>
#include <QLabel>
#include <QSqlQuery>

TotalSalesReport::TotalSalesReport(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("مجموع المبيعات حسب الزبائن"));
    setLayoutDirection(Qt::RightToLeft);
    setFixedSize(640, 560);
    setupUI(); applyStyles();
}

void TotalSalesReport::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(12);

    // Title
    QLabel *titleLabel = new QLabel(QString::fromUtf8("مجموع المبيعات حسب الزبائن"));
    titleLabel->setObjectName("titleLabel");
    titleLabel->setFont(QFont("Tahoma", 14, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Four filter rows on the right with checkboxes
    auto makeRow = [&](QCheckBox *&chk, QPushButton *&btn,
                        const QString &label, const QString &obj) -> QHBoxLayout* {
        QHBoxLayout *row = new QHBoxLayout;
        chk = new QCheckBox;
        btn = new QPushButton(label);
        btn->setObjectName(obj);
        btn->setFont(QFont("Tahoma", 11, QFont::Bold));
        btn->setFixedHeight(36);
        btn->setMinimumWidth(140);
        row->addStretch();
        row->addWidget(chk);
        row->addWidget(btn);
        return row;
    };

    mainLayout->addLayout(makeRow(m_groupCheck, m_groupBtn,
                                   QString::fromUtf8("مجموعة المادة"), "cyanBtn"));
    mainLayout->addLayout(makeRow(m_productCheck, m_productBtn,
                                   QString::fromUtf8("المادة"), "cyanBtn"));

    // Customer row with combo + name edit
    QHBoxLayout *custRow = new QHBoxLayout;
    m_customerCheck = new QCheckBox;
    m_customerCheck->setChecked(true);
    m_customerBtn = new QPushButton(QString::fromUtf8("الزبون"));
    m_customerBtn->setObjectName("cyanBtnChecked");
    m_customerBtn->setFont(QFont("Tahoma", 11, QFont::Bold));
    m_customerBtn->setFixedHeight(36);
    m_customerBtn->setMinimumWidth(140);

    QLabel *custNameLbl = new QLabel(QString::fromUtf8("أسم الزبون"));
    custNameLbl->setFont(QFont("Tahoma", 10));

    m_customerCombo = new QComboBox;
    m_customerCombo->setFixedWidth(60);
    m_customerNameEdit = new QLineEdit;
    m_customerNameEdit->setMinimumWidth(160);

    custRow->addWidget(m_customerNameEdit);
    custRow->addWidget(m_customerCombo);
    custRow->addWidget(new QLabel(custNameLbl->text()));
    custRow->addStretch();
    custRow->addWidget(m_customerCheck);
    custRow->addWidget(m_customerBtn);
    mainLayout->addLayout(custRow);

    mainLayout->addLayout(makeRow(m_regionCheck, m_regionBtn,
                                   QString::fromUtf8("المنطقة"), "cyanBtn"));

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

    QPushButton *fromBtn = new QPushButton(QString::fromUtf8("للفترة من"));
    fromBtn->setObjectName("cyanBtn2");
    fromBtn->setFont(QFont("Tahoma", 10, QFont::Bold));
    QPushButton *toBtn = new QPushButton(QString::fromUtf8("الى"));
    toBtn->setObjectName("cyanBtn2");
    toBtn->setFont(QFont("Tahoma", 10, QFont::Bold));

    dateLayout->addWidget(m_fromDate);
    dateLayout->addWidget(fromBtn);
    dateLayout->addSpacing(10);
    dateLayout->addWidget(m_toDate);
    dateLayout->addWidget(toBtn);
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

    // Load customers
    QSqlQuery q = Database::getCustomers();
    m_customerCombo->addItem(QString::fromUtf8("الكل"), -1);
    while (q.next())
        m_customerCombo->addItem(q.value(1).toString(), q.value(0));

    connect(m_runBtn, &QPushButton::clicked, this, &TotalSalesReport::runReport);
    connect(m_printBtn, &QPushButton::clicked, this, &TotalSalesReport::printReport);
}

void TotalSalesReport::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #f0f4d8; }
        #titleLabel {
            background: #ffaacc;
            color: #003366;
            border: 1px solid #cc6688;
            border-radius: 4px;
            padding: 6px;
            font-size: 14pt;
        }
        #cyanBtn {
            background: #80e0e8;
            color: #003366;
            border: 1px solid #40b0c0;
            border-radius: 3px;
            font-weight: bold;
        }
        #cyanBtn:hover { background: #60d0e0; }
        #cyanBtnChecked {
            background: #80e0e8;
            color: #003366;
            border: 2px solid #006080;
            border-radius: 3px;
            font-weight: bold;
        }
        #cyanBtn2 {
            background: #80e0e8;
            color: #003366;
            border: 1px solid #40b0c0;
            border-radius: 3px;
            padding: 4px 10px;
        }
        QCheckBox::indicator {
            width: 18px; height: 18px;
            background: white;
            border: 2px solid #003366;
            border-radius: 2px;
        }
        QCheckBox::indicator:checked {
            background: #003366;
            image: url(none);
        }
        #dateBox { background: transparent; border: 1px solid #88aacc; border-radius: 4px; }
        #btnBox { background: transparent; border: 1px solid #88aacc; border-radius: 4px; }
        #printBtn, #runBtn {
            background: #c8e0ff;
            border: 1px solid #88aacc;
            border-radius: 3px;
            font-size: 14pt;
        }
        QComboBox { background: white; border: 1px solid #aaa; border-radius: 2px; }
        QDateEdit { background: white; border: 1px solid #aaa; border-radius: 2px; padding: 2px; }
        QLineEdit { background: white; border: 1px solid #99aacc; border-radius: 2px; }
        QLabel { color: #003366; }
    )");
}

void TotalSalesReport::runReport()
{
    int custId = m_customerCombo->currentData().toInt();
    QString groupBy = m_groupCheck->isChecked() ? "group" :
                      m_productCheck->isChecked() ? "product" : "customer";
    PrintManager pm;
    QString html = PrintManager::generateTotalSalesHtml(m_fromDate->date(), m_toDate->date(),
                                                         custId, groupBy);
    pm.printPreview(this, html, QString::fromUtf8("مجموع المبيعات"));
}

void TotalSalesReport::printReport() { runReport(); }
