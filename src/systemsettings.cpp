#include "systemsettings.h"
#include "database.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QFont>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSpinBox>
#include <QScrollArea>
#include <QKeyEvent>
#include <QTimer>
#include <QFrame>
#include <QDateTime>

SystemSettings::SystemSettings(QWidget *parent) : QDialog(parent),
    m_editingCustomerId(-1), m_editingProductId(-1)
{
    setWindowTitle(QString::fromUtf8("أعداد النظام"));
    setLayoutDirection(Qt::RightToLeft);
    resize(900, 650);
    setupUI();
    applyStyles();
    loadCustomers();
    loadProducts();
    loadProductGroups();
    updateNextCustomerId();
    updateNextProductId();

    m_companyNameEdit->setText(Database::getCompanyName());
    m_exchangeRateSpin->setValue(Database::getExchangeRate());

    // Populate live treasury balances
    auto refreshLiveBal = [this]() {
        QSqlQuery q;
        q.exec("SELECT "
               "SUM(CASE WHEN type IN (1,2) THEN amount_dollar ELSE -amount_dollar END),"
               "SUM(CASE WHEN type IN (1,3) THEN amount_dinar  ELSE -amount_dinar  END) "
               "FROM cash_transactions");
        double balD = 0, balDn = 0;
        if (q.next()) { balD = q.value(0).toDouble(); balDn = q.value(1).toDouble(); }
        m_liveBalDollarLabel->setText(QString::number(balD,  'f', 2));
        m_liveBalDinarLabel->setText(QString::number(balDn, 'f', 0));
    };
    refreshLiveBal();

    connect(m_tabs, &QTabWidget::currentChanged, this, [this, refreshLiveBal](int idx) {
        onTabChanged(idx);
        if (idx == 0) refreshLiveBal();   // refresh balances when settings tab shown
    });
}

// ── Static helper ──────────────────────────────────────────────────────────
QLabel *SystemSettings::makeLabel(const QString &text)
{
    QLabel *l = new QLabel(text);
    l->setFont(QFont("Tahoma", 10));
    l->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    return l;
}

// ── Event-filter helpers ───────────────────────────────────────────────────
void SystemSettings::installFocusClear(QWidget *w)
{
    w->installEventFilter(this);
}

void SystemSettings::installEnterNav(QList<QWidget*> &order)
{
    for (QWidget *w : order)
        w->installEventFilter(this);
}

bool SystemSettings::eventFilter(QObject *obj, QEvent *event)
{
    // Enter key → advance to next widget in tab-order list
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter) {
            QWidget *w = qobject_cast<QWidget*>(obj);
            auto advance = [&](QList<QWidget*> &order) -> bool {
                int idx = order.indexOf(w);
                if (idx >= 0 && idx < order.size() - 1) {
                    order[idx + 1]->setFocus();
                    return true;
                }
                return false;
            };
            if (advance(m_custTabOrder)) return true;
            if (advance(m_prodTabOrder)) return true;
        }
    }
    // FocusIn on spinbox with value 0 → select all
    if (event->type() == QEvent::FocusIn) {
        if (auto *spin = qobject_cast<QDoubleSpinBox*>(obj)) {
            if (spin->value() == 0.0)
                QTimer::singleShot(0, spin, &QDoubleSpinBox::selectAll);
        } else if (auto *spin = qobject_cast<QSpinBox*>(obj)) {
            if (spin->value() == 0)
                QTimer::singleShot(0, spin, &QSpinBox::selectAll);
        }
    }
    return QDialog::eventFilter(obj, event);
}

// ── UI Construction ────────────────────────────────────────────────────────
void SystemSettings::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(6);

    m_tabs = new QTabWidget;
    m_tabs->setLayoutDirection(Qt::RightToLeft);

    // ===== TAB 1: General Settings =====
    QWidget *settingsTab = new QWidget;
    QGridLayout *settingsLayout = new QGridLayout(settingsTab);
    settingsLayout->setSpacing(8);
    settingsLayout->setContentsMargins(12, 12, 12, 12);

    m_companyNameEdit = new QLineEdit;
    m_companyNameEdit->setFont(QFont("Tahoma", 12, QFont::Bold));

    m_exchangeRateSpin = new QDoubleSpinBox;
    m_exchangeRateSpin->setRange(1, 99999);
    m_exchangeRateSpin->setDecimals(0);
    m_exchangeRateSpin->setFont(QFont("Tahoma", 12, QFont::Bold));
    m_exchangeRateSpin->setObjectName("rateSpin");

    m_cashboxDollarEdit = new QLineEdit("0");
    m_cashboxDinarEdit  = new QLineEdit("0");

    // RTL: col 0 = rightmost (label side), col 1 = input side
    settingsLayout->addWidget(makeLabel(QString::fromUtf8("اسم الشركة / المحل:")),    0, 0);
    settingsLayout->addWidget(m_companyNameEdit,                                        0, 1);
    settingsLayout->addWidget(makeLabel(QString::fromUtf8("سعر صرف الدولار:")),        1, 0);
    settingsLayout->addWidget(m_exchangeRateSpin,                                       1, 1);
    settingsLayout->addWidget(makeLabel(QString::fromUtf8("رصيد الصندوق الأولي $:")),  2, 0);
    settingsLayout->addWidget(m_cashboxDollarEdit,                                      2, 1);
    settingsLayout->addWidget(makeLabel(QString::fromUtf8("رصيد الصندوق الأولي دينار:")), 3, 0);
    settingsLayout->addWidget(m_cashboxDinarEdit,                                       3, 1);

    // ── Live treasury balance card ────────────────────────────────
    QWidget *balCard = new QWidget;
    balCard->setObjectName("balCard");
    QVBoxLayout *balCardVL = new QVBoxLayout(balCard);
    balCardVL->setContentsMargins(14, 10, 14, 10);
    balCardVL->setSpacing(8);

    QLabel *balCardTitle = new QLabel(QString::fromUtf8("رصيد الخزانة الحالي"));
    balCardTitle->setObjectName("balCardTitle");
    balCardTitle->setFont(QFont("Tahoma", 10, QFont::Bold));
    balCardTitle->setAlignment(Qt::AlignCenter);
    balCardVL->addWidget(balCardTitle);

    // Two side-by-side tiles (dollar | dinar)
    QHBoxLayout *tilesRow = new QHBoxLayout;
    tilesRow->setSpacing(16);

    auto makeTile = [](const QString &titleText, const QString &objName) -> QWidget* {
        QWidget *tile = new QWidget;
        tile->setObjectName(objName);
        QVBoxLayout *tl = new QVBoxLayout(tile);
        tl->setContentsMargins(12, 10, 12, 10);
        tl->setSpacing(4);
        QLabel *ttl = new QLabel(titleText);
        ttl->setObjectName("tileTitle");
        ttl->setFont(QFont("Tahoma", 9, QFont::Bold));
        ttl->setAlignment(Qt::AlignCenter);
        tl->addWidget(ttl);
        return tile;
    };

    QWidget *dollarTile = makeTile(QString::fromUtf8("الدولار  $"), "dollarTile");
    m_liveBalDollarLabel = new QLabel("0.00");
    m_liveBalDollarLabel->setObjectName("dollarBalLbl");
    m_liveBalDollarLabel->setFont(QFont("Tahoma", 22, QFont::Bold));
    m_liveBalDollarLabel->setAlignment(Qt::AlignCenter);
    dollarTile->layout()->addWidget(m_liveBalDollarLabel);

    QWidget *dinarTile = makeTile(QString::fromUtf8("الدينار  د"), "dinarTile");
    m_liveBalDinarLabel = new QLabel("0");
    m_liveBalDinarLabel->setObjectName("dinarBalLbl");
    m_liveBalDinarLabel->setFont(QFont("Tahoma", 22, QFont::Bold));
    m_liveBalDinarLabel->setAlignment(Qt::AlignCenter);
    dinarTile->layout()->addWidget(m_liveBalDinarLabel);

    tilesRow->addWidget(dollarTile);
    tilesRow->addWidget(dinarTile);
    balCardVL->addLayout(tilesRow);

    settingsLayout->addWidget(balCard, 4, 0, 1, 2);
    settingsLayout->setRowStretch(5, 1);
    settingsLayout->setColumnStretch(1, 1);

    m_tabs->addTab(settingsTab, QString::fromUtf8("الإعدادات العامة"));

    // ===== TAB 2: Customers =====
    QWidget *custTab = new QWidget;
    QVBoxLayout *custLayout = new QVBoxLayout(custTab);
    custLayout->setContentsMargins(6, 6, 6, 6);
    custLayout->setSpacing(4);

    m_custIdLabel = new QLabel;
    m_custIdLabel->setFont(QFont("Tahoma", 10, QFont::Bold));
    m_custIdLabel->setAlignment(Qt::AlignRight);
    custLayout->addWidget(m_custIdLabel);

    QGroupBox *custFormBox = new QGroupBox(QString::fromUtf8("بيانات الشخص / الشركة"));
    custFormBox->setObjectName("formBox");
    QGridLayout *custFormLayout = new QGridLayout(custFormBox);
    custFormLayout->setSpacing(6);

    m_custNameEdit    = new QLineEdit;
    m_custRegionEdit  = new QLineEdit;
    m_custPhoneEdit   = new QLineEdit;
    m_custAddressEdit = new QLineEdit;
    m_custNotesEdit   = new QLineEdit;
    m_custBalDollar   = new QDoubleSpinBox;
    m_custBalDollar->setRange(-999999, 999999);
    m_custBalDollar->setDecimals(2);
    m_custBalDinar    = new QDoubleSpinBox;
    m_custBalDinar->setRange(-999999999, 999999999);
    m_custBalDinar->setDecimals(0);
    m_custTypeCombo   = new QComboBox;
    m_custTypeCombo->addItems({
        QString::fromUtf8("زبون"),
        QString::fromUtf8("مجهز"),
        QString::fromUtf8("زبون ومجهز")
    });

    // RTL 4-col: col 0=rightmost (label), col 1=input, col 2=label, col 3=input (leftmost)
    custFormLayout->addWidget(makeLabel(QString::fromUtf8("الاسم:")),        0, 0); custFormLayout->addWidget(m_custNameEdit,    0, 1);
    custFormLayout->addWidget(makeLabel(QString::fromUtf8("المنطقة:")),      0, 2); custFormLayout->addWidget(m_custRegionEdit,  0, 3);
    custFormLayout->addWidget(makeLabel(QString::fromUtf8("الهاتف:")),       1, 0); custFormLayout->addWidget(m_custPhoneEdit,   1, 1);
    custFormLayout->addWidget(makeLabel(QString::fromUtf8("العنوان:")),      1, 2); custFormLayout->addWidget(m_custAddressEdit, 1, 3);
    custFormLayout->addWidget(makeLabel(QString::fromUtf8("الرصيد $:")),     2, 0); custFormLayout->addWidget(m_custBalDollar,   2, 1);
    custFormLayout->addWidget(makeLabel(QString::fromUtf8("الرصيد دينار:")), 2, 2); custFormLayout->addWidget(m_custBalDinar,    2, 3);
    custFormLayout->addWidget(makeLabel(QString::fromUtf8("النوع:")),        3, 0); custFormLayout->addWidget(m_custTypeCombo,   3, 1);
    custFormLayout->addWidget(makeLabel(QString::fromUtf8("ملاحظات:")),      3, 2); custFormLayout->addWidget(m_custNotesEdit,   3, 3);
    custFormLayout->setColumnStretch(1, 1);
    custFormLayout->setColumnStretch(3, 1);
    custLayout->addWidget(custFormBox);

    // Search row
    m_custSearchEdit = new QLineEdit;
    m_custSearchEdit->setPlaceholderText(QString::fromUtf8("بحث بالاسم..."));
    QPushButton *searchCustBtn = new QPushButton(QString::fromUtf8("بحث"));
    searchCustBtn->setObjectName("actionBtn");
    searchCustBtn->setFixedHeight(28);
    QHBoxLayout *custSearchRow = new QHBoxLayout;
    custSearchRow->addWidget(makeLabel(QString::fromUtf8("بحث:")));
    custSearchRow->addWidget(m_custSearchEdit, 1);
    custSearchRow->addWidget(searchCustBtn);
    custLayout->addLayout(custSearchRow);

    // Buttons: حفظ · رجوع · إلغاء · حذف
    QHBoxLayout *custBtnRow = new QHBoxLayout;
    QPushButton *saveCustBtn   = new QPushButton(QString::fromUtf8("\U0001F4BE حفظ"));
    QPushButton *backCustBtn   = new QPushButton(QString::fromUtf8("رجوع"));
    QPushButton *cancelCustBtn = new QPushButton(QString::fromUtf8("إلغاء"));
    QPushButton *delCustBtn    = new QPushButton(QString::fromUtf8("\u2716 حذف"));
    for (auto *b : {saveCustBtn, backCustBtn, cancelCustBtn, delCustBtn}) {
        b->setFont(QFont("Tahoma", 10));
        b->setFixedHeight(32);
        b->setObjectName("actionBtn");
    }
    custBtnRow->addWidget(saveCustBtn);
    custBtnRow->addWidget(backCustBtn);
    custBtnRow->addWidget(cancelCustBtn);
    custBtnRow->addStretch();
    custBtnRow->addWidget(delCustBtn);
    custLayout->addLayout(custBtnRow);

    m_customersTable = new QTableWidget(0, 6);
    m_customersTable->setHorizontalHeaderLabels({
        QString::fromUtf8("الاسم"),    QString::fromUtf8("المنطقة"),
        QString::fromUtf8("الهاتف"),   QString::fromUtf8("الرصيد $"),
        QString::fromUtf8("الرصيد د"), QString::fromUtf8("النوع")
    });
    m_customersTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_customersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_customersTable->setObjectName("dataTable");
    custLayout->addWidget(m_customersTable, 1);

    connect(saveCustBtn,   &QPushButton::clicked, this, &SystemSettings::addCustomer);
    connect(delCustBtn,    &QPushButton::clicked, this, &SystemSettings::deleteCustomer);
    connect(backCustBtn,   &QPushButton::clicked, this, &SystemSettings::clearCustomerForm);
    connect(cancelCustBtn, &QPushButton::clicked, this, &SystemSettings::clearCustomerForm);
    connect(searchCustBtn, &QPushButton::clicked, this, &SystemSettings::searchCustomer);
    connect(m_custSearchEdit, &QLineEdit::returnPressed, this, &SystemSettings::searchCustomer);
    connect(m_customersTable, &QTableWidget::cellClicked, this, &SystemSettings::onCustomerCellClicked);

    // Enter-nav: Name→Region→Phone→Address→Notes→BalDollar→BalDinar→Type→Save
    m_custTabOrder = { m_custNameEdit, m_custRegionEdit, m_custPhoneEdit,
                       m_custAddressEdit, m_custNotesEdit,
                       m_custBalDollar, m_custBalDinar,
                       m_custTypeCombo, saveCustBtn };
    installEnterNav(m_custTabOrder);

    m_tabs->addTab(custTab, QString::fromUtf8("الأشخاص والشركات"));

    // ===== TAB 3: Products =====
    QWidget *prodTab = new QWidget;
    QVBoxLayout *prodLayout = new QVBoxLayout(prodTab);
    prodLayout->setContentsMargins(6, 6, 6, 6);
    prodLayout->setSpacing(4);

    m_prodIdLabel = new QLabel;
    m_prodIdLabel->setFont(QFont("Tahoma", 10, QFont::Bold));
    m_prodIdLabel->setAlignment(Qt::AlignRight);
    prodLayout->addWidget(m_prodIdLabel);

    QGroupBox *prodFormBox = new QGroupBox(QString::fromUtf8("بيانات المادة"));
    prodFormBox->setObjectName("formBox");
    QGridLayout *prodFormLayout = new QGridLayout(prodFormBox);
    prodFormLayout->setSpacing(6);

    m_prodBarcodeEdit = new QLineEdit;
    m_prodNameEdit    = new QLineEdit;
    m_prodGroupCombo  = new QComboBox;
    m_prodGroupCombo->setEditable(true);
    m_prodTypeEdit    = new QLineEdit;

    m_prodCostSpin    = new QDoubleSpinBox; m_prodCostSpin->setRange(0, 999999);    m_prodCostSpin->setDecimals(3);
    m_prodCostDinSpin = new QDoubleSpinBox; m_prodCostDinSpin->setRange(0, 1e9);    m_prodCostDinSpin->setDecimals(0);
    m_prodWsDolSpin   = new QDoubleSpinBox; m_prodWsDolSpin->setRange(0, 999999);   m_prodWsDolSpin->setDecimals(3);
    m_prodWsDinSpin   = new QDoubleSpinBox; m_prodWsDinSpin->setRange(0, 9999999);  m_prodWsDinSpin->setDecimals(0);
    m_prodRetDolSpin  = new QDoubleSpinBox; m_prodRetDolSpin->setRange(0, 999999);  m_prodRetDolSpin->setDecimals(3);
    m_prodRetDinSpin  = new QDoubleSpinBox; m_prodRetDinSpin->setRange(0, 9999999); m_prodRetDinSpin->setDecimals(0);
    m_prodStockSpin   = new QDoubleSpinBox; m_prodStockSpin->setRange(-999999, 999999); m_prodStockSpin->setDecimals(2);
    m_prodCartoonSpin = new QSpinBox;       m_prodCartoonSpin->setRange(1, 9999);    m_prodCartoonSpin->setValue(1);
    m_prodReorderSpin = new QDoubleSpinBox; m_prodReorderSpin->setRange(0, 999999); m_prodReorderSpin->setDecimals(2);

    // Barcode row: input + generate button
    QPushButton *genBarcodeBtn = new QPushButton(QString::fromUtf8("إنشاء باركود"));
    genBarcodeBtn->setObjectName("actionBtn");
    genBarcodeBtn->setFixedHeight(26);
    QHBoxLayout *barcodeHBox = new QHBoxLayout;
    barcodeHBox->setContentsMargins(0, 0, 0, 0);
    barcodeHBox->addWidget(m_prodBarcodeEdit, 1);
    barcodeHBox->addWidget(genBarcodeBtn);
    QWidget *barcodeCell = new QWidget;
    barcodeCell->setLayout(barcodeHBox);

    // RTL 4-col: col 0=rightmost (label), col 1=input, col 2=label, col 3=input (leftmost)
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("الباركود:")),        0, 0); prodFormLayout->addWidget(barcodeCell,        0, 1);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("اسم المادة:")),      0, 2); prodFormLayout->addWidget(m_prodNameEdit,      0, 3);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("المجموعة:")),        1, 0); prodFormLayout->addWidget(m_prodGroupCombo,    1, 1);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("النوع:")),           1, 2); prodFormLayout->addWidget(m_prodTypeEdit,      1, 3);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("سعر الكلفة $:")),    2, 0); prodFormLayout->addWidget(m_prodCostSpin,      2, 1);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("سعر الكلفة دينار:")),2, 2); prodFormLayout->addWidget(m_prodCostDinSpin,   2, 3);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("جملة $:")),          3, 0); prodFormLayout->addWidget(m_prodWsDolSpin,     3, 1);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("جملة دينار:")),      3, 2); prodFormLayout->addWidget(m_prodWsDinSpin,     3, 3);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("مفرد $:")),          4, 0); prodFormLayout->addWidget(m_prodRetDolSpin,    4, 1);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("مفرد دينار:")),      4, 2); prodFormLayout->addWidget(m_prodRetDinSpin,    4, 3);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("تعبئة الكرتون:")),   5, 0); prodFormLayout->addWidget(m_prodCartoonSpin,   5, 1);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("حد إعادة الطلب:")), 5, 2); prodFormLayout->addWidget(m_prodReorderSpin,   5, 3);
    prodFormLayout->addWidget(makeLabel(QString::fromUtf8("الرصيد الأولي:")),   6, 0); prodFormLayout->addWidget(m_prodStockSpin,     6, 1);
    prodFormLayout->setColumnStretch(1, 1);
    prodFormLayout->setColumnStretch(3, 1);
    prodLayout->addWidget(prodFormBox);

    // Buttons: حفظ · جديد · تعديل · حذف
    QHBoxLayout *prodBtnRow = new QHBoxLayout;
    QPushButton *saveProdBtn = new QPushButton(QString::fromUtf8("\U0001F4BE حفظ"));
    QPushButton *newProdBtn  = new QPushButton(QString::fromUtf8("+ جديد"));
    QPushButton *editProdBtn = new QPushButton(QString::fromUtf8("\u270F تعديل"));
    QPushButton *delProdBtn  = new QPushButton(QString::fromUtf8("\u2716 حذف"));
    for (auto *b : {saveProdBtn, newProdBtn, editProdBtn, delProdBtn}) {
        b->setFont(QFont("Tahoma", 10));
        b->setFixedHeight(32);
        b->setObjectName("actionBtn");
    }
    prodBtnRow->addWidget(saveProdBtn);
    prodBtnRow->addWidget(newProdBtn);
    prodBtnRow->addWidget(editProdBtn);
    prodBtnRow->addStretch();
    prodBtnRow->addWidget(delProdBtn);
    prodLayout->addLayout(prodBtnRow);

    m_productsTable = new QTableWidget(0, 7);
    m_productsTable->setHorizontalHeaderLabels({
        QString::fromUtf8("الباركود"),   QString::fromUtf8("اسم المادة"),
        QString::fromUtf8("المجموعة"),   QString::fromUtf8("الكلفة"),
        QString::fromUtf8("جملة $"),     QString::fromUtf8("مفرد $"),
        QString::fromUtf8("الرصيد")
    });
    m_productsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_productsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_productsTable->setObjectName("dataTable");
    prodLayout->addWidget(m_productsTable, 1);

    connect(saveProdBtn,  &QPushButton::clicked, this, &SystemSettings::addProduct);
    connect(newProdBtn,   &QPushButton::clicked, this, &SystemSettings::clearProductForm);
    connect(editProdBtn,  &QPushButton::clicked, this, &SystemSettings::editProduct);
    connect(delProdBtn,   &QPushButton::clicked, this, &SystemSettings::deleteProduct);
    connect(genBarcodeBtn,&QPushButton::clicked, this, &SystemSettings::generateBarcode);

    connect(m_prodBarcodeEdit, &QLineEdit::returnPressed, m_prodNameEdit,
            QOverload<>::of(&QWidget::setFocus));

    connect(m_productsTable, &QTableWidget::cellClicked, this,
            [this](int row, int /*col*/) {
        if (row < 0) return;
        QTableWidgetItem *item = m_productsTable->item(row, 0);
        if (!item) return;
        int id = item->data(Qt::UserRole).toInt();
        QSqlQuery q;
        q.prepare("SELECT barcode,name,product_group,product_type,"
                  "cost_price,cost_price_dinar,"
                  "wholesale_dollar,retail_dollar,"
                  "wholesale_dinar,retail_dinar,"
                  "cartoon_qty,stock_qty,reorder_level "
                  "FROM products WHERE id=?");
        q.addBindValue(id);
        if (q.exec() && q.next()) {
            m_editingProductId = id;
            m_prodBarcodeEdit->setText(q.value(0).toString());
            m_prodNameEdit->setText(q.value(1).toString());
            m_prodGroupCombo->setCurrentText(q.value(2).toString());
            m_prodTypeEdit->setText(q.value(3).toString());
            m_prodCostSpin->setValue(q.value(4).toDouble());
            m_prodCostDinSpin->setValue(q.value(5).toDouble());
            m_prodWsDolSpin->setValue(q.value(6).toDouble());
            m_prodRetDolSpin->setValue(q.value(7).toDouble());
            m_prodWsDinSpin->setValue(q.value(8).toDouble());
            m_prodRetDinSpin->setValue(q.value(9).toDouble());
            m_prodCartoonSpin->setValue(q.value(10).toInt());
            m_prodStockSpin->setValue(q.value(11).toDouble());
            m_prodReorderSpin->setValue(q.value(12).toDouble());
            m_prodIdLabel->setText(QString::fromUtf8("رقم المادة: ") + QString::number(id));
        }
    });

    // Real-time dollar → dinar conversion
    connect(m_prodCostSpin,   QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SystemSettings::onCostDollarChanged);
    connect(m_prodWsDolSpin,  QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SystemSettings::onWsDollarChanged);
    connect(m_prodRetDolSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SystemSettings::onRetDollarChanged);

    // Enter-nav: Barcode→Name→Group→Type→CostDol→CostDin→WsDol→WsDin→
    //            RetDol→RetDin→Cartoon→Reorder→Stock→Save
    m_prodTabOrder = { m_prodBarcodeEdit, m_prodNameEdit, m_prodGroupCombo,
                       m_prodTypeEdit,
                       m_prodCostSpin, m_prodCostDinSpin,
                       m_prodWsDolSpin, m_prodWsDinSpin,
                       m_prodRetDolSpin, m_prodRetDinSpin,
                       m_prodCartoonSpin, m_prodReorderSpin,
                       m_prodStockSpin, saveProdBtn };
    installEnterNav(m_prodTabOrder);

    m_tabs->addTab(prodTab, QString::fromUtf8("المواد"));

    mainLayout->addWidget(m_tabs, 1);

    // Bottom row
    QHBoxLayout *bottomRow = new QHBoxLayout;
    m_saveBtn  = new QPushButton(QString::fromUtf8("\U0001F4BE حفظ الإعدادات"));
    m_closeBtn = new QPushButton(QString::fromUtf8("\u2716 إغلاق"));
    m_saveBtn->setObjectName("saveBtn");   m_saveBtn->setFixedHeight(36);
    m_closeBtn->setObjectName("closeBtn"); m_closeBtn->setFixedHeight(36);
    bottomRow->addWidget(m_saveBtn);
    bottomRow->addStretch();
    bottomRow->addWidget(m_closeBtn);
    mainLayout->addLayout(bottomRow);

    connect(m_saveBtn,  &QPushButton::clicked, this, &SystemSettings::saveSettings);
    connect(m_closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}

void SystemSettings::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #d8dce8; }
        QTabWidget::pane { background: #dde2ee; border: 1px solid #99aacc; }
        QTabBar::tab { background: #b0bacc; color: #003366; font-family: Tahoma;
                       font-size: 10pt; font-weight: bold; padding: 6px 16px; border-radius: 3px; }
        QTabBar::tab:selected { background: #003366; color: white; }
        #formBox { background: rgba(200,210,240,0.6); border: 1px solid #88aacc;
                   border-radius: 4px; color: #003366; font-family: Tahoma; }
        #dataTable { background: white; }
        #dataTable QHeaderView::section {
            background: #003366; color: #FFD700; font-weight: bold;
            padding: 4px; border: 1px solid #001144;
        }
        #actionBtn {
            background: #003366; color: white; font-weight: bold;
            border-radius: 3px; border: 1px solid #001144; padding: 0 12px;
        }
        #actionBtn:hover { background: #0044aa; }
        #saveBtn  { background: #228844; color: white; font-weight: bold; border-radius: 4px; border: none; padding: 0 16px; }
        #closeBtn { background: #882222; color: white; font-weight: bold; border-radius: 4px; border: none; padding: 0 16px; }
        #rateSpin { background: #ffffc0; font-weight: bold; border: 1px solid #aaaa60; font-size: 13pt; }
        QLineEdit { background: white; border: 1px solid #99aacc; border-radius: 2px; padding: 2px 4px; min-height: 24px; }
        QDoubleSpinBox, QSpinBox { background: white; border: 1px solid #99aacc; border-radius: 2px; }
        QComboBox { background: white; border: 1px solid #99aacc; border-radius: 2px; }
        QLabel { font-family: Tahoma; color: #003366; }
        QGroupBox::title { font-family: Tahoma; color: #003366; font-weight: bold; }

        /* ── Treasury balance card ── */
        #balCard {
            background: #f0f4ff;
            border: 1px solid #c0ccee;
            border-radius: 8px;
            margin-top: 10px;
        }
        #balCardTitle {
            color: #1a3a6e;
            font-size: 11pt;
            padding-bottom: 4px;
            border-bottom: 1px solid #c0ccee;
        }
        #dollarTile {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #1a6e44, stop:1 #124e30);
            border-radius: 8px;
            min-width: 160px;
        }
        #dinarTile {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #1a3a9e, stop:1 #0d2266);
            border-radius: 8px;
            min-width: 160px;
        }
        #dollarTile .tileTitle, #dinarTile .tileTitle { /* fallback via objectName below */ }
        #dollarBalLbl { color: #ffffff; font-size: 22pt; }
        #dinarBalLbl  { color: #ffffff; font-size: 22pt; }
        #tileTitle    { color: rgba(255,255,255,0.85); font-size: 9pt; }
    )");
}

// ── Settings ───────────────────────────────────────────────────────────────
void SystemSettings::saveSettings()
{
    Database::setCompanyName(m_companyNameEdit->text());
    Database::setExchangeRate(m_exchangeRateSpin->value());
    QMessageBox::information(this, QString::fromUtf8("تم"),
                             QString::fromUtf8("تم حفظ الإعدادات بنجاح"));
}

// ── Customer operations ────────────────────────────────────────────────────
void SystemSettings::loadCustomers()
{
    m_customersTable->setRowCount(0);
    QSqlQuery q = Database::getCustomers();
    int row = 0;
    static const QStringList typeLabels = {
        QString::fromUtf8("زبون"),
        QString::fromUtf8("مجهز"),
        QString::fromUtf8("زبون ومجهز")
    };
    while (q.next()) {
        m_customersTable->insertRow(row);
        m_customersTable->setItem(row, 0, new QTableWidgetItem(q.value(1).toString()));
        m_customersTable->setItem(row, 1, new QTableWidgetItem(q.value(2).toString()));
        m_customersTable->setItem(row, 2, new QTableWidgetItem(q.value(4).toString()));
        m_customersTable->setItem(row, 3, new QTableWidgetItem(
            QString::number(q.value(6).toDouble(), 'f', 2)));
        m_customersTable->setItem(row, 4, new QTableWidgetItem(
            QString::number(q.value(7).toDouble(), 'f', 0)));
        int typeIdx = q.value(8).toInt();
        m_customersTable->setItem(row, 5, new QTableWidgetItem(
            (typeIdx >= 0 && typeIdx < typeLabels.size()) ? typeLabels[typeIdx] : QString()));
        m_customersTable->item(row, 0)->setData(Qt::UserRole, q.value(0));
        row++;
    }
}

void SystemSettings::clearCustomerForm()
{
    m_custNameEdit->clear();
    m_custRegionEdit->clear();
    m_custPhoneEdit->clear();
    m_custAddressEdit->clear();
    m_custNotesEdit->clear();
    m_custBalDollar->setValue(0);
    m_custBalDinar->setValue(0);
    m_custTypeCombo->setCurrentIndex(0);
    m_editingCustomerId = -1;
    updateNextCustomerId();
    m_custNameEdit->setFocus();
}

void SystemSettings::updateNextCustomerId()
{
    QSqlQuery q("SELECT COALESCE(MAX(id),0)+1 FROM customers");
    if (q.next())
        m_custIdLabel->setText(QString::fromUtf8("رقم الزبون: ") + q.value(0).toString());
}

void SystemSettings::addCustomer()
{
    if (m_editingCustomerId > 0) { editCustomer(); return; }
    if (m_custNameEdit->text().trimmed().isEmpty()) {
        m_custNameEdit->setFocus();
        return;
    }
    int id = Database::addCustomer(
        m_custNameEdit->text(), m_custRegionEdit->text(),
        m_custAddressEdit->text(), m_custPhoneEdit->text(),
        m_custNotesEdit->text(), m_custBalDollar->value(),
        m_custBalDinar->value(), m_custTypeCombo->currentIndex());
    if (id > 0) {
        loadCustomers();
        clearCustomerForm();
    } else {
        QMessageBox::warning(this, QString::fromUtf8("خطأ"),
                             QString::fromUtf8("فشل الحفظ - تأكد من اسم غير مكرر"));
    }
}

void SystemSettings::editCustomer()
{
    if (m_editingCustomerId <= 0) return;
    bool ok = Database::updateCustomer(
        m_editingCustomerId,
        m_custNameEdit->text(), m_custRegionEdit->text(),
        m_custAddressEdit->text(), m_custPhoneEdit->text(),
        m_custNotesEdit->text(), m_custBalDollar->value(),
        m_custBalDinar->value(), m_custTypeCombo->currentIndex());
    if (ok) {
        loadCustomers();
        clearCustomerForm();
    } else {
        QMessageBox::warning(this, QString::fromUtf8("خطأ"),
                             QString::fromUtf8("فشل التعديل"));
    }
}

void SystemSettings::onCustomerCellClicked(int row, int /*col*/)
{
    if (row < 0) return;
    QTableWidgetItem *item = m_customersTable->item(row, 0);
    if (!item) return;
    int id = item->data(Qt::UserRole).toInt();
    QSqlQuery q;
    q.prepare("SELECT name,region,address,phone,notes,"
              "balance_dollar,balance_dinar,type FROM customers WHERE id=?");
    q.addBindValue(id);
    if (q.exec() && q.next()) {
        m_editingCustomerId = id;
        m_custNameEdit->setText(q.value(0).toString());
        m_custRegionEdit->setText(q.value(1).toString());
        m_custAddressEdit->setText(q.value(2).toString());
        m_custPhoneEdit->setText(q.value(3).toString());
        m_custNotesEdit->setText(q.value(4).toString());
        m_custBalDollar->setValue(q.value(5).toDouble());
        m_custBalDinar->setValue(q.value(6).toDouble());
        m_custTypeCombo->setCurrentIndex(q.value(7).toInt());
        m_custIdLabel->setText(QString::fromUtf8("رقم الزبون: ") + QString::number(id));
    }
}

void SystemSettings::deleteCustomer()
{
    int row = m_customersTable->currentRow();
    if (row < 0) return;
    int id = m_customersTable->item(row, 0)->data(Qt::UserRole).toInt();
    if (QMessageBox::question(this, QString::fromUtf8("تأكيد"),
        QString::fromUtf8("هل تريد حذف هذا السجل؟")) == QMessageBox::Yes) {
        Database::deleteCustomer(id);
        loadCustomers();
        clearCustomerForm();
    }
}

void SystemSettings::searchCustomer()
{
    QString text = m_custSearchEdit->text().trimmed();
    for (int i = 0; i < m_customersTable->rowCount(); i++) {
        QTableWidgetItem *nameItem = m_customersTable->item(i, 0);
        bool match = text.isEmpty() ||
                     (nameItem && nameItem->text().contains(text, Qt::CaseInsensitive));
        m_customersTable->setRowHidden(i, !match);
    }
}

// ── Product operations ─────────────────────────────────────────────────────
void SystemSettings::loadProducts()
{
    m_productsTable->setRowCount(0);
    QSqlQuery q = Database::getProducts();
    int row = 0;
    while (q.next()) {
        m_productsTable->insertRow(row);
        m_productsTable->setItem(row, 0, new QTableWidgetItem(q.value(1).toString()));
        m_productsTable->setItem(row, 1, new QTableWidgetItem(q.value(2).toString()));
        m_productsTable->setItem(row, 2, new QTableWidgetItem(q.value(3).toString()));
        m_productsTable->setItem(row, 3, new QTableWidgetItem(
            QString::number(q.value(5).toDouble(), 'f', 3)));
        m_productsTable->setItem(row, 4, new QTableWidgetItem(
            QString::number(q.value(6).toDouble(), 'f', 3)));
        m_productsTable->setItem(row, 5, new QTableWidgetItem(
            QString::number(q.value(7).toDouble(), 'f', 3)));
        m_productsTable->setItem(row, 6, new QTableWidgetItem(
            QString::number(q.value(11).toDouble(), 'f', 2)));
        m_productsTable->item(row, 0)->setData(Qt::UserRole, q.value(0));
        row++;
    }
}

void SystemSettings::clearProductForm()
{
    m_prodBarcodeEdit->clear();
    m_prodNameEdit->clear();
    m_prodGroupCombo->clearEditText();
    m_prodTypeEdit->clear();
    m_prodCostSpin->setValue(0);
    m_prodCostDinSpin->setValue(0);
    m_prodWsDolSpin->setValue(0);
    m_prodWsDinSpin->setValue(0);
    m_prodRetDolSpin->setValue(0);
    m_prodRetDinSpin->setValue(0);
    m_prodCartoonSpin->setValue(1);
    m_prodReorderSpin->setValue(0);
    m_prodStockSpin->setValue(0);
    m_editingProductId = -1;
    updateNextProductId();
    m_prodBarcodeEdit->setFocus();
}

void SystemSettings::updateNextProductId()
{
    QSqlQuery q("SELECT COALESCE(MAX(id),0)+1 FROM products");
    if (q.next())
        m_prodIdLabel->setText(QString::fromUtf8("رقم المادة: ") + q.value(0).toString());
}

void SystemSettings::generateBarcode()
{
    QSqlQuery q("SELECT COALESCE(MAX(id),0)+1 FROM products");
    if (q.next()) {
        int nextId = q.value(0).toInt();
        m_prodBarcodeEdit->setText(QString("8964%1").arg(nextId, 8, 10, QChar('0')));
    }
}

void SystemSettings::onCostDollarChanged(double val)
{
    m_prodCostDinSpin->blockSignals(true);
    m_prodCostDinSpin->setValue(val * m_exchangeRateSpin->value());
    m_prodCostDinSpin->blockSignals(false);
}

void SystemSettings::onWsDollarChanged(double val)
{
    m_prodWsDinSpin->blockSignals(true);
    m_prodWsDinSpin->setValue(val * m_exchangeRateSpin->value());
    m_prodWsDinSpin->blockSignals(false);
}

void SystemSettings::onRetDollarChanged(double val)
{
    m_prodRetDinSpin->blockSignals(true);
    m_prodRetDinSpin->setValue(val * m_exchangeRateSpin->value());
    m_prodRetDinSpin->blockSignals(false);
}

void SystemSettings::loadProductGroups()
{
    QString current = m_prodGroupCombo->currentText();
    m_prodGroupCombo->clear();
    for (const QString &g : Database::getProductGroups())
        m_prodGroupCombo->addItem(g);
    if (!current.isEmpty())
        m_prodGroupCombo->setCurrentText(current);
}

void SystemSettings::onTabChanged(int index)
{
    if (index == 2) // Products tab
        QTimer::singleShot(50, m_prodBarcodeEdit, QOverload<>::of(&QWidget::setFocus));
}

void SystemSettings::addProduct()
{
    if (m_editingProductId > 0) { editProduct(); return; }
    if (m_prodNameEdit->text().trimmed().isEmpty()) {
        m_prodNameEdit->setFocus();
        return;
    }
    QString group = m_prodGroupCombo->currentText().trimmed();
    Database::ensureProductGroup(group);
    int id = Database::addProduct(
        m_prodBarcodeEdit->text(), m_prodNameEdit->text(),
        group, m_prodTypeEdit->text(),
        m_prodCostSpin->value(), m_prodCostDinSpin->value(),
        m_prodWsDolSpin->value(), m_prodRetDolSpin->value(),
        m_prodWsDinSpin->value(), m_prodRetDinSpin->value(),
        m_prodCartoonSpin->value(), m_prodStockSpin->value(), 0,
        m_prodReorderSpin->value(), m_exchangeRateSpin->value());
    if (id > 0) {
        loadProducts();
        loadProductGroups();
        clearProductForm();
    } else {
        QMessageBox::warning(this, QString::fromUtf8("خطأ"),
                             QString::fromUtf8("فشل الحفظ - تأكد من باركود غير مكرر"));
    }
}

void SystemSettings::editProduct()
{
    if (m_editingProductId <= 0) return;
    QString group = m_prodGroupCombo->currentText().trimmed();
    Database::ensureProductGroup(group);
    bool ok = Database::updateProduct(
        m_editingProductId,
        m_prodNameEdit->text(), group, m_prodTypeEdit->text(),
        m_prodCostSpin->value(), m_prodCostDinSpin->value(),
        m_prodWsDolSpin->value(), m_prodRetDolSpin->value(),
        m_prodWsDinSpin->value(), m_prodRetDinSpin->value(),
        m_prodStockSpin->value(), m_prodReorderSpin->value());
    if (ok) {
        loadProducts();
        loadProductGroups();
        clearProductForm();
    } else {
        QMessageBox::warning(this, QString::fromUtf8("خطأ"),
                             QString::fromUtf8("فشل التعديل"));
    }
}

void SystemSettings::deleteProduct()
{
    int row = m_productsTable->currentRow();
    if (row < 0) return;
    int id = m_productsTable->item(row, 0)->data(Qt::UserRole).toInt();
    if (QMessageBox::question(this, QString::fromUtf8("تأكيد"),
        QString::fromUtf8("هل تريد حذف هذه المادة؟")) == QMessageBox::Yes) {
        QSqlQuery q;
        q.prepare("DELETE FROM products WHERE id=?");
        q.addBindValue(id);
        q.exec();
        loadProducts();
        clearProductForm();
    }
}
