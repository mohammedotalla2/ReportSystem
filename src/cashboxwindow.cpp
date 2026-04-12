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

CashBoxWindow::CashBoxWindow(QWidget *parent) : QDialog(parent), m_currentId(-1)
{
    setWindowTitle(QString::fromUtf8("الصندوق"));
    setLayoutDirection(Qt::RightToLeft);
    resize(1150, 720);
    m_printer = new PrintManager(this);
    setupUI();
    applyStyles();
    clearForm();
}

void CashBoxWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->setSpacing(6);

    // ===== TITLE =====
    QLabel *titleLabel = new QLabel(QString::fromUtf8("أدامة حركات الصندوق"));
    titleLabel->setObjectName("titleLabel");
    titleLabel->setFont(QFont("Tahoma", 16, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // ===== HEADER GRID =====
    // In RTL mode, QGridLayout column 0 is the VISUAL RIGHT edge.
    // So: label at col N, widget at col N+1 (label RIGHT of widget).
    //
    // Layout (visual left → right in RTL = high col → low col):
    //   col 0 = right edge (labels start here)
    //   col 1 = widget immediately left of label
    //   …
    //
    // We use a 12-column grid. Column layout per row:
    //   RIGHT side:   [label col0] [widget col1]  [label col2] [widget col3]
    //   LEFT side:    [label col4] [widget col5]  [label col6] [widget col7]
    //   (stretch col 8–11 fills remaining space)

    QWidget *headerWidget = new QWidget;
    headerWidget->setObjectName("headerWidget");
    QGridLayout *grid = new QGridLayout(headerWidget);
    grid->setSpacing(8);
    grid->setContentsMargins(10, 8, 10, 8);

    // Make the rightmost label columns fixed-width, stretch center columns
    grid->setColumnStretch(3, 1);   // widget area stretches
    grid->setColumnStretch(7, 1);   // widget area stretches

    // Helper: right-aligned label
    auto lbl = [](const QString &t) -> QLabel* {
        QLabel *l = new QLabel(t);
        l->setFont(QFont("Tahoma", 9));
        l->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        l->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        return l;
    };

    // Helper: read-only styled field
    auto roField = [](const QString &obj = QString()) -> QLineEdit* {
        QLineEdit *e = new QLineEdit;
        e->setReadOnly(true);
        if (!obj.isEmpty()) e->setObjectName(obj);
        return e;
    };

    // ---------------------------------------------------------------
    // ROW 0:  [نوع الحركة | combo] ... [رقم السند | edit] ... [المنظم | spin] [الوقت | lbl]
    // Visual right→left: نوع الحركة(0) combo(1,2) | رقم السند(3) edit(4) | المنظم(5) spin(6) | الوقت(7) lbl(8)
    // ---------------------------------------------------------------
    m_typeCombo = new QComboBox;
    m_typeCombo->addItems({
        QString::fromUtf8("صرف مصاريف عامة"),
        QString::fromUtf8("صرف ذمم عملاء"),
        QString::fromUtf8("قبض من عملاء"),
        QString::fromUtf8("إيداع في الصندوق"),
        QString::fromUtf8("سحب من الصندوق")
    });

    m_docNoEdit = new QLineEdit;

    m_organiserSpin = new QSpinBox;
    m_organiserSpin->setValue(1);
    m_organiserSpin->setFixedWidth(65);
    m_organiserSpin->setObjectName("organiserSpin");

    m_timeLabel = new QLabel(QTime::currentTime().toString("hh:mm AP"));
    m_timeLabel->setObjectName("timeLbl");
    m_timeLabel->setMinimumWidth(90);

    // col:  0=label  1-2=combo  3=label  4=edit  5=label  6=spin  7=label  8=timeLabel
    grid->addWidget(lbl(QString::fromUtf8("نوع الحركة")), 0, 0);
    grid->addWidget(m_typeCombo,   0, 1, 1, 2);
    grid->addWidget(lbl(QString::fromUtf8("رقم السند")), 0, 3);
    grid->addWidget(m_docNoEdit,   0, 4);
    grid->addWidget(lbl(QString::fromUtf8("المنظم")),    0, 5);
    grid->addWidget(m_organiserSpin, 0, 6);
    grid->addWidget(lbl(QString::fromUtf8("الوقت")),     0, 7);
    grid->addWidget(m_timeLabel,   0, 8);

    // ---------------------------------------------------------------
    // ROW 1: التاريخ  [day] [month] [year]
    // ---------------------------------------------------------------
    m_dateDay   = new QDateEdit(QDate::currentDate()); m_dateDay->setDisplayFormat("d");
    m_dateMonth = new QDateEdit(QDate::currentDate()); m_dateMonth->setDisplayFormat("M");
    m_dateYear  = new QDateEdit(QDate::currentDate()); m_dateYear->setDisplayFormat("yyyy");
    m_dateDay->setFixedWidth(50);
    m_dateMonth->setFixedWidth(50);
    m_dateYear->setFixedWidth(75);

    grid->addWidget(lbl(QString::fromUtf8("التاريخ")), 1, 0);
    grid->addWidget(m_dateDay,   1, 1);
    grid->addWidget(m_dateMonth, 1, 2);
    grid->addWidget(m_dateYear,  1, 3);

    // ---------------------------------------------------------------
    // ROW 2: الحساب الرئيسي [combo]   |   الحساب الفرعي [combo]
    // ---------------------------------------------------------------
    m_mainAccountCombo = new QComboBox;
    m_mainAccountCombo->setEditable(true);
    m_mainAccountCombo->addItems({
        QString::fromUtf8("مصاريف عامة"),
        QString::fromUtf8("ذمم عملاء"),
        QString::fromUtf8("إيجار"),
        QString::fromUtf8("رواتب"),
        QString::fromUtf8("مستلزمات"),
        QString::fromUtf8("أخرى")
    });

    m_subAccountCombo = new QComboBox;
    m_subAccountCombo->setEditable(true);

    grid->addWidget(lbl(QString::fromUtf8("الحساب الرئيسي")), 2, 0);
    grid->addWidget(m_mainAccountCombo, 2, 1, 1, 2);
    grid->addWidget(lbl(QString::fromUtf8("الحساب الفرعي")),  2, 3);
    grid->addWidget(m_subAccountCombo,  2, 4, 1, 3);

    // ---------------------------------------------------------------
    // ROW 3: ر.سابق للزبون $ [field]   |   الرصيد المتبقي $ [field]
    // ---------------------------------------------------------------
    m_prevCustDollarEdit = roField("yellowField");
    m_prevCustDollarEdit->setText("0");
    m_prevBalDollarEdit  = roField("greyField");
    m_prevBalDollarEdit->setText("0.00");

    grid->addWidget(lbl(QString::fromUtf8("ر.سابق للزبون $")),    3, 0);
    grid->addWidget(m_prevCustDollarEdit, 3, 1);
    grid->addWidget(lbl(QString::fromUtf8("الرصيد المتبقي $")),   3, 3);
    grid->addWidget(m_prevBalDollarEdit,  3, 4);

    // ---------------------------------------------------------------
    // ROW 4: ر.سابق دينار [field]   |   الرصيد المتبقي دينار [field]
    // ---------------------------------------------------------------
    m_prevCustDinarEdit = roField("greyField");
    m_prevCustDinarEdit->setText("0");
    m_prevBalDinarEdit  = roField("greyField");
    m_prevBalDinarEdit->setText("0");

    grid->addWidget(lbl(QString::fromUtf8("ر.سابق دينار")),          4, 0);
    grid->addWidget(m_prevCustDinarEdit, 4, 1);
    grid->addWidget(lbl(QString::fromUtf8("الرصيد المتبقي دينار")), 4, 3);
    grid->addWidget(m_prevBalDinarEdit,  4, 4);

    // ---------------------------------------------------------------
    // ROW 5: المبلغ بالدولار [field]   |   مايعادل بالدينار [field]
    // ---------------------------------------------------------------
    m_amountDollarEdit = new QLineEdit("0");
    m_equivDinarEdit   = new QLineEdit("0");
    m_equivDinarEdit->setReadOnly(true);

    grid->addWidget(lbl(QString::fromUtf8("المبلغ بالدولار")),   5, 0);
    grid->addWidget(m_amountDollarEdit, 5, 1);
    grid->addWidget(lbl(QString::fromUtf8("مايعادل بالدينار")), 5, 3);
    grid->addWidget(m_equivDinarEdit,   5, 4);

    // ---------------------------------------------------------------
    // ROW 6: المبلغ بالدينار [field]  |  مايعادل بالدولار [field]  | [مايعادل btn]
    // ---------------------------------------------------------------
    m_amountDinarEdit = new QLineEdit("0");
    m_equivDollarEdit = new QLineEdit("0");
    m_equivDollarEdit->setReadOnly(true);

    m_equivBtn = new QPushButton(QString::fromUtf8("مايعادل"));
    m_equivBtn->setObjectName("equivBtn");
    m_equivBtn->setFixedWidth(80);

    grid->addWidget(lbl(QString::fromUtf8("المبلغ بالدينار")),   6, 0);
    grid->addWidget(m_amountDinarEdit, 6, 1);
    grid->addWidget(lbl(QString::fromUtf8("مايعادل بالدولار")), 6, 3);
    grid->addWidget(m_equivDollarEdit, 6, 4);
    grid->addWidget(m_equivBtn,        6, 6);

    // ---------------------------------------------------------------
    // ROW 7: جهة الصرف [combo]   |   سعر صرف $ [field]
    // ---------------------------------------------------------------
    m_cashboxCombo = new QComboBox;
    m_cashboxCombo->setEditable(true);
    m_cashboxCombo->addItems({
        QString::fromUtf8("الصندوق الرئيسي"),
        QString::fromUtf8("صندوق 2"),
        QString::fromUtf8("البنك")
    });

    m_exchangeRateEdit = new QLineEdit(
        QString::number(Database::getExchangeRate(), 'f', 0));
    m_exchangeRateEdit->setObjectName("yellowField");
    m_exchangeRateEdit->setFixedWidth(100);

    grid->addWidget(lbl(QString::fromUtf8("جهة الصرف")),  7, 0);
    grid->addWidget(m_cashboxCombo,    7, 1, 1, 2);
    grid->addWidget(lbl(QString::fromUtf8("سعر صرف $")), 7, 3);
    grid->addWidget(m_exchangeRateEdit, 7, 4);

    // ---------------------------------------------------------------
    // ROW 8: قائمة الشراء [combo]
    // ---------------------------------------------------------------
    m_purchaseListCombo = new QComboBox;
    m_purchaseListCombo->setEditable(true);

    grid->addWidget(lbl(QString::fromUtf8("قائمة الشراء")), 8, 0);
    grid->addWidget(m_purchaseListCombo, 8, 1, 1, 2);

    // ---------------------------------------------------------------
    // ROW 9: الملاحظات [full-width edit]
    // ---------------------------------------------------------------
    m_notesEdit = new QLineEdit;

    grid->addWidget(lbl(QString::fromUtf8("الملاحظات")), 9, 0);
    grid->addWidget(m_notesEdit, 9, 1, 1, 8);   // spans to col 8

    // ---------------------------------------------------------------
    // ROW 10: المنظم [full-width edit]
    // ---------------------------------------------------------------
    m_organiserEdit = new QLineEdit;

    grid->addWidget(lbl(QString::fromUtf8("المنظم")), 10, 0);
    grid->addWidget(m_organiserEdit, 10, 1, 1, 6);

    mainLayout->addWidget(headerWidget, 1);

    // ===== BOTTOM TOOLBAR =====
    QWidget *toolbar = new QWidget;
    toolbar->setObjectName("toolbar");
    QHBoxLayout *toolLayout = new QHBoxLayout(toolbar);
    toolLayout->setContentsMargins(6, 6, 6, 6);
    toolLayout->setSpacing(4);

    auto makeBtn = [](const QString &txt,
                      const QString &tip = QString(),
                      const QString &obj = "toolBtn") -> QPushButton* {
        QPushButton *b = new QPushButton(txt);
        b->setFixedSize(42, 38);
        b->setToolTip(tip);
        b->setObjectName(obj);
        b->setFont(QFont("Tahoma", 13));
        return b;
    };

    // Left group of buttons (visual right in RTL toolbar)
    m_saveBtn   = makeBtn("💾", QString::fromUtf8("حفظ"));
    m_editBtn   = makeBtn("✎",  QString::fromUtf8("تعديل"));
    m_undoBtn   = makeBtn("↩",  QString::fromUtf8("تراجع"));
    m_searchBtn = makeBtn("🔍", QString::fromUtf8("بحث"));
    m_deleteBtn = makeBtn("✖",  QString::fromUtf8("حذف"));
    m_printBtn  = makeBtn("🖨", QString::fromUtf8("طباعة"));

    m_pdfBtn = new QPushButton("PDF");
    m_pdfBtn->setFixedHeight(38);
    m_pdfBtn->setObjectName("pdfBtn");
    m_pdfBtn->setFont(QFont("Tahoma", 10, QFont::Bold));
    m_pdfBtn->setMinimumWidth(50);

    // Navigation labels
    m_navLabel   = new QLabel("0");
    m_navLabel->setObjectName("navLbl");
    m_navLabel->setFixedWidth(55);
    m_navLabel->setAlignment(Qt::AlignCenter);

    m_totalLabel = new QLabel("1");
    m_totalLabel->setFixedWidth(55);
    m_totalLabel->setAlignment(Qt::AlignCenter);

    // Navigation buttons
    QPushButton *firstBtn = makeBtn("⏮");
    QPushButton *prevBtn  = makeBtn("◀");
    QPushButton *nextBtn  = makeBtn("▶");
    QPushButton *lastBtn  = makeBtn("⏭");

    // Receive / Pay buttons
    m_receiveBtn = makeBtn("🤲", QString::fromUtf8("قبض من عميل"), "recvBtn");
    m_payBtn     = makeBtn("🤝", QString::fromUtf8("صرف لعميل"),  "payBtn");

    // Toolbar layout (RTL: items added left-to-right appear right-to-left visually)
    toolLayout->addWidget(m_saveBtn);
    toolLayout->addWidget(m_editBtn);
    toolLayout->addWidget(m_undoBtn);
    toolLayout->addWidget(m_searchBtn);
    toolLayout->addWidget(m_deleteBtn);
    toolLayout->addWidget(m_printBtn);
    toolLayout->addWidget(m_pdfBtn);
    toolLayout->addStretch();
    toolLayout->addWidget(m_navLabel);
    toolLayout->addWidget(new QLabel(QString::fromUtf8("من")));
    toolLayout->addWidget(m_totalLabel);
    toolLayout->addStretch();
    toolLayout->addWidget(m_receiveBtn);
    toolLayout->addWidget(m_payBtn);
    toolLayout->addWidget(firstBtn);
    toolLayout->addWidget(prevBtn);
    toolLayout->addWidget(nextBtn);
    toolLayout->addWidget(lastBtn);

    mainLayout->addWidget(toolbar);

    // ===== SIGNAL CONNECTIONS =====
    connect(m_saveBtn,   &QPushButton::clicked, this, &CashBoxWindow::saveTransaction);
    connect(m_deleteBtn, &QPushButton::clicked, this, &CashBoxWindow::deleteTransaction);
    connect(m_searchBtn, &QPushButton::clicked, this, &CashBoxWindow::searchTransaction);
    connect(m_printBtn,  &QPushButton::clicked, this, &CashBoxWindow::printReceipt);
    connect(m_pdfBtn,    &QPushButton::clicked, this, &CashBoxWindow::exportPDF);
    connect(firstBtn,    &QPushButton::clicked, this, &CashBoxWindow::navigateFirst);
    connect(prevBtn,     &QPushButton::clicked, this, &CashBoxWindow::navigatePrev);
    connect(nextBtn,     &QPushButton::clicked, this, &CashBoxWindow::navigateNext);
    connect(lastBtn,     &QPushButton::clicked, this, &CashBoxWindow::navigateLast);
    connect(m_equivBtn,  &QPushButton::clicked, this, &CashBoxWindow::calculateEquivalent);
    connect(m_receiveBtn, &QPushButton::clicked, this, &CashBoxWindow::receiveFromCustomer);
    connect(m_payBtn,     &QPushButton::clicked, this, &CashBoxWindow::payToCustomer);

    connect(m_amountDollarEdit, &QLineEdit::textChanged,
            this, &CashBoxWindow::calculateEquivalent);
    connect(m_amountDinarEdit,  &QLineEdit::textChanged,
            this, &CashBoxWindow::calculateEquivalent);
    connect(m_typeCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CashBoxWindow::onTypeChanged);
    connect(m_mainAccountCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CashBoxWindow::onMainAccountChanged);
}

void CashBoxWindow::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #c0c4cc; }

        #titleLabel {
            color: #FFD700;
            background: #1a1a4a;
            font-size: 16pt;
            font-weight: bold;
            padding: 8px;
            border-radius: 4px;
        }

        #headerWidget {
            background: #b8bcc8;
            border: 1px solid #8890a0;
            border-radius: 4px;
        }

        QLineEdit {
            background: white;
            border: 1px solid #99aacc;
            border-radius: 2px;
            padding: 2px 6px;
            font-family: Tahoma;
            font-size: 10pt;
            min-height: 24px;
        }

        #yellowField {
            background: #ffffc0;
            border: 1px solid #aaaa60;
            font-weight: bold;
            color: #003300;
        }

        #greyField {
            background: #e8e8e8;
            color: #444444;
        }

        #organiserSpin {
            background: #cc88ff;
            font-weight: bold;
            border: 1px solid #9944bb;
            border-radius: 2px;
            min-height: 24px;
        }

        #timeLbl {
            background: #ffffc0;
            color: #003366;
            font-weight: bold;
            padding: 4px 10px;
            border: 1px solid #aaaa60;
            border-radius: 2px;
            font-size: 11pt;
        }

        #equivBtn {
            background: #c8d8e8;
            border: 1px solid #88aacc;
            border-radius: 3px;
            color: #003366;
            font-size: 10pt;
            font-weight: bold;
            min-height: 26px;
        }
        #equivBtn:hover { background: #b0c8e0; }

        QComboBox {
            background: #ffffc0;
            border: 1px solid #99aacc;
            border-radius: 2px;
            min-height: 24px;
            padding: 1px 4px;
            font-family: Tahoma;
            font-size: 10pt;
        }
        QComboBox:editable { background: white; }
        QComboBox::drop-down { border: none; }

        QLabel {
            font-family: Tahoma;
            font-size: 9pt;
            color: #003366;
        }

        QDateEdit {
            background: white;
            border: 1px solid #99aacc;
            border-radius: 2px;
            min-height: 24px;
            font-family: Tahoma;
        }

        QSpinBox {
            background: white;
            border: 1px solid #99aacc;
            border-radius: 2px;
            min-height: 24px;
        }

        #toolbar {
            background: #1a1a4a;
            border-top: 2px solid #0000aa;
            border-radius: 4px;
        }

        #toolBtn {
            background: #3a3a7a;
            border: 1px solid #5555aa;
            color: white;
            border-radius: 3px;
            font-size: 13pt;
        }
        #toolBtn:hover { background: #5555aa; }

        #pdfBtn {
            background: #cc2200;
            color: white;
            font-weight: bold;
            border-radius: 3px;
            border: 1px solid #880000;
            padding: 0 8px;
        }
        #pdfBtn:hover { background: #ee2200; }

        #navLbl {
            background: #FFD700;
            color: #003366;
            font-weight: bold;
            border: 1px solid #aa8800;
            border-radius: 2px;
            font-size: 11pt;
        }

        #recvBtn {
            background: #33aa55;
            color: white;
            border-radius: 3px;
            border: 1px solid #228844;
            font-size: 13pt;
        }
        #recvBtn:hover { background: #44bb66; }

        #payBtn {
            background: #aa3322;
            color: white;
            border-radius: 3px;
            border: 1px solid #882211;
            font-size: 13pt;
        }
        #payBtn:hover { background: #bb4433; }
    )");
}

// ===== SLOT IMPLEMENTATIONS =====

void CashBoxWindow::calculateEquivalent()
{
    double rate = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();
    double dollar = m_amountDollarEdit->text().toDouble();
    double dinar  = m_amountDinarEdit->text().toDouble();
    m_equivDinarEdit->setText(QString::number(dollar * rate, 'f', 0));
    m_equivDollarEdit->setText(QString::number(dinar  / rate, 'f', 2));
}

void CashBoxWindow::onTypeChanged()
{
    m_subAccountCombo->clear();
    int type = m_typeCombo->currentIndex();
    if (type == 0) {   // مصاريف عامة
        m_subAccountCombo->addItems({
            QString::fromUtf8("إيجار المحل"),
            QString::fromUtf8("الرواتب"),
            QString::fromUtf8("الكهرباء"),
            QString::fromUtf8("الإنترنت"),
            QString::fromUtf8("مستلزمات"),
            QString::fromUtf8("العمال"),
            QString::fromUtf8("المولدة"),
            QString::fromUtf8("أخرى")
        });
    } else if (type == 1 || type == 2) {   // ذمم / قبض عملاء
        QSqlQuery q = Database::getCustomers();
        while (q.next())
            m_subAccountCombo->addItem(q.value(1).toString(), q.value(0));
    }
}

void CashBoxWindow::onMainAccountChanged(int) {}

void CashBoxWindow::saveTransaction()
{
    int    type        = m_typeCombo->currentIndex();
    QDate  date        = m_dateDay->date();
    double rate        = m_exchangeRateEdit->text().toDouble();
    double amtDollar   = m_amountDollarEdit->text().toDouble();
    double amtDinar    = m_amountDinarEdit->text().toDouble();
    double equivDinar  = m_equivDinarEdit->text().toDouble();
    double equivDollar = m_equivDollarEdit->text().toDouble();

    int id = Database::addCashTransaction(
        type, date, QTime::currentTime().toString("hh:mm AP"),
        m_organiserSpin->value(), m_docNoEdit->text(),
        m_mainAccountCombo->currentText(),
        m_subAccountCombo->currentText(),
        amtDollar, amtDinar, equivDinar, equivDollar, rate,
        m_cashboxCombo->currentText(),
        m_notesEdit->text()
    );

    if (id > 0) {
        m_currentId = id;
        m_navLabel->setText(QString::number(id));
        QMessageBox::information(this,
            QString::fromUtf8("تم"),
            QString::fromUtf8("تم حفظ الحركة رقم: ") + QString::number(id));
    } else {
        QMessageBox::critical(this,
            QString::fromUtf8("خطأ"),
            QString::fromUtf8("فشل الحفظ"));
    }
}

void CashBoxWindow::newTransaction()    { clearForm(); }

void CashBoxWindow::deleteTransaction()
{
    if (m_currentId < 0) return;
    QSqlQuery q;
    q.prepare("DELETE FROM cash_transactions WHERE id=?");
    q.addBindValue(m_currentId);
    if (q.exec()) {
        QMessageBox::information(this,
            QString::fromUtf8("تم"),
            QString::fromUtf8("تم الحذف"));
        clearForm();
    }
}

void CashBoxWindow::searchTransaction() {}

void CashBoxWindow::printReceipt()
{
    if (m_currentId < 0) return;
    QStringList lines;
    lines << "**" + Database::getCompanyName();
    lines << "---";
    lines << QString::fromUtf8("وصل رقم: ")   + QString::number(m_currentId);
    lines << QString::fromUtf8("النوع: ")      + m_typeCombo->currentText();
    lines << QString::fromUtf8("الحساب: ")     + m_mainAccountCombo->currentText();
    lines << QString::fromUtf8("المبلغ $: ")   + m_amountDollarEdit->text();
    lines << QString::fromUtf8("المبلغ د: ")   + m_amountDinarEdit->text();
    lines << "---";
    lines << "**" + QString::fromUtf8("التوقيع: ____________");
    m_printer->printThermalReceipt(this, lines);
}

void CashBoxWindow::exportPDF()
{
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
    html += QString("<p>%1: %2 %3</p>").arg(QString::fromUtf8("المبلغ بالدينار")).arg(m_amountDinarEdit->text()).arg(QString::fromUtf8("د"));
    html += QString("<p>%1: %2</p>").arg(QString::fromUtf8("سعر الصرف")).arg(m_exchangeRateEdit->text());
    html += QString("<p>%1: %2</p>").arg(QString::fromUtf8("الملاحظات")).arg(m_notesEdit->text());
    html += "<div class='sig'><p>" + QString::fromUtf8("التوقيع: _______________") + "</p></div>";
    html += "</body></html>";
    m_printer->exportToPDF(this, html, QString::fromUtf8("وصل_صندوق"));
}

void CashBoxWindow::loadTransaction(int id)
{
    QSqlQuery q;
    q.prepare(
        "SELECT type, date, time, operator_id, doc_no, main_account, sub_account, "
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
    m_organiserEdit->clear();
    m_navLabel->setText("0");

    double balDollar = Database::getCashBoxBalance("$");
    double balDinar  = Database::getCashBoxBalance(
        QString::fromUtf8("دينار"));
    m_prevBalDollarEdit->setText(QString::number(balDollar, 'f', 2));
    m_prevBalDinarEdit->setText(QString::number(balDinar,  'f', 0));
}

void CashBoxWindow::navigateFirst() { loadTransaction(1); }
void CashBoxWindow::navigatePrev()  { if (m_currentId > 1) loadTransaction(m_currentId - 1); }
void CashBoxWindow::navigateNext()  { loadTransaction(m_currentId + 1); }
void CashBoxWindow::navigateLast()
{
    QSqlQuery q;
    q.exec("SELECT MAX(id) FROM cash_transactions");
    if (q.next()) loadTransaction(q.value(0).toInt());
}

void CashBoxWindow::receiveFromCustomer() { m_typeCombo->setCurrentIndex(2); }
void CashBoxWindow::payToCustomer()       { m_typeCombo->setCurrentIndex(1); }
