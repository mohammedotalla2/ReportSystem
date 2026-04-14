#include "cashboxwindow.h"
#include "database.h"
#include "arabichelper.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QFont>
#include <QDate>
#include <QTime>
#include <QSqlQuery>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>

CashBoxWindow::CashBoxWindow(QWidget *parent)
    : QDialog(parent), m_currentId(-1)
{
    setWindowTitle(QString::fromUtf8("إدارة حركات الصندوق"));
    setLayoutDirection(Qt::RightToLeft);
    resize(980, 620);
    m_printer = new PrintManager(this);
    setupUI();
    applyStyles();
    clearForm();
}

void CashBoxWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ══════════════════════════════════════════════════════════════
    //  BLUE TITLE BAR
    // ══════════════════════════════════════════════════════════════
    QWidget *titleBar = new QWidget;
    titleBar->setObjectName("titleBar");
    titleBar->setFixedHeight(46);
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(18, 4, 18, 4);

    QLabel *titleLbl = new QLabel(QString::fromUtf8("إدارة حركات الصندوق"));
    titleLbl->setObjectName("titleLbl");
    titleLbl->setFont(QFont("Tahoma", 14, QFont::Bold));
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLayout->addStretch();
    titleLayout->addWidget(titleLbl);
    titleLayout->addStretch();

    // Current time on the left side of the title bar
    m_timeLabel = new QLabel(QTime::currentTime().toString("hh:mm AP"));
    m_timeLabel->setObjectName("timeLbl");
    titleLayout->addWidget(m_timeLabel);

    mainLayout->addWidget(titleBar);

    // ══════════════════════════════════════════════════════════════
    //  BALANCE PANEL (yellow strip)
    // ══════════════════════════════════════════════════════════════
    QWidget *balPanel = new QWidget;
    balPanel->setObjectName("balPanel");
    QHBoxLayout *balRow = new QHBoxLayout(balPanel);
    balRow->setContentsMargins(12, 4, 12, 4);
    balRow->setSpacing(8);

    auto balLbl = [](const QString &t) {
        QLabel *l = new QLabel(t);
        l->setFont(QFont("Tahoma", 9, QFont::Bold));
        l->setObjectName("balTitle");
        return l;
    };
    auto valLbl = [](const QString &obj) {
        QLabel *l = new QLabel("0");
        l->setObjectName(obj);
        l->setFont(QFont("Tahoma", 10, QFont::Bold));
        l->setMinimumWidth(80);
        l->setAlignment(Qt::AlignCenter);
        return l;
    };

    m_curBalDollarLabel  = valLbl("curBalVal");
    m_curBalDinarLabel   = valLbl("curBalVal");
    m_projBalDollarLabel = valLbl("projBalVal");
    m_projBalDinarLabel  = valLbl("projBalVal");

    // Visual right-to-left (RTL HBox): first added appears on the right
    balRow->addWidget(balLbl(QString::fromUtf8("رصيد الصندوق $:")));
    balRow->addWidget(m_curBalDollarLabel);
    balRow->addWidget(balLbl(QString::fromUtf8("  دينار:")));
    balRow->addWidget(m_curBalDinarLabel);
    balRow->addStretch();
    balRow->addWidget(balLbl(QString::fromUtf8("بعد الحركة $:")));
    balRow->addWidget(m_projBalDollarLabel);
    balRow->addWidget(balLbl(QString::fromUtf8("  دينار:")));
    balRow->addWidget(m_projBalDinarLabel);

    mainLayout->addWidget(balPanel);

    // ══════════════════════════════════════════════════════════════
    //  MAIN FORM CARD
    // ══════════════════════════════════════════════════════════════
    QWidget *card = new QWidget;
    card->setObjectName("formCard");
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(16, 12, 16, 8);
    cardLayout->setSpacing(8);

    QGridLayout *grid = new QGridLayout;
    grid->setSpacing(6);

    // RTL grid: col 0 = visual right, higher col = visual left
    // Pattern: [field(col+1)] [label(col)]  ← label is right of field in RTL

    auto lbl = [](const QString &t) -> QLabel* {
        QLabel *l = new QLabel(t);
        l->setFont(QFont("Tahoma", 9));
        l->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        return l;
    };
    auto roEdit = [](const QString &obj = "") -> QLineEdit* {
        QLineEdit *e = new QLineEdit("0");
        e->setReadOnly(true);
        if (!obj.isEmpty()) e->setObjectName(obj);
        return e;
    };

    // ── Row 0: نوع الحركة | رقم السند | المنظم | الوقت ──
    m_typeCombo = new QComboBox;
    m_typeCombo->addItems({
        QString::fromUtf8("صرف"),
        QString::fromUtf8("قبض"),
        QString::fromUtf8("شراء دولار"),
        QString::fromUtf8("بيع دولار")
    });
    m_typeCombo->setMinimumWidth(130);

    m_docNoEdit = new QLineEdit;
    m_docNoEdit->setPlaceholderText(QString::fromUtf8("رقم السند"));

    m_organiserSpin = new QSpinBox;
    m_organiserSpin->setValue(1);
    m_organiserSpin->setFixedWidth(65);
    m_organiserSpin->setObjectName("organiserSpin");

    // col:  label(0) field(1-2) | label(3) field(4) | label(5) field(6) | label(7) field(8)
    grid->addWidget(lbl(QString::fromUtf8("نوع الحركة")), 0, 0);
    grid->addWidget(m_typeCombo,     0, 1, 1, 2);
    grid->addWidget(lbl(QString::fromUtf8("رقم السند")), 0, 3);
    grid->addWidget(m_docNoEdit,     0, 4);
    grid->addWidget(lbl(QString::fromUtf8("المنظم")),    0, 5);
    grid->addWidget(m_organiserSpin, 0, 6);

    // ── Row 1: التاريخ — compact day/month/year side by side ──
    m_dateDay   = new QDateEdit(QDate::currentDate()); m_dateDay->setDisplayFormat("d");
    m_dateMonth = new QDateEdit(QDate::currentDate()); m_dateMonth->setDisplayFormat("M");
    m_dateYear  = new QDateEdit(QDate::currentDate()); m_dateYear->setDisplayFormat("yyyy");
    m_dateDay->setFixedWidth(44);
    m_dateMonth->setFixedWidth(44);
    m_dateYear->setFixedWidth(62);
    m_dateDay->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_dateMonth->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_dateYear->setButtonSymbols(QAbstractSpinBox::NoButtons);

    QHBoxLayout *dateRow = new QHBoxLayout;
    dateRow->setSpacing(2);
    dateRow->addWidget(m_dateDay);
    dateRow->addWidget(m_dateMonth);
    dateRow->addWidget(m_dateYear);
    dateRow->addStretch();
    QWidget *dateWidget = new QWidget;
    dateWidget->setLayout(dateRow);

    grid->addWidget(lbl(QString::fromUtf8("التاريخ")), 1, 0);
    grid->addWidget(dateWidget, 1, 1, 1, 3);

    // ── Row 2: الحساب الرئيسي | الحساب الفرعي ──
    m_mainAccountCombo = new QComboBox;
    m_mainAccountCombo->addItems({
        QString::fromUtf8("مصاريف"),
        QString::fromUtf8("عملاء"),
        QString::fromUtf8("موردون"),
        QString::fromUtf8("أخرى")
    });

    m_subAccountCombo = new QComboBox;
    m_subAccountCombo->setEditable(true);
    m_subAccountCombo->setMinimumWidth(160);

    grid->addWidget(lbl(QString::fromUtf8("الحساب الرئيسي")), 2, 0);
    grid->addWidget(m_mainAccountCombo, 2, 1, 1, 2);
    grid->addWidget(lbl(QString::fromUtf8("الحساب الفرعي")),  2, 3);
    grid->addWidget(m_subAccountCombo,  2, 4, 1, 3);

    // ── Row 3: Customer widget (hidden by default) ──
    m_customerWidget = new QWidget;
    QHBoxLayout *custRow = new QHBoxLayout(m_customerWidget);
    custRow->setContentsMargins(0, 0, 0, 0);
    custRow->setSpacing(6);
    m_customerCombo = new QComboBox;
    m_customerCombo->setEditable(true);
    m_customerCombo->setMinimumWidth(200);
    m_custBalDollarLabel = new QLabel("$: 0");
    m_custBalDollarLabel->setObjectName("custBalLbl");
    m_custBalDinarLabel  = new QLabel(QString::fromUtf8("د: 0"));
    m_custBalDinarLabel->setObjectName("custBalLbl");
    custRow->addWidget(lbl(QString::fromUtf8("العميل")));
    custRow->addWidget(m_customerCombo);
    custRow->addWidget(m_custBalDollarLabel);
    custRow->addWidget(m_custBalDinarLabel);
    custRow->addStretch();
    m_customerWidget->hide();
    grid->addWidget(m_customerWidget, 3, 0, 1, 7);

    // ── Row 4: المبلغ بالدولار | مايعادل بالدينار ──
    m_amountDollarEdit = new QLineEdit("0");
    m_equivDinarEdit   = roEdit("yellowField");

    grid->addWidget(lbl(QString::fromUtf8("المبلغ بالدولار")),   4, 0);
    grid->addWidget(m_amountDollarEdit, 4, 1);
    grid->addWidget(lbl(QString::fromUtf8("يعادل دينار")),       4, 3);
    grid->addWidget(m_equivDinarEdit,   4, 4);

    // ── Row 5: المبلغ بالدينار | مايعادل بالدولار | [مايعادل btn] ──
    m_amountDinarEdit = new QLineEdit("0");
    m_equivDollarEdit = roEdit("yellowField");
    m_equivBtn = new QPushButton(QString::fromUtf8("احتساب"));
    m_equivBtn->setObjectName("equivBtn");
    m_equivBtn->setFixedWidth(80);

    grid->addWidget(lbl(QString::fromUtf8("المبلغ بالدينار")),   5, 0);
    grid->addWidget(m_amountDinarEdit, 5, 1);
    grid->addWidget(lbl(QString::fromUtf8("يعادل دولار")),       5, 3);
    grid->addWidget(m_equivDollarEdit, 5, 4);
    grid->addWidget(m_equivBtn,        5, 6);

    // ── Row 6: سعر الصرف | جهة الصرف ──
    m_exchangeRateEdit = new QLineEdit(
        QString::number(Database::getExchangeRate(), 'f', 0));
    m_exchangeRateEdit->setObjectName("yellowField");
    m_exchangeRateEdit->setFixedWidth(90);

    m_cashboxCombo = new QComboBox;
    m_cashboxCombo->addItems({
        QString::fromUtf8("الصندوق الرئيسي"),
        QString::fromUtf8("صندوق 2"),
        QString::fromUtf8("البنك")
    });

    grid->addWidget(lbl(QString::fromUtf8("سعر صرف $")), 6, 0);
    grid->addWidget(m_exchangeRateEdit, 6, 1);
    grid->addWidget(lbl(QString::fromUtf8("جهة الصرف")), 6, 3);
    grid->addWidget(m_cashboxCombo,     6, 4, 1, 3);

    // ── Row 7: الملاحظات (full-width) ──
    m_notesEdit = new QLineEdit;
    m_notesEdit->setPlaceholderText(QString::fromUtf8("الملاحظات ..."));
    grid->addWidget(lbl(QString::fromUtf8("الملاحظات")), 7, 0);
    grid->addWidget(m_notesEdit, 7, 1, 1, 6);

    grid->setColumnStretch(2, 1);
    grid->setColumnStretch(4, 1);

    cardLayout->addLayout(grid);
    mainLayout->addWidget(card, 1);

    // ══════════════════════════════════════════════════════════════
    //  BOTTOM TOOLBAR
    // ══════════════════════════════════════════════════════════════
    QWidget *toolbar = new QWidget;
    toolbar->setObjectName("toolbar");
    QHBoxLayout *toolLayout = new QHBoxLayout(toolbar);
    toolLayout->setContentsMargins(10, 6, 10, 6);
    toolLayout->setSpacing(4);

    auto makeBtn = [](const QString &txt, const QString &tip = "",
                      const QString &obj = "toolBtn") -> QPushButton* {
        QPushButton *b = new QPushButton(txt);
        b->setFixedSize(42, 36);
        b->setToolTip(tip);
        b->setObjectName(obj);
        b->setFont(QFont("Tahoma", 13));
        return b;
    };

    m_saveBtn   = makeBtn("💾", QString::fromUtf8("حفظ"));
    m_searchBtn = makeBtn("🔍", QString::fromUtf8("بحث"));
    m_deleteBtn = makeBtn("✖",  QString::fromUtf8("حذف"));
    m_printBtn  = makeBtn("🖨", QString::fromUtf8("طباعة"));

    m_pdfBtn = new QPushButton("PDF");
    m_pdfBtn->setFixedHeight(36);
    m_pdfBtn->setObjectName("pdfBtn");
    m_pdfBtn->setFont(QFont("Tahoma", 10, QFont::Bold));
    m_pdfBtn->setMinimumWidth(50);

    QPushButton *newBtn = new QPushButton(QString::fromUtf8("جديد"));
    newBtn->setFixedHeight(36);
    newBtn->setObjectName("newBtn");
    newBtn->setFont(QFont("Tahoma", 10));

    QPushButton *closeBtn = new QPushButton(QString::fromUtf8("إغلاق"));
    closeBtn->setFixedHeight(36);
    closeBtn->setObjectName("toolBtn");

    m_firstBtn = makeBtn("⏮");
    m_prevBtn  = makeBtn("◀");
    m_nextBtn  = makeBtn("▶");
    m_lastBtn  = makeBtn("⏭");

    m_navLabel = new QLabel("0");
    m_navLabel->setObjectName("navLbl");
    m_navLabel->setFixedWidth(55);
    m_navLabel->setAlignment(Qt::AlignCenter);

    m_totalLabel = new QLabel("0");
    m_totalLabel->setFixedWidth(40);
    m_totalLabel->setAlignment(Qt::AlignCenter);

    toolLayout->addWidget(m_saveBtn);
    toolLayout->addWidget(m_searchBtn);
    toolLayout->addWidget(m_deleteBtn);
    toolLayout->addWidget(m_printBtn);
    toolLayout->addWidget(m_pdfBtn);
    toolLayout->addWidget(newBtn);
    toolLayout->addStretch();
    toolLayout->addWidget(m_navLabel);
    toolLayout->addWidget(new QLabel(QString::fromUtf8("/")));
    toolLayout->addWidget(m_totalLabel);
    toolLayout->addStretch();
    toolLayout->addWidget(closeBtn);
    toolLayout->addWidget(m_firstBtn);
    toolLayout->addWidget(m_prevBtn);
    toolLayout->addWidget(m_nextBtn);
    toolLayout->addWidget(m_lastBtn);

    mainLayout->addWidget(toolbar);

    // ══════════════════════════════════════════════════════════════
    //  SIGNAL CONNECTIONS
    // ══════════════════════════════════════════════════════════════
    connect(m_saveBtn,   &QPushButton::clicked, this, &CashBoxWindow::saveTransaction);
    connect(m_deleteBtn, &QPushButton::clicked, this, &CashBoxWindow::deleteTransaction);
    connect(m_searchBtn, &QPushButton::clicked, this, &CashBoxWindow::searchTransaction);
    connect(m_printBtn,  &QPushButton::clicked, this, &CashBoxWindow::printReceipt);
    connect(m_pdfBtn,    &QPushButton::clicked, this, &CashBoxWindow::exportPDF);
    connect(newBtn,      &QPushButton::clicked, this, &CashBoxWindow::newTransaction);
    connect(closeBtn,    &QPushButton::clicked, this, &QDialog::close);
    connect(m_firstBtn,  &QPushButton::clicked, this, &CashBoxWindow::navigateFirst);
    connect(m_prevBtn,   &QPushButton::clicked, this, &CashBoxWindow::navigatePrev);
    connect(m_nextBtn,   &QPushButton::clicked, this, &CashBoxWindow::navigateNext);
    connect(m_lastBtn,   &QPushButton::clicked, this, &CashBoxWindow::navigateLast);
    connect(m_equivBtn,  &QPushButton::clicked, this, &CashBoxWindow::calculateEquivalent);

    connect(m_amountDollarEdit, &QLineEdit::textChanged, this, &CashBoxWindow::calculateEquivalent);
    connect(m_amountDinarEdit,  &QLineEdit::textChanged, this, &CashBoxWindow::calculateEquivalent);
    connect(m_amountDollarEdit, &QLineEdit::textChanged, this, &CashBoxWindow::updateBalance);
    connect(m_amountDinarEdit,  &QLineEdit::textChanged, this, &CashBoxWindow::updateBalance);

    connect(m_typeCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CashBoxWindow::onTypeChanged);
    connect(m_mainAccountCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CashBoxWindow::onMainAccountChanged);

    connect(m_customerCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int idx) {
        if (idx < 0) return;
        int custId = m_customerCombo->itemData(idx).toInt();
        if (custId <= 0) return;
        QSqlQuery q;
        q.prepare("SELECT balance_dollar, balance_dinar FROM customers WHERE id=?");
        q.addBindValue(custId); q.exec();
        if (q.next()) {
            m_custBalDollarLabel->setText("$: " + QString::number(q.value(0).toDouble(), 'f', 2));
            m_custBalDinarLabel->setText(QString::fromUtf8("د: ") + QString::number(q.value(1).toDouble(), 'f', 0));
        }
    });
}

void CashBoxWindow::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #F5F5F5; }

        /* ─── Blue title bar ─── */
        #titleBar {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #1a4a9e, stop:1 #0d2a6e);
        }
        #titleLbl {
            color: #ffffff;
            font-size: 14pt;
        }
        #timeLbl {
            color: #ffffc0;
            font-weight: bold;
            font-size: 10pt;
            padding: 2px 6px;
        }

        /* ─── Balance panel ─── */
        #balPanel {
            background: #ffffc0;
            border-bottom: 2px solid #cccc60;
        }
        #balTitle { color: #003366; }
        #curBalVal {
            background: white;
            border: 1px solid #aaaa60;
            border-radius: 3px;
            padding: 1px 6px;
            color: #003300;
        }
        #projBalVal {
            background: #c0ffc0;
            border: 1px solid #60aa60;
            border-radius: 3px;
            padding: 1px 6px;
            color: #003300;
        }

        /* ─── Form card ─── */
        #formCard {
            background: #ffffff;
            border: 1px solid #d0d8e8;
            border-radius: 6px;
            margin: 8px;
        }

        QLineEdit {
            background: white;
            border: 1px solid #99aacc;
            border-radius: 3px;
            padding: 2px 6px;
            font-family: Tahoma;
            font-size: 10pt;
            min-height: 26px;
        }
        QLineEdit:focus {
            border: 1px solid #1a4a9e;
        }
        #yellowField {
            background: #ffffc0;
            border: 1px solid #aaaa60;
            font-weight: bold;
        }

        QComboBox {
            background: white;
            border: 1px solid #99aacc;
            border-radius: 3px;
            min-height: 26px;
            padding: 1px 6px;
            font-family: Tahoma;
            font-size: 10pt;
        }
        QComboBox::drop-down { border: none; width: 18px; }

        QDateEdit {
            background: white;
            border: 1px solid #99aacc;
            border-radius: 3px;
            min-height: 26px;
            font-family: Tahoma;
            font-size: 10pt;
            padding: 1px 2px;
        }
        QDateEdit::up-button, QDateEdit::down-button { width: 0; }

        QSpinBox {
            background: white;
            border: 1px solid #99aacc;
            border-radius: 3px;
            min-height: 26px;
        }
        #organiserSpin {
            background: #e8d0ff;
            font-weight: bold;
            border: 1px solid #aa66ee;
        }

        QLabel { font-family: Tahoma; font-size: 9pt; color: #334466; }
        #custBalLbl {
            font-weight: bold;
            color: #003366;
            background: #e8f0ff;
            padding: 2px 8px;
            border-radius: 3px;
            border: 1px solid #99aadd;
        }

        #equivBtn {
            background: #dde8f8;
            border: 1px solid #99aacc;
            border-radius: 3px;
            font-weight: bold;
            min-height: 26px;
        }
        #equivBtn:hover { background: #c8d8f0; }

        /* ─── Toolbar ─── */
        #toolbar {
            background: #1a3a6e;
            border-top: 2px solid #0d2a5e;
        }
        #toolBtn {
            background: #2a5aaa;
            border: 1px solid #4477cc;
            color: white;
            border-radius: 3px;
        }
        #toolBtn:hover { background: #3366bb; }
        #newBtn {
            background: #2a8844;
            color: white;
            border: 1px solid #1a6633;
            border-radius: 3px;
            padding: 0 10px;
            font-weight: bold;
        }
        #newBtn:hover { background: #33aa55; }
        #pdfBtn {
            background: #cc2200;
            color: white;
            font-weight: bold;
            border-radius: 3px;
            border: 1px solid #880000;
            padding: 0 8px;
        }
        #navLbl {
            background: #FFD700;
            color: #003366;
            font-weight: bold;
            border: 1px solid #aa8800;
            border-radius: 2px;
            font-size: 11pt;
        }
        QLabel#toolbar { color: white; }
    )");
}

// ══════════════════════════════════════════════════════════════
//  SLOT IMPLEMENTATIONS
// ══════════════════════════════════════════════════════════════

void CashBoxWindow::onTypeChanged(int /*index*/)
{
    // Nothing extra per type for now; balance updates via updateBalance
    updateBalance();
}

void CashBoxWindow::onMainAccountChanged(int /*index*/)
{
    m_subAccountCombo->clear();
    QString main = m_mainAccountCombo->currentText();

    if (main == QString::fromUtf8("مصاريف")) {
        m_subAccountCombo->addItems({
            QString::fromUtf8("إيجار المحل"),
            QString::fromUtf8("رواتب الموظفين"),
            QString::fromUtf8("فاتورة الكهرباء"),
            QString::fromUtf8("فاتورة الإنترنت"),
            QString::fromUtf8("مولدة"),
            QString::fromUtf8("مستلزمات مكتبية"),
            QString::fromUtf8("عمال"),
            QString::fromUtf8("نقل وشحن"),
            QString::fromUtf8("أخرى")
        });
        m_customerWidget->hide();

    } else if (main == QString::fromUtf8("عملاء") ||
               main == QString::fromUtf8("موردون")) {
        // Load customers into sub-account combo AND customer combo
        m_customerCombo->clear();
        m_customerCombo->addItem(QString::fromUtf8("-- اختر --"), -1);
        QSqlQuery q;
        q.exec("SELECT id, name FROM customers ORDER BY name");
        while (q.next()) {
            m_subAccountCombo->addItem(q.value(1).toString(), q.value(0));
            m_customerCombo->addItem(q.value(1).toString(), q.value(0));
        }
        m_custBalDollarLabel->setText("$: 0");
        m_custBalDinarLabel->setText(QString::fromUtf8("د: 0"));
        m_customerWidget->show();

    } else {
        // أخرى – free text
        m_subAccountCombo->setEditable(true);
        m_customerWidget->hide();
    }
}

void CashBoxWindow::calculateEquivalent()
{
    double rate = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();
    double dollar = m_amountDollarEdit->text().toDouble();
    double dinar  = m_amountDinarEdit->text().toDouble();
    m_equivDinarEdit->setText(QString::number(dollar * rate, 'f', 0));
    m_equivDollarEdit->setText(QString::number(dinar  / rate, 'f', 2));
    updateBalance();
}

void CashBoxWindow::updateBalance()
{
    // Compute projected balance after this (unsaved) transaction
    double curD  = m_curBalDollarLabel->text().toDouble();
    double curDn = m_curBalDinarLabel->text().toDouble();
    double amtD  = m_amountDollarEdit->text().toDouble();
    double amtDn = m_amountDinarEdit->text().toDouble();
    int    type  = m_typeCombo->currentIndex();

    double projD  = curD;
    double projDn = curDn;

    switch (type) {
    case 0: // صرف — cash goes out
        projD  -= amtD;
        projDn -= amtDn;
        break;
    case 1: // قبض — cash comes in
        projD  += amtD;
        projDn += amtDn;
        break;
    case 2: // شراء دولار — pay dinars, get dollars
        projD  += amtD;
        projDn -= amtDn;
        break;
    case 3: // بيع دولار — pay dollars, get dinars
        projD  -= amtD;
        projDn += amtDn;
        break;
    }

    m_projBalDollarLabel->setText(QString::number(projD,  'f', 2));
    m_projBalDinarLabel->setText(QString::number(projDn, 'f', 0));
}

void CashBoxWindow::saveTransaction()
{
    double rate        = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();
    double amtDollar   = m_amountDollarEdit->text().toDouble();
    double amtDinar    = m_amountDinarEdit->text().toDouble();
    double equivDinar  = m_equivDinarEdit->text().toDouble();
    double equivDollar = m_equivDollarEdit->text().toDouble();
    int    type        = m_typeCombo->currentIndex();
    QDate  date        = m_dateDay->date();
    date.setDate(m_dateYear->date().year(),
                 m_dateMonth->date().month(),
                 m_dateDay->date().day());

    QString mainAcct = m_mainAccountCombo->currentText();
    QString subAcct  = m_subAccountCombo->currentText();

    int id = Database::addCashTransaction(
        type, date, QTime::currentTime().toString("hh:mm AP"),
        m_organiserSpin->value(),
        m_docNoEdit->text(),
        mainAcct, subAcct,
        amtDollar, amtDinar,
        equivDinar, equivDollar,
        rate,
        m_cashboxCombo->currentText(),
        m_notesEdit->text());

    if (id > 0) {
        m_currentId = id;
        m_navLabel->setText(QString::number(id));
        // Refresh balance display
        refreshBalanceDisplay();
        QMessageBox::information(this,
            QString::fromUtf8("تم"),
            QString::fromUtf8("تم حفظ الحركة رقم: ") + QString::number(id));
    } else {
        QMessageBox::critical(this,
            QString::fromUtf8("خطأ"),
            QString::fromUtf8("فشل حفظ الحركة"));
    }
}

void CashBoxWindow::newTransaction()  { clearForm(); }

void CashBoxWindow::deleteTransaction()
{
    if (m_currentId < 0) return;
    if (QMessageBox::question(this, "",
            QString::fromUtf8("حذف الحركة رقم ") + QString::number(m_currentId) + "؟",
            QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) return;
    QSqlQuery q;
    q.prepare("DELETE FROM cash_transactions WHERE id=?");
    q.addBindValue(m_currentId);
    if (q.exec()) {
        clearForm();
        QMessageBox::information(this, "", QString::fromUtf8("تم الحذف"));
    }
}

void CashBoxWindow::searchTransaction() {}

void CashBoxWindow::printReceipt()
{
    if (m_currentId < 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("احفظ الحركة أولاً"));
        return;
    }
    QStringList lines;
    lines << "**" + Database::getCompanyName();
    lines << "---";
    lines << QString::fromUtf8("وصل رقم: ")  + QString::number(m_currentId);
    lines << QString::fromUtf8("النوع: ")     + m_typeCombo->currentText();
    lines << QString::fromUtf8("الحساب: ")    + m_mainAccountCombo->currentText()
                                              + " / " + m_subAccountCombo->currentText();
    lines << QString::fromUtf8("المبلغ $: ")  + m_amountDollarEdit->text();
    lines << QString::fromUtf8("المبلغ د: ")  + m_amountDinarEdit->text();
    lines << "---";
    lines << "**" + QString::fromUtf8("التوقيع: ____________");
    m_printer->printThermalReceipt(this, lines);
}

void CashBoxWindow::exportPDF()
{
    if (m_currentId < 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("احفظ الحركة أولاً"));
        return;
    }
    QString html = PrintManager::arabicHtmlHeader();
    html += QString("<div class='company'>%1</div>").arg(Database::getCompanyName());
    html += QString("<h2>%1 %2</h2>")
            .arg(QString::fromUtf8("وصل صندوق رقم:"))
            .arg(m_currentId);
    html += QString("<p>%1: %2</p>").arg(QString::fromUtf8("النوع")).arg(m_typeCombo->currentText());
    html += QString("<p>%1: %2 / %3</p>")
            .arg(QString::fromUtf8("الحساب"))
            .arg(m_mainAccountCombo->currentText())
            .arg(m_subAccountCombo->currentText());
    html += QString("<p>%1: %2 $</p>").arg(QString::fromUtf8("المبلغ بالدولار")).arg(m_amountDollarEdit->text());
    html += QString("<p>%1: %2</p>").arg(QString::fromUtf8("المبلغ بالدينار")).arg(m_amountDinarEdit->text());
    html += QString("<p>%1: %2</p>").arg(QString::fromUtf8("سعر الصرف")).arg(m_exchangeRateEdit->text());
    html += QString("<p>%1: %2</p>").arg(QString::fromUtf8("الملاحظات")).arg(m_notesEdit->text());
    html += "<div class='sig'><p>" + QString::fromUtf8("التوقيع: _______________") + "</p></div>";
    html += "</body></html>";
    m_printer->exportToPDF(this, html, QString::fromUtf8("وصل_صندوق"));
}

void CashBoxWindow::loadTransaction(int id)
{
    QSqlQuery q;
    q.prepare("SELECT type, date, time, operator_id, doc_no, main_account, sub_account, "
              "amount_dollar, amount_dinar, equiv_dinar, equiv_dollar, exchange_rate, "
              "direction, notes FROM cash_transactions WHERE id=?");
    q.addBindValue(id);
    q.exec();
    if (!q.next()) return;

    m_currentId = id;
    m_typeCombo->setCurrentIndex(q.value(0).toInt());
    m_docNoEdit->setText(q.value(4).toString());
    m_mainAccountCombo->setCurrentText(q.value(5).toString());
    m_subAccountCombo->setCurrentText(q.value(6).toString());
    m_amountDollarEdit->setText(q.value(7).toString());
    m_amountDinarEdit->setText(q.value(8).toString());
    m_equivDinarEdit->setText(q.value(9).toString());
    m_equivDollarEdit->setText(q.value(10).toString());
    m_exchangeRateEdit->setText(q.value(11).toString());
    m_cashboxCombo->setCurrentText(q.value(12).toString());
    m_notesEdit->setText(q.value(13).toString());
    m_navLabel->setText(QString::number(id));
}

void CashBoxWindow::clearForm()
{
    m_currentId = -1;
    m_amountDollarEdit->setText("0");
    m_amountDinarEdit->setText("0");
    m_equivDinarEdit->setText("0");
    m_equivDollarEdit->setText("0");
    m_docNoEdit->clear();
    m_notesEdit->clear();
    m_navLabel->setText("0");
    m_dateDay->setDate(QDate::currentDate());
    m_dateMonth->setDate(QDate::currentDate());
    m_dateYear->setDate(QDate::currentDate());
    m_typeCombo->setCurrentIndex(0);
    m_mainAccountCombo->setCurrentIndex(0);
    // Trigger sub-account populate
    onMainAccountChanged(0);
    refreshBalanceDisplay();
}

void CashBoxWindow::refreshBalanceDisplay()
{
    // Sum all cashbox transactions to get running dollar and dinar totals
    QSqlQuery q;
    q.exec("SELECT "
           "SUM(CASE WHEN type=1 OR type=2 THEN amount_dollar ELSE -amount_dollar END),"
           "SUM(CASE WHEN type=1 OR type=3 THEN amount_dinar  ELSE -amount_dinar  END) "
           "FROM cash_transactions");
    double balD = 0, balDn = 0;
    if (q.next()) {
        balD  = q.value(0).toDouble();
        balDn = q.value(1).toDouble();
    }
    m_curBalDollarLabel->setText(QString::number(balD,  'f', 2));
    m_curBalDinarLabel->setText(QString::number(balDn, 'f', 0));
    // Reset projected to current
    m_projBalDollarLabel->setText(m_curBalDollarLabel->text());
    m_projBalDinarLabel->setText(m_curBalDinarLabel->text());
}

void CashBoxWindow::navigateFirst() { loadTransaction(1); }
void CashBoxWindow::navigatePrev()  { if (m_currentId > 1) loadTransaction(m_currentId - 1); }
void CashBoxWindow::navigateNext()  { loadTransaction(m_currentId + 1); }
void CashBoxWindow::navigateLast()
{
    QSqlQuery q;
    q.exec("SELECT MAX(id) FROM cash_transactions");
    if (q.next() && !q.value(0).isNull()) loadTransaction(q.value(0).toInt());
}
