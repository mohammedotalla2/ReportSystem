#include "saleswindow.h"
#include "database.h"
#include "arabichelper.h"
#include "customerstatement.h"

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
#include <QLabel>
#include <QStyledItemDelegate>
#include <QCompleter>
#include <QAbstractItemView>
#include <QApplication>
#include <QTimer>
#include <QInputDialog>


/* ================================================================
   Constructor
   ================================================================ */
/* ================================================================
   ProductDelegate – makes the اسم المادة column (col 3) in the
   items table behave as a searchable combo / drop-list.
   ================================================================ */
class ProductDelegate : public QStyledItemDelegate
{
public:
    explicit ProductDelegate(const QStringList &names,
                             const QList<int>  &ids,
                             QObject *parent = nullptr)
        : QStyledItemDelegate(parent), m_names(names), m_ids(ids) {}

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &,
                          const QModelIndex &) const override
    {
        QComboBox *cb = new QComboBox(parent);
        cb->setEditable(true);
        cb->setInsertPolicy(QComboBox::NoInsert);
        cb->addItems(m_names);

        // Searchable completer
        QCompleter *comp = new QCompleter(m_names, cb);
        comp->setCaseSensitivity(Qt::CaseInsensitive);
        comp->setFilterMode(Qt::MatchContains);
        comp->setCompletionMode(QCompleter::PopupCompletion);
        cb->setCompleter(comp);

        // Open drop-down immediately when editor appears
        QTimer::singleShot(0, cb, &QComboBox::showPopup);
        return cb;
    }

    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override
    {
        QComboBox *cb = qobject_cast<QComboBox*>(editor);
        if (!cb) return;
        int idx = cb->findText(index.data().toString());
        if (idx >= 0) cb->setCurrentIndex(idx);
        else          cb->setEditText(index.data().toString());
    }

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex  &index) const override
    {
        QComboBox *cb = qobject_cast<QComboBox*>(editor);
        if (!cb) return;

        QString name = cb->currentText();
        int     listIdx = cb->currentIndex();

        // Write product name into col 3
        model->setData(index, name);

        // Also update رمز المادة (col 2) and product-id (col 8)
        if (listIdx >= 0 && listIdx < m_ids.size()) {
            QModelIndex codeIdx = model->index(index.row(), 2);
            QModelIndex idIdx   = model->index(index.row(), 8);
            model->setData(codeIdx, QString::number(m_ids[listIdx]));
            model->setData(idIdx,   m_ids[listIdx]);
        }
    }

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &) const override
    {
        editor->setGeometry(option.rect);
    }

private:
    QStringList m_names;
    QList<int>  m_ids;
};



SalesWindow::SalesWindow(QWidget *parent)
    : QDialog(parent), m_currentInvoiceId(-1), m_totalInvoices(0)
{
    setWindowTitle(QString::fromUtf8("المبيعات"));
    setLayoutDirection(Qt::RightToLeft);
    resize(1350, 800);
    m_printer = new PrintManager(this);
    setupUI();
    applyStyles();
    loadCustomers();
    loadProducts();
    clearForm();
}

/* ================================================================
   UI Setup
   ================================================================ */
void SalesWindow::setupUI()
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(4, 4, 4, 4);
    root->setSpacing(4);

    /* ── helper lambdas ── */
    auto lbl = [](const QString &t) -> QLabel* {
        QLabel *l = new QLabel(t);
        l->setFont(QFont("Tahoma", 9));
        l->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        return l;
    };

    /* ===================================================
       HEADER GRID
       =================================================== */
    /* ===================================================
   HEADER GRID
   =================================================== */
    QWidget *headerWidget = new QWidget;
    headerWidget->setObjectName("headerWidget");
    QGridLayout *hg = new QGridLayout(headerWidget);
    hg->setSpacing(4);
    hg->setContentsMargins(6, 4, 6, 4);

    /* invoice number */
    m_invoiceNoSpin = new QSpinBox;
    m_invoiceNoSpin->setRange(1, 9999999);
    m_invoiceNoSpin->setFont(QFont("Tahoma", 10, QFont::Bold));
    m_invoiceNoSpin->setObjectName("invoiceNoSpin");

    /* date widgets */
    m_dateEdit = new QDateEdit(QDate::currentDate());
    m_dateEdit->setDisplayFormat("d");
    QDateEdit *monthEdit = new QDateEdit(QDate::currentDate());
    monthEdit->setDisplayFormat("M");
    QDateEdit *yearEdit = new QDateEdit(QDate::currentDate());
    yearEdit->setDisplayFormat("yyyy");

    /* day name label */
    m_dayLabel = new QLabel;
    m_dayLabel->setObjectName("dayNameLbl");
    QStringList dayNames = {
        QString::fromUtf8("الأحد"), QString::fromUtf8("الاثنين"),
        QString::fromUtf8("الثلاثاء"), QString::fromUtf8("الأربعاء"),
        QString::fromUtf8("الخميس"), QString::fromUtf8("الجمعة"),
        QString::fromUtf8("السبت")
    };
    m_dayLabel->setText(dayNames[QDate::currentDate().dayOfWeek() % 7]);

    /* customer */
    m_customerCombo = new QComboBox;
    m_customerCombo->setEditable(true);
    m_customerCombo->setObjectName("customerCombo");
    m_customerCombo->setMinimumWidth(200);

    /* payment / currency / type */
    m_paymentTypeCombo = new QComboBox;
    m_paymentTypeCombo->addItems({
        QString::fromUtf8("نقدي"),
        QString::fromUtf8("آجل"),
        QString::fromUtf8("جملة")
    });
    m_currencyCombo = new QComboBox;
    m_currencyCombo->addItems({"$", QString::fromUtf8("دينار")});
    m_invoiceTypeCombo = new QComboBox;
    m_invoiceTypeCombo->addItems({
        QString::fromUtf8("مفرد"),
        QString::fromUtf8("جملة")
    });

    /* exchange rate */
    m_exchangeRateEdit = new QLineEdit(
        QString::number(Database::getExchangeRate(), 'f', 0));
    m_exchangeRateEdit->setFixedWidth(80);
    m_exchangeRateEdit->setObjectName("yellowField");

    /* receiver / notes */
    m_receiverEdit = new QLineEdit;
    m_receiverEdit->setPlaceholderText(QString::fromUtf8("المستلم"));
    m_notesEdit = new QLineEdit;
    m_notesEdit->setPlaceholderText(QString::fromUtf8("الملاحظات"));

    /* layout row 0 - FIXED INDICES */
    // Widget at 11 (Left), Label at 10 (Right)
    hg->addWidget(m_invoiceNoSpin,   0, 11);
    hg->addWidget(lbl(QString::fromUtf8("رقم القائمة")), 0, 10);
    
    hg->addWidget(yearEdit,          0, 9);
    hg->addWidget(monthEdit,         0, 8);
    hg->addWidget(m_dateEdit,        0, 7);
    hg->addWidget(m_dayLabel,        0, 6);
    hg->addWidget(lbl(QString::fromUtf8("التاريخ")),     0, 5); // Label to right of date
    
    hg->addWidget(m_customerCombo,   0, 3, 1, 2);
    hg->addWidget(lbl(QString::fromUtf8("اسم الزبون")), 0, 2);
    
    hg->addWidget(m_paymentTypeCombo,0, 1);
    hg->addWidget(lbl(QString::fromUtf8("نوع الدفع")),  0, 0);

    /* layout row 1 - FIXED INDICES */
    // Widget at 7 (Left), Label at 6 (Right)
    hg->addWidget(m_receiverEdit,     1, 7);
    hg->addWidget(lbl(QString::fromUtf8("المستلم")),      1, 6);
    
    hg->addWidget(m_exchangeRateEdit, 1, 5);
    hg->addWidget(lbl(QString::fromUtf8("سعر صرف $")),   1, 4);
    
    hg->addWidget(m_currencyCombo,    1, 3);
    hg->addWidget(lbl(QString::fromUtf8("العملة")),       1, 2);
    
    hg->addWidget(m_invoiceTypeCombo, 1, 1);
    hg->addWidget(lbl(QString::fromUtf8("نوع التسعير")), 1, 0);

    /* layout row 2 – notes */
    hg->addWidget(m_notesEdit, 2, 0, 1, 12);

    root->addWidget(headerWidget);
    /* ===================================================
       MIDDLE: table area  +  right product panel
       =================================================== */
    QHBoxLayout *mid = new QHBoxLayout;
    mid->setSpacing(4);

    /* ── left: items table area ── */
    QWidget *tableArea = new QWidget;
    tableArea->setObjectName("tableArea");
    QVBoxLayout *tvl = new QVBoxLayout(tableArea);
    tvl->setContentsMargins(2, 2, 2, 2);
    tvl->setSpacing(2);

    /* item-entry row */
    QHBoxLayout *itemRow = new QHBoxLayout;

    m_barcodeCombo = new QComboBox;
    m_barcodeCombo->setEditable(true);
    m_barcodeCombo->setFixedWidth(120);
    m_barcodeCombo->setPlaceholderText(QString::fromUtf8("الباركود"));

    m_productCombo = new QComboBox;
    m_productCombo->setEditable(true);
    m_productCombo->setMinimumWidth(200);
    m_productCombo->setPlaceholderText(QString::fromUtf8("اسم المادة"));

    m_unitCombo = new QComboBox;
    m_unitCombo->addItems({
        QString::fromUtf8("كرتونة"),
        QString::fromUtf8("قطعة")
    });
    m_unitCombo->setFixedWidth(75);

    m_qtyEdit       = new QLineEdit("0"); m_qtyEdit->setFixedWidth(60);
    m_unitPriceEdit = new QLineEdit("0"); m_unitPriceEdit->setFixedWidth(90);
    m_totalEdit     = new QLineEdit("0"); m_totalEdit->setFixedWidth(90);
    m_totalEdit->setReadOnly(true);
    m_totalEdit->setObjectName("yellowField");

    m_stockLabel = new QLabel("0");
    m_stockLabel->setObjectName("stockLbl");
    m_stockLabel->setFixedWidth(70);

    QPushButton *addBtn  = new QPushButton("✔");
    QPushButton *editBtn = new QPushButton("✎");
    QPushButton *delBtn  = new QPushButton("✖");
    for (QPushButton *b : {addBtn, editBtn, delBtn})
        b->setFixedSize(34, 28);
    addBtn->setObjectName("addItemBtn");
    delBtn->setObjectName("delItemBtn");

    itemRow->addWidget(lbl("ت"));
    itemRow->addWidget(lbl("1"));
    itemRow->addWidget(m_barcodeCombo);
    itemRow->addWidget(m_productCombo);
    itemRow->addWidget(m_unitCombo);
    itemRow->addWidget(lbl(QString::fromUtf8("الكمية")));
    itemRow->addWidget(m_qtyEdit);
    itemRow->addWidget(lbl(QString::fromUtf8("السعر")));
    itemRow->addWidget(m_unitPriceEdit);
    itemRow->addWidget(lbl(QString::fromUtf8("المجموع")));
    itemRow->addWidget(m_totalEdit);
    itemRow->addWidget(m_stockLabel);
    itemRow->addWidget(addBtn);
    itemRow->addWidget(editBtn);
    itemRow->addWidget(delBtn);
    tvl->addLayout(itemRow);

    /* items table */
    m_itemsTable = new QTableWidget(0, 8);
    m_itemsTable->setObjectName("itemsTable");
    m_itemsTable->setHorizontalHeaderLabels({
        QString::fromUtf8("ت"),
        QString::fromUtf8("الباركود"),
        QString::fromUtf8("رمز المادة"),
        QString::fromUtf8("اسم المادة"),
        QString::fromUtf8("الكمية"),
        QString::fromUtf8("سعر المفرد"),
        QString::fromUtf8("المبلغ الكلي"),
        QString::fromUtf8("الملاحظات"),
        QString::fromUtf8("العملة"),  // col 8 – visible currency
        ""                        // col 9 – hidden product-id
    });
    m_itemsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_itemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
   // m_itemsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_itemsTable->setMinimumHeight(240);
    // Allow clicking col 3 (اسم المادة) to open the searchable combo
    m_itemsTable->setEditTriggers(QAbstractItemView::DoubleClicked |
                                  QAbstractItemView::SelectedClicked);
    tvl->addWidget(m_itemsTable);
    

    connect(m_currencyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this](int) {
    double rate = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = 1.0;
    bool isDinar = (m_currencyCombo->currentText() == QString::fromUtf8("دينار"));

    for (int r = 0; r < m_itemsTable->rowCount(); ++r) {
        QTableWidgetItem *priceItem = m_itemsTable->item(r, 5);
        QTableWidgetItem *totalItem = m_itemsTable->item(r, 6);
        QTableWidgetItem *curItem   = m_itemsTable->item(r, 9); // currency col
        if (!priceItem || !totalItem) continue;

        double oldPrice = priceItem->text().toDouble();
        double newPrice = isDinar ? (oldPrice * rate) : (oldPrice / rate);

        double qty = 1.0;
        if (auto *qitem = m_itemsTable->item(r, 4))
            qty = qitem->text().toDouble();

        priceItem->setText(QString::number(newPrice, 'f', 2));
        totalItem->setText(QString::number(newPrice * qty, 'f', 2));
        if (curItem)
            curItem->setText(m_currencyCombo->currentText());
    }
    updateTotals();
});

   connect(m_itemsTable, &QTableWidget::itemChanged,
            [this](QTableWidgetItem *item) {
        if (item->column() != 3) return;   // only اسم المادة column
        int row = item->row();
        // Read product id from hidden col 8
        QTableWidgetItem *idItem = m_itemsTable->item(row, 8);
        if (!idItem) return;
        int pid = idItem->text().toInt();
        if (pid <= 0) return;

        QSqlQuery q;
        q.prepare("SELECT retail_dollar, retail_dinar, wholesale_dollar, "
                  "wholesale_dinar, stock_qty, barcode "
                  "FROM products WHERE id=?");
        q.addBindValue(pid); q.exec();
        if (!q.next()) return;

        bool isWholesale = (m_invoiceTypeCombo->currentIndex() == 1);
        double price = isWholesale ? q.value(2).toDouble() : q.value(0).toDouble();

        // Fill barcode, price, recalc total in the same row
        if (auto *bc = m_itemsTable->item(row, 1))
            bc->setText(q.value(5).toString());
        if (auto *pr = m_itemsTable->item(row, 5))
            pr->setText(QString::number(price, 'f', 2));

        // Recalculate row total
        double qty = 1.0;
        if (auto *qitem = m_itemsTable->item(row, 4))
            qty = qitem->text().toDouble();
        if (auto *tot = m_itemsTable->item(row, 6))
            tot->setText(QString::number(qty * price, 'f', 2));

        updateTotals();
    });



    /* summary rows */
    auto sumRow = [&](std::initializer_list<QWidget*> ws) {
        QHBoxLayout *r = new QHBoxLayout;
        for (QWidget *w : ws) r->addWidget(w);
        tvl->addLayout(r);
    };

    m_totalDollarLabel       = new QLabel("0"); m_totalDollarLabel->setObjectName("sumLbl");
    m_totalDinarLabel        = new QLabel("0"); m_totalDinarLabel->setObjectName("sumLbl");
    m_balDollarLabel         = new QLabel("0");
    m_balDinarLabel          = new QLabel("0");
    m_currentBalDollarLabel  = new QLabel("0");
    m_currentBalDinarLabel   = new QLabel("0");

    m_receivedDollarEdit = new QLineEdit("0"); m_receivedDollarEdit->setFixedWidth(80);
    m_receivedDinarEdit  = new QLineEdit("0"); m_receivedDinarEdit->setFixedWidth(80);
    sumRow({
        lbl(QString::fromUtf8("رصيد مادة")), new QLabel("0"),
        new QLabel(""),
        lbl(QString::fromUtf8("الواصل $")),  m_receivedDollarEdit,
        lbl(QString::fromUtf8("الواصل د")),  m_receivedDinarEdit
    });
    sumRow({
        lbl(QString::fromUtf8("الخصم دينار")), new QLineEdit("0"),
        lbl(QString::fromUtf8("مايعادل $")),   new QLabel("0"),
        lbl(QString::fromUtf8("حمالية")),       new QLabel("0"),
        lbl(QString::fromUtf8("استرجاع")),      new QLabel("0")
    });
    sumRow({
        lbl(QString::fromUtf8("رصيد الزبون")), m_balDollarLabel,
        lbl("$"), m_balDinarLabel,
        lbl(QString::fromUtf8("مايعادل دينار")), new QLabel("0"),
        lbl(QString::fromUtf8("الواصل د")), new QLabel("0")
    });
    sumRow({
        lbl(QString::fromUtf8("الرصيد الحالي")), m_currentBalDollarLabel,
        lbl("$"), m_currentBalDinarLabel, lbl(QString::fromUtf8("دينار"))
    });

    /* customer extra info */
    QGridLayout *custInfo = new QGridLayout;
    m_addressEdit = new QLineEdit; m_addressEdit->setReadOnly(true);
    m_phoneEdit   = new QLineEdit; m_phoneEdit->setReadOnly(true);
    m_driverCombo = new QComboBox; m_driverCombo->setEditable(true);
    m_carNoEdit   = new QLineEdit;
    m_operatorLabel = new QLabel("1"); m_operatorLabel->setObjectName("operLbl");
    m_timeLabel   = new QLabel(QTime::currentTime().toString("hh:mm AP"));
    m_timeLabel->setObjectName("timeLbl");

    custInfo->addWidget(lbl(QString::fromUtf8("العنوان")),     0, 0);
    custInfo->addWidget(m_addressEdit,                          0, 1);
    custInfo->addWidget(lbl(QString::fromUtf8("هاتف الزبون")),0, 2);
    custInfo->addWidget(m_phoneEdit,                            0, 3);
    custInfo->addWidget(lbl(QString::fromUtf8("اسم السائق")), 1, 0);
    custInfo->addWidget(m_driverCombo,                          1, 1);
    custInfo->addWidget(lbl(QString::fromUtf8("رقم السيارة")),1, 2);
    custInfo->addWidget(m_carNoEdit,                            1, 3);
    custInfo->addWidget(lbl(QString::fromUtf8("المنظم")),      1, 4);
    custInfo->addWidget(m_operatorLabel,                        1, 5);
    custInfo->addWidget(lbl(QString::fromUtf8("الوقت")),       1, 6);
    custInfo->addWidget(m_timeLabel,                            1, 7);
    tvl->addLayout(custInfo);

    /* ── right: product selection panel ── */
    QWidget *rightPanel = new QWidget;
    rightPanel->setObjectName("productPanel");
    rightPanel->setFixedWidth(225);
    QVBoxLayout *rvl = new QVBoxLayout(rightPanel);
    rvl->setContentsMargins(4, 4, 4, 4);
    rvl->setSpacing(4);

    QLabel *grpLbl = new QLabel(QString::fromUtf8("حسب المجموعة"));
    grpLbl->setObjectName("panelTitle");

    // جدول صغير للمجموعات بدلاً من القائمة المنسدلة
    m_groupTable = new QTableWidget(0, 1);
    m_groupTable->setObjectName("groupTable");
    m_groupTable->setHorizontalHeaderLabels({QString::fromUtf8("المجموعة")});
    m_groupTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_groupTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_groupTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_groupTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_groupTable->setMaximumHeight(140);

    QLabel *partLbl = new QLabel(QString::fromUtf8("جزء من الاسم / باركود"));
    m_partNameEdit = new QLineEdit;

    m_productList = new QTableWidget(0, 2);
    m_productList->setObjectName("productList");
    m_productList->setHorizontalHeaderLabels({
        QString::fromUtf8("اختيار"),
        QString::fromUtf8("اسم المادة")
    });
    m_productList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_productList->setSelectionBehavior(QAbstractItemView::SelectRows);

    rvl->addWidget(grpLbl);
    rvl->addWidget(m_groupTable);
    rvl->addWidget(partLbl);
    rvl->addWidget(m_partNameEdit);
    rvl->addWidget(m_productList);

    mid->addWidget(tableArea, 1);
    mid->addWidget(rightPanel);
    root->addLayout(mid, 1);

    /* ===================================================
       BOTTOM TOOLBAR
       =================================================== */
    QWidget *toolbar = new QWidget;
    toolbar->setObjectName("toolbar");
    QHBoxLayout *tbl = new QHBoxLayout(toolbar);
    tbl->setContentsMargins(4, 4, 4, 4);
    tbl->setSpacing(4);

    auto toolBtn = [](const QString &t, const QString &tip = "") -> QPushButton* {
        QPushButton *b = new QPushButton(t);
        b->setFixedSize(40, 36);
        b->setToolTip(tip);
        b->setObjectName("toolBtn");
        b->setFont(QFont("Tahoma", 13));
        return b;
    };

    m_saveBtn        = toolBtn("💾", QString::fromUtf8("حفظ"));
    m_searchBtn      = toolBtn("🔍", QString::fromUtf8("بحث"));
    m_deleteBtn      = toolBtn("✖", QString::fromUtf8("حذف"));
    m_printBtn       = toolBtn("🖨", QString::fromUtf8("طباعة"));

    m_pdfBtn = new QPushButton("PDF");
    m_pdfBtn->setObjectName("pdfBtn");
    m_pdfBtn->setFixedHeight(36);
    m_pdfBtn->setFont(QFont("Tahoma", 10, QFont::Bold));

    m_thermalBtn = new QPushButton(QString::fromUtf8("الطابعة الشريطية"));
    m_thermalBtn->setObjectName("thermalBtn");
    m_thermalBtn->setFixedHeight(36);

    m_waitBtn = new QPushButton(QString::fromUtf8("انتظار"));
    m_waitBtn->setFixedHeight(36);
    m_waitBtn->setObjectName("waitBtn");

    m_retrieveBtn = new QPushButton(QString::fromUtf8("استرجاع"));
    m_retrieveBtn->setFixedHeight(36);
    m_retrieveBtn->setObjectName("auxBtn");

    m_prepListBtn = new QPushButton(QString::fromUtf8("قائمة تجهيز"));
    m_prepListBtn->setFixedHeight(36);
    m_prepListBtn->setObjectName("auxBtn");

    m_customerStatBtn = new QPushButton(QString::fromUtf8("كشف حساب"));
    m_customerStatBtn->setFixedHeight(36);
    m_customerStatBtn->setObjectName("auxBtn");

    m_productMoveBtn = new QPushButton(QString::fromUtf8("حركة مادة"));
    m_productMoveBtn->setFixedHeight(36);
    m_productMoveBtn->setObjectName("auxBtn");

    m_copiesSpinBox = new QSpinBox;
    m_copiesSpinBox->setRange(1, 99);
    m_copiesSpinBox->setValue(1);
    m_copiesSpinBox->setFixedWidth(50);

    /* navigation */
    m_firstBtn = toolBtn("⏮");
    m_lastBtn  = toolBtn("⏭");
    m_navLabel = new QLabel("1");
    m_navLabel->setObjectName("navLbl");
    m_navLabel->setAlignment(Qt::AlignCenter);
    m_navLabel->setFixedWidth(45);
    m_totalCountLabel = new QLabel("1");
    m_totalCountLabel->setAlignment(Qt::AlignCenter);
    m_totalCountLabel->setFixedWidth(45);

    /* receipt / sign buttons (right side of toolbar) */
    QPushButton *recvBtn = toolBtn("🤲", QString::fromUtf8("قبض"));
    QPushButton *viewBtn = toolBtn("👁",  QString::fromUtf8("معاينة"));
    QPushButton *fwdBtn  = toolBtn("➡",  QString::fromUtf8("إرسال"));
    QPushButton *sigBtn  = toolBtn("✋",  QString::fromUtf8("توقيع"));

    tbl->addWidget(m_saveBtn);
    tbl->addWidget(m_waitBtn);
    tbl->addWidget(m_retrieveBtn);
    tbl->addWidget(m_searchBtn);
    tbl->addWidget(m_deleteBtn);
    tbl->addWidget(m_printBtn);
    tbl->addWidget(lbl(QString::fromUtf8("عدد النسخ")));
    tbl->addWidget(m_copiesSpinBox);
    tbl->addWidget(new QCheckBox(QString::fromUtf8("طبعت")));
    tbl->addWidget(m_thermalBtn);
    tbl->addWidget(m_prepListBtn);
    tbl->addWidget(m_customerStatBtn);
    tbl->addWidget(m_productMoveBtn);
    tbl->addWidget(m_pdfBtn);
    tbl->addStretch();
    tbl->addWidget(m_firstBtn);
    tbl->addWidget(m_navLabel);
    tbl->addWidget(lbl(QString::fromUtf8("من")));
    tbl->addWidget(m_totalCountLabel);
    tbl->addWidget(m_lastBtn);
    tbl->addWidget(recvBtn);
    tbl->addWidget(viewBtn);
    tbl->addWidget(fwdBtn);
    tbl->addWidget(sigBtn);

    root->addWidget(toolbar);

    /* ── Signal connections ── */
    connect(addBtn,           &QPushButton::clicked, this, &SalesWindow::addItem);
    connect(delBtn,           &QPushButton::clicked, this, &SalesWindow::deleteItem);
    connect(m_saveBtn,        &QPushButton::clicked, this, &SalesWindow::saveInvoice);
    connect(m_printBtn,       &QPushButton::clicked, this, &SalesWindow::printInvoice);
    connect(m_pdfBtn,         &QPushButton::clicked, this, &SalesWindow::exportPDF);
    connect(m_thermalBtn,     &QPushButton::clicked, this, &SalesWindow::printThermal);
    connect(m_prepListBtn,    &QPushButton::clicked, this, &SalesWindow::printPreparationList);
    connect(m_customerStatBtn,&QPushButton::clicked, this, &SalesWindow::showCustomerStatement);
    connect(m_firstBtn,       &QPushButton::clicked, this, &SalesWindow::navigateFirst);
    connect(m_lastBtn,        &QPushButton::clicked, this, &SalesWindow::navigateLast);
    connect(m_waitBtn,        &QPushButton::clicked, this, &SalesWindow::saveToWaiting);
    connect(m_retrieveBtn,    &QPushButton::clicked, this, &SalesWindow::retrieveFromWaiting);

    connect(m_customerCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SalesWindow::onCustomerSelected);
    connect(m_productCombo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SalesWindow::onProductSelected);
    connect(m_qtyEdit,        &QLineEdit::textChanged, this, &SalesWindow::onQtyChanged);
    connect(m_unitPriceEdit,  &QLineEdit::textChanged, this, &SalesWindow::onQtyChanged);

    /* helper lambda: apply both filters */
    auto applyFilters = [this]() {
        QString txt = m_partNameEdit->text();
        QString grp;
        auto sel = m_groupTable->selectedItems();
        if (!sel.isEmpty()) grp = sel.first()->text();
        bool allGroups = (grp == QString::fromUtf8("كل الاصناف") || grp.isEmpty());
        for (int r = 0; r < m_productList->rowCount(); ++r) {
            auto *nm = m_productList->item(r, 1);
            if (!nm) continue;
            bool nameMatch = txt.isEmpty() || nm->text().contains(txt, Qt::CaseInsensitive);
            QString itemGrp = nm->data(Qt::UserRole + 1).toString();
            bool grpMatch = allGroups || (itemGrp == grp);
            m_productList->setRowHidden(r, !(nameMatch && grpMatch));
        }
    };

    /* filter by name/barcode */
    connect(m_partNameEdit, &QLineEdit::textChanged, [applyFilters](const QString &) {
        applyFilters();
    });

    /* filter by group table selection */
    connect(m_groupTable, &QTableWidget::itemSelectionChanged, [applyFilters]() {
        applyFilters();
    });

    /* select product from right panel */
    connect(m_productList, &QTableWidget::itemDoubleClicked,
            [this](QTableWidgetItem *item) {
        int row = item->row();
        QTableWidgetItem *nameItem = m_productList->item(row, 1);
        if (!nameItem) return;
        int idx = m_productCombo->findText(nameItem->text());
        if (idx >= 0) m_productCombo->setCurrentIndex(idx);
    });
}

/* ================================================================
   Styles – matches the screenshot colour palette
   ================================================================ */
void SalesWindow::applyStyles()
{
    setStyleSheet(R"(
        QDialog           { background: #d0d8e8; }
        #headerWidget     { background: #c0ccdd; border-bottom:1px solid #8899aa; border-radius:4px; }
        #tableArea        { background: #b8c8d8; }

        #itemsTable { background: white; gridline-color:#aaa; }
        #itemsTable QHeaderView::section {
            background:#8b0000; color:#FFD700; font-weight:bold;
            padding:4px; border:1px solid #600000; font-family:Tahoma;
        }

        #productPanel { background:#c8d8e8; border-left:1px solid #8899aa; }
        #panelTitle   { color:#003366; font-weight:bold; font-size:10pt; }
        #productList QHeaderView::section {
            background:#003388; color:white; font-weight:bold; padding:3px;
        }
        #groupTable QHeaderView::section {
            background:#005522; color:white; font-weight:bold; padding:3px;
        }
        #groupTable { background:white; selection-background-color:#c8f0d8; }

        #toolbar      { background:#556677; border-top:1px solid #334455; }
        #toolBtn      { background:#778899; border:1px solid #556; color:white; border-radius:3px; }
        #toolBtn:hover{ background:#889aab; }
        #pdfBtn       { background:#cc2200; color:white; font-weight:bold; border-radius:3px; padding:0 8px; }
        #thermalBtn   { background:#ff6666; color:white; font-weight:bold; border-radius:3px; padding:0 6px; }
        #auxBtn       { background:#c8d8f0; border:1px solid #88aacc; border-radius:3px; color:#003366; padding:0 6px; }
        #waitBtn      { background:#ff9900; color:white; font-weight:bold; border-radius:3px; border:1px solid #cc7700; padding:0 6px; }
        #navLbl       { background:#FFD700; color:#003366; font-weight:bold; border:1px solid #aa8800; border-radius:2px; }
        #operLbl      { background:#cc88ff; color:#330033; font-weight:bold; padding:2px 8px; border-radius:3px; }
        #timeLbl      { background:#ffffc0; color:#003366; font-weight:bold; padding:2px 6px; border:1px solid #aaaa60; border-radius:2px; }

        #yellowField  { background:#ffffc0; border:1px solid #aaaa60; font-weight:bold; }
        #stockLbl     { color:#003366; font-weight:bold; padding:2px 4px; background:#e8f4ff; border:1px solid #99bbdd; border-radius:2px; }
        #sumLbl       { background:#ffffc0; color:#003366; font-weight:bold; padding:2px 8px; border:1px solid #aaaa60; border-radius:2px; }
        #addItemBtn   { background:#44aa44; color:white; border-radius:3px; border:1px solid #228822; }
        #delItemBtn   { background:#aa4422; color:white; border-radius:3px; border:1px solid #882200; }

        QComboBox     { background:#ffffc0; border:1px solid #aaaa60; border-radius:2px; }
        QLineEdit     { background:#eef4ff; border:1px solid #99aacc; border-radius:2px; padding:1px 3px; }
        QDateEdit     { background:white; border:1px solid #99aacc; border-radius:2px; }
        QLabel        { font-family:Tahoma; color:#003366; }
    )");
}

/* ================================================================
   Data helpers
   ================================================================ */
void SalesWindow::loadCustomers()
{
    m_customerCombo->clear();
    m_customerCombo->addItem(QString::fromUtf8("-- اختر الزبون --"), -1);
    QSqlQuery q = Database::getCustomers();
    while (q.next())
        m_customerCombo->addItem(q.value(1).toString(), q.value(0));
}

void SalesWindow::loadProducts()
{
    m_productCombo->clear();
    m_barcodeCombo->clear();
    m_productList->setRowCount(0);

    // Reload group table
    m_groupTable->setRowCount(0);
    {
        int gr = 0;
        m_groupTable->insertRow(gr);
        m_groupTable->setItem(gr++, 0, new QTableWidgetItem(QString::fromUtf8("كل الاصناف")));
        for (const QString &g : Database::getProductGroups()) {
            m_groupTable->insertRow(gr);
            m_groupTable->setItem(gr++, 0, new QTableWidgetItem(g));
        }
        m_groupTable->selectRow(0); // select "كل الاصناف" by default
    }

    QSqlQuery q = Database::getProducts();
    int row = 0;
    while (q.next()) {
        m_productCombo->addItem(q.value(2).toString(), q.value(0));
        m_barcodeCombo->addItem(q.value(1).toString(), q.value(0));

        m_productList->insertRow(row);
        QTableWidgetItem *chk = new QTableWidgetItem;
        chk->setCheckState(Qt::Unchecked);
        m_productList->setItem(row, 0, chk);
        QTableWidgetItem *nm = new QTableWidgetItem(q.value(2).toString());
        nm->setData(Qt::UserRole, q.value(0));             // product id
        nm->setData(Qt::UserRole + 1, q.value(3).toString()); // product_group
        m_productList->setItem(row, 1, nm);
        ++row;
    }

    // Rebuild the delegate AFTER the loop, once all products are loaded
    QStringList names;
    QList<int>  ids;
    for (int i = 0; i < m_productCombo->count(); ++i) {
        names << m_productCombo->itemText(i);
        ids   << m_productCombo->itemData(i).toInt();
    }
    m_itemsTable->setItemDelegateForColumn(3, new ProductDelegate(names, ids, m_itemsTable));
}
void SalesWindow::clearForm()
{
    m_currentInvoiceId = -1;
    m_itemsTable->setRowCount(0);
    if (m_customerCombo->count() > 0) m_customerCombo->setCurrentIndex(0);
    m_dateEdit->setDate(QDate::currentDate());
    m_notesEdit->clear();
    m_qtyEdit->setText("0");
    m_unitPriceEdit->setText("0");
    m_totalEdit->setText("0");
    m_navLabel->setText("0");
}

void SalesWindow::loadInvoice(int id)
{
    QSqlQuery q;
    q.prepare("SELECT customer_id, date, payment_type, currency, notes "
              "FROM sales_invoices WHERE id=?");
    q.addBindValue(id); q.exec();
    if (!q.next()) return;

    m_currentInvoiceId = id;
    int custId = q.value(0).toInt();
    for (int i = 0; i < m_customerCombo->count(); ++i) {
        if (m_customerCombo->itemData(i).toInt() == custId) {
            m_customerCombo->setCurrentIndex(i); break;
        }
    }
    m_dateEdit->setDate(QDate::fromString(q.value(1).toString(), "yyyy-MM-dd"));
    m_notesEdit->setText(q.value(4).toString());
    m_invoiceNoSpin->setValue(id);
    m_navLabel->setText(QString::number(id));

    m_itemsTable->setRowCount(0);
    QSqlQuery qi;
    qi.prepare("SELECT p.name, si.qty, si.unit_price_dollar, si.total_dollar, "
               "p.barcode, si.product_id "
               "FROM sales_items si "
               "LEFT JOIN products p ON si.product_id = p.id "
               "WHERE si.invoice_id = ?");
    qi.addBindValue(id); qi.exec();
    int row = 0;
    while (qi.next()) {
        m_itemsTable->insertRow(row);
        m_itemsTable->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
        m_itemsTable->setItem(row, 1, new QTableWidgetItem(qi.value(4).toString()));
        m_itemsTable->setItem(row, 2, new QTableWidgetItem(QString::number(qi.value(5).toInt())));
        m_itemsTable->setItem(row, 3, new QTableWidgetItem(qi.value(0).toString()));
        m_itemsTable->setItem(row, 4, new QTableWidgetItem(QString::number(qi.value(1).toDouble())));
        m_itemsTable->setItem(row, 5, new QTableWidgetItem(QString::number(qi.value(2).toDouble(),'f',2)));
        m_itemsTable->setItem(row, 6, new QTableWidgetItem(QString::number(qi.value(3).toDouble(),'f',2)));
        m_itemsTable->setItem(row, 7, new QTableWidgetItem(""));
        ++row;
    }
    updateTotals();
}

/* ================================================================
   Slots
   ================================================================ */
void SalesWindow::onProductSelected(int index)
{
    if (index < 0) return;
    int pid = m_productCombo->currentData().toInt();
    QSqlQuery q;
    q.prepare("SELECT retail_dollar, retail_dinar, wholesale_dollar, wholesale_dinar, "
              "stock_qty, barcode FROM products WHERE id=?");
    q.addBindValue(pid); q.exec();
    if (!q.next()) return;

    bool isWholesale = (m_invoiceTypeCombo->currentIndex() == 1);
    double price = isWholesale ? q.value(2).toDouble() : q.value(0).toDouble();
    m_unitPriceEdit->setText(QString::number(price, 'f', 2));
    m_stockLabel->setText(q.value(4).toString());
    m_barcodeCombo->setCurrentText(q.value(5).toString());
    onQtyChanged();
}

void SalesWindow::onCustomerSelected(int index)
{
    if (index <= 0) return;
    int cid = m_customerCombo->currentData().toInt();
    QSqlQuery q;
    q.prepare("SELECT balance_dollar, balance_dinar, address, phone "
              "FROM customers WHERE id=?");
    q.addBindValue(cid); q.exec();
    if (!q.next()) return;
    m_balDollarLabel->setText(QString::number(q.value(0).toDouble(),'f',2));
    m_balDinarLabel->setText(QString::number(q.value(1).toDouble(),'f',0));
    m_addressEdit->setText(q.value(2).toString());
    m_phoneEdit->setText(q.value(3).toString());
}

void SalesWindow::onQtyChanged()
{
    double qty   = m_qtyEdit->text().toDouble();
    double price = m_unitPriceEdit->text().toDouble();
    m_totalEdit->setText(QString::number(qty * price, 'f', 2));
}

void SalesWindow::addItem()
{
    if (m_productCombo->currentText().isEmpty()) return;
    double qty   = m_qtyEdit->text().toDouble();
    double price = m_unitPriceEdit->text().toDouble();
    double total = qty * price;

    int row = m_itemsTable->rowCount();
    m_itemsTable->insertRow(row);
    m_itemsTable->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
    m_itemsTable->setItem(row, 1, new QTableWidgetItem(m_barcodeCombo->currentText()));
    m_itemsTable->setItem(row, 2, new QTableWidgetItem(
        QString::number(m_productCombo->currentData().toInt())));
    m_itemsTable->setItem(row, 3, new QTableWidgetItem(m_productCombo->currentText()));
    m_itemsTable->setItem(row, 4, new QTableWidgetItem(QString::number(qty)));
    m_itemsTable->setItem(row, 5, new QTableWidgetItem(QString::number(price,'f',2)));
    m_itemsTable->setItem(row, 6, new QTableWidgetItem(QString::number(total,'f',2)));
    m_itemsTable->setItem(row, 7, new QTableWidgetItem(""));
   m_itemsTable->setItem(row, 8, new QTableWidgetItem(m_currencyCombo->currentText()));
   m_itemsTable->setItem(row, 9, new QTableWidgetItem(
    QString::number(m_productCombo->currentData().toInt())));
    m_qtyEdit->setText("0");
    updateTotals();
}

void SalesWindow::deleteItem()
{
    int row = m_itemsTable->currentRow();
    if (row >= 0) { m_itemsTable->removeRow(row); updateTotals(); }
}

void SalesWindow::updateTotals()
{
    double total = 0;
    for (int r = 0; r < m_itemsTable->rowCount(); ++r)
        total += m_itemsTable->item(r, 6)->text().toDouble();
    m_totalDollarLabel->setText(QString::number(total,'f',2));
    double rate = m_exchangeRateEdit->text().toDouble();
    m_totalDinarLabel->setText(QString::number(total * rate,'f',0));
}

void SalesWindow::saveInvoice()
{
    int custId = m_customerCombo->currentData().toInt();
    QString customerName = m_customerCombo->currentText().trimmed();
    
    // If no valid customer selected, check if name was typed
    if (custId <= 0) {
        if (customerName.isEmpty()) {
            QMessageBox::warning(this, "", QString::fromUtf8("أدخل اسم الزبون"));
            m_customerCombo->setFocus();
            return;
        }
        
        // Ask if user wants to create new customer
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, 
            QString::fromUtf8("زبون جديد"),
            QString::fromUtf8("الزبون '%1' غير موجود. هل تريد إضافته؟").arg(customerName),
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            // Create new customer using existing addCustomer method
            custId = Database::addCustomer(
                customerName,           // name
                "",                     // region (empty for now)
                m_addressEdit->text(),  // address
                m_phoneEdit->text(),    // phone
                "",                     // notes
                0, 0,                   // balance_dollar, balance_dinar
                0                       // type (0=person, 1=company)
            );
            
            if (custId < 0) {
                QMessageBox::critical(this, "", 
                    QString::fromUtf8("فشل إضافة الزبون الجديد"));
                return;
            }
            
            // Reload customers and select the new one
            loadCustomers();
            for (int i = 0; i < m_customerCombo->count(); ++i) {
                if (m_customerCombo->itemData(i).toInt() == custId) {
                    m_customerCombo->setCurrentIndex(i);
                    break;
                }
            }
        } else {
            return; // User cancelled
        }
    }
    
    if (m_itemsTable->rowCount() == 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("لا توجد مواد في الفاتورة"));
        return;
    }

    double total = m_totalDollarLabel->text().toDouble();
    double totalDinar = m_totalDinarLabel->text().toDouble();

    int id = Database::createSalesInvoice(
        custId, m_dateEdit->date(),
        m_paymentTypeCombo->currentText(),
        m_currencyCombo->currentText(),
        m_notesEdit->text(),
        total, totalDinar, 1);

    if (id < 0) {
        QMessageBox::critical(this, "", QString::fromUtf8("فشل حفظ الفاتورة"));
        return;
    }

    for (int r = 0; r < m_itemsTable->rowCount(); ++r) {
        int    pid   = m_itemsTable->item(r, 2)->text().toInt();
        double qty   = m_itemsTable->item(r, 4)->text().toDouble();
        double price = m_itemsTable->item(r, 5)->text().toDouble();
        double tot   = m_itemsTable->item(r, 6)->text().toDouble();
        Database::addSalesItem(id, pid, qty, price, 0, tot, 0, "");
    }

    m_currentInvoiceId = id;
    m_invoiceNoSpin->setValue(id);
    m_navLabel->setText(QString::number(id));

    // ── معالجة النقص (Shortage handling) ──
    double invoiceTotal = m_totalDollarLabel->text().toDouble();
    double receivedDollar = m_receivedDollarEdit->text().toDouble();
    double receivedDinar  = m_receivedDinarEdit->text().toDouble();
    double rate = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = Database::getExchangeRate();
    bool isDinar = (m_currencyCombo->currentText() == QString::fromUtf8("دينار"));

    // حساب المبلغ المستلم بعملة الفاتورة
    double totalReceived = isDinar
        ? (receivedDinar + receivedDollar * rate)
        : (receivedDollar + receivedDinar / rate);
    double shortage = invoiceTotal - totalReceived;

    if (shortage > 0.001) {
        // خصم النقص من رصيد الزبون
        QSqlQuery qBal;
        qBal.prepare("SELECT balance_dollar, balance_dinar FROM customers WHERE id=?");
        qBal.addBindValue(custId); qBal.exec();
        if (qBal.next()) {
            double balD  = qBal.value(0).toDouble();
            double balDn = qBal.value(1).toDouble();
            double newBalD, newBalDn;
            if (isDinar) {
                newBalDn = balDn - shortage;
                newBalD  = balD;
            } else {
                newBalD  = balD - shortage;
                newBalDn = balDn;
            }
            QSqlQuery qUpd;
            qUpd.prepare("UPDATE customers SET balance_dollar=?, balance_dinar=? WHERE id=?");
            qUpd.addBindValue(newBalD);
            qUpd.addBindValue(newBalDn);
            qUpd.addBindValue(custId);
            qUpd.exec();
            // تحديث عرض الرصيد
            m_balDollarLabel->setText(QString::number(newBalD,'f',2));
            m_balDinarLabel->setText(QString::number(newBalDn,'f',0));
        }
    }

    QMessageBox::information(this, QString::fromUtf8("تم"),
        QString::fromUtf8("تم حفظ الفاتورة رقم: ") + QString::number(id) +
        (shortage > 0.001 ? QString::fromUtf8("\nمتبقي على الزبون: ") +
            QString::number(shortage,'f',2) : ""));
}

void SalesWindow::printInvoice()
{
    if (m_currentInvoiceId < 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("احفظ الفاتورة أولاً"));
        return;
    }
    QString html = PrintManager::generateSalesInvoiceHtml(
        m_currentInvoiceId,
        Database::getCompanyName(),
        Database::getExchangeRate());
    m_printer->printPreview(this, html, QString::fromUtf8("فاتورة مبيعات"));
}

void SalesWindow::exportPDF()
{
    if (m_currentInvoiceId < 0) return;
    QString html = PrintManager::generateSalesInvoiceHtml(
        m_currentInvoiceId,
        Database::getCompanyName(),
        Database::getExchangeRate());
    m_printer->exportToPDF(this, html, QString::fromUtf8("فاتورة_مبيعات"));
}

void SalesWindow::printThermal()
{
    QStringList lines;
    lines << "**" + Database::getCompanyName();
    lines << "---";
    lines << QString::fromUtf8("فاتورة رقم: ") + QString::number(m_invoiceNoSpin->value());
    lines << QString::fromUtf8("الزبون: ")      + m_customerCombo->currentText();
    lines << QString::fromUtf8("التاريخ: ")     + m_dateEdit->date().toString("dd/MM/yyyy");
    lines << "---";
    for (int r = 0; r < m_itemsTable->rowCount(); ++r) {
        lines << m_itemsTable->item(r, 3)->text()
                 + " x" + m_itemsTable->item(r, 4)->text()
                 + " = " + m_itemsTable->item(r, 6)->text();
    }
    lines << "---";
    lines << QString::fromUtf8("الإجمالي: ") + m_totalDollarLabel->text() + " $";
    lines << "**" + QString::fromUtf8("شكراً لتعاملكم معنا");
    m_printer->printThermalReceipt(this, lines);
}

void SalesWindow::printPreparationList()
{
    /* Same as normal print but titled "قائمة تجهيز" */
    printInvoice();
}

void SalesWindow::showCustomerStatement()
{
    int custId = m_customerCombo->currentData().toInt();
    if (custId <= 0) return;
    CustomerStatement *dlg = new CustomerStatement(this);
    dlg->show();
}

void SalesWindow::navigateFirst()
{
    QSqlQuery q;
    q.exec("SELECT MIN(id) FROM sales_invoices");
    if (q.next() && !q.value(0).isNull()) loadInvoice(q.value(0).toInt());
}

void SalesWindow::navigateLast()
{
    QSqlQuery q;
    q.exec("SELECT MAX(id) FROM sales_invoices");
    if (q.next() && !q.value(0).isNull()) loadInvoice(q.value(0).toInt());
}

void SalesWindow::saveToWaiting()
{
    if (m_itemsTable->rowCount() == 0) {
        QMessageBox::information(this, "", QString::fromUtf8("لا توجد مواد في الفاتورة"));
        return;
    }
    WaitingInvoice wi;
    wi.customerId   = m_customerCombo->currentData().toInt();
    wi.customerName = m_customerCombo->currentText();
    wi.currency     = m_currencyCombo->currentText();
    wi.paymentType  = m_paymentTypeCombo->currentText();
    wi.notes        = m_notesEdit->text();
    for (int r = 0; r < m_itemsTable->rowCount(); ++r) {
        QStringList row;
        for (int c = 0; c < m_itemsTable->columnCount(); ++c)
            row << (m_itemsTable->item(r,c) ? m_itemsTable->item(r,c)->text() : "");
        wi.items << row;
    }
    m_waitingList << wi;
    clearForm();
    QMessageBox::information(this, QString::fromUtf8("انتظار"),
        QString::fromUtf8("تم حفظ الفاتورة في الانتظار. الفواتير المعلقة: ") +
        QString::number(m_waitingList.size()));
}

void SalesWindow::retrieveFromWaiting()
{
    if (m_waitingList.isEmpty()) {
        QMessageBox::information(this, "", QString::fromUtf8("لا توجد فواتير في الانتظار"));
        return;
    }
    // Build selection list
    QStringList choices;
    for (int i = 0; i < m_waitingList.size(); ++i)
        choices << QString::fromUtf8("الزبون: ") + m_waitingList[i].customerName +
                   QString::fromUtf8(" | مواد: ") + QString::number(m_waitingList[i].items.size());

    bool ok = false;
    QString chosen = QInputDialog::getItem(this,
        QString::fromUtf8("فواتير الانتظار"),
        QString::fromUtf8("اختر فاتورة للاسترجاع:"),
        choices, 0, false, &ok);
    if (!ok) return;

    int idx = choices.indexOf(chosen);
    if (idx < 0 || idx >= m_waitingList.size()) return;

    const WaitingInvoice &wi = m_waitingList[idx];
    clearForm();

    // Restore customer
    for (int i = 0; i < m_customerCombo->count(); ++i) {
        if (m_customerCombo->itemData(i).toInt() == wi.customerId) {
            m_customerCombo->setCurrentIndex(i); break;
        }
    }
    // Restore combos
    m_currencyCombo->setCurrentText(wi.currency);
    m_paymentTypeCombo->setCurrentText(wi.paymentType);
    m_notesEdit->setText(wi.notes);

    // Restore table rows
    m_itemsTable->setRowCount(0);
    for (const QStringList &row : wi.items) {
        int r = m_itemsTable->rowCount();
        m_itemsTable->insertRow(r);
        for (int c = 0; c < row.size() && c < m_itemsTable->columnCount(); ++c)
            m_itemsTable->setItem(r, c, new QTableWidgetItem(row[c]));
    }
    m_waitingList.removeAt(idx);
    updateTotals();
}
