#include "customerstatement.h"
#include "database.h"
#include "printmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFont>
#include <QSqlQuery>

CustomerStatement::CustomerStatement(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("كشف حساب زبون"));
    setLayoutDirection(Qt::RightToLeft);
    setFixedSize(640, 380);
    setupUI(); applyStyles();
}

void CustomerStatement::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);

    // Gray header box with globe background (from screenshot)
    QGroupBox *headerBox = new QGroupBox;
    headerBox->setObjectName("headerBox");
    QVBoxLayout *headerLayout = new QVBoxLayout(headerBox);

    // Customer row
    QHBoxLayout *custRow = new QHBoxLayout;
    QPushButton *custBtn = new QPushButton(QString::fromUtf8("الزبون"));
    custBtn->setObjectName("labelBtn");
    custBtn->setFixedHeight(32);
    m_customerCombo = new QComboBox;
    m_customerCombo->setEditable(true);
    m_customerCombo->setMinimumWidth(280);

    custRow->addWidget(m_customerCombo);
    custRow->addWidget(custBtn);
    headerLayout->addLayout(custRow);

    // Date range row
    QHBoxLayout *dateRow = new QHBoxLayout;
    m_fromDate = new QDateEdit(QDate::currentDate());
    m_fromDate->setDisplayFormat("dd/MM/yyyy");
    m_fromDate->setCalendarPopup(true);
    m_toDate = new QDateEdit(QDate::currentDate());
    m_toDate->setDisplayFormat("dd/MM/yyyy");
    m_toDate->setCalendarPopup(true);

    QPushButton *fromBtn = new QPushButton(QString::fromUtf8("للفترة من"));
    fromBtn->setObjectName("labelBtn2");
    QPushButton *toBtn = new QPushButton(QString::fromUtf8("الى"));
    toBtn->setObjectName("labelBtn2");

    dateRow->addWidget(m_fromDate);
    dateRow->addWidget(fromBtn);
    dateRow->addSpacing(10);
    dateRow->addWidget(m_toDate);
    dateRow->addWidget(toBtn);
    headerLayout->addLayout(dateRow);
    mainLayout->addWidget(headerBox);

    // Currency labels
    QHBoxLayout *currRow = new QHBoxLayout;
    currRow->addStretch();
    QLabel *dolLbl = new QLabel(QString::fromUtf8("دولار"));
    dolLbl->setObjectName("currLbl");
    dolLbl->setFont(QFont("Tahoma", 11));
    QLabel *dinLbl = new QLabel(QString::fromUtf8("دينار"));
    dinLbl->setObjectName("currLbl");
    dinLbl->setFont(QFont("Tahoma", 11));
    currRow->addStretch();
    currRow->addWidget(dolLbl);
    currRow->addSpacing(80);
    currRow->addWidget(dinLbl);
    currRow->addStretch();
    mainLayout->addLayout(currRow);

    // Action buttons
    QHBoxLayout *btnRow = new QHBoxLayout;
    m_printBtn = new QPushButton;
    m_printBtn->setFixedSize(44, 38);
    m_printBtn->setText("🖨");
    m_printBtn->setObjectName("printBtn");

    auto makeBtn = [](const QString &t, const QString &obj) {
        QPushButton *b = new QPushButton(t);
        b->setFont(QFont("Tahoma", 11, QFont::Bold));
        b->setFixedHeight(38);
        b->setObjectName(obj);
        return b;
    };

    QPushButton *detailDolBtn = makeBtn(QString::fromUtf8("مفصل"), "reportBtn");
    QPushButton *summDolBtn = makeBtn(QString::fromUtf8("عام"), "reportBtn");
    QPushButton *detailDinBtn = makeBtn(QString::fromUtf8("مفصل"), "reportBtn");
    QPushButton *summDinBtn = makeBtn(QString::fromUtf8("عام"), "reportBtn");

    btnRow->addWidget(m_printBtn);
    btnRow->addStretch();
    btnRow->addWidget(detailDolBtn);
    btnRow->addWidget(summDolBtn);
    btnRow->addSpacing(20);
    btnRow->addWidget(detailDinBtn);
    btnRow->addWidget(summDinBtn);
    mainLayout->addLayout(btnRow);

    // Load customers
    QSqlQuery q = Database::getCustomers();
    while (q.next())
        m_customerCombo->addItem(q.value(1).toString(), q.value(0));

    connect(detailDolBtn, &QPushButton::clicked, this, &CustomerStatement::runDetailedDollar);
    connect(summDolBtn, &QPushButton::clicked, this, &CustomerStatement::runSummaryDollar);
    connect(detailDinBtn, &QPushButton::clicked, this, &CustomerStatement::runDetailedDinar);
    connect(summDinBtn, &QPushButton::clicked, this, &CustomerStatement::runSummaryDinar);
    connect(m_printBtn, &QPushButton::clicked, this, &CustomerStatement::printReport);
}

void CustomerStatement::applyStyles()
{
    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                stop:0 #6698cc, stop:0.5 #88aacc, stop:1 #4466aa);
        }
        #headerBox {
            background: rgba(80,80,120,0.7);
            border: 2px solid #334488;
            border-radius: 6px;
        }
        #labelBtn {
            background: #003399;
            color: #FFD700;
            font-weight: bold;
            font-size: 11pt;
            border: 1px solid #001166;
            border-radius: 3px;
            padding: 4px 14px;
        }
        #labelBtn2 {
            background: #003399;
            color: #FFD700;
            font-weight: bold;
            border: 1px solid #001166;
            border-radius: 3px;
            padding: 4px 10px;
        }
        #currLbl { color: white; font-weight: bold; }
        #printBtn {
            background: rgba(200,220,255,0.7);
            border: 1px solid #88aacc;
            border-radius: 3px;
            font-size: 14pt;
        }
        #reportBtn {
            background: rgba(200,220,255,0.8);
            border: 1px solid #88aacc;
            border-radius: 4px;
            color: #003366;
            font-weight: bold;
            min-width: 80px;
        }
        #reportBtn:hover { background: rgba(180,200,240,0.9); }
        QComboBox { background: white; border: 1px solid #aaa; border-radius: 2px; }
        QDateEdit { background: white; border: 1px solid #aaa; border-radius: 2px; padding: 2px; }
        QLabel { color: white; }
    )");
}

void CustomerStatement::runDetailedDollar()
{
    int custId = m_customerCombo->currentData().toInt();
    if (custId <= 0) return;
    PrintManager pm;
    QString html = PrintManager::generateCustomerStatementHtml(custId, m_fromDate->date(), m_toDate->date());
    pm.printPreview(this, html, QString::fromUtf8("كشف حساب زبون"));
}

void CustomerStatement::runSummaryDollar() { runDetailedDollar(); }
void CustomerStatement::runDetailedDinar() { runDetailedDollar(); }
void CustomerStatement::runSummaryDinar() { runDetailedDollar(); }
void CustomerStatement::printReport() { runDetailedDollar(); }
