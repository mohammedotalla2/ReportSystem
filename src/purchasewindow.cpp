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
    m_currencyCombo->addItems({"$", QString::fromUtf8("دينار")});
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
        QMessageBox::information(this,
            QString::fromUtf8("كشف الحساب"),
            QString::fromUtf8("المجهز: ") + m_supplierCombo->currentText());
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
        #addBtn { background: #44aa44; color: white; border-radius: 3px; border: 1px solid #228822; }
        #delBtn { background: #aa4422; color: white; border-radius: 3px; border: 1px solid #882200; }
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
        m_costPriceEdit->setText(QString::number(q.value(0).toDouble(), 'f', 2));
        m_wholesaleDollarEdit->setText(QString::number(q.value(1).toDouble(), 'f', 2));
        m_retailDollarEdit->setText(QString::number(q.value(2).toDouble(), 'f', 2));
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
    int    prodId = m_productCombo->currentData().toInt();
    double qty    = m_qtyEdit->text().toDouble();
    double cost   = m_costPriceEdit->text().toDouble();
    double rate   = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();
    double totalD  = qty * cost;
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
    cell(5,  QString::number(cost, 'f', 2));                   // الكلفة $
    cell(6,  QString::number(totalD,  'f', 2));                // المبلغ $
    cell(7,  QString::number(totalDn, 'f', 0));                // المبلغ دينار
    cell(8,  m_wholesaleDollarEdit->text());                   // سعر جملة $
    cell(9,  m_retailDollarEdit->text());                      // سعر مفرد $
    cell(10, QString::number(prodId));                         // hidden ID

    // Update grand totals
    double grand = 0;
    for (int r = 0; r < m_itemsTable->rowCount(); r++)
        if (m_itemsTable->item(r, 6))
            grand += m_itemsTable->item(r, 6)->text().toDouble();
    m_grandTotalDollarEdit->setText(QString::number(grand, 'f', 2));
    m_grandTotalDinarEdit->setText(QString::number(grand * rate, 'f', 0));

    m_qtyEdit->setText("0");
    m_totalDollarEdit->setText("0");
    m_totalDinarEdit->setText("0");
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

    // ── Accounting logic ──
    QSqlQuery upd;
    if (payType == QString::fromUtf8("آجل")) {
        // Deferred: full net amount becomes a debit (we owe the supplier)
        double netD  = totalDollar - (isDinar ? 0 : discount / rate);
        double netDn = totalDinar  - (isDinar ? discount : 0);
        upd.prepare("UPDATE customers SET balance_dollar = balance_dollar + ?,"
                    " balance_dinar = balance_dinar + ? WHERE id=?");
        upd.addBindValue(netD  > 0 ? netD  : 0.0);
        upd.addBindValue(netDn > 0 ? netDn : 0.0);
        upd.addBindValue(suppId);
        upd.exec();
    } else {
        // نقدي: add unpaid shortfall to supplier balance
        double netD      = totalDollar - (isDinar ? 0 : discount / rate);
        double netDn     = totalDinar  - (isDinar ? discount : 0);
        double shortD    = netD  - paidDollar;
        double shortDn   = netDn - paidDinar;
        if (shortD > 0.001 || shortDn > 0.001) {
            upd.prepare("UPDATE customers SET balance_dollar = balance_dollar + ?,"
                        " balance_dinar = balance_dinar + ? WHERE id=?");
            upd.addBindValue(shortD  > 0 ? shortD  : 0.0);
            upd.addBindValue(shortDn > 0 ? shortDn : 0.0);
            upd.addBindValue(suppId);
            upd.exec();
        }
    }

    m_currentInvoiceId = invId;
    m_invoiceNoSpin->setValue(invId);
    // Refresh supplier balance display
    onSupplierChanged(m_supplierCombo->currentIndex());

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

    // ── Update all currency-symbol labels ──
    m_costCurrLbl->setText(QString::fromUtf8("كلفة ") + syms);
    m_wsCurrLbl->setText(QString::fromUtf8("جملة ") + syms);
    m_retCurrLbl->setText(QString::fromUtf8("مفرد ") + syms);
    m_lineTotCurrLbl->setText(syms);
    m_grandCurrLbl->setText(syms);
    m_paidCurrLbl->setText(QString::fromUtf8("مدفوع ") + sym);

    // ── Update items table column headers ──
    m_itemsTable->setHorizontalHeaderItem(5, new QTableWidgetItem(
        QString::fromUtf8("الكلفة ") + syms));
    m_itemsTable->setHorizontalHeaderItem(6, new QTableWidgetItem(
        QString::fromUtf8("المبلغ ") + syms));

    // ── Recalculate existing rows ──
    double rate = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();

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
    QString fmt = isDinar ? QString::number(grand, 'f', 0) : QString::number(grand, 'f', 2);
    m_grandTotalDollarEdit->setText(fmt);
    m_grandTotalDinarEdit->setText(QString::number(grand * (isDinar ? 1.0 : rate), 'f', 0));

    // ── Recalc current line entry ──
    calculateTotals();
}
