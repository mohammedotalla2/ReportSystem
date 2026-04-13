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
#include <QCheckBox>
#include <QListWidget>
#include <QDialogButtonBox>


/* ================================================================
   ProductDelegate – searchable combo in اسم المادة column (col 3)
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

        QCompleter *comp = new QCompleter(m_names, cb);
        comp->setCaseSensitivity(Qt::CaseInsensitive);
        comp->setFilterMode(Qt::MatchContains);
        comp->setCompletionMode(QCompleter::PopupCompletion);
        cb->setCompleter(comp);

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

        model->setData(index, name);

        if (listIdx >= 0 && listIdx < m_ids.size()) {
            QModelIndex codeIdx = model->index(index.row(), 2);
            QModelIndex idIdx   = model->index(index.row(), 9);
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
    setWindowTitle(QString::fromUtf8("قائمة المبيعات"));
    setLayoutDirection(Qt::RightToLeft);
    resize(1350, 800);
    m_printer = new PrintManager(this);
    setupUI();
    applyStyles();
    loadCustomers();
    loadProducts();
    clearForm();
    refreshInvoiceCount();
}

/* ================================================================
   UI Setup
   ================================================================ */
void SalesWindow::setupUI()
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(4, 4, 4, 4);
    root->setSpacing(4);

    auto lbl = [](const QString &t) -> QLabel* {
        QLabel *l = new QLabel(t);
        l->setFont(QFont("Tahoma", 9));
        l->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        return l;
    };

    /* ===================================================
       HEADER GRID  (3 rows)
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
    m_monthEdit = new QDateEdit(QDate::currentDate());
    m_monthEdit->setDisplayFormat("M");
    m_yearEdit = new QDateEdit(QDate::currentDate());
    m_yearEdit->setDisplayFormat("yyyy");

    connect(m_monthEdit, &QDateEdit::dateChanged, [this](const QDate &d) {
        QDate cur = m_dateEdit->date();
        m_dateEdit->setDate(QDate(cur.year(), d.month(), qMin(cur.day(), d.daysInMonth())));
    });
    connect(m_yearEdit, &QDateEdit::dateChanged, [this](const QDate &d) {
        QDate cur = m_dateEdit->date();
        m_dateEdit->setDate(QDate(d.year(), cur.month(), qMin(cur.day(), cur.daysInMonth())));
    });

    /* day name label */
    m_dayLabel = new QLabel;
    m_dayLabel->setObjectName("dayNameLbl");
    updateDayName();
    connect(m_dateEdit, &QDateEdit::dateChanged, [this](const QDate &) { updateDayName(); });

    /* customer */
    m_customerCombo = new QComboBox;
    m_customerCombo->setEditable(true);
    m_customerCombo->setObjectName("customerCombo");
    m_customerCombo->setMinimumWidth(200);
    QCompleter *custComp = new QCompleter(this);
    custComp->setCaseSensitivity(Qt::CaseInsensitive);
    custComp->setFilterMode(Qt::MatchContains);
    custComp->setCompletionMode(QCompleter::PopupCompletion);
    m_customerCombo->setCompleter(custComp);

    /* payment / currency / type */
    m_paymentTypeCombo = new QComboBox;
    m_paymentTypeCombo->addItems({
        QString::fromUtf8("نقدي"),
        QString::fromUtf8("آجل"),
        QString::fromUtf8("جملة")
    });
    m_currencyCombo = new QComboBox;
    m_currencyCombo->addItems({QString::fromUtf8("دينار"), "$"});
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

    /* receiver / notes / due date */
    m_receiverEdit = new QLineEdit;
    m_receiverEdit->setPlaceholderText(QString::fromUtf8("المستلم"));
    m_notesEdit = new QLineEdit;
    m_notesEdit->setPlaceholderText(QString::fromUtf8("الملاحظات"));
    m_dueDateEdit = new QDateEdit(QDate::currentDate());
    m_dueDateEdit->setDisplayFormat("yyyy/MM/dd");
    m_dueDateEdit->setCalendarPopup(true);

    /* ── Row 0 ── */
    hg->addWidget(lbl(QString::fromUtf8("رقم القائمة")), 0, 0);
    hg->addWidget(m_invoiceNoSpin,                        0, 1);
    hg->addWidget(lbl(QString::fromUtf8("التاريخ")),      0, 2);
    hg->addWidget(m_dayLabel,                              0, 3);
    hg->addWidget(m_dateEdit,                              0, 4);
    hg->addWidget(m_monthEdit,                             0, 5);
    hg->addWidget(m_yearEdit,                              0, 6);
    hg->addWidget(lbl(QString::fromUtf8("اسم الزبون")),   0, 7);
    hg->addWidget(m_customerCombo,                         0, 8, 1, 2);
    hg->addWidget(lbl(QString::fromUtf8("نوع الدفع")),    0, 10);
    hg->addWidget(m_paymentTypeCombo,                      0, 11);

    /* ── Row 1 ── */
    hg->addWidget(lbl(QString::fromUtf8("نوع التسعير")),  1, 0);
    hg->addWidget(m_invoiceTypeCombo,                      1, 1);
    hg->addWidget(lbl(QString::fromUtf8("العملة")),        1, 2);
    hg->addWidget(m_currencyCombo,                         1, 3);
    hg->addWidget(lbl(QString::fromUtf8("سعر صرف $")),    1, 4);
    hg->addWidget(m_exchangeRateEdit,                      1, 5);
    hg->addWidget(lbl(QString::fromUtf8("المستلم")),       1, 6);
    hg->addWidget(m_receiverEdit,                          1, 7);
    hg->addWidget(lbl(QString::fromUtf8("تاريخ الاستحقاق")), 1, 10);
    hg->addWidget(m_dueDateEdit,                           1, 11);

    /* ── Row 2 – notes ── */
    hg->addWidget(lbl(QString::fromUtf8("الملاحظات")),     2, 0);
    hg->addWidget(m_notesEdit,                              2, 1, 1, 11);

    root->addWidget(headerWidget);

    /* ===================================================
       MIDDLE: table area  +  right product panel
       =================================================== */
    QHBoxLayout *mid = new QHBoxLayout;
    mid->setSpacing(4);

    QWidget *tableArea = new QWidget;
    tableArea->setObjectName("tableArea");
    QVBoxLayout *tvl = new QVBoxLayout(tableArea);
    tvl->setContentsMargins(2, 2, 2, 2);
    tvl->setSpacing(2);

    /* ── item-entry row ── */
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
    m_editItemBtn = new QPushButton("✎");
    QPushButton *delBtn  = new QPushButton("✖");
    for (QPushButton *b : {addBtn, m_editItemBtn, delBtn})
        b->setFixedSize(34, 28);
    addBtn->setObjectName("addItemBtn");
    delBtn->setObjectName("delItemBtn");

    QPushButton *scrollRightBtn = new QPushButton("◀");
    QPushButton *scrollLeftBtn  = new QPushButton("▶");
    scrollRightBtn->setFixedSize(20, 28);
    scrollLeftBtn->setFixedSize(20, 28);

    m_itemSeqLabel = new QLabel("1");
    m_itemSeqLabel->setFixedWidth(20);
    m_itemSeqLabel->setAlignment(Qt::AlignCenter);

    itemRow->addWidget(lbl("ت"));
    itemRow->addWidget(m_itemSeqLabel);
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
    itemRow->addWidget(m_editItemBtn);
    itemRow->addWidget(delBtn);
    itemRow->addWidget(scrollLeftBtn);
    itemRow->addWidget(scrollRightBtn);
    tvl->addLayout(itemRow);

    /* ── items table (10 columns, col 9 hidden = product-id) ── */
    m_itemsTable = new QTableWidget(0, 10);
    m_itemsTable->setObjectName("itemsTable");
    m_itemsTable->setHorizontalHeaderLabels({
        QString::fromUtf8("ت"),            // 0
        QString::fromUtf8("الباركود"),      // 1
        QString::fromUtf8("رمز المادة"),    // 2
        QString::fromUtf8("اسم المادة"),    // 3
        QString::fromUtf8("الكمية"),        // 4
        QString::fromUtf8("سعر المفرد"),    // 5
        QString::fromUtf8("المبلغ الكلي"),  // 6
        QString::fromUtf8("الملاحظات"),     // 7
        QString::fromUtf8("العملة"),        // 8
        ""                                   // 9 – hidden product-id
    });
    m_itemsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_itemsTable->setColumnHidden(9, true);
    m_itemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_itemsTable->setMinimumHeight(240);
    m_itemsTable->setEditTriggers(QAbstractItemView::DoubleClicked |
                                  QAbstractItemView::SelectedClicked);
    tvl->addWidget(m_itemsTable);

    /* ===================================================
       MIDDLE TOTALS PANEL
       رصيد المادة | totals | اخر سعر | edit buttons
       =================================================== */
    QWidget *middlePanel = new QWidget;
    middlePanel->setObjectName("middlePanel");
    QHBoxLayout *mpl = new QHBoxLayout(middlePanel);
    mpl->setContentsMargins(4, 3, 4, 3);
    mpl->setSpacing(6);

    m_totalDollarLabel = new QLabel("0"); m_totalDollarLabel->setObjectName("sumLbl");
    m_totalDinarLabel  = new QLabel("0"); m_totalDinarLabel->setObjectName("sumLbl");
    m_totalDollarLabel->setMinimumWidth(90);
    m_totalDinarLabel->setMinimumWidth(90);

    m_itemStockBalLabel = new QLabel("0");
    m_itemStockBalLabel->setObjectName("yellowField");
    m_itemStockBalLabel->setAlignment(Qt::AlignCenter);
    m_itemStockBalLabel->setMinimumWidth(60);

    m_lastPriceDollarLabel = new QLabel("0");
    m_lastPriceDollarLabel->setObjectName("sumGreen");
    m_lastPriceDollarLabel->setAlignment(Qt::AlignCenter);
    m_lastPriceDollarLabel->setMinimumWidth(60);

    m_lastPriceDinarLabel = new QLabel("0");
    m_lastPriceDinarLabel->setObjectName("sumYellow");
    m_lastPriceDinarLabel->setAlignment(Qt::AlignCenter);
    m_lastPriceDinarLabel->setMinimumWidth(60);

    QPushButton *editPenBtn  = new QPushButton("🖊");
    QPushButton *undoBtn     = new QPushButton("↩");
    QPushButton *clearAllBtn = new QPushButton("✖✖");
    for (auto *b : {editPenBtn, undoBtn, clearAllBtn}) {
        b->setFixedSize(36, 30);
        b->setObjectName("toolBtn");
    }

    mpl->addWidget(editPenBtn);
    mpl->addWidget(undoBtn);
    mpl->addWidget(clearAllBtn);
    mpl->addWidget(lbl(QString::fromUtf8("رصيد المادة")));
    mpl->addWidget(m_itemStockBalLabel);
    mpl->addStretch();
    mpl->addWidget(m_totalDinarLabel);
    mpl->addWidget(m_totalDollarLabel);
    mpl->addStretch();
    mpl->addWidget(lbl(QString::fromUtf8("اخر سعر د")));
    mpl->addWidget(m_lastPriceDinarLabel);
    mpl->addWidget(lbl(QString::fromUtf8("اخر سعر $")));
    mpl->addWidget(m_lastPriceDollarLabel);

    tvl->addWidget(middlePanel);

    /* ===================================================
       SUMMARY PANEL – رصيد الزبون / الواصل / الخصم / الرصيد الحالي
       =================================================== */
    m_balDollarLabel         = new QLabel("0");
    m_balDinarLabel          = new QLabel("0");
    m_currentBalDollarLabel  = new QLabel("0");
    m_currentBalDinarLabel   = new QLabel("0");

    QWidget *summaryPanel = new QWidget;
    summaryPanel->setObjectName("summaryPanel");
    QGridLayout *sg = new QGridLayout(summaryPanel);
    sg->setSpacing(2);
    sg->setContentsMargins(4, 3, 4, 3);

    auto yellowVal = [](const QString &t = "0") -> QLabel* {
        QLabel *l = new QLabel(t);
        l->setObjectName("sumYellow");
        l->setAlignment(Qt::AlignCenter);
        l->setMinimumWidth(55);
        return l;
    };
    auto greenVal = [](const QString &t = "0") -> QLabel* {
        QLabel *l = new QLabel(t);
        l->setObjectName("sumGreen");
        l->setAlignment(Qt::AlignCenter);
        l->setMinimumWidth(55);
        return l;
    };
    auto pinkVal = [](const QString &t = "0") -> QLabel* {
        QLabel *l = new QLabel(t);
        l->setObjectName("sumPink");
        l->setAlignment(Qt::AlignCenter);
        l->setMinimumWidth(55);
        return l;
    };
    auto greyLbl = [](const QString &t) -> QLabel* {
        QLabel *l = new QLabel(t);
        l->setObjectName("sumGreyLbl");
        l->setAlignment(Qt::AlignCenter);
        return l;
    };

    QPushButton *addToWaitBtn = new QPushButton(QString::fromUtf8("اضافة الى\nالانتظار"));
    addToWaitBtn->setObjectName("waitBtn");
    addToWaitBtn->setFixedSize(80, 48);
    QPushButton *retrieveWaitBtn = new QPushButton(QString::fromUtf8("استرجاع\nمن الانتظار"));
    retrieveWaitBtn->setObjectName("waitBtn");
    retrieveWaitBtn->setFixedSize(80, 48);

    for (QLabel *lb : {m_balDollarLabel, m_balDinarLabel,
                       m_currentBalDollarLabel, m_currentBalDinarLabel}) {
        lb->setObjectName("sumYellow");
        lb->setAlignment(Qt::AlignCenter);
        lb->setMinimumWidth(55);
    }

    m_receivedDollarEdit = new QLineEdit("0"); m_receivedDollarEdit->setFixedWidth(60);
    m_receivedDinarEdit  = new QLineEdit("0"); m_receivedDinarEdit->setFixedWidth(60);
    m_discountDinarEdit  = new QLineEdit("0"); m_discountDinarEdit->setFixedWidth(60);

    QLabel *portageLabel   = pinkVal();
    QLabel *equivDollarLbl = greenVal();
    QLabel *equivDinarLbl  = greenVal();

    int c = 0;
    sg->addWidget(addToWaitBtn,     0, c, 2, 1); c++;
    sg->addWidget(retrieveWaitBtn,  0, c, 2, 1); c++;
    sg->addWidget(greyLbl(QString::fromUtf8("حمالية")),       0, c++);
    sg->addWidget(portageLabel,                                0, c++);
    sg->addWidget(greyLbl(QString::fromUtf8("الواصل $")),     0, c++);
    sg->addWidget(m_receivedDollarEdit,                        0, c++);
    sg->addWidget(greyLbl(QString::fromUtf8("الواصل دينار")), 0, c++);
    sg->addWidget(m_receivedDinarEdit,                         0, c++);
    sg->addWidget(greyLbl(QString::fromUtf8("رصيد الزبون")),  0, c++);
    sg->addWidget(m_balDollarLabel,                            0, c++);
    sg->addWidget(greyLbl("$"),                                0, c++);
    sg->addWidget(m_balDinarLabel,                             0, c++);
    sg->addWidget(greyLbl(QString::fromUtf8("دينار")),        0, c++);

    c = 2;
    sg->addWidget(greyLbl(QString::fromUtf8("الخصم دينار")),   1, c++);
    sg->addWidget(m_discountDinarEdit,                           1, c++);
    sg->addWidget(greyLbl(QString::fromUtf8("ما يعادل $")),    1, c++);
    sg->addWidget(equivDollarLbl,                                1, c++);
    sg->addWidget(greyLbl(QString::fromUtf8("مايعادل دينار")), 1, c++);
    sg->addWidget(equivDinarLbl,                                 1, c++);
    sg->addWidget(greyLbl(QString::fromUtf8("الرصيد الحالي")), 1, c++);
    sg->addWidget(m_currentBalDollarLabel,                       1, c++);
    sg->addWidget(greyLbl("$"),                                  1, c++);
    sg->addWidget(m_currentBalDinarLabel,                        1, c++);
    sg->addWidget(greyLbl(QString::fromUtf8("دينار")),          1, c++);

    tvl->addWidget(summaryPanel);

    /* ── recalculate current balance when received/discount changes ── */
    auto recalcBalance = [this, equivDollarLbl, equivDinarLbl]() {
        double rate = m_exchangeRateEdit->text().toDouble();
        if (rate <= 0) rate = 1.0;
        double totalDollar = m_totalDollarLabel->text().toDouble();
        double balDollar   = m_balDollarLabel->text().toDouble();
        double balDinar    = m_balDinarLabel->text().toDouble();
        double recvDollar  = m_receivedDollarEdit->text().toDouble();
        double recvDinar   = m_receivedDinarEdit->text().toDouble();
        double discount    = m_discountDinarEdit->text().toDouble();

        double discountDollarEquiv = discount / rate;
        equivDollarLbl->setText(QString::number(discountDollarEquiv, 'f', 2));
        equivDinarLbl->setText(QString::number(totalDollar * rate, 'f', 0));

        double curBalDollar = balDollar + totalDollar - recvDollar - discountDollarEquiv;
        double curBalDinar  = balDinar - recvDinar;
        m_currentBalDollarLabel->setText(QString::number(curBalDollar, 'f', 2));
        m_currentBalDinarLabel->setText(QString::number(curBalDinar, 'f', 0));
    };
    connect(m_receivedDollarEdit, &QLineEdit::textChanged, this, recalcBalance);
    connect(m_receivedDinarEdit,  &QLineEdit::textChanged, this, recalcBalance);
    connect(m_discountDinarEdit,  &QLineEdit::textChanged, this, recalcBalance);

    /* ── customer extra info ── */
    QGridLayout *custInfo = new QGridLayout;
    m_addressEdit = new QLineEdit; m_addressEdit->setReadOnly(true);
    m_phoneEdit   = new QLineEdit; m_phoneEdit->setReadOnly(true);
    m_driverCombo = new QComboBox; m_driverCombo->setEditable(true);
    m_driverCombo->setPlaceholderText(QString::fromUtf8("المجهز"));
    m_carNoEdit   = new QLineEdit;
    m_operatorLabel = new QLabel("1"); m_operatorLabel->setObjectName("operLbl");
    m_timeLabel   = new QLabel(QTime::currentTime().toString("hh:mm AP"));
    m_timeLabel->setObjectName("timeLbl");

    custInfo->addWidget(lbl(QString::fromUtf8("العنوان")),     0, 0);
    custInfo->addWidget(m_addressEdit,                          0, 1);
    custInfo->addWidget(lbl(QString::fromUtf8("هاتف الزبون")),0, 2);
    custInfo->addWidget(m_phoneEdit,                            0, 3);
    custInfo->addWidget(lbl(QString::fromUtf8("المجهز")),      0, 4);
    custInfo->addWidget(m_driverCombo,                          0, 5);
    custInfo->addWidget(m_unitCombo,                            0, 6);
    custInfo->addWidget(lbl(QString::fromUtf8("اسم السائق")), 1, 0);
    custInfo->addWidget(new QComboBox,                          1, 1);
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
    m_groupCombo = new QComboBox;
    m_groupCombo->setEditable(true);
    m_groupCombo->addItem(QString::fromUtf8("كل الاصناف"));

    QLabel *partLbl = new QLabel(QString::fromUtf8("جزء من الاسم"));
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
    rvl->addWidget(m_groupCombo);
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

    m_saveBtn   = toolBtn("💾", QString::fromUtf8("حفظ"));
    m_newBtn    = toolBtn("📄", QString::fromUtf8("جديد"));
    m_searchBtn = toolBtn("🔍", QString::fromUtf8("بحث"));
    m_deleteBtn = toolBtn("✖",  QString::fromUtf8("حذف"));
    m_printBtn  = toolBtn("🖨", QString::fromUtf8("طباعة"));

    m_pdfBtn = new QPushButton("PDF");
    m_pdfBtn->setObjectName("pdfBtn");
    m_pdfBtn->setFixedHeight(36);
    m_pdfBtn->setFont(QFont("Tahoma", 10, QFont::Bold));

    m_thermalBtn = new QPushButton(QString::fromUtf8("الطابعة\nالشريطية"));
    m_thermalBtn->setObjectName("thermalBtn");
    m_thermalBtn->setFixedHeight(36);

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
    m_printedCheck = new QCheckBox(QString::fromUtf8("طبعت"));

    m_firstBtn = toolBtn("⏮");
    m_prevBtn  = toolBtn("◀");
    m_nextBtn  = toolBtn("▶");
    m_lastBtn  = toolBtn("⏭");
    m_navLabel = new QLabel("1");
    m_navLabel->setObjectName("navLbl");
    m_navLabel->setAlignment(Qt::AlignCenter);
    m_navLabel->setFixedWidth(45);
    m_totalCountLabel = new QLabel("1");
    m_totalCountLabel->setAlignment(Qt::AlignCenter);
    m_totalCountLabel->setFixedWidth(45);

    QPushButton *recvBtn = toolBtn("🤲", QString::fromUtf8("قبض"));
    QPushButton *viewBtn = toolBtn("👁",  QString::fromUtf8("معاينة"));
    QPushButton *fwdBtn  = toolBtn("➡",  QString::fromUtf8("إرسال"));
    QPushButton *sigBtn  = toolBtn("✋",  QString::fromUtf8("توقيع"));

    tbl->addWidget(m_saveBtn);
    tbl->addWidget(m_newBtn);
    tbl->addWidget(m_searchBtn);
    tbl->addWidget(m_deleteBtn);
    tbl->addWidget(m_printBtn);
    tbl->addWidget(lbl(QString::fromUtf8("عدد النسخ")));
    tbl->addWidget(m_copiesSpinBox);
    tbl->addWidget(m_printedCheck);
    tbl->addWidget(m_thermalBtn);
    tbl->addWidget(m_prepListBtn);
    tbl->addWidget(m_customerStatBtn);
    tbl->addWidget(m_productMoveBtn);
    tbl->addWidget(m_pdfBtn);
    tbl->addStretch();
    tbl->addWidget(m_firstBtn);
    tbl->addWidget(m_prevBtn);
    tbl->addWidget(m_navLabel);
    tbl->addWidget(lbl(QString::fromUtf8("من")));
    tbl->addWidget(m_totalCountLabel);
    tbl->addWidget(m_nextBtn);
    tbl->addWidget(m_lastBtn);
    tbl->addWidget(recvBtn);
    tbl->addWidget(viewBtn);
    tbl->addWidget(fwdBtn);
    tbl->addWidget(sigBtn);

    root->addWidget(toolbar);

    /* ===================================================
       SIGNAL CONNECTIONS
       =================================================== */
    connect(addBtn,            &QPushButton::clicked, this, &SalesWindow::addItem);
    connect(m_editItemBtn,     &QPushButton::clicked, this, &SalesWindow::editItem);
    connect(delBtn,            &QPushButton::clicked, this, &SalesWindow::deleteItem);
    connect(m_saveBtn,         &QPushButton::clicked, this, &SalesWindow::saveInvoice);
    connect(m_newBtn,          &QPushButton::clicked, this, &SalesWindow::newInvoice);
    connect(m_searchBtn,       &QPushButton::clicked, this, &SalesWindow::searchInvoice);
    connect(m_deleteBtn,       &QPushButton::clicked, this, &SalesWindow::deleteInvoice);
    connect(m_printBtn,        &QPushButton::clicked, this, &SalesWindow::printInvoice);
    connect(m_pdfBtn,          &QPushButton::clicked, this, &SalesWindow::exportPDF);
    connect(m_thermalBtn,      &QPushButton::clicked, this, &SalesWindow::printThermal);
    connect(m_prepListBtn,     &QPushButton::clicked, this, &SalesWindow::printPreparationList);
    connect(m_customerStatBtn, &QPushButton::clicked, this, &SalesWindow::showCustomerStatement);
    connect(addToWaitBtn,      &QPushButton::clicked, this, &SalesWindow::addToWait);
    connect(retrieveWaitBtn,   &QPushButton::clicked, this, &SalesWindow::retrieveFromWait);
    connect(m_firstBtn,        &QPushButton::clicked, this, &SalesWindow::navigateFirst);
    connect(m_prevBtn,         &QPushButton::clicked, this, &SalesWindow::navigatePrev);
    connect(m_nextBtn,         &QPushButton::clicked, this, &SalesWindow::navigateNext);
    connect(m_lastBtn,         &QPushButton::clicked, this, &SalesWindow::navigateLast);

    connect(m_customerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SalesWindow::onCustomerSelected);
    connect(m_productCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SalesWindow::onProductSelected);
    connect(m_qtyEdit,       &QLineEdit::textChanged, this, &SalesWindow::onQtyChanged);
    connect(m_unitPriceEdit, &QLineEdit::textChanged, this, &SalesWindow::onQtyChanged);

    /* currency change → re-fetch correct prices from DB */
    connect(m_currencyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SalesWindow::onCurrencyChanged);
    /* invoice type change (مفرد/جملة) → re-price all items */
    connect(m_invoiceTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SalesWindow::onInvoiceTypeChanged);

    /* in-table product name change via delegate */
    connect(m_itemsTable, &QTableWidget::itemChanged,
            [this](QTableWidgetItem *item) {
        if (item->column() != 3) return;
        int row = item->row();
        QTableWidgetItem *idItem = m_itemsTable->item(row, 9);
        if (!idItem) return;
        int pid = idItem->text().toInt();
        if (pid <= 0) return;
        repriceRow(row, pid);
    });

    /* row selection → update stock/last-price labels */
    connect(m_itemsTable, &QTableWidget::currentCellChanged,
            [this](int row, int, int, int) {
        if (row < 0) return;
        QTableWidgetItem *idItem = m_itemsTable->item(row, 9);
        if (!idItem) return;
        int pid = idItem->text().toInt();
        if (pid <= 0) return;
        updateProductInfoLabels(pid);
    });

    /* filter product list by group + name/barcode */
    auto filterProductList = [this]() {
        QString grp = m_groupCombo->currentText();
        bool allGroups = (grp == QString::fromUtf8("كل الاصناف") || grp.isEmpty());
        QString txt = m_partNameEdit->text();
        for (int r = 0; r < m_productList->rowCount(); ++r) {
            QTableWidgetItem *nm = m_productList->item(r, 1);
            if (!nm) { m_productList->setRowHidden(r, true); continue; }
            bool groupOk = allGroups || nm->data(Qt::UserRole+2).toString() == grp;
            bool textOk  = txt.isEmpty()
                || nm->text().contains(txt, Qt::CaseInsensitive)
                || nm->data(Qt::UserRole+1).toString().contains(txt, Qt::CaseInsensitive);
            m_productList->setRowHidden(r, !(groupOk && textOk));
        }
    };
    connect(m_partNameEdit, &QLineEdit::textChanged, [filterProductList](const QString &){ filterProductList(); });
    connect(m_groupCombo, &QComboBox::currentTextChanged, [filterProductList](const QString &){ filterProductList(); });

    /* select product from right panel */
    connect(m_productList, &QTableWidget::itemDoubleClicked,
            [this](QTableWidgetItem *item) {
        int row = item->row();
        QTableWidgetItem *nameItem = m_productList->item(row, 1);
        if (!nameItem) return;
        int idx = m_productCombo->findText(nameItem->text());
        if (idx >= 0) m_productCombo->setCurrentIndex(idx);
    });

    /* clear-all button */
    connect(clearAllBtn, &QPushButton::clicked, [this]() {
        if (QMessageBox::question(this, "",
            QString::fromUtf8("هل تريد مسح جميع المواد؟"),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            m_itemsTable->setRowCount(0);
            updateTotals();
        }
    });

    /* undo last item */
    connect(undoBtn, &QPushButton::clicked, [this]() {
        int lastRow = m_itemsTable->rowCount() - 1;
        if (lastRow >= 0) { m_itemsTable->removeRow(lastRow); updateTotals(); }
    });

    /* invoice number spin → jump to invoice */
    connect(m_invoiceNoSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int val) {
        if (m_currentInvoiceId == val) return;
        QSqlQuery q;
        q.prepare("SELECT id FROM sales_invoices WHERE id=?");
        q.addBindValue(val); q.exec();
        if (q.next()) loadInvoice(val);
    });
}

/* ================================================================
   Styles
   ================================================================ */
void SalesWindow::applyStyles()
{
    setStyleSheet(R"(
        QDialog           { background: #d0d8e8; }
        #headerWidget     { background: #c0ccdd; border-bottom:1px solid #8899aa; border-radius:4px; }
        #tableArea        { background: #b8c8d8; }
        #middlePanel      { background: #b8c8d8; }

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

        #toolbar      { background:#556677; border-top:1px solid #334455; }
        #toolBtn      { background:#778899; border:1px solid #556; color:white; border-radius:3px; }
        #toolBtn:hover{ background:#889aab; }
        #pdfBtn       { background:#cc2200; color:white; font-weight:bold; border-radius:3px; padding:0 8px; }
        #thermalBtn   { background:#ff6666; color:white; font-weight:bold; border-radius:3px; padding:0 6px; }
        #auxBtn       { background:#c8d8f0; border:1px solid #88aacc; border-radius:3px; color:#003366; padding:0 6px; }
        #navLbl       { background:#FFD700; color:#003366; font-weight:bold; border:1px solid #aa8800; border-radius:2px; }
        #operLbl      { background:#cc88ff; color:#330033; font-weight:bold; padding:2px 8px; border-radius:3px; }
        #timeLbl      { background:#ffffc0; color:#003366; font-weight:bold; padding:2px 6px; border:1px solid #aaaa60; border-radius:2px; }

        #yellowField  { background:#ffffc0; border:1px solid #aaaa60; font-weight:bold; }
        #stockLbl     { color:#003366; font-weight:bold; padding:2px 4px; background:#e8f4ff; border:1px solid #99bbdd; border-radius:2px; }
        #sumLbl       { background:#ffffc0; color:#003366; font-weight:bold; padding:2px 8px; border:1px solid #aaaa60; border-radius:2px; }
        #addItemBtn   { background:#44aa44; color:white; border-radius:3px; border:1px solid #228822; }
        #delItemBtn   { background:#aa4422; color:white; border-radius:3px; border:1px solid #882200; }

        #summaryPanel { background:#c0c0c0; border:1px solid #999999; border-radius:3px; }
        #sumYellow { background:#ffffc0; color:#003366; font-weight:bold; padding:2px 6px; border:1px solid #aaaa60; border-radius:2px; font-family:Tahoma; }
        #sumGreen  { background:#c0ffc0; color:#003366; font-weight:bold; padding:2px 6px; border:1px solid #60aa60; border-radius:2px; font-family:Tahoma; }
        #sumPink   { background:#ffc0c0; color:#660000; font-weight:bold; padding:2px 6px; border:1px solid #cc8888; border-radius:2px; font-family:Tahoma; }
        #sumGreyLbl { background:#d8d8d8; color:#003366; font-weight:bold; padding:2px 4px; border:1px solid #bbbbbb; border-radius:2px; font-family:Tahoma; font-size:9pt; }
        #waitBtn { background:#d0d8e0; color:#003366; font-weight:bold; border:1px solid #8899aa; border-radius:3px; font-family:Tahoma; font-size:8pt; }
        #waitBtn:hover { background:#e0e8f0; }

        QComboBox  { background:#ffffc0; border:1px solid #aaaa60; border-radius:2px; }
        QLineEdit  { background:#eef4ff; border:1px solid #99aacc; border-radius:2px; padding:1px 3px; }
        QDateEdit  { background:white; border:1px solid #99aacc; border-radius:2px; }
        QLabel     { font-family:Tahoma; color:#003366; }
    )");
}

/* ================================================================
   Helpers
   ================================================================ */
void SalesWindow::updateDayName()
{
    QStringList dayNames = {
        QString::fromUtf8("الأحد"), QString::fromUtf8("الاثنين"),
        QString::fromUtf8("الثلاثاء"), QString::fromUtf8("الأربعاء"),
        QString::fromUtf8("الخميس"), QString::fromUtf8("الجمعة"),
        QString::fromUtf8("السبت")
    };
    m_dayLabel->setText(dayNames[m_dateEdit->date().dayOfWeek() % 7]);
}

double SalesWindow::getProductPrice(int productId)
{
    QSqlQuery q;
    q.prepare("SELECT retail_dollar, retail_dinar, wholesale_dollar, wholesale_dinar "
              "FROM products WHERE id=?");
    q.addBindValue(productId); q.exec();
    if (!q.next()) return 0.0;

    bool isWholesale = (m_invoiceTypeCombo->currentIndex() == 1);
    bool isDinar     = (m_currencyCombo->currentText() == QString::fromUtf8("دينار"));

    if (isWholesale)
        return isDinar ? q.value(3).toDouble() : q.value(2).toDouble();
    else
        return isDinar ? q.value(1).toDouble() : q.value(0).toDouble();
}

void SalesWindow::repriceRow(int row, int productId)
{
    double price = getProductPrice(productId);

    QSqlQuery q;
    q.prepare("SELECT stock_qty, barcode FROM products WHERE id=?");
    q.addBindValue(productId); q.exec();
    if (q.next()) {
        if (auto *bc = m_itemsTable->item(row, 1))
            bc->setText(q.value(1).toString());
    }

    if (auto *pr = m_itemsTable->item(row, 5))
        pr->setText(QString::number(price, 'f', 2));

    double qty = 1.0;
    if (auto *qitem = m_itemsTable->item(row, 4))
        qty = qitem->text().toDouble();
    if (auto *tot = m_itemsTable->item(row, 6))
        tot->setText(QString::number(qty * price, 'f', 2));

    if (auto *curItem = m_itemsTable->item(row, 8))
        curItem->setText(m_currencyCombo->currentText());

    updateTotals();
}

void SalesWindow::updateProductInfoLabels(int productId)
{
    QSqlQuery q;
    q.prepare("SELECT retail_dollar, retail_dinar, stock_qty FROM products WHERE id=?");
    q.addBindValue(productId); q.exec();
    if (!q.next()) return;
    m_lastPriceDollarLabel->setText(QString::number(q.value(0).toDouble(), 'f', 2));
    m_lastPriceDinarLabel->setText(QString::number(q.value(1).toDouble(), 'f', 0));
    m_itemStockBalLabel->setText(q.value(2).toString());
}

void SalesWindow::refreshInvoiceCount()
{
    QSqlQuery q;
    q.exec("SELECT COUNT(*) FROM sales_invoices");
    if (q.next()) {
        m_totalInvoices = q.value(0).toInt();
        m_totalCountLabel->setText(QString::number(m_totalInvoices));
    }
}

void SalesWindow::renumberRows()
{
    for (int r = 0; r < m_itemsTable->rowCount(); ++r)
        if (auto *item = m_itemsTable->item(r, 0))
            item->setText(QString::number(r + 1));
    m_itemSeqLabel->setText(QString::number(m_itemsTable->rowCount() + 1));
}

/* ================================================================
   Data loading
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

    // Reload groups into combo (keep "كل الاصناف" as first item)
    m_groupCombo->blockSignals(true);
    QString prevGroup = m_groupCombo->currentText();
    m_groupCombo->clear();
    m_groupCombo->addItem(QString::fromUtf8("كل الاصناف"));
    QStringList groups = Database::getProductGroups();
    for (const QString &g : groups)
        if (!g.isEmpty()) m_groupCombo->addItem(g);
    int gi = m_groupCombo->findText(prevGroup);
    m_groupCombo->setCurrentIndex(gi >= 0 ? gi : 0);
    m_groupCombo->blockSignals(false);

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
        nm->setData(Qt::UserRole,    q.value(0));           // product id
        nm->setData(Qt::UserRole+1,  q.value(1).toString()); // barcode
        nm->setData(Qt::UserRole+2,  q.value(3).toString()); // product_group
        m_productList->setItem(row, 1, nm);
        ++row;
    }

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
    m_receivedDollarEdit->setText("0");
    m_receivedDinarEdit->setText("0");
    m_discountDinarEdit->setText("0");
    m_totalDollarLabel->setText("0");
    m_totalDinarLabel->setText("0");
    m_balDollarLabel->setText("0");
    m_balDinarLabel->setText("0");
    m_currentBalDollarLabel->setText("0");
    m_currentBalDinarLabel->setText("0");
    m_lastPriceDollarLabel->setText("0");
    m_lastPriceDinarLabel->setText("0");
    m_itemStockBalLabel->setText("0");
    m_itemSeqLabel->setText("1");
    m_timeLabel->setText(QTime::currentTime().toString("hh:mm AP"));
    m_printedCheck->setChecked(false);
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
    int payIdx = m_paymentTypeCombo->findText(q.value(2).toString());
    if (payIdx >= 0) m_paymentTypeCombo->setCurrentIndex(payIdx);
    int curIdx = m_currencyCombo->findText(q.value(3).toString());
    if (curIdx >= 0) m_currencyCombo->setCurrentIndex(curIdx);
    m_notesEdit->setText(q.value(4).toString());
    m_invoiceNoSpin->blockSignals(true);
    m_invoiceNoSpin->setValue(id);
    m_invoiceNoSpin->blockSignals(false);
    m_navLabel->setText(QString::number(id));

    m_itemsTable->blockSignals(true);
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
        m_itemsTable->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        m_itemsTable->setItem(row, 1, new QTableWidgetItem(qi.value(4).toString()));
        m_itemsTable->setItem(row, 2, new QTableWidgetItem(QString::number(qi.value(5).toInt())));
        m_itemsTable->setItem(row, 3, new QTableWidgetItem(qi.value(0).toString()));
        m_itemsTable->setItem(row, 4, new QTableWidgetItem(QString::number(qi.value(1).toDouble())));
        m_itemsTable->setItem(row, 5, new QTableWidgetItem(QString::number(qi.value(2).toDouble(), 'f', 2)));
        m_itemsTable->setItem(row, 6, new QTableWidgetItem(QString::number(qi.value(3).toDouble(), 'f', 2)));
        m_itemsTable->setItem(row, 7, new QTableWidgetItem(""));
        m_itemsTable->setItem(row, 8, new QTableWidgetItem(q.value(3).toString()));
        m_itemsTable->setItem(row, 9, new QTableWidgetItem(QString::number(qi.value(5).toInt())));
        ++row;
    }
    m_itemsTable->blockSignals(false);
    m_itemSeqLabel->setText(QString::number(row + 1));
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

    double price = getProductPrice(pid);
    m_unitPriceEdit->setText(QString::number(price, 'f', 2));
    m_stockLabel->setText(q.value(4).toString());
    m_barcodeCombo->setCurrentText(q.value(5).toString());

    m_lastPriceDollarLabel->setText(QString::number(q.value(0).toDouble(), 'f', 2));
    m_lastPriceDinarLabel->setText(QString::number(q.value(1).toDouble(), 'f', 0));
    m_itemStockBalLabel->setText(q.value(4).toString());

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
    m_balDollarLabel->setText(QString::number(q.value(0).toDouble(), 'f', 2));
    m_balDinarLabel->setText(QString::number(q.value(1).toDouble(), 'f', 0));
    m_addressEdit->setText(q.value(2).toString());
    m_phoneEdit->setText(q.value(3).toString());
}

void SalesWindow::onQtyChanged()
{
    double qty   = m_qtyEdit->text().toDouble();
    double price = m_unitPriceEdit->text().toDouble();
    m_totalEdit->setText(QString::number(qty * price, 'f', 2));
}

void SalesWindow::onCurrencyChanged()
{
    m_itemsTable->blockSignals(true);
    for (int r = 0; r < m_itemsTable->rowCount(); ++r) {
        QTableWidgetItem *idItem = m_itemsTable->item(r, 9);
        if (!idItem) continue;
        int pid = idItem->text().toInt();
        if (pid <= 0) continue;
        repriceRow(r, pid);
    }
    m_itemsTable->blockSignals(false);
    updateTotals();

    if (m_productCombo->currentIndex() >= 0) {
        int pid = m_productCombo->currentData().toInt();
        double price = getProductPrice(pid);
        m_unitPriceEdit->setText(QString::number(price, 'f', 2));
    }
}

void SalesWindow::onInvoiceTypeChanged()
{
    m_itemsTable->blockSignals(true);
    for (int r = 0; r < m_itemsTable->rowCount(); ++r) {
        QTableWidgetItem *idItem = m_itemsTable->item(r, 9);
        if (!idItem) continue;
        int pid = idItem->text().toInt();
        if (pid <= 0) continue;
        repriceRow(r, pid);
    }
    m_itemsTable->blockSignals(false);
    updateTotals();

    if (m_productCombo->currentIndex() >= 0) {
        int pid = m_productCombo->currentData().toInt();
        double price = getProductPrice(pid);
        m_unitPriceEdit->setText(QString::number(price, 'f', 2));
    }
}

/* ================================================================
   Item management
   ================================================================ */
void SalesWindow::addItem()
{
    if (m_productCombo->currentText().isEmpty()) return;
    double qty   = m_qtyEdit->text().toDouble();
    double price = m_unitPriceEdit->text().toDouble();
    double total = qty * price;
    int    pid   = m_productCombo->currentData().toInt();

    int row = m_itemsTable->rowCount();
    m_itemsTable->blockSignals(true);
    m_itemsTable->insertRow(row);
    m_itemsTable->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
    m_itemsTable->setItem(row, 1, new QTableWidgetItem(m_barcodeCombo->currentText()));
    m_itemsTable->setItem(row, 2, new QTableWidgetItem(QString::number(pid)));
    m_itemsTable->setItem(row, 3, new QTableWidgetItem(m_productCombo->currentText()));
    m_itemsTable->setItem(row, 4, new QTableWidgetItem(QString::number(qty)));
    m_itemsTable->setItem(row, 5, new QTableWidgetItem(QString::number(price, 'f', 2)));
    m_itemsTable->setItem(row, 6, new QTableWidgetItem(QString::number(total, 'f', 2)));
    m_itemsTable->setItem(row, 7, new QTableWidgetItem(""));
    m_itemsTable->setItem(row, 8, new QTableWidgetItem(m_currencyCombo->currentText()));
    m_itemsTable->setItem(row, 9, new QTableWidgetItem(QString::number(pid)));
    m_itemsTable->blockSignals(false);

    m_qtyEdit->setText("0");
    m_itemSeqLabel->setText(QString::number(row + 2));
    updateTotals();
}

void SalesWindow::editItem()
{
    int row = m_itemsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("اختر مادة من الجدول للتعديل"));
        return;
    }
    m_barcodeCombo->setCurrentText(m_itemsTable->item(row, 1)->text());
    int idx = m_productCombo->findText(m_itemsTable->item(row, 3)->text());
    if (idx >= 0) m_productCombo->setCurrentIndex(idx);
    m_qtyEdit->setText(m_itemsTable->item(row, 4)->text());
    m_unitPriceEdit->setText(m_itemsTable->item(row, 5)->text());

    m_itemsTable->removeRow(row);
    renumberRows();
    updateTotals();
}

void SalesWindow::deleteItem()
{
    int row = m_itemsTable->currentRow();
    if (row >= 0) {
        m_itemsTable->removeRow(row);
        renumberRows();
        updateTotals();
    }
}

void SalesWindow::updateTotals()
{
    double total = 0;
    for (int r = 0; r < m_itemsTable->rowCount(); ++r)
        if (auto *item = m_itemsTable->item(r, 6))
            total += item->text().toDouble();

    bool isDinar = (m_currencyCombo->currentText() == QString::fromUtf8("دينار"));
    double rate = m_exchangeRateEdit->text().toDouble();
    if (rate <= 0) rate = 1.0;

    if (isDinar) {
        m_totalDinarLabel->setText(QString::number(total, 'f', 0));
        m_totalDollarLabel->setText(QString::number(total / rate, 'f', 2));
    } else {
        m_totalDollarLabel->setText(QString::number(total, 'f', 2));
        m_totalDinarLabel->setText(QString::number(total * rate, 'f', 0));
    }
}

/* ================================================================
   Save / New / Delete / Search
   ================================================================ */
void SalesWindow::saveInvoice()
{
    int custId = m_customerCombo->currentData().toInt();
    QString customerName = m_customerCombo->currentText().trimmed();

    if (custId <= 0) {
        if (customerName.isEmpty()) {
            QMessageBox::warning(this, "", QString::fromUtf8("أدخل اسم الزبون"));
            m_customerCombo->setFocus();
            return;
        }
        auto reply = QMessageBox::question(this,
            QString::fromUtf8("زبون جديد"),
            QString::fromUtf8("الزبون '%1' غير موجود. هل تريد إضافته؟").arg(customerName),
            QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            custId = Database::addCustomer(customerName, "", m_addressEdit->text(),
                m_phoneEdit->text(), "", 0, 0, 0);
            if (custId < 0) {
                QMessageBox::critical(this, "", QString::fromUtf8("فشل إضافة الزبون الجديد"));
                return;
            }
            loadCustomers();
            for (int i = 0; i < m_customerCombo->count(); ++i)
                if (m_customerCombo->itemData(i).toInt() == custId) {
                    m_customerCombo->setCurrentIndex(i); break;
                }
        } else return;
    }

    if (m_itemsTable->rowCount() == 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("لا توجد مواد في الفاتورة"));
        return;
    }

    double total = m_totalDollarLabel->text().toDouble();
    double totalDinar = m_totalDinarLabel->text().toDouble();

    int id = Database::createSalesInvoice(
        custId, m_dateEdit->date(), m_paymentTypeCombo->currentText(),
        m_currencyCombo->currentText(), m_notesEdit->text(),
        total, totalDinar, 1);

    if (id < 0) {
        QMessageBox::critical(this, "", QString::fromUtf8("فشل حفظ الفاتورة"));
        return;
    }

    for (int r = 0; r < m_itemsTable->rowCount(); ++r) {
        int    pid     = m_itemsTable->item(r, 9)->text().toInt();
        double qty     = m_itemsTable->item(r, 4)->text().toDouble();
        double price   = m_itemsTable->item(r, 5)->text().toDouble();
        double tot     = m_itemsTable->item(r, 6)->text().toDouble();
        QString rowCur = m_itemsTable->item(r, 8) ? m_itemsTable->item(r, 8)->text()
                                                  : m_currencyCombo->currentText();
        QString itemNotes = m_itemsTable->item(r, 7) ? m_itemsTable->item(r, 7)->text() : "";
        bool dollar = (rowCur == "$");
        Database::addSalesItem(id, pid, qty,
            dollar ? price : 0.0,   // unit_price_dollar
            dollar ? 0.0  : price,  // unit_price_dinar
            dollar ? tot  : 0.0,    // total_dollar
            dollar ? 0.0  : tot,    // total_dinar
            itemNotes);
    }

    // ── Accounting: cash box + customer balance ──────────────────
    QString currency    = m_currencyCombo->currentText();
    QString paymentType = m_paymentTypeCombo->currentText();
    double  rate        = m_exchangeRateEdit->text().toDouble();
    double  recDollar   = m_receivedDollarEdit->text().toDouble();
    double  recDinar    = m_receivedDinarEdit->text().toDouble();
    bool    isCash      = (paymentType == QString::fromUtf8("نقدي")
                        || paymentType == QString::fromUtf8("جملة"));
    QString docNo       = QString::number(id);
    QString custName    = m_customerCombo->currentText();

    if (isCash) {
        // Add received amount to cash box
        double cashDollar = (currency == "$" && recDollar > 0) ? recDollar : 0.0;
        double cashDinar  = (currency != "$" && recDinar  > 0) ? recDinar  : 0.0;
        if (cashDollar > 0 || cashDinar > 0) {
            double equivDinar  = cashDollar * rate;
            double equivDollar = (rate > 0) ? (cashDinar / rate) : 0.0;
            Database::addCashTransaction(
                0 /*قبض*/, m_dateEdit->date(),
                QTime::currentTime().toString("hh:mm AP"),
                1, docNo,
                QString::fromUtf8("مبيعات"), custName,
                cashDollar, cashDinar,
                equivDinar, equivDollar,
                rate, QString::fromUtf8("صندوق"), "");
        }
        // Shortfall: invoice amount not covered by received cash → add to customer balance
        if (currency == "$") {
            double shortfall = total - recDollar;
            if (shortfall > 0.001) {
                QSqlQuery uq;
                uq.prepare("UPDATE customers SET balance_dollar = balance_dollar + ? WHERE id=?");
                uq.addBindValue(shortfall);
                uq.addBindValue(custId);
                uq.exec();
            }
        } else {
            double shortfall = totalDinar - recDinar;
            if (shortfall > 0.001) {
                QSqlQuery uq;
                uq.prepare("UPDATE customers SET balance_dinar = balance_dinar + ? WHERE id=?");
                uq.addBindValue(shortfall);
                uq.addBindValue(custId);
                uq.exec();
            }
        }
    } else {
        // Credit (آجل): entire invoice amount added to customer balance
        QSqlQuery uq;
        if (currency == "$") {
            uq.prepare("UPDATE customers SET balance_dollar = balance_dollar + ? WHERE id=?");
            uq.addBindValue(total);
        } else {
            uq.prepare("UPDATE customers SET balance_dinar = balance_dinar + ? WHERE id=?");
            uq.addBindValue(totalDinar);
        }
        uq.addBindValue(custId);
        uq.exec();
        // If any cash was also received on a credit invoice, add it to the box and deduct from balance
        if (recDollar > 0.001) {
            double equivDinar = recDollar * rate;
            Database::addCashTransaction(
                0, m_dateEdit->date(),
                QTime::currentTime().toString("hh:mm AP"),
                1, docNo,
                QString::fromUtf8("مبيعات"), custName,
                recDollar, 0, equivDinar, recDollar, rate,
                QString::fromUtf8("صندوق"), "");
            QSqlQuery uq2;
            uq2.prepare("UPDATE customers SET balance_dollar = balance_dollar - ? WHERE id=?");
            uq2.addBindValue(recDollar);
            uq2.addBindValue(custId);
            uq2.exec();
        }
        if (recDinar > 0.001) {
            double equivDollar = (rate > 0) ? (recDinar / rate) : 0.0;
            Database::addCashTransaction(
                0, m_dateEdit->date(),
                QTime::currentTime().toString("hh:mm AP"),
                1, docNo,
                QString::fromUtf8("مبيعات"), custName,
                0, recDinar, recDinar, equivDollar, rate,
                QString::fromUtf8("صندوق"), "");
            QSqlQuery uq2;
            uq2.prepare("UPDATE customers SET balance_dinar = balance_dinar - ? WHERE id=?");
            uq2.addBindValue(recDinar);
            uq2.addBindValue(custId);
            uq2.exec();
        }
    }

    m_currentInvoiceId = id;
    m_invoiceNoSpin->blockSignals(true);
    m_invoiceNoSpin->setValue(id);
    m_invoiceNoSpin->blockSignals(false);
    m_navLabel->setText(QString::number(id));
    refreshInvoiceCount();
    QMessageBox::information(this, QString::fromUtf8("تم"),
        QString::fromUtf8("تم حفظ الفاتورة رقم: ") + QString::number(id));
}

void SalesWindow::newInvoice()
{
    clearForm();
    refreshInvoiceCount();
    QSqlQuery q;
    q.exec("SELECT MAX(id) FROM sales_invoices");
    if (q.next() && !q.value(0).isNull())
        m_invoiceNoSpin->setValue(q.value(0).toInt() + 1);
    else
        m_invoiceNoSpin->setValue(1);
}

void SalesWindow::deleteInvoice()
{
    if (m_currentInvoiceId < 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("لا توجد فاتورة محملة للحذف"));
        return;
    }
    auto reply = QMessageBox::question(this,
        QString::fromUtf8("تأكيد الحذف"),
        QString::fromUtf8("هل تريد حذف الفاتورة رقم %1؟").arg(m_currentInvoiceId),
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    QSqlQuery q;
    q.prepare("DELETE FROM sales_items WHERE invoice_id=?");
    q.addBindValue(m_currentInvoiceId); q.exec();
    q.prepare("DELETE FROM sales_invoices WHERE id=?");
    q.addBindValue(m_currentInvoiceId); q.exec();

    QMessageBox::information(this, "",
        QString::fromUtf8("تم حذف الفاتورة رقم: ") + QString::number(m_currentInvoiceId));
    clearForm();
    refreshInvoiceCount();
}

void SalesWindow::searchInvoice()
{
    bool ok;
    int id = QInputDialog::getInt(this,
        QString::fromUtf8("بحث عن فاتورة"),
        QString::fromUtf8("أدخل رقم الفاتورة:"),
        1, 1, 9999999, 1, &ok);
    if (!ok) return;

    QSqlQuery q;
    q.prepare("SELECT id FROM sales_invoices WHERE id=?");
    q.addBindValue(id); q.exec();
    if (q.next()) loadInvoice(id);
    else QMessageBox::warning(this, "",
        QString::fromUtf8("الفاتورة رقم %1 غير موجودة").arg(id));
}

/* ================================================================
   Print / Export
   ================================================================ */
void SalesWindow::printInvoice()
{
    if (m_currentInvoiceId < 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("احفظ الفاتورة أولاً"));
        return;
    }
    QString html = PrintManager::generateSalesInvoiceHtml(
        m_currentInvoiceId, Database::getCompanyName(), Database::getExchangeRate());
    int copies = m_copiesSpinBox->value();
    for (int i = 0; i < copies; ++i)
        m_printer->printPreview(this, html, QString::fromUtf8("فاتورة مبيعات"));
    m_printedCheck->setChecked(true);
}

void SalesWindow::exportPDF()
{
    if (m_currentInvoiceId < 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("احفظ الفاتورة أولاً"));
        return;
    }
    QString html = PrintManager::generateSalesInvoiceHtml(
        m_currentInvoiceId, Database::getCompanyName(), Database::getExchangeRate());
    m_printer->exportToPDF(this, html, QString::fromUtf8("فاتورة_مبيعات"));
}

void SalesWindow::printThermal()
{
    if (m_currentInvoiceId < 0 && m_itemsTable->rowCount() == 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("لا توجد فاتورة للطباعة"));
        return;
    }
    QStringList lines;
    lines << "**" + Database::getCompanyName();
    lines << "---";
    lines << QString::fromUtf8("فاتورة رقم: ") + QString::number(m_invoiceNoSpin->value());
    lines << QString::fromUtf8("الزبون: ")      + m_customerCombo->currentText();
    lines << QString::fromUtf8("التاريخ: ")     + m_dateEdit->date().toString("dd/MM/yyyy");
    lines << "---";
    for (int r = 0; r < m_itemsTable->rowCount(); ++r)
        lines << m_itemsTable->item(r, 3)->text()
                 + " x" + m_itemsTable->item(r, 4)->text()
                 + " = " + m_itemsTable->item(r, 6)->text();
    lines << "---";
    lines << QString::fromUtf8("الإجمالي: ") + m_totalDollarLabel->text() + " $";
    lines << QString::fromUtf8("الإجمالي: ") + m_totalDinarLabel->text() + QString::fromUtf8(" دينار");
    lines << "**" + QString::fromUtf8("شكراً لتعاملكم معنا");
    m_printer->printThermalReceipt(this, lines);
}

void SalesWindow::printPreparationList()
{
    printInvoice();
}

void SalesWindow::showCustomerStatement()
{
    int custId = m_customerCombo->currentData().toInt();
    if (custId <= 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("اختر زبون أولاً"));
        return;
    }
    CustomerStatement *dlg = new CustomerStatement(this);
    dlg->show();
}

/* ================================================================
   Navigation
   ================================================================ */
void SalesWindow::navigateFirst()
{
    QSqlQuery q;
    q.exec("SELECT MIN(id) FROM sales_invoices");
    if (q.next() && !q.value(0).isNull()) loadInvoice(q.value(0).toInt());
}

void SalesWindow::navigatePrev()
{
    if (m_currentInvoiceId <= 0) { navigateFirst(); return; }
    QSqlQuery q;
    q.prepare("SELECT MAX(id) FROM sales_invoices WHERE id < ?");
    q.addBindValue(m_currentInvoiceId); q.exec();
    if (q.next() && !q.value(0).isNull()) loadInvoice(q.value(0).toInt());
}

void SalesWindow::navigateNext()
{
    if (m_currentInvoiceId <= 0) { navigateLast(); return; }
    QSqlQuery q;
    q.prepare("SELECT MIN(id) FROM sales_invoices WHERE id > ?");
    q.addBindValue(m_currentInvoiceId); q.exec();
    if (q.next() && !q.value(0).isNull()) loadInvoice(q.value(0).toInt());
}

void SalesWindow::navigateLast()
{
    QSqlQuery q;
    q.exec("SELECT MAX(id) FROM sales_invoices");
    if (q.next() && !q.value(0).isNull()) loadInvoice(q.value(0).toInt());
}

/* ================================================================
   Waiting System
   ================================================================ */
void SalesWindow::addToWait()
{
    if (m_itemsTable->rowCount() == 0) {
        QMessageBox::warning(this, "", QString::fromUtf8("لا توجد مواد في الفاتورة"));
        return;
    }

    WaitedInvoice inv;
    inv.customerId    = m_customerCombo->currentData().toInt();
    inv.customerName  = m_customerCombo->currentText();
    inv.address       = m_addressEdit->text();
    inv.phone         = m_phoneEdit->text();
    inv.date          = m_dateEdit->date();
    inv.paymentType   = m_paymentTypeCombo->currentText();
    inv.currency      = m_currencyCombo->currentText();
    inv.invoiceType   = m_invoiceTypeCombo->currentText();
    inv.exchangeRate  = m_exchangeRateEdit->text().toDouble();
    inv.receiver      = m_receiverEdit->text();
    inv.notes         = m_notesEdit->text();
    inv.receivedDollar = m_receivedDollarEdit->text().toDouble();
    inv.receivedDinar  = m_receivedDinarEdit->text().toDouble();
    inv.discount       = m_discountDinarEdit->text().toDouble();

    for (int r = 0; r < m_itemsTable->rowCount(); ++r) {
        WaitedInvoiceItem it;
        it.productId   = m_itemsTable->item(r, 9) ? m_itemsTable->item(r, 9)->text().toInt() : 0;
        it.productName = m_itemsTable->item(r, 3) ? m_itemsTable->item(r, 3)->text() : "";
        it.unit        = m_itemsTable->item(r, 2) ? m_itemsTable->item(r, 2)->text() : "";
        it.qty         = m_itemsTable->item(r, 4) ? m_itemsTable->item(r, 4)->text().toDouble() : 0;
        it.unitPrice   = m_itemsTable->item(r, 5) ? m_itemsTable->item(r, 5)->text().toDouble() : 0;
        it.total       = m_itemsTable->item(r, 6) ? m_itemsTable->item(r, 6)->text().toDouble() : 0;
        it.notes       = m_itemsTable->item(r, 7) ? m_itemsTable->item(r, 7)->text() : "";
        inv.items.append(it);
    }

    m_waitList.append(inv);
    clearForm();
    QMessageBox::information(this, "",
        QString::fromUtf8("تم حفظ الفاتورة في الانتظار. عدد الفواتير المعلقة: %1")
        .arg(m_waitList.size()));
}

void SalesWindow::retrieveFromWait()
{
    if (m_waitList.isEmpty()) {
        QMessageBox::information(this, "", QString::fromUtf8("لا توجد فواتير في الانتظار"));
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(QString::fromUtf8("الفواتير المعلقة"));
    dlg.setLayoutDirection(Qt::RightToLeft);
    dlg.resize(400, 300);

    QVBoxLayout *vl = new QVBoxLayout(&dlg);
    QListWidget *lw = new QListWidget;
    lw->setLayoutDirection(Qt::RightToLeft);
    for (int i = 0; i < m_waitList.size(); ++i) {
        const WaitedInvoice &inv = m_waitList.at(i);
        QString label = QString::fromUtf8("فاتورة %1 — %2 — %3 مادة")
            .arg(i + 1)
            .arg(inv.customerName)
            .arg(inv.items.size());
        lw->addItem(label);
    }
    lw->setCurrentRow(0);
    QDialogButtonBox *btns = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    btns->setLayoutDirection(Qt::RightToLeft);
    vl->addWidget(lw);
    vl->addWidget(btns);

    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    connect(lw, &QListWidget::itemDoubleClicked, &dlg, &QDialog::accept);

    if (dlg.exec() != QDialog::Accepted) return;
    int sel = lw->currentRow();
    if (sel < 0 || sel >= m_waitList.size()) return;

    WaitedInvoice inv = m_waitList.takeAt(sel);

    // Restore the form
    clearForm();

    // Customer
    for (int i = 0; i < m_customerCombo->count(); ++i) {
        if (m_customerCombo->itemData(i).toInt() == inv.customerId) {
            m_customerCombo->setCurrentIndex(i);
            break;
        }
    }
    if (m_customerCombo->currentData().toInt() != inv.customerId)
        m_customerCombo->setEditText(inv.customerName);

    m_addressEdit->setText(inv.address);
    m_phoneEdit->setText(inv.phone);
    m_dateEdit->setDate(inv.date);
    m_notesEdit->setText(inv.notes);
    m_receiverEdit->setText(inv.receiver);
    m_exchangeRateEdit->setText(QString::number(inv.exchangeRate));
    m_receivedDollarEdit->setText(QString::number(inv.receivedDollar));
    m_receivedDinarEdit->setText(QString::number(inv.receivedDinar));
    m_discountDinarEdit->setText(QString::number(inv.discount));

    // Set combos without triggering repricing until items are loaded
    m_paymentTypeCombo->blockSignals(true);
    m_currencyCombo->blockSignals(true);
    m_invoiceTypeCombo->blockSignals(true);
    int pi = m_paymentTypeCombo->findText(inv.paymentType);
    if (pi >= 0) m_paymentTypeCombo->setCurrentIndex(pi);
    int ci = m_currencyCombo->findText(inv.currency);
    if (ci >= 0) m_currencyCombo->setCurrentIndex(ci);
    int ii = m_invoiceTypeCombo->findText(inv.invoiceType);
    if (ii >= 0) m_invoiceTypeCombo->setCurrentIndex(ii);
    m_paymentTypeCombo->blockSignals(false);
    m_currencyCombo->blockSignals(false);
    m_invoiceTypeCombo->blockSignals(false);

    // Restore items
    m_itemsTable->blockSignals(true);
    for (const WaitedInvoiceItem &it : inv.items) {
        int row = m_itemsTable->rowCount();
        m_itemsTable->insertRow(row);
        m_itemsTable->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
        m_itemsTable->setItem(row, 1, new QTableWidgetItem(""));  // seq placeholder
        m_itemsTable->setItem(row, 2, new QTableWidgetItem(it.unit));
        m_itemsTable->setItem(row, 3, new QTableWidgetItem(it.productName));
        m_itemsTable->setItem(row, 4, new QTableWidgetItem(QString::number(it.qty)));
        m_itemsTable->setItem(row, 5, new QTableWidgetItem(QString::number(it.unitPrice)));
        m_itemsTable->setItem(row, 6, new QTableWidgetItem(QString::number(it.total)));
        m_itemsTable->setItem(row, 7, new QTableWidgetItem(it.notes));
        m_itemsTable->setItem(row, 8, new QTableWidgetItem(""));
        m_itemsTable->setItem(row, 9, new QTableWidgetItem(QString::number(it.productId)));
    }
    m_itemsTable->blockSignals(false);
    renumberRows();
    updateTotals();
}
