#include "purchasewindow.h"
#include "database.h"
#include "arabichelper.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QTime>
#include <QSqlQuery>
#include <QFont>
#include <QFrame>

PurchaseWindow::PurchaseWindow(QWidget *parent)
    : QDialog(parent), m_currentInvoiceId(-1)
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

    auto lbl = [](const QString &t) {
        QLabel *l = new QLabel(t);
        l->setFont(QFont("Tahoma", 9));
        l->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        return l;
    };

    // Row 0: رقم القائمة | التاريخ | نوع الشراء | اسم المجهز | نوع الدفع
    m_invoiceNoSpin = new QSpinBox;
    m_invoiceNoSpin->setRange(1, 9999999);
    m_invoiceNoSpin->setObjectName("invoiceNo");

    m_dateEdit = new QDateEdit(QDate::currentDate());
    m_dateEdit->setDisplayFormat("d");
    QDateEdit *monthEdit = new QDateEdit(QDate::currentDate());
    monthEdit->setDisplayFormat("M");
    QDateEdit *yearEdit = new QDateEdit(QDate::currentDate());
    yearEdit->setDisplayFormat("yyyy");

    m_purchaseTypeCombo = new QComboBox;
    m_purchaseTypeCombo->addItems({QString::fromUtf8("محلي"), QString::fromUtf8("استيراد")});

    m_supplierCombo = new QComboBox; m_supplierCombo->setEditable(true);
    m_supplierCombo->setMinimumWidth(180);

    m_payTypeCombo = new QComboBox;
    m_payTypeCombo->addItems({QString::fromUtf8("نقدي"), QString::fromUtf8("آجل")});

    m_currencyCombo = new QComboBox;
    m_currencyCombo->addItems({"$", QString::fromUtf8("دينار")});

    m_invoiceRefEdit = new QLineEdit;
    m_invoiceRefEdit->setPlaceholderText(QString::fromUtf8("سند الصرف"));

    m_exchangeRateEdit = new QLineEdit(QString::number(Database::getExchangeRate(), 'f', 0));
    m_exchangeRateEdit->setObjectName("yellowField");
    m_exchangeRateEdit->setFixedWidth(80);

    headerGrid->addWidget(lbl(QString::fromUtf8("رقم القائمة")), 0, 12);
    headerGrid->addWidget(m_invoiceNoSpin, 0, 11);
    headerGrid->addWidget(lbl(QString::fromUtf8("التاريخ")), 0, 10);
    headerGrid->addWidget(m_dateEdit, 0, 9);
    headerGrid->addWidget(monthEdit, 0, 8);
    headerGrid->addWidget(yearEdit, 0, 7);
    headerGrid->addWidget(lbl(QString::fromUtf8("نوع الشراء")), 0, 6);
    headerGrid->addWidget(m_purchaseTypeCombo, 0, 5);
    headerGrid->addWidget(lbl(QString::fromUtf8("اسم المجهز")), 0, 3);
    headerGrid->addWidget(m_supplierCombo, 0, 2);
    headerGrid->addWidget(lbl(QString::fromUtf8("نوع الدفع")), 0, 1);
    headerGrid->addWidget(m_payTypeCombo, 0, 0);

    // Row 1
    headerGrid->addWidget(lbl(QString::fromUtf8("العملة")), 1, 12);
    headerGrid->addWidget(m_currencyCombo, 1, 11);
    headerGrid->addWidget(lbl(QString::fromUtf8("سند الصرف")), 1, 9);
    headerGrid->addWidget(m_invoiceRefEdit, 1, 8);
    headerGrid->addWidget(lbl(QString::fromUtf8("سعر صرف $")), 1, 6);
    headerGrid->addWidget(m_exchangeRateEdit, 1, 5);

    m_notesEdit = new QLineEdit;
    m_notesEdit->setPlaceholderText(QString::fromUtf8("الملاحظات"));
    headerGrid->addWidget(m_notesEdit, 2, 0, 1, 13);

    mainLayout->addWidget(headerWidget);

    // ===== ITEMS TABLE AREA =====
    QWidget *tableArea = new QWidget;
    tableArea->setObjectName("tableArea");
    QVBoxLayout *tableLayout = new QVBoxLayout(tableArea);
    tableLayout->setContentsMargins(2, 2, 2, 2);
    tableLayout->setSpacing(2);

    // Column headers match المشتريات screenshot exactly
    m_itemsTable = new QTableWidget(0, 9);
    m_itemsTable->setHorizontalHeaderLabels({
        QString::fromUtf8("الباركود"),
        QString::fromUtf8("رمز المادة"),
        QString::fromUtf8("اسم المـادة"),
        QString::fromUtf8("الكمية"),
        QString::fromUtf8("السعر"),
        QString::fromUtf8("المبلغ دينار"),
        QString::fromUtf8("الكلفة $"),
        QString::fromUtf8("مبلغ الكلفة"),
        QString::fromUtf8("سعر البيع $مفرد"),
    });
    m_itemsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_itemsTable->setObjectName("itemsTable");
    m_itemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_itemsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_itemsTable->setMinimumHeight(300);

    tableLayout->addWidget(m_itemsTable);

    // Item entry row
    QHBoxLayout *itemRow = new QHBoxLayout;
    m_barcodeCombo = new QComboBox; m_barcodeCombo->setEditable(true);
    m_barcodeCombo->setPlaceholderText(QString::fromUtf8("الباركود"));
    m_productCombo = new QComboBox; m_productCombo->setEditable(true);
    m_productCombo->setPlaceholderText(QString::fromUtf8("اسم المادة"));
    m_productCombo->setMinimumWidth(180);

    m_qtyEdit = new QLineEdit("0"); m_qtyEdit->setFixedWidth(60);
    m_costPriceEdit = new QLineEdit("0"); m_costPriceEdit->setFixedWidth(80);
    m_wholesaleDollarEdit = new QLineEdit("0"); m_wholesaleDollarEdit->setFixedWidth(80);
    m_retailDollarEdit = new QLineEdit("0"); m_retailDollarEdit->setFixedWidth(80);
    m_wholesaleDinarEdit = new QLineEdit("0"); m_wholesaleDinarEdit->setFixedWidth(80);
    m_retailDinarEdit = new QLineEdit("0"); m_retailDinarEdit->setFixedWidth(80);

    m_stockLabel = new QLabel(QString::fromUtf8("رصيد المادة: 0"));
    m_stockLabel->setObjectName("stockLbl");

    QPushButton *addBtn = new QPushButton("✔");
    addBtn->setFixedSize(36, 30); addBtn->setObjectName("addBtn");
    QPushButton *editBtn = new QPushButton("✎");
    editBtn->setFixedSize(36, 30);
    QPushButton *delBtn = new QPushButton("✖");
    delBtn->setFixedSize(36, 30); delBtn->setObjectName("delBtn");

    itemRow->addWidget(m_barcodeCombo);
    itemRow->addWidget(m_productCombo);
    itemRow->addWidget(new QLabel(QString::fromUtf8("الكمية"))); itemRow->addWidget(m_qtyEdit);
    itemRow->addWidget(new QLabel(QString::fromUtf8("كلفة $"))); itemRow->addWidget(m_costPriceEdit);
    itemRow->addWidget(new QLabel(QString::fromUtf8("جملة $"))); itemRow->addWidget(m_wholesaleDollarEdit);
    itemRow->addWidget(new QLabel(QString::fromUtf8("مفرد $"))); itemRow->addWidget(m_retailDollarEdit);
    itemRow->addWidget(addBtn); itemRow->addWidget(editBtn); itemRow->addWidget(delBtn);
    itemRow->addWidget(m_stockLabel);
    tableLayout->insertLayout(0, itemRow);

    // Summary bottom row
    QHBoxLayout *summRow = new QHBoxLayout;
    m_grandTotalDollarEdit = new QLineEdit("0"); m_grandTotalDollarEdit->setReadOnly(true);
    m_grandTotalDollarEdit->setObjectName("yellowField");
    m_grandTotalDinarEdit = new QLineEdit("0"); m_grandTotalDinarEdit->setReadOnly(true);
    m_discountEdit = new QLineEdit("0");
    m_discountEdit->setObjectName("greenField");
    m_supplierBalLabel = new QLabel(QString::fromUtf8("رصيدالمجهز: 0"));

    summRow->addWidget(m_supplierBalLabel);
    summRow->addStretch();
    summRow->addWidget(new QLabel("$")); summRow->addWidget(m_grandTotalDollarEdit);
    summRow->addWidget(new QLabel(QString::fromUtf8("دينار"))); summRow->addWidget(m_grandTotalDinarEdit);
    summRow->addWidget(new QLabel(QString::fromUtf8("الخصم دينار"))); summRow->addWidget(m_discountEdit);

    tableLayout->addLayout(summRow);
    mainLayout->addWidget(tableArea, 1);

    // ===== TOOLBAR =====
    QWidget *toolbar = new QWidget;
    toolbar->setObjectName("toolbar");
    QHBoxLayout *toolLayout = new QHBoxLayout(toolbar);
    toolLayout->setContentsMargins(6, 4, 6, 4);
    toolLayout->setSpacing(4);

    auto makeBtn = [](const QString &t, const QString &tip = "", const QString &obj = "toolBtn") {
        QPushButton *b = new QPushButton(t); b->setFixedSize(42, 36);
        b->setToolTip(tip); b->setObjectName(obj); b->setFont(QFont("Tahoma", 13));
        return b;
    };

    m_saveBtn   = makeBtn("💾", QString::fromUtf8("حفظ"));
    m_deleteBtn = makeBtn("✖", QString::fromUtf8("حذف"));
    m_searchBtn = makeBtn("🔍", QString::fromUtf8("بحث"));
    m_printBtn  = makeBtn("🖨", QString::fromUtf8("طباعة"));
    m_pdfBtn    = new QPushButton("PDF"); m_pdfBtn->setFixedHeight(36); m_pdfBtn->setObjectName("pdfBtn");

    m_firstBtn = makeBtn("⏮"); m_prevBtn = makeBtn("◀");
    m_nextBtn  = makeBtn("▶"); m_lastBtn = makeBtn("⏭");

    m_navLabel       = new QLabel("1"); m_navLabel->setObjectName("navLbl");
    m_navLabel->setFixedWidth(50); m_navLabel->setAlignment(Qt::AlignCenter);
    m_totalCountLabel = new QLabel("1"); m_totalCountLabel->setFixedWidth(50);
    m_totalCountLabel->setAlignment(Qt::AlignCenter);

    m_organiserSpin = new QSpinBox; m_organiserSpin->setValue(1);
    m_organiserSpin->setFixedWidth(50); m_organiserSpin->setObjectName("organiserSpin");
    m_timeLabel = new QLabel(QTime::currentTime().toString("hh:mm AP"));
    m_timeLabel->setObjectName("timeLbl");

    m_totalExpensesBtn = new QPushButton(QString::fromUtf8("مجموع المصاريف"));
    m_totalExpensesBtn->setObjectName("expBtn");
    m_totalExpensesBtn->setFixedHeight(36);
    m_expensesRatioLabel = new QLabel(QString::fromUtf8("نسبة المصاريف"));
    m_expensesRatioLabel->setObjectName("expLbl");

    toolLayout->addWidget(m_saveBtn);
    toolLayout->addWidget(m_searchBtn);
    toolLayout->addWidget(m_deleteBtn);
    toolLayout->addWidget(m_printBtn);
    toolLayout->addWidget(m_pdfBtn);
    toolLayout->addWidget(new QLabel(QString::fromUtf8("المنظم")));
    toolLayout->addWidget(m_organiserSpin);
    toolLayout->addWidget(new QLabel(QString::fromUtf8("الوقت")));
    toolLayout->addWidget(m_timeLabel);
    toolLayout->addStretch();
    toolLayout->addWidget(m_expensesRatioLabel);
    toolLayout->addWidget(m_totalExpensesBtn);
    toolLayout->addStretch();
    toolLayout->addWidget(m_firstBtn);
    toolLayout->addWidget(m_navLabel);
    toolLayout->addWidget(new QLabel(QString::fromUtf8("من")));
    toolLayout->addWidget(m_totalCountLabel);
    toolLayout->addWidget(m_lastBtn);

    // Extra nav buttons
    QPushButton *rcvBtn = makeBtn("🤲", "", "rcvBtn");
    QPushButton *rvwBtn = makeBtn("👁", "", "toolBtn");
    QPushButton *fwdBtn = makeBtn("➡", "", "toolBtn");
    QPushButton *sigBtn = makeBtn("✋", "", "toolBtn");
    toolLayout->addWidget(rcvBtn); toolLayout->addWidget(rvwBtn);
    toolLayout->addWidget(fwdBtn); toolLayout->addWidget(sigBtn);

    mainLayout->addWidget(toolbar);

    // Connect
    connect(addBtn, &QPushButton::clicked, this, &PurchaseWindow::addItem);
    connect(delBtn, &QPushButton::clicked, this, &PurchaseWindow::deleteItem);
    connect(m_saveBtn, &QPushButton::clicked, this, &PurchaseWindow::saveInvoice);
    connect(m_printBtn, &QPushButton::clicked, this, &PurchaseWindow::printInvoice);
    connect(m_pdfBtn, &QPushButton::clicked, this, &PurchaseWindow::exportPDF);
    connect(m_firstBtn, &QPushButton::clicked, this, &PurchaseWindow::navigateFirst);
    connect(m_prevBtn, &QPushButton::clicked, this, &PurchaseWindow::navigatePrev);
    connect(m_nextBtn, &QPushButton::clicked, this, &PurchaseWindow::navigateNext);
    connect(m_lastBtn, &QPushButton::clicked, this, &PurchaseWindow::navigateLast);
    connect(m_productCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PurchaseWindow::onProductSelected);
    connect(m_qtyEdit, &QLineEdit::textChanged, this, &PurchaseWindow::calculateTotals);
    connect(m_costPriceEdit, &QLineEdit::textChanged, this, &PurchaseWindow::calculateTotals);
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
        #pdfBtn { background: #cc2200; color: white; font-weight: bold; border-radius: 3px; border: 1px solid #880000; padding: 0 8px; }
        #navLbl { background: #FFD700; color: #003366; font-weight: bold; border: 1px solid #aa8800; border-radius: 2px; }
        #yellowField { background: #ffffc0; border: 1px solid #aaaa60; font-weight: bold; }
        #greenField  { background: #c0ffc0; border: 1px solid #60aa60; }
        #stockLbl { color: white; font-weight: bold; padding: 2px 8px; }
        #organiserSpin { background: #cc88ff; font-weight: bold; border: 1px solid #9944bb; border-radius: 2px; }
        #timeLbl { background: #ffffc0; font-weight: bold; padding: 2px 6px; border: 1px solid #aaaa60; border-radius: 2px; }
        #expBtn { background: #bb44ff; color: white; font-weight: bold; border-radius: 3px; border: 1px solid #880099; padding: 0 8px; }
        #expLbl { color: #ffff00; font-weight: bold; }
        #addBtn { background: #44aa44; color: white; border-radius: 3px; border: 1px solid #228822; }
        #delBtn { background: #aa4422; color: white; border-radius: 3px; border: 1px solid #882200; }
        #rcvBtn { background: #44aa77; color: white; border-radius: 3px; border: 1px solid #228855; }
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
    QSqlQuery q; q.exec("SELECT id, name FROM customers ORDER BY name");
    while (q.next()) m_supplierCombo->addItem(q.value(1).toString(), q.value(0));
}

void PurchaseWindow::loadProducts()
{
    m_productCombo->clear(); m_barcodeCombo->clear();
    QSqlQuery q = Database::getProducts();
    while (q.next()) {
        m_productCombo->addItem(q.value(2).toString(), q.value(0));
        m_barcodeCombo->addItem(q.value(1).toString(), q.value(0));
    }
}

void PurchaseWindow::onProductSelected(int index)
{
    if (index < 0) return;
    int prodId = m_productCombo->currentData().toInt();
    QSqlQuery q;
    q.prepare("SELECT name, cost_price, wholesale_dollar, retail_dollar, "
              "wholesale_dinar, retail_dinar, stock_qty, barcode FROM products WHERE id=?");
    q.addBindValue(prodId); q.exec();
    if (q.next()) {
        m_costPriceEdit->setText(QString::number(q.value(1).toDouble(), 'f', 2));
        m_wholesaleDollarEdit->setText(QString::number(q.value(2).toDouble(), 'f', 2));
        m_retailDollarEdit->setText(QString::number(q.value(3).toDouble(), 'f', 2));
        m_wholesaleDinarEdit->setText(QString::number(q.value(4).toDouble(), 'f', 0));
        m_retailDinarEdit->setText(QString::number(q.value(5).toDouble(), 'f', 0));
        m_stockLabel->setText(QString::fromUtf8("رصيد: ") + q.value(6).toString());
        m_barcodeCombo->setCurrentText(q.value(7).toString());
    }
    calculateTotals();
}

void PurchaseWindow::calculateTotals()
{
    double qty = m_qtyEdit->text().toDouble();
    double cost = m_costPriceEdit->text().toDouble();
    double rate = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();
    m_totalDollarEdit->setText(QString::number(qty * cost, 'f', 2));
    m_totalDinarEdit->setText(QString::number(qty * cost * rate, 'f', 0));
}

void PurchaseWindow::addItem()
{
    if (m_productCombo->currentText().isEmpty()) return;
    double qty   = m_qtyEdit->text().toDouble();
    double cost  = m_costPriceEdit->text().toDouble();
    double rate  = m_exchangeRateEdit->text().toDouble();
    double total = qty * cost;

    int row = m_itemsTable->rowCount();
    m_itemsTable->insertRow(row);
    m_itemsTable->setItem(row, 0, new QTableWidgetItem(m_barcodeCombo->currentText()));
    m_itemsTable->setItem(row, 1, new QTableWidgetItem(QString::number(m_productCombo->currentData().toInt())));
    m_itemsTable->setItem(row, 2, new QTableWidgetItem(m_productCombo->currentText()));
    m_itemsTable->setItem(row, 3, new QTableWidgetItem(QString::number(qty)));
    m_itemsTable->setItem(row, 4, new QTableWidgetItem(QString::number(cost, 'f', 2)));
    m_itemsTable->setItem(row, 5, new QTableWidgetItem(QString::number(total * rate, 'f', 0)));
    m_itemsTable->setItem(row, 6, new QTableWidgetItem(QString::number(cost, 'f', 2)));
    m_itemsTable->setItem(row, 7, new QTableWidgetItem(QString::number(total, 'f', 2)));
    m_itemsTable->setItem(row, 8, new QTableWidgetItem(m_retailDollarEdit->text()));

    // Update grand total
    double grand = 0;
    for (int r = 0; r < m_itemsTable->rowCount(); r++)
        grand += m_itemsTable->item(r, 7)->text().toDouble();
    m_grandTotalDollarEdit->setText(QString::number(grand, 'f', 2));
    m_grandTotalDinarEdit->setText(QString::number(grand * rate, 'f', 0));
    m_qtyEdit->setText("0");
}

void PurchaseWindow::deleteItem()
{
    int row = m_itemsTable->currentRow();
    if (row >= 0) m_itemsTable->removeRow(row);
}

void PurchaseWindow::saveInvoice()
{
    int suppId = m_supplierCombo->currentData().toInt();
    if (suppId <= 0) { QMessageBox::warning(this, "", QString::fromUtf8("اختر المجهز")); return; }
    if (m_itemsTable->rowCount() == 0) { QMessageBox::warning(this, "", QString::fromUtf8("لا توجد مواد")); return; }

    double rate = m_exchangeRateEdit->text().toDouble();
    int invId = Database::createPurchaseInvoice(
        suppId, m_dateEdit->date(),
        m_purchaseTypeCombo->currentText(),
        m_payTypeCombo->currentText(),
        m_currencyCombo->currentText(),
        rate, m_notesEdit->text(), 1);

    if (invId < 0) { QMessageBox::critical(this, "", QString::fromUtf8("فشل الحفظ")); return; }

    for (int r = 0; r < m_itemsTable->rowCount(); r++) {
        int prodId = m_itemsTable->item(r, 1)->text().toInt();
        double qty = m_itemsTable->item(r, 3)->text().toDouble();
        double cost = m_itemsTable->item(r, 6)->text().toDouble();
        double wsD = m_wholesaleDollarEdit->text().toDouble();
        double retD = m_retailDollarEdit->text().toDouble();
        Database::addPurchaseItem(invId, prodId, qty, cost, wsD, retD, 0, 0, "");
    }

    m_invoiceNoSpin->setValue(invId);
    QMessageBox::information(this, QString::fromUtf8("تم"),
                              QString::fromUtf8("تم حفظ الفاتورة رقم: ") + QString::number(invId));
}

void PurchaseWindow::clearForm()
{
    m_currentInvoiceId = -1;
    m_itemsTable->setRowCount(0);
    m_qtyEdit->setText("0");
    m_costPriceEdit->setText("0");
    m_grandTotalDollarEdit->setText("0");
    m_grandTotalDinarEdit->setText("0");
    m_notesEdit->clear();
}

void PurchaseWindow::printInvoice()
{
    QString html = PrintManager::arabicHtmlHeader();
    html += QString("<div class='company'>%1</div>").arg(Database::getCompanyName());
    html += QString("<h2>فاتورة شراء رقم: %1</h2>").arg(m_invoiceNoSpin->value());
    html += QString("<p>المجهز: %1 | التاريخ: %2</p>")
            .arg(m_supplierCombo->currentText(), m_dateEdit->date().toString("dd/MM/yyyy"));
    html += "<table><tr><th>اسم المادة</th><th>الكمية</th><th>الكلفة $</th><th>المجموع</th></tr>";
    for (int r = 0; r < m_itemsTable->rowCount(); r++) {
        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td></tr>")
                .arg(m_itemsTable->item(r, 2)->text())
                .arg(m_itemsTable->item(r, 3)->text())
                .arg(m_itemsTable->item(r, 6)->text())
                .arg(m_itemsTable->item(r, 7)->text());
    }
    html += QString("<tr class='total'><td colspan='3'>الإجمالي</td><td>%1 $</td></tr>")
            .arg(m_grandTotalDollarEdit->text());
    html += "</table></body></html>";
    m_printer->printDocument(this, html, QString::fromUtf8("فاتورة مشتريات"));
}

void PurchaseWindow::exportPDF()
{
    QString html = PrintManager::arabicHtmlHeader();
    html += QString("<div class='company'>%1</div>").arg(Database::getCompanyName());
    html += QString("<h2>فاتورة شراء رقم: %1</h2>").arg(m_invoiceNoSpin->value());
    html += "</body></html>";
    m_printer->exportToPDF(this, html, QString::fromUtf8("فاتورة_مشتريات"));
}

void PurchaseWindow::navigateFirst() {}
void PurchaseWindow::navigatePrev()  { if (m_currentInvoiceId > 1) m_currentInvoiceId--; }
void PurchaseWindow::navigateNext()  { m_currentInvoiceId++; }
void PurchaseWindow::navigateLast()  {}
