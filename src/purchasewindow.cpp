#include "purchasewindow.h"
#include "database.h"
#include "arabichelper.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <QSqlQuery>
#include <QFont>
#include <QFrame>

PurchaseWindow::PurchaseWindow(QWidget *parent)
    : QDialog(parent), m_currentInvoiceId(-1), m_blockBarcodeSignal(false)
{
    setWindowTitle(QString::fromUtf8("المشتريات"));
    setLayoutDirection(Qt::RightToLeft);
    resize(1350, 800);
    m_printer = new PrintManager(this);
    setupUI(); applyStyles();
    loadSuppliers(); loadProducts(); clearForm();
    onCurrencyChanged();   // initialise all currency-symbol labels for default (دينار)
}

void PurchaseWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(4);

    // ===== HEADER =====
    QWidget *headerWidget = new QWidget;
    headerWidget->setObjectName("headerWidget");
    QGridLayout *headerGrid = new QGridLayout(headerWidget);
    headerGrid->setSpacing(4);
    headerGrid->setContentsMargins(6, 4, 6, 4);

    // In RTL QGridLayout: col 0 = rightmost, col 12 = leftmost
    // For Arabic: field at higher col (left), label at lower col (right of field)
    auto lbl = [](const QString &t) {
        QLabel *l = new QLabel(t);
        l->setFont(QFont("Tahoma", 9));
        l->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        return l;
    };

    // ── Row 0 ──
    m_invoiceNoSpin = new QSpinBox;
    m_invoiceNoSpin->setRange(1, 9999999);
    m_invoiceNoSpin->setObjectName("invoiceNo");
    m_invoiceNoSpin->setFixedWidth(70);

    m_dateEdit = new QDateEdit(QDate::currentDate());
    m_dateEdit->setDisplayFormat("d");
    m_dateEdit->setFixedWidth(38);
    QDateEdit *monthEdit = new QDateEdit(QDate::currentDate());
    monthEdit->setDisplayFormat("M");
    monthEdit->setFixedWidth(38);
    QDateEdit *yearEdit = new QDateEdit(QDate::currentDate());
    yearEdit->setDisplayFormat("yyyy");
    yearEdit->setFixedWidth(56);

    m_purchaseTypeCombo = new QComboBox;
    m_purchaseTypeCombo->addItems({
        QString::fromUtf8("محلي"),
        QString::fromUtf8("استيراد"),
        QString::fromUtf8("مرتجع")
    });

    m_supplierCombo = new QComboBox;
    m_supplierCombo->setEditable(true);
    m_supplierCombo->setMinimumWidth(180);

    m_payTypeCombo = new QComboBox;
    m_payTypeCombo->addItems({
        QString::fromUtf8("نقدي"),
        QString::fromUtf8("آجل")
    });

    // Row 0: field at higher col, label at lower (label is to the RIGHT of field in RTL)
    // Visual left→right: [invoiceNo(12)][lbl(11)][day(10)][mon(9)][yr(8)][lbl التاريخ(7)][purchaseType(6)][lbl(5)]...[supplier(3)][lbl(2)][payType(1)][lbl(0)]
    headerGrid->addWidget(m_invoiceNoSpin,          0, 12);
    headerGrid->addWidget(lbl(QString::fromUtf8("رقم القائمة")), 0, 11);
    headerGrid->addWidget(m_dateEdit,               0, 10);
    headerGrid->addWidget(monthEdit,                0,  9);
    headerGrid->addWidget(yearEdit,                 0,  8);
    headerGrid->addWidget(lbl(QString::fromUtf8("التاريخ")),     0,  7);
    headerGrid->addWidget(m_purchaseTypeCombo,      0,  6);
    headerGrid->addWidget(lbl(QString::fromUtf8("نوع الشراء")),  0,  5);
    // col 4: spacer
    headerGrid->addWidget(m_supplierCombo,          0,  3);
    headerGrid->addWidget(lbl(QString::fromUtf8("اسم المجهز")),  0,  2);
    headerGrid->addWidget(m_payTypeCombo,           0,  1);
    headerGrid->addWidget(lbl(QString::fromUtf8("نوع الدفع")),   0,  0);

    // ── Row 1 ──
    m_currencyCombo = new QComboBox;
    m_currencyCombo->addItems({QString::fromUtf8("دينار"), "$"});
    m_currencyCombo->setFixedWidth(70);

    m_invoiceRefEdit = new QLineEdit;
    m_invoiceRefEdit->setPlaceholderText(QString::fromUtf8("سند الصرف"));

    m_exchangeRateEdit = new QLineEdit(QString::number(Database::getExchangeRate(), 'f', 0));
    m_exchangeRateEdit->setObjectName("yellowField");
    m_exchangeRateEdit->setFixedWidth(80);

    // Row 1: same rule – field at higher col, label to its right (lower col)
    headerGrid->addWidget(m_currencyCombo,          1, 12);
    headerGrid->addWidget(lbl(QString::fromUtf8("العملة")),       1, 11);
    headerGrid->addWidget(m_invoiceRefEdit,         1,  9);
    headerGrid->addWidget(lbl(QString::fromUtf8("سند الصرف")),   1,  8);
    headerGrid->addWidget(m_exchangeRateEdit,       1,  6);
    headerGrid->addWidget(lbl(QString::fromUtf8("سعر صرف $")),   1,  5);

    // ── Row 2: Notes ──
    m_notesEdit = new QLineEdit;
    m_notesEdit->setPlaceholderText(QString::fromUtf8("الملاحظات"));
    headerGrid->addWidget(m_notesEdit, 2, 0, 1, 13);

    mainLayout->addWidget(headerWidget);

    // ===== TABLE AREA =====
    QWidget *tableArea = new QWidget;
    tableArea->setObjectName("tableArea");
    QVBoxLayout *tableLayout = new QVBoxLayout(tableArea);
    tableLayout->setContentsMargins(2, 2, 2, 2);
    tableLayout->setSpacing(2);

    // ── Item entry row (HBoxLayout, RTL: first added = rightmost) ──
    // Visual right→left: barcode | product | qty | cost | wholesale$ | retail$ | total$ | totalDinar | ✔ ✎ ✖ | stock
    QHBoxLayout *itemRow = new QHBoxLayout;
    itemRow->setSpacing(3);

    m_barcodeCombo = new QComboBox;
    m_barcodeCombo->setEditable(true);
    m_barcodeCombo->setPlaceholderText(QString::fromUtf8("الباركود"));
    m_barcodeCombo->setFixedWidth(110);

    m_productCombo = new QComboBox;
    m_productCombo->setEditable(true);
    m_productCombo->setPlaceholderText(QString::fromUtf8("اسم المادة"));
    m_productCombo->setMinimumWidth(180);

    m_qtyEdit = new QLineEdit("0");
    m_qtyEdit->setFixedWidth(55);

    m_costPriceEdit = new QLineEdit("0");
    m_costPriceEdit->setFixedWidth(75);

    m_wholesaleDollarEdit = new QLineEdit("0");
    m_wholesaleDollarEdit->setFixedWidth(75);

    m_retailDollarEdit = new QLineEdit("0");
    m_retailDollarEdit->setFixedWidth(75);

    // Line totals (shown before adding to table)
    m_totalDollarEdit = new QLineEdit("0");
    m_totalDollarEdit->setReadOnly(true);
    m_totalDollarEdit->setFixedWidth(75);
    m_totalDollarEdit->setObjectName("yellowField");

    m_totalDinarEdit = new QLineEdit("0");
    m_totalDinarEdit->setReadOnly(true);
    m_totalDinarEdit->setFixedWidth(85);
    m_totalDinarEdit->setObjectName("yellowField");

    m_stockLabel = new QLabel(QString::fromUtf8("رصيد: 0"));
    m_stockLabel->setObjectName("stockLbl");

    QPushButton *addBtn = new QPushButton(QString::fromUtf8("✔"));
    addBtn->setFixedSize(36, 30); addBtn->setObjectName("addBtn");
    QPushButton *editBtn = new QPushButton(QString::fromUtf8("✎"));
    editBtn->setFixedSize(36, 30);
    QPushButton *delBtn = new QPushButton(QString::fromUtf8("✖"));
    delBtn->setFixedSize(36, 30); delBtn->setObjectName("delBtn");

    // Currency-symbol labels stored as members so onCurrencyChanged can update them
    m_costCurrLbl     = new QLabel(QString::fromUtf8("كلفة $"));
    m_wsCurrLbl       = new QLabel(QString::fromUtf8("جملة $"));
    m_retCurrLbl      = new QLabel(QString::fromUtf8("مفرد $"));
    m_lineTotCurrLbl  = new QLabel("$");

    // Add in order: rightmost first, leftmost last (RTL HBox)
    itemRow->addWidget(m_barcodeCombo);
    itemRow->addWidget(m_productCombo);
    itemRow->addWidget(lbl(QString::fromUtf8("الكمية")));
    itemRow->addWidget(m_qtyEdit);
    itemRow->addWidget(m_costCurrLbl);
    itemRow->addWidget(m_costPriceEdit);
    itemRow->addWidget(m_wsCurrLbl);
    itemRow->addWidget(m_wholesaleDollarEdit);
    itemRow->addWidget(m_retCurrLbl);
    itemRow->addWidget(m_retailDollarEdit);
    itemRow->addWidget(m_lineTotCurrLbl);
    itemRow->addWidget(m_totalDollarEdit);
    itemRow->addWidget(lbl(QString::fromUtf8("دينار")));
    itemRow->addWidget(m_totalDinarEdit);
    itemRow->addWidget(addBtn);
    itemRow->addWidget(editBtn);
    itemRow->addWidget(delBtn);
    itemRow->addWidget(m_stockLabel);

    tableLayout->addLayout(itemRow);

    // ── Items table (like saleswindow) ──
    // Cols: ت | باركود | رمز | اسم(stretch) | كمية | كلفة$ | مبلغ$ | مبلغ د | جملة$ | مفرد$ | [hidden prodId]
    m_itemsTable = new QTableWidget(0, 11);
    m_itemsTable->setHorizontalHeaderLabels({
        QString::fromUtf8("ت"),
        QString::fromUtf8("الباركود"),
        QString::fromUtf8("رمز المادة"),
        QString::fromUtf8("اسم المـادة"),
        QString::fromUtf8("الكمية"),
        QString::fromUtf8("الكلفة $"),
        QString::fromUtf8("المبلغ $"),
        QString::fromUtf8("المبلغ دينار"),
        QString::fromUtf8("سعر جملة $"),
        QString::fromUtf8("سعر مفرد $"),
        "ID"
    });
    m_itemsTable->setColumnHidden(10, true);
    m_itemsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_itemsTable->setColumnWidth(0, 35);
    m_itemsTable->setObjectName("itemsTable");
    m_itemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_itemsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_itemsTable->setMinimumHeight(280);

    tableLayout->addWidget(m_itemsTable);

    // ── Summary row ──
    QHBoxLayout *summRow = new QHBoxLayout;
    summRow->setSpacing(4);

    m_supplierBalLabel      = new QLabel(QString::fromUtf8("رصيد المجهز $: 0"));
    m_supplierBalLabel->setObjectName("balLbl");
    m_supplierBalDinarLabel = new QLabel(QString::fromUtf8("0"));
    m_supplierBalDinarLabel->setObjectName("balLbl");
    m_lastPurchasePriceLabel = new QLabel("0");

    m_paidDollarEdit = new QLineEdit("0");
    m_paidDollarEdit->setFixedWidth(90);
    m_paidDollarEdit->setObjectName("greenField");
    // m_paidDinarEdit kept for accounting but not shown separately (label changes with currency)
    m_paidDinarEdit = new QLineEdit("0");   // hidden accounting helper

    m_grandTotalDollarEdit = new QLineEdit("0");
    m_grandTotalDollarEdit->setReadOnly(true);
    m_grandTotalDollarEdit->setObjectName("yellowField");
    m_grandTotalDollarEdit->setFixedWidth(90);
    m_grandTotalDinarEdit  = new QLineEdit("0");
    m_grandTotalDinarEdit->setReadOnly(true);
    m_grandTotalDinarEdit->setObjectName("yellowField");
    m_grandTotalDinarEdit->setFixedWidth(100);

    m_discountEdit = new QLineEdit("0");
    m_discountEdit->setObjectName("greenField");
    m_discountEdit->setFixedWidth(80);

    // Currency labels stored as members so onCurrencyChanged can update them
    m_paidCurrLbl  = new QLabel(QString::fromUtf8("مدفوع $"));
    m_grandCurrLbl = new QLabel("$");

    // Right side: supplier balance
    summRow->addWidget(m_supplierBalLabel);
    summRow->addWidget(lbl(QString::fromUtf8("د:")));
    summRow->addWidget(m_supplierBalDinarLabel);
    summRow->addStretch();
    // Payment field (label before field = label on right in RTL HBox)
    summRow->addWidget(m_paidCurrLbl);
    summRow->addWidget(m_paidDollarEdit);
    summRow->addStretch();
    // Grand total
    summRow->addWidget(m_grandCurrLbl);
    summRow->addWidget(m_grandTotalDollarEdit);
    summRow->addWidget(lbl(QString::fromUtf8("دينار")));
    summRow->addWidget(m_grandTotalDinarEdit);
    summRow->addWidget(lbl(QString::fromUtf8("خصم دينار")));
    summRow->addWidget(m_discountEdit);

    tableLayout->addLayout(summRow);
    mainLayout->addWidget(tableArea, 1);

    // ===== TOOLBAR =====
    QWidget *toolbar = new QWidget;
    toolbar->setObjectName("toolbar");
    QHBoxLayout *toolLayout = new QHBoxLayout(toolbar);
    toolLayout->setContentsMargins(6, 4, 6, 4);
    toolLayout->setSpacing(4);

    auto makeBtn = [](const QString &t, const QString &tip = "", const QString &obj = "toolBtn") {
        QPushButton *b = new QPushButton(t);
        b->setFixedSize(42, 36);
        b->setToolTip(tip);
        b->setObjectName(obj);
        b->setFont(QFont("Tahoma", 13));
        return b;
    };

    m_saveBtn   = makeBtn(QString::fromUtf8("💾"), QString::fromUtf8("حفظ"));
    m_deleteBtn = makeBtn(QString::fromUtf8("✖"),  QString::fromUtf8("حذف"));
    m_searchBtn = makeBtn(QString::fromUtf8("🔍"), QString::fromUtf8("بحث"));
    m_printBtn  = makeBtn(QString::fromUtf8("🖨"),  QString::fromUtf8("طباعة"));
    m_newBtn    = new QPushButton(QString::fromUtf8("فاتورة جديدة"));
    m_newBtn->setFixedHeight(36); m_newBtn->setObjectName("newInvBtn");
    m_pdfBtn    = new QPushButton("PDF");
    m_pdfBtn->setFixedHeight(36);
    m_pdfBtn->setObjectName("pdfBtn");

    m_firstBtn = makeBtn(QString::fromUtf8("⏮"));
    m_prevBtn  = makeBtn(QString::fromUtf8("◀"));
    m_nextBtn  = makeBtn(QString::fromUtf8("▶"));
    m_lastBtn  = makeBtn(QString::fromUtf8("⏭"));

    m_navLabel = new QLabel("1");
    m_navLabel->setObjectName("navLbl");
    m_navLabel->setFixedWidth(50);
    m_navLabel->setAlignment(Qt::AlignCenter);
    m_totalCountLabel = new QLabel("1");
    m_totalCountLabel->setFixedWidth(50);
    m_totalCountLabel->setAlignment(Qt::AlignCenter);

    m_organiserSpin = new QSpinBox;
    m_organiserSpin->setValue(1);
    m_organiserSpin->setFixedWidth(50);
    m_organiserSpin->setObjectName("organiserSpin");

    m_timeLabel = new QLabel(QTime::currentTime().toString("hh:mm AP"));
    m_timeLabel->setObjectName("timeLbl");

    m_totalExpensesBtn = new QPushButton(QString::fromUtf8("مجموع المصاريف"));
    m_totalExpensesBtn->setObjectName("expBtn");
    m_totalExpensesBtn->setFixedHeight(36);
    m_expensesRatioLabel = new QLabel(QString::fromUtf8("نسبة المصاريف"));
    m_expensesRatioLabel->setObjectName("expLbl");

    m_supplierStatBtn = new QPushButton(QString::fromUtf8("كشف الحساب"));
    m_supplierStatBtn->setFixedHeight(36);
    m_supplierStatBtn->setObjectName("toolBtn");

    m_closeBtn = new QPushButton(QString::fromUtf8("إغلاق"));
    m_closeBtn->setFixedHeight(36);
    m_closeBtn->setObjectName("toolBtn");

    toolLayout->addWidget(m_saveBtn);
    toolLayout->addWidget(m_searchBtn);
    toolLayout->addWidget(m_deleteBtn);
    toolLayout->addWidget(m_printBtn);
    toolLayout->addWidget(m_newBtn);
    toolLayout->addWidget(m_pdfBtn);
    toolLayout->addWidget(lbl(QString::fromUtf8("المنظم")));
    toolLayout->addWidget(m_organiserSpin);
    toolLayout->addWidget(lbl(QString::fromUtf8("الوقت")));
    toolLayout->addWidget(m_timeLabel);
    toolLayout->addStretch();
    toolLayout->addWidget(m_expensesRatioLabel);
    toolLayout->addWidget(m_totalExpensesBtn);
    toolLayout->addWidget(m_supplierStatBtn);
    toolLayout->addWidget(m_closeBtn);
    toolLayout->addStretch();
    toolLayout->addWidget(m_firstBtn);
    toolLayout->addWidget(m_prevBtn);
    toolLayout->addWidget(m_navLabel);
    toolLayout->addWidget(lbl(QString::fromUtf8("من")));
    toolLayout->addWidget(m_totalCountLabel);
    toolLayout->addWidget(m_nextBtn);
    toolLayout->addWidget(m_lastBtn);

    mainLayout->addWidget(toolbar);

    // ── Smart search: case-insensitive contains-match on productCombo ──
    if (m_productCombo->completer()) {
        m_productCombo->completer()->setCaseSensitivity(Qt::CaseInsensitive);
        m_productCombo->completer()->setFilterMode(Qt::MatchContains);
    }
    connect(m_productCombo->lineEdit(), &QLineEdit::returnPressed, [this]() {
        QString text = m_productCombo->currentText().trimmed();
        if (text.isEmpty()) return;
        int idx = m_productCombo->findText(text, Qt::MatchContains | Qt::MatchCaseSensitive);
        if (idx < 0) idx = m_productCombo->findText(text, Qt::MatchContains);
        if (idx >= 0) {
            if (m_productCombo->currentIndex() == idx)
                onProductSelected(idx);   // force refresh even if index unchanged
            else
                m_productCombo->setCurrentIndex(idx);
        }
    });

    // ── Connections ──
    connect(addBtn,  &QPushButton::clicked, this, &PurchaseWindow::addItem);
    connect(delBtn,  &QPushButton::clicked, this, &PurchaseWindow::deleteItem);
    connect(m_saveBtn,   &QPushButton::clicked, this, &PurchaseWindow::saveInvoice);
    connect(m_printBtn,  &QPushButton::clicked, this, &PurchaseWindow::printInvoice);
    connect(m_pdfBtn,    &QPushButton::clicked, this, &PurchaseWindow::exportPDF);
    connect(m_firstBtn,  &QPushButton::clicked, this, &PurchaseWindow::navigateFirst);
    connect(m_prevBtn,   &QPushButton::clicked, this, &PurchaseWindow::navigatePrev);
    connect(m_nextBtn,   &QPushButton::clicked, this, &PurchaseWindow::navigateNext);
    connect(m_lastBtn,   &QPushButton::clicked, this, &PurchaseWindow::navigateLast);
    connect(m_closeBtn,  &QPushButton::clicked, this, &PurchaseWindow::close);
    connect(m_newBtn,    &QPushButton::clicked, this, &PurchaseWindow::newInvoice);

    connect(m_productCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PurchaseWindow::onProductSelected);

    connect(m_barcodeCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PurchaseWindow::onBarcodeSelected);

    connect(m_qtyEdit,       &QLineEdit::textChanged, this, &PurchaseWindow::calculateTotals);
    connect(m_costPriceEdit, &QLineEdit::textChanged, this, &PurchaseWindow::calculateTotals);

    connect(m_currencyCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int){ onCurrencyChanged(); });

    connect(m_supplierCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PurchaseWindow::onSupplierChanged);

    connect(m_supplierStatBtn, &QPushButton::clicked, this, [this](){
        int suppId = m_supplierCombo->currentData().toInt();
        if (suppId <= 0) {
            QMessageBox::warning(this, "", QString::fromUtf8("اختر المجهز أولاً"));
            return;
        }
        // Fetch supplier info
        QString suppName = m_supplierCombo->currentText();
        double balD = 0, balDn = 0;
        {
            QSqlQuery q;
            q.prepare("SELECT balance_dollar, balance_dinar FROM customers WHERE id=?");
            q.addBindValue(suppId); q.exec();
            if (q.next()) { balD = q.value(0).toDouble(); balDn = q.value(1).toDouble(); }
        }
        // Determine statement currency from active combo
        bool useDinar = (m_currencyCombo->currentText() == QString::fromUtf8("دينار"));
        double rate = m_exchangeRateEdit->text().toDouble();
        if (rate <= 0) rate = Database::getExchangeRate();

        // Build HTML table
        QString html;
        html += "<html><head><meta charset='utf-8'>"
                "<style>body{font-family:Tahoma,Arial;direction:rtl;}"
                "h2{text-align:center;color:#003366;}table{width:100%;border-collapse:collapse;}"
                "th{background:#1a3a6a;color:white;padding:5px 8px;font-size:10pt;}"
                "td{border:1px solid #aaa;padding:4px 8px;text-align:center;font-size:9pt;}"
                "tr:nth-child(even){background:#f0f4ff;}"
                ".deb{color:#cc0000;font-weight:bold;}"
                ".cred{color:#007700;font-weight:bold;}"
                ".bal{background:#ffffc0;font-weight:bold;}"
                "</style></head><body>";
        html += QString("<h2>%1 — %2</h2>").arg(
            QString::fromUtf8("كشف حساب مجهز"),
            suppName.toHtmlEscaped());

        QString sym = useDinar ? QString::fromUtf8("دينار") : "$";
        html += QString("<table><thead><tr>"
                        "<th>%1</th><th>%2</th><th>%3</th><th>%4</th><th>%5</th><th>%6</th><th>%7</th>"
                        "</tr></thead><tbody>")
                .arg(QString::fromUtf8("ت"),
                     QString::fromUtf8("التاريخ"),
                     QString::fromUtf8("نوع"),
                     QString::fromUtf8("الدفع"),
                     QString::fromUtf8("مدين ") + sym,
                     QString::fromUtf8("دائن ") + sym,
                     QString::fromUtf8("الرصيد ") + sym);

        // Fetch purchase invoices for this supplier
        QSqlQuery q;
        q.prepare("SELECT pi.id, pi.date, pi.purchase_type, pi.payment_type, "
                  "pi.total_dollar, pi.total_dinar, pi.discount_dollar, pi.discount_dinar, "
                  "pi.paid_dollar, pi.paid_dinar "
                  "FROM purchase_invoices pi "
                  "WHERE pi.supplier_id=? ORDER BY pi.date, pi.id");
        q.addBindValue(suppId);
        q.exec();

        double runBal = 0;
        int seq = 1;
        while (q.next()) {
            double totD  = q.value(4).toDouble();
            double totDn = q.value(5).toDouble();
            double discD = q.value(6).toDouble();
            double discDn= q.value(7).toDouble();
            double paidD = q.value(8).toDouble();
            double paidDn= q.value(9).toDouble();

            double netD  = totD  - discD;
            double netDn = totDn - discDn;
            double delta = useDinar ? (paidDn - netDn) : (paidD - netD);
            runBal += delta;

            double debit  = (delta < 0) ? -delta : 0;   // we owe supplier
            double credit = (delta > 0) ?  delta : 0;   // supplier owes us

            html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td>"
                            "<td class='deb'>%5</td><td class='cred'>%6</td><td class='bal'>%7</td></tr>")
                    .arg(seq++)
                    .arg(q.value(1).toString())
                    .arg(q.value(2).toString().toHtmlEscaped())
                    .arg(q.value(3).toString().toHtmlEscaped())
                    .arg(debit  > 0 ? QString::number(debit,  'f', useDinar ? 0 : 2) : "")
                    .arg(credit > 0 ? QString::number(credit, 'f', useDinar ? 0 : 2) : "")
                    .arg(QString::number(runBal, 'f', useDinar ? 0 : 2));
        }

        // Final balance row
        double finalBal = useDinar ? balDn : balD;
        QString balClass = (finalBal < 0) ? "deb" : "cred";
        QString balNote  = (finalBal < 0)
            ? QString::fromUtf8("(دين على المحل)")
            : QString::fromUtf8("(بذمة المجهز)");
        html += QString("<tr><td colspan='6' style='font-weight:bold;text-align:right;'>%1</td>"
                        "<td class='%2'>%3 %4</td></tr>")
                .arg(QString::fromUtf8("الرصيد الحالي:"))
                .arg(balClass)
                .arg(QString::number(finalBal, 'f', useDinar ? 0 : 2))
                .arg(balNote);

        html += "</tbody></table></body></html>";
        m_printer->printPreview(this, html,
            QString::fromUtf8("كشف حساب مجهز — ") + suppName);
    });
}

void PurchaseWindow::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #8890a0; }
        #headerWidget { background: #9098a8; border-bottom: 1px solid #7080a0; border-radius: 4px; }
        #tableArea { background: #7880a0; }
        #itemsTable { background: white; }
        #itemsTable QHeaderView::section {
            background: #1a3a6a; color: white; font-weight: bold;
            padding: 4px; border: 1px solid #0a2a5a; font-size: 9pt;
        }
        #toolbar { background: #9898cc; border-top: 1px solid #6666aa; border-radius: 3px; }
        #toolBtn { background: #7878aa; border: 1px solid #5555aa; color: white; border-radius: 3px; }
        #toolBtn:hover { background: #9898cc; }
        #pdfBtn { background: #cc2200; color: white; font-weight: bold; border-radius: 3px;
                  border: 1px solid #880000; padding: 0 8px; }
        #navLbl { background: #FFD700; color: #003366; font-weight: bold;
                  border: 1px solid #aa8800; border-radius: 2px; }
        #yellowField { background: #ffffc0; border: 1px solid #aaaa60; font-weight: bold; }
        #greenField  { background: #c0ffc0; border: 1px solid #60aa60; }
        #stockLbl { color: white; font-weight: bold; padding: 2px 8px; }
        #balLbl { color: #ffff88; font-weight: bold; padding: 2px 6px; }
        #organiserSpin { background: #cc88ff; font-weight: bold;
                         border: 1px solid #9944bb; border-radius: 2px; }
        #timeLbl { background: #ffffc0; font-weight: bold; padding: 2px 6px;
                   border: 1px solid #aaaa60; border-radius: 2px; }
        #expBtn { background: #bb44ff; color: white; font-weight: bold; border-radius: 3px;
                  border: 1px solid #880099; padding: 0 8px; }
        #expLbl { color: #ffff00; font-weight: bold; }
        #addBtn    { background: #44aa44; color: white; border-radius: 3px; border: 1px solid #228822; }
        #delBtn    { background: #aa4422; color: white; border-radius: 3px; border: 1px solid #882200; }
        #newInvBtn { background: #2266cc; color: white; font-weight: bold; border-radius: 3px;
                     border: 1px solid #1144aa; padding: 0 8px; }
        QComboBox { background: white; border: 1px solid #99aacc; border-radius: 2px; }
        QLineEdit { background: white; border: 1px solid #99aacc; border-radius: 2px; padding: 1px 4px; }
        QLabel { font-family: Tahoma; color: #003366; }
        QDateEdit { background: white; border: 1px solid #99aacc; border-radius: 2px; }
    )");
}

void PurchaseWindow::loadSuppliers()
{
    m_supplierCombo->clear();
    m_supplierCombo->addItem(QString::fromUtf8("-- اختر المجهز --"), -1);
    QSqlQuery q;
    q.exec("SELECT id, name FROM customers ORDER BY name");
    while (q.next())
        m_supplierCombo->addItem(q.value(1).toString(), q.value(0));
}

void PurchaseWindow::loadProducts()
{
    // Block both combos while loading to prevent spurious onProductSelected calls
    m_productCombo->blockSignals(true);
    m_barcodeCombo->blockSignals(true);
    m_productCombo->clear();
    m_barcodeCombo->clear();
    QSqlQuery q = Database::getProducts();
    while (q.next()) {
        int    id      = q.value(0).toInt();
        QString barcode = q.value(1).toString();
        QString name    = q.value(2).toString();
        m_productCombo->addItem(name,    id);
        m_barcodeCombo->addItem(barcode, id);
    }
    m_productCombo->blockSignals(false);
    m_barcodeCombo->blockSignals(false);
}

void PurchaseWindow::onProductSelected(int index)
{
    if (index < 0 || m_blockBarcodeSignal) return;
    int prodId = m_productCombo->itemData(index).toInt();
    if (prodId <= 0) return;

    QSqlQuery q;
    q.prepare("SELECT cost_price, wholesale_dollar, retail_dollar, "
              "wholesale_dinar, retail_dinar, stock_qty, barcode FROM products WHERE id=?");
    q.addBindValue(prodId);
    q.exec();
    if (q.next()) {
        bool   isDinar = (m_currencyCombo->currentText() == QString::fromUtf8("دينار"));
        double rate    = m_exchangeRateEdit->text().toDouble();
        if (rate <= 0) rate = Database::getExchangeRate();
        double costD = q.value(0).toDouble();
        double wsD   = q.value(1).toDouble();
        double retD  = q.value(2).toDouble();
        // Show prices in the active currency
        int dp = isDinar ? 0 : 2;
        double mul = isDinar ? rate : 1.0;
        m_costPriceEdit->setText(QString::number(costD * mul, 'f', dp));
        m_wholesaleDollarEdit->setText(QString::number(wsD * mul, 'f', dp));
        m_retailDollarEdit->setText(QString::number(retD * mul, 'f', dp));
        m_stockLabel->setText(QString::fromUtf8("رصيد: ") + q.value(5).toString());

        // Update barcode without re-triggering onBarcodeSelected
        m_blockBarcodeSignal = true;
        m_barcodeCombo->setCurrentText(q.value(6).toString());
        m_blockBarcodeSignal = false;
    }
    calculateTotals();
}

void PurchaseWindow::onBarcodeSelected(int index)
{
    if (index < 0 || m_blockBarcodeSignal) return;
    int prodId = m_barcodeCombo->itemData(index).toInt();
    if (prodId <= 0) return;

    // Find matching product in productCombo and select it
    for (int i = 0; i < m_productCombo->count(); i++) {
        if (m_productCombo->itemData(i).toInt() == prodId) {
            m_blockBarcodeSignal = true;
            m_productCombo->setCurrentIndex(i);
            m_blockBarcodeSignal = false;
            onProductSelected(i);
            break;
        }
    }
}

void PurchaseWindow::calculateTotals()
{
    bool isDinar = (m_currencyCombo->currentText() == QString::fromUtf8("دينار"));
    double qty   = m_qtyEdit->text().toDouble();
    double cost  = m_costPriceEdit->text().toDouble();
    double rate  = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();
    double lineAmt = qty * cost;
    m_totalDollarEdit->setText(QString::number(lineAmt, 'f', isDinar ? 0 : 2));
    m_totalDinarEdit->setText(QString::number(lineAmt * (isDinar ? 1.0 : rate), 'f', 0));
}

void PurchaseWindow::addItem()
{
    if (m_productCombo->currentText().isEmpty()) return;
    bool   isDinar = (m_currencyCombo->currentText() == QString::fromUtf8("دينار"));
    int    prodId  = m_productCombo->currentData().toInt();
    double qty     = m_qtyEdit->text().toDouble();
    double cost    = m_costPriceEdit->text().toDouble();   // in active currency
    double rate    = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();
    // Always store dollar cost in col 5; dinar amount in col 7
    double costDollar = isDinar ? (cost / rate) : cost;
    double totalD  = qty * costDollar;
    double totalDn = totalD * rate;

    int row = m_itemsTable->rowCount();
    m_itemsTable->insertRow(row);

    auto cell = [&](int col, const QString &val) {
        auto *item = new QTableWidgetItem(val);
        item->setTextAlignment(Qt::AlignCenter);
        m_itemsTable->setItem(row, col, item);
    };

    cell(0,  QString::number(row + 1));                        // ت
    cell(1,  m_barcodeCombo->currentText());                   // باركود
    cell(2,  QString::number(prodId));                         // رمز (product id as code)
    cell(3,  m_productCombo->currentText());                   // اسم المادة
    cell(4,  QString::number(qty));                            // الكمية
    cell(5,  QString::number(costDollar, 'f', 2));             // الكلفة $ (always dollars)
    cell(6,  QString::number(totalD,  'f', 2));                // المبلغ $ (always dollars)
    cell(7,  QString::number(totalDn, 'f', 0));                // المبلغ دينار
    cell(8,  m_wholesaleDollarEdit->text());                   // سعر جملة $
    cell(9,  m_retailDollarEdit->text());                      // سعر مفرد $
    cell(10, QString::number(prodId));                         // hidden ID

    // Update grand totals (always in dollars)
    double grand = 0;
    for (int r = 0; r < m_itemsTable->rowCount(); r++)
        if (m_itemsTable->item(r, 6))
            grand += m_itemsTable->item(r, 6)->text().toDouble();
    m_grandTotalDollarEdit->setText(QString::number(grand, 'f', 2));
    m_grandTotalDinarEdit->setText(QString::number(grand * rate, 'f', 0));

    m_qtyEdit->setText("0");
    m_totalDollarEdit->setText("0");
    m_totalDinarEdit->setText("0");
    updateCurrencyLock();
}

void PurchaseWindow::deleteItem()
{
    int row = m_itemsTable->currentRow();
    if (row < 0) return;
    m_itemsTable->removeRow(row);
    // Re-number sequence
    for (int r = 0; r < m_itemsTable->rowCount(); r++)
        m_itemsTable->item(r, 0)->setText(QString::number(r + 1));
    // Recalc totals
    double grand = 0, rate = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();
    for (int r = 0; r < m_itemsTable->rowCount(); r++)
        if (m_itemsTable->item(r, 6))
            grand += m_itemsTable->item(r, 6)->text().toDouble();
    m_grandTotalDollarEdit->setText(QString::number(grand, 'f', 2));
    m_grandTotalDinarEdit->setText(QString::number(grand * rate, 'f', 0));
    updateCurrencyLock();
}

void PurchaseWindow::saveInvoice()
{
    int suppId = m_supplierCombo->currentData().toInt();
    if (suppId <= 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("اختر المجهز"));
        return;
    }
    if (m_itemsTable->rowCount() == 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("لا توجد مواد في الفاتورة"));
        return;
    }

    double rate        = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();
    bool   isDinar     = (m_currencyCombo->currentText() == QString::fromUtf8("دينار"));
    double grandAmt    = m_grandTotalDollarEdit->text().toDouble(); // in active currency
    double totalDollar = isDinar ? (grandAmt / rate) : grandAmt;
    double totalDinar  = isDinar ? grandAmt : (grandAmt * rate);
    double discount    = m_discountEdit->text().toDouble();
    double paidAmt     = m_paidDollarEdit->text().toDouble();       // in active currency
    double paidDollar  = isDinar ? (paidAmt / rate) : paidAmt;
    double paidDinar   = isDinar ? paidAmt : (paidAmt * rate);
    QString payType    = m_payTypeCombo->currentText();             // نقدي / آجل
    QString currency   = m_currencyCombo->currentText();

    int invId = Database::createPurchaseInvoice(
        suppId, m_dateEdit->date(),
        m_purchaseTypeCombo->currentText(),
        payType,
        currency,
        rate, m_notesEdit->text(), 1);

    if (invId < 0) {
        QMessageBox::critical(this, "", QString::fromUtf8("فشل حفظ الفاتورة"));
        return;
    }

    for (int r = 0; r < m_itemsTable->rowCount(); r++) {
        int    prodId = m_itemsTable->item(r, 10)->text().toInt();
        double qty    = m_itemsTable->item(r,  4)->text().toDouble();
        double cost   = m_itemsTable->item(r,  5)->text().toDouble();
        double wsD    = m_itemsTable->item(r,  8)->text().toDouble();
        double retD   = m_itemsTable->item(r,  9)->text().toDouble();
        Database::addPurchaseItem(invId, prodId, qty, cost, wsD, retD, 0, 0, "");
    }

    // ── Save totals, discount and paid amount to invoice record ──
    {
        QSqlQuery updInv;
        updInv.prepare("UPDATE purchase_invoices SET "
                       "total_dollar=?, total_dinar=?,"
                       "discount_dollar=?, discount_dinar=?,"
                       "paid_dollar=?, paid_dinar=? WHERE id=?");
        updInv.addBindValue(totalDollar);
        updInv.addBindValue(totalDinar);
        updInv.addBindValue(isDinar ? 0.0     : discount);
        updInv.addBindValue(isDinar ? discount : 0.0);
        updInv.addBindValue(paidDollar);
        updInv.addBindValue(paidDinar);
        updInv.addBindValue(invId);
        updInv.exec();
    }

    // ── Accounting: unified delta formula ──────────────────────────
    // new_balance = old_balance + paid - (total - discount)
    // positive balance = supplier owes us (we overpaid or have credit)
    // negative balance = we owe supplier (underpaid or deferred)
    double discD  = isDinar ? 0.0     : discount;
    double discDn = isDinar ? discount : 0.0;
    double deltaD  = paidDollar - (totalDollar - discD);
    double deltaDn = paidDinar  - (totalDinar  - discDn);

    QSqlQuery upd;
    upd.prepare("UPDATE customers SET balance_dollar = balance_dollar + ?,"
                " balance_dinar = balance_dinar + ? WHERE id=?");
    upd.addBindValue(deltaD);
    upd.addBindValue(deltaDn);
    upd.addBindValue(suppId);
    upd.exec();

    m_currentInvoiceId = invId;
    m_invoiceNoSpin->setValue(invId);

    // Reload products so next selection shows updated prices and stock
    loadProducts();

    // Refresh supplier balance display
    onSupplierChanged(m_supplierCombo->currentIndex());
    updateCurrencyLock();

    QMessageBox::information(this,
        QString::fromUtf8("تم"),
        QString::fromUtf8("تم حفظ الفاتورة رقم: ") + QString::number(invId));
}

void PurchaseWindow::clearForm()
{
    m_currentInvoiceId = -1;
    m_itemsTable->setRowCount(0);
    m_qtyEdit->setText("0");
    m_costPriceEdit->setText("0");
    m_totalDollarEdit->setText("0");
    m_totalDinarEdit->setText("0");
    m_grandTotalDollarEdit->setText("0");
    m_grandTotalDinarEdit->setText("0");
    m_paidDollarEdit->setText("0");
    m_paidDinarEdit->setText("0");
    m_discountEdit->setText("0");
    m_notesEdit->clear();
    m_stockLabel->setText(QString::fromUtf8("رصيد: 0"));
    updateCurrencyLock();   // re-enable currency combo when table is empty
}

void PurchaseWindow::newInvoice()
{
    // Full reset: table + all invoice fields + supplier + item entry row
    clearForm();

    // Reset supplier combo
    m_supplierCombo->blockSignals(true);
    m_supplierCombo->setCurrentIndex(0);
    m_supplierCombo->blockSignals(false);
    m_supplierBalLabel->setText(QString::fromUtf8("رصيد المجهز $: 0"));
    m_supplierBalDinarLabel->setText("0");

    // Reset item entry row
    m_blockBarcodeSignal = true;
    m_productCombo->setCurrentIndex(-1);
    m_barcodeCombo->setCurrentIndex(-1);
    m_blockBarcodeSignal = false;
    m_wholesaleDollarEdit->setText("0");
    m_retailDollarEdit->setText("0");

    // Reset invoice header fields
    m_invoiceRefEdit->clear();
    m_dateEdit->setDate(QDate::currentDate());
    m_exchangeRateEdit->setText(
        QString::number(Database::getExchangeRate(), 'f', 0));
}

void PurchaseWindow::updateCurrencyLock()
{
    bool hasItems = (m_itemsTable->rowCount() > 0);
    m_currencyCombo->setEnabled(!hasItems);
    // Visual cue: grey out when locked
    m_currencyCombo->setStyleSheet(hasItems
        ? "background:#cccccc; color:#666666; border:1px solid #aaaaaa;"
        : "");
}

void PurchaseWindow::printInvoice()
{
    if (m_currentInvoiceId < 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("احفظ الفاتورة أولاً"));
        return;
    }
    QString html = PrintManager::generatePurchaseInvoiceHtml(
        m_currentInvoiceId, Database::getCompanyName());
    m_printer->printPreview(this, html, QString::fromUtf8("فاتورة مشتريات"));
}

void PurchaseWindow::exportPDF()
{
    if (m_currentInvoiceId < 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("احفظ الفاتورة أولاً"));
        return;
    }
    QString html = PrintManager::generatePurchaseInvoiceHtml(
        m_currentInvoiceId, Database::getCompanyName());
    m_printer->exportToPDF(this, html, QString::fromUtf8("فاتورة_مشتريات"));
}

void PurchaseWindow::navigateFirst() {}
void PurchaseWindow::navigatePrev()  { if (m_currentInvoiceId > 1) m_currentInvoiceId--; }
void PurchaseWindow::navigateNext()  { m_currentInvoiceId++; }
void PurchaseWindow::navigateLast()  {}

void PurchaseWindow::onSupplierChanged(int index)
{
    if (index < 0) return;
    int suppId = m_supplierCombo->currentData().toInt();
    if (suppId <= 0) {
        m_supplierBalLabel->setText(QString::fromUtf8("رصيد المجهز $: 0"));
        m_supplierBalDinarLabel->setText("0");
        return;
    }
    QSqlQuery q;
    q.prepare("SELECT balance_dollar, balance_dinar FROM customers WHERE id=?");
    q.addBindValue(suppId);
    q.exec();
    if (q.next()) {
        m_supplierBalLabel->setText(
            QString::fromUtf8("رصيد المجهز $: ") +
            QString::number(q.value(0).toDouble(), 'f', 2));
        m_supplierBalDinarLabel->setText(
            QString::number(q.value(1).toDouble(), 'f', 0));
    }
}

void PurchaseWindow::onCurrencyChanged()
{
    bool isDinar = (m_currencyCombo->currentText() == QString::fromUtf8("دينار"));
    QString sym  = isDinar ? QString::fromUtf8("دينار") : "$";
    QString syms = isDinar ? QString::fromUtf8("د")     : "$";
    double rate  = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();

    // ── Update all currency-symbol labels ──
    m_costCurrLbl->setText(QString::fromUtf8("كلفة ") + syms);
    m_wsCurrLbl->setText(QString::fromUtf8("جملة ")  + syms);
    m_retCurrLbl->setText(QString::fromUtf8("مفرد ") + syms);
    m_lineTotCurrLbl->setText(syms);
    m_grandCurrLbl->setText(syms);
    m_paidCurrLbl->setText(QString::fromUtf8("مدفوع ") + sym);

    // ── Update items table column headers ──
    m_itemsTable->setHorizontalHeaderItem(5, new QTableWidgetItem(
        QString::fromUtf8("الكلفة ") + syms));
    m_itemsTable->setHorizontalHeaderItem(6, new QTableWidgetItem(
        QString::fromUtf8("المبلغ ") + syms));

    // ── Convert entry-row price fields (كلفة / جملة / مفرد) ──
    auto convertField = [&](QLineEdit *field) {
        double v = field->text().toDouble();
        if (isDinar)
            field->setText(QString::number(v * rate, 'f', 0));   // $ → دينار
        else
            field->setText(QString::number(v / rate, 'f', 2));   // دينار → $
    };
    convertField(m_costPriceEdit);
    convertField(m_wholesaleDollarEdit);
    convertField(m_retailDollarEdit);

    // ── Recalculate existing table rows ──
    for (int r = 0; r < m_itemsTable->rowCount(); r++) {
        auto *costItem = m_itemsTable->item(r, 5);
        auto *amtItem  = m_itemsTable->item(r, 6);
        auto *qtyItem  = m_itemsTable->item(r, 4);
        if (!costItem || !amtItem || !qtyItem) continue;
        double cost = costItem->text().toDouble();
        double qty  = qtyItem->text().toDouble();
        double newCost = isDinar ? (cost * rate) : (cost / rate);
        costItem->setText(QString::number(newCost, 'f', isDinar ? 0 : 2));
        amtItem->setText(QString::number(newCost * qty, 'f', isDinar ? 0 : 2));
    }

    // ── Recalc grand total ──
    double grand = 0;
    for (int r = 0; r < m_itemsTable->rowCount(); r++)
        if (m_itemsTable->item(r, 6))
            grand += m_itemsTable->item(r, 6)->text().toDouble();
    m_grandTotalDollarEdit->setText(
        isDinar ? QString::number(grand, 'f', 0) : QString::number(grand, 'f', 2));
    m_grandTotalDinarEdit->setText(
        QString::number(grand * (isDinar ? 1.0 : rate), 'f', 0));

    calculateTotals();
}
