#include "allcustomersreport.h"
#include "database.h"
#include "printmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFont>
#include <QLabel>

AllCustomersReport::AllCustomersReport(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("اسماء الزبائن"));
    setLayoutDirection(Qt::RightToLeft);
    setFixedSize(480, 360);
    setupUI(); applyStyles();
}

void AllCustomersReport::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // Region section (left group)
    QHBoxLayout *topRow = new QHBoxLayout;

    QGroupBox *regionBox = new QGroupBox(QString::fromUtf8("المنطقة"));
    regionBox->setObjectName("regionBox");
    regionBox->setFont(QFont("Tahoma", 11));
    QVBoxLayout *regionLayout = new QVBoxLayout(regionBox);
    m_regionCombo = new QComboBox;
    m_regionCombo->setEditable(true);
    m_regionCombo->setMinimumWidth(140);
    m_regionCombo->addItems({QString::fromUtf8("-- الكل --"),
                               QString::fromUtf8("بغداد"), QString::fromUtf8("البصرة"),
                               QString::fromUtf8("الموصل"), QString::fromUtf8("أربيل")});
    regionLayout->addWidget(m_regionCombo);

    // Right group: filter type buttons
    QGroupBox *filterBox = new QGroupBox;
    filterBox->setObjectName("filterBox");
    QVBoxLayout *filterLayout = new QVBoxLayout(filterBox);
    filterLayout->setSpacing(8);

    m_regionBtn = new QPushButton(QString::fromUtf8("لمنطقة معينة"));
    m_typeBtn = new QPushButton(QString::fromUtf8("لنوع معين"));
    m_bothBtn = new QPushButton(QString::fromUtf8("لمنطقة و نوع"));
    m_generalBtn = new QPushButton(QString::fromUtf8("عام"));

    for (auto *b : {m_regionBtn, m_typeBtn, m_bothBtn, m_generalBtn}) {
        b->setObjectName("filterBtn");
        b->setFont(QFont("Tahoma", 11, QFont::Bold));
        b->setFixedHeight(36);
        filterLayout->addWidget(b);
    }

    topRow->addWidget(regionBox);
    topRow->addWidget(filterBox);
    mainLayout->addLayout(topRow);

    // Action buttons
    QHBoxLayout *btnRow = new QHBoxLayout;
    m_printBtn = new QPushButton;
    m_printBtn->setFixedSize(44, 38);
    m_printBtn->setText("🖨");
    m_printBtn->setObjectName("printBtn");
    m_runBtn = new QPushButton("▽");
    m_runBtn->setFixedSize(44, 38);
    m_runBtn->setObjectName("runBtn");

    btnRow->addWidget(m_printBtn);
    btnRow->addWidget(m_runBtn);
    mainLayout->addLayout(btnRow);

    connect(m_regionBtn, &QPushButton::clicked, this, &AllCustomersReport::runByRegion);
    connect(m_typeBtn, &QPushButton::clicked, this, &AllCustomersReport::runByType);
    connect(m_bothBtn, &QPushButton::clicked, this, &AllCustomersReport::runByRegionAndType);
    connect(m_generalBtn, &QPushButton::clicked, this, &AllCustomersReport::runGeneral);
    connect(m_printBtn, &QPushButton::clicked, this, &AllCustomersReport::printReport);
    connect(m_runBtn, &QPushButton::clicked, this, &AllCustomersReport::runGeneral);
}

void AllCustomersReport::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #008080; }
        #regionBox {
            background: rgba(200,220,240,0.7);
            border: 1px solid #aaccc0;
            border-radius: 4px;
            color: #003366;
        }
        #filterBox {
            background: rgba(200,220,240,0.7);
            border: 2px solid #FFD700;
            border-radius: 4px;
        }
        #filterBtn {
            background: #00aacc;
            color: #003366;
            border: 1px solid #0088aa;
            border-radius: 3px;
            font-weight: bold;
        }
        #filterBtn:hover { background: #0099bb; }
        #printBtn, #runBtn {
            background: rgba(200,220,255,0.7);
            border: 1px solid #88aacc;
            border-radius: 3px;
            font-size: 14pt;
        }
        QComboBox { background: white; border: 1px solid #aaa; border-radius: 2px; min-height: 28px; }
        QLabel { color: white; }
        QGroupBox::title { color: #003366; font-weight: bold; }
    )");
}

void AllCustomersReport::runByRegion()
{
    QString region = m_regionCombo->currentText();
    PrintManager pm;
    QString html = PrintManager::generateAllCustomersHtml(region, QString());
    pm.printPreview(this, html, QString::fromUtf8("تقرير أسماء الزبائن"));
}

void AllCustomersReport::runByType()
{
    PrintManager pm;
    QString html = PrintManager::generateAllCustomersHtml(QString(), "0");
    pm.printPreview(this, html, QString::fromUtf8("تقرير أسماء الزبائن"));
}

void AllCustomersReport::runByRegionAndType()
{
    QString region = m_regionCombo->currentText();
    PrintManager pm;
    QString html = PrintManager::generateAllCustomersHtml(region, "0");
    pm.printPreview(this, html, QString::fromUtf8("تقرير أسماء الزبائن"));
}

void AllCustomersReport::runGeneral()
{
    PrintManager pm;
    QString html = PrintManager::generateAllCustomersHtml(QString(), QString());
    pm.printPreview(this, html, QString::fromUtf8("تقرير أسماء الزبائن"));
}

void AllCustomersReport::printReport() { runGeneral(); }
