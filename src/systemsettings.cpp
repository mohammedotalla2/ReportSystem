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

SystemSettings::SystemSettings(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("أعداد النظام"));
    setLayoutDirection(Qt::RightToLeft);
    resize(900, 650);
    setupUI(); applyStyles();
    loadCustomers(); loadProducts();

    // Load settings
    m_companyNameEdit->setText(Database::getCompanyName());
    m_exchangeRateSpin->setValue(Database::getExchangeRate());
}

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

    auto lbl = [](const QString &t) {
        QLabel *l = new QLabel(t); l->setFont(QFont("Tahoma", 10));
        l->setAlignment(Qt::AlignRight | Qt::AlignVCenter); return l;
    };

    m_companyNameEdit = new QLineEdit;
    m_companyNameEdit->setFont(QFont("Tahoma", 12, QFont::Bold));

    m_exchangeRateSpin = new QDoubleSpinBox;
    m_exchangeRateSpin->setRange(1, 99999); m_exchangeRateSpin->setDecimals(0);
    m_exchangeRateSpin->setFont(QFont("Tahoma", 12, QFont::Bold));
    m_exchangeRateSpin->setObjectName("rateSpin");

    m_cashboxDollarEdit = new QLineEdit("0");
    m_cashboxDinarEdit  = new QLineEdit("0");

    settingsLayout->addWidget(lbl(QString::fromUtf8("اسم الشركة / المحل:")), 0, 1);
    settingsLayout->addWidget(m_companyNameEdit, 0, 0);
    settingsLayout->addWidget(lbl(QString::fromUtf8("سعر صرف الدولار:")), 1, 1);
    settingsLayout->addWidget(m_exchangeRateSpin, 1, 0);
    settingsLayout->addWidget(lbl(QString::fromUtf8("رصيد الصندوق الأولي $:")), 2, 1);
    settingsLayout->addWidget(m_cashboxDollarEdit, 2, 0);
    settingsLayout->addWidget(lbl(QString::fromUtf8("رصيد الصندوق الأولي دينار:")), 3, 1);
    settingsLayout->addWidget(m_cashboxDinarEdit, 3, 0);
    settingsLayout->setRowStretch(4, 1);

    m_tabs->addTab(settingsTab, QString::fromUtf8("الإعدادات العامة"));

    // ===== TAB 2: Customers / Persons =====
    QWidget *custTab = new QWidget;
    QVBoxLayout *custLayout = new QVBoxLayout(custTab);
    custLayout->setContentsMargins(6, 6, 6, 6);
    custLayout->setSpacing(4);

    // Entry form
    QGroupBox *custFormBox = new QGroupBox(QString::fromUtf8("بيانات الشخص / الشركة"));
    custFormBox->setObjectName("formBox");
    QGridLayout *custFormLayout = new QGridLayout(custFormBox);
    custFormLayout->setSpacing(6);

    m_custNameEdit    = new QLineEdit;
    m_custRegionEdit  = new QLineEdit;
    m_custPhoneEdit   = new QLineEdit;
    m_custAddressEdit = new QLineEdit;
    m_custNotesEdit   = new QLineEdit;
    m_custBalDollar   = new QDoubleSpinBox; m_custBalDollar->setRange(-999999, 999999); m_custBalDollar->setDecimals(2);
    m_custBalDinar    = new QDoubleSpinBox; m_custBalDinar->setRange(-999999999, 999999999); m_custBalDinar->setDecimals(0);
    m_custTypeCombo   = new QComboBox;
    m_custTypeCombo->addItems({QString::fromUtf8("شخص"), QString::fromUtf8("شركة")});

    custFormLayout->addWidget(lbl(QString::fromUtf8("الاسم:")), 0, 3); custFormLayout->addWidget(m_custNameEdit, 0, 2);
    custFormLayout->addWidget(lbl(QString::fromUtf8("المنطقة:")), 0, 1); custFormLayout->addWidget(m_custRegionEdit, 0, 0);
    custFormLayout->addWidget(lbl(QString::fromUtf8("الهاتف:")), 1, 3); custFormLayout->addWidget(m_custPhoneEdit, 1, 2);
    custFormLayout->addWidget(lbl(QString::fromUtf8("العنوان:")), 1, 1); custFormLayout->addWidget(m_custAddressEdit, 1, 0);
    custFormLayout->addWidget(lbl(QString::fromUtf8("الرصيد $:")), 2, 3); custFormLayout->addWidget(m_custBalDollar, 2, 2);
    custFormLayout->addWidget(lbl(QString::fromUtf8("الرصيد دينار:")), 2, 1); custFormLayout->addWidget(m_custBalDinar, 2, 0);
    custFormLayout->addWidget(lbl(QString::fromUtf8("النوع:")), 3, 3); custFormLayout->addWidget(m_custTypeCombo, 3, 2);
    custFormLayout->addWidget(lbl(QString::fromUtf8("ملاحظات:")), 3, 1); custFormLayout->addWidget(m_custNotesEdit, 3, 0);

    // Buttons
    QHBoxLayout *custBtnRow = new QHBoxLayout;
    QPushButton *addCustBtn  = new QPushButton("💾 " + QString::fromUtf8("حفظ"));
    QPushButton *delCustBtn  = new QPushButton("✖ " + QString::fromUtf8("حذف"));
    QPushButton *newCustBtn  = new QPushButton("+ " + QString::fromUtf8("جديد"));
    for (auto *b : {addCustBtn, delCustBtn, newCustBtn}) {
        b->setFont(QFont("Tahoma", 10)); b->setFixedHeight(32);
        b->setObjectName("actionBtn");
    }
    custBtnRow->addWidget(addCustBtn); custBtnRow->addWidget(newCustBtn);
    custBtnRow->addStretch(); custBtnRow->addWidget(delCustBtn);

    // Table
    m_customersTable = new QTableWidget(0, 6);
    m_customersTable->setHorizontalHeaderLabels({
        QString::fromUtf8("الاسم"), QString::fromUtf8("المنطقة"),
        QString::fromUtf8("الهاتف"), QString::fromUtf8("الرصيد $"),
        QString::fromUtf8("الرصيد د"), QString::fromUtf8("النوع")
    });
    m_customersTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_customersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_customersTable->setObjectName("dataTable");

    custLayout->addWidget(custFormBox);
    custLayout->addLayout(custBtnRow);
    custLayout->addWidget(m_customersTable, 1);

    connect(addCustBtn, &QPushButton::clicked, this, &SystemSettings::addCustomer);
    connect(delCustBtn, &QPushButton::clicked, this, &SystemSettings::deleteCustomer);
    connect(newCustBtn, &QPushButton::clicked, [this]() {
        m_custNameEdit->clear(); m_custRegionEdit->clear();
        m_custPhoneEdit->clear(); m_custBalDollar->setValue(0);
        m_custBalDinar->setValue(0);
    });

    m_tabs->addTab(custTab, QString::fromUtf8("الأشخاص والشركات"));

    // ===== TAB 3: Products =====
    QWidget *prodTab = new QWidget;
    QVBoxLayout *prodLayout = new QVBoxLayout(prodTab);
    prodLayout->setContentsMargins(6, 6, 6, 6);
    prodLayout->setSpacing(4);

    QGroupBox *prodFormBox = new QGroupBox(QString::fromUtf8("بيانات المادة"));
    prodFormBox->setObjectName("formBox");
    QGridLayout *prodFormLayout = new QGridLayout(prodFormBox);
    prodFormLayout->setSpacing(6);

    m_prodBarcodeEdit = new QLineEdit;
    m_prodNameEdit    = new QLineEdit;
    m_prodGroupEdit   = new QLineEdit;
    m_prodTypeEdit    = new QLineEdit;
    m_prodCostSpin    = new QDoubleSpinBox; m_prodCostSpin->setRange(0,999999); m_prodCostSpin->setDecimals(3);
    m_prodWsDolSpin   = new QDoubleSpinBox; m_prodWsDolSpin->setRange(0,999999); m_prodWsDolSpin->setDecimals(3);
    m_prodRetDolSpin  = new QDoubleSpinBox; m_prodRetDolSpin->setRange(0,999999); m_prodRetDolSpin->setDecimals(3);
    m_prodWsDinSpin   = new QDoubleSpinBox; m_prodWsDinSpin->setRange(0,9999999); m_prodWsDinSpin->setDecimals(0);
    m_prodRetDinSpin  = new QDoubleSpinBox; m_prodRetDinSpin->setRange(0,9999999); m_prodRetDinSpin->setDecimals(0);
    m_prodStockSpin   = new QDoubleSpinBox; m_prodStockSpin->setRange(-999999,999999); m_prodStockSpin->setDecimals(2);
    m_prodCartoonSpin = new QSpinBox; m_prodCartoonSpin->setRange(1,9999); m_prodCartoonSpin->setValue(1);

    prodFormLayout->addWidget(lbl(QString::fromUtf8("الباركود:")), 0, 5); prodFormLayout->addWidget(m_prodBarcodeEdit, 0, 4);
    prodFormLayout->addWidget(lbl(QString::fromUtf8("اسم المادة:")), 0, 3); prodFormLayout->addWidget(m_prodNameEdit, 0, 2, 1, 2);
    prodFormLayout->addWidget(lbl(QString::fromUtf8("المجموعة:")), 1, 5); prodFormLayout->addWidget(m_prodGroupEdit, 1, 4);
    prodFormLayout->addWidget(lbl(QString::fromUtf8("النوع:")), 1, 3); prodFormLayout->addWidget(m_prodTypeEdit, 1, 2);
    prodFormLayout->addWidget(lbl(QString::fromUtf8("سعر الكلفة:")), 2, 5); prodFormLayout->addWidget(m_prodCostSpin, 2, 4);
    prodFormLayout->addWidget(lbl(QString::fromUtf8("تعبئة الكرتون:")), 2, 3); prodFormLayout->addWidget(m_prodCartoonSpin, 2, 2);
    prodFormLayout->addWidget(lbl(QString::fromUtf8("جملة $:")), 3, 5); prodFormLayout->addWidget(m_prodWsDolSpin, 3, 4);
    prodFormLayout->addWidget(lbl(QString::fromUtf8("مفرد $:")), 3, 3); prodFormLayout->addWidget(m_prodRetDolSpin, 3, 2);
    prodFormLayout->addWidget(lbl(QString::fromUtf8("جملة د:")), 4, 5); prodFormLayout->addWidget(m_prodWsDinSpin, 4, 4);
    prodFormLayout->addWidget(lbl(QString::fromUtf8("مفرد د:")), 4, 3); prodFormLayout->addWidget(m_prodRetDinSpin, 4, 2);
    prodFormLayout->addWidget(lbl(QString::fromUtf8("الرصيد الأولي:")), 5, 5); prodFormLayout->addWidget(m_prodStockSpin, 5, 4);

    QHBoxLayout *prodBtnRow = new QHBoxLayout;
    QPushButton *addProdBtn = new QPushButton("💾 " + QString::fromUtf8("حفظ"));
    QPushButton *delProdBtn = new QPushButton("✖ " + QString::fromUtf8("حذف"));
    QPushButton *newProdBtn = new QPushButton("+ " + QString::fromUtf8("جديد"));
    for (auto *b : {addProdBtn, delProdBtn, newProdBtn}) {
        b->setFont(QFont("Tahoma", 10)); b->setFixedHeight(32); b->setObjectName("actionBtn");
    }
    prodBtnRow->addWidget(addProdBtn); prodBtnRow->addWidget(newProdBtn);
    prodBtnRow->addStretch(); prodBtnRow->addWidget(delProdBtn);

    m_productsTable = new QTableWidget(0, 7);
    m_productsTable->setHorizontalHeaderLabels({
        QString::fromUtf8("الباركود"), QString::fromUtf8("اسم المادة"),
        QString::fromUtf8("المجموعة"), QString::fromUtf8("الكلفة"),
        QString::fromUtf8("جملة $"), QString::fromUtf8("مفرد $"),
        QString::fromUtf8("الرصيد")
    });
    m_productsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_productsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_productsTable->setObjectName("dataTable");

    prodLayout->addWidget(prodFormBox);
    prodLayout->addLayout(prodBtnRow);
    prodLayout->addWidget(m_productsTable, 1);

    connect(addProdBtn, &QPushButton::clicked, this, &SystemSettings::addProduct);
    connect(delProdBtn, &QPushButton::clicked, this, &SystemSettings::deleteProduct);

    m_tabs->addTab(prodTab, QString::fromUtf8("المواد"));

    mainLayout->addWidget(m_tabs, 1);

    // Bottom buttons
    QHBoxLayout *bottomRow = new QHBoxLayout;
    m_saveBtn  = new QPushButton("💾 " + QString::fromUtf8("حفظ الإعدادات"));
    m_closeBtn = new QPushButton("✖ " + QString::fromUtf8("إغلاق"));
    m_saveBtn->setObjectName("saveBtn"); m_saveBtn->setFixedHeight(36);
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
    )");
}

void SystemSettings::saveSettings()
{
    Database::setCompanyName(m_companyNameEdit->text());
    Database::setExchangeRate(m_exchangeRateSpin->value());
    QMessageBox::information(this, QString::fromUtf8("تم"),
                              QString::fromUtf8("تم حفظ الإعدادات بنجاح"));
}

void SystemSettings::loadCustomers()
{
    m_customersTable->setRowCount(0);
    QSqlQuery q = Database::getCustomers();
    int row = 0;
    while (q.next()) {
        m_customersTable->insertRow(row);
        m_customersTable->setItem(row, 0, new QTableWidgetItem(q.value(1).toString()));
        m_customersTable->setItem(row, 1, new QTableWidgetItem(q.value(2).toString()));
        m_customersTable->setItem(row, 2, new QTableWidgetItem(q.value(4).toString()));
        m_customersTable->setItem(row, 3, new QTableWidgetItem(QString::number(q.value(6).toDouble(), 'f', 2)));
        m_customersTable->setItem(row, 4, new QTableWidgetItem(QString::number(q.value(7).toDouble(), 'f', 0)));
        m_customersTable->setItem(row, 5, new QTableWidgetItem(q.value(8).toInt() == 0 ?
            QString::fromUtf8("شخص") : QString::fromUtf8("شركة")));
        m_customersTable->item(row, 0)->setData(Qt::UserRole, q.value(0));
        row++;
    }
}

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
        m_productsTable->setItem(row, 3, new QTableWidgetItem(QString::number(q.value(5).toDouble(), 'f', 3)));
        m_productsTable->setItem(row, 4, new QTableWidgetItem(QString::number(q.value(6).toDouble(), 'f', 3)));
        m_productsTable->setItem(row, 5, new QTableWidgetItem(QString::number(q.value(7).toDouble(), 'f', 3)));
        m_productsTable->setItem(row, 6, new QTableWidgetItem(QString::number(q.value(11).toDouble(), 'f', 2)));
        m_productsTable->item(row, 0)->setData(Qt::UserRole, q.value(0));
        row++;
    }
}

void SystemSettings::addCustomer()
{
    int id = Database::addCustomer(
        m_custNameEdit->text(), m_custRegionEdit->text(),
        m_custAddressEdit->text(), m_custPhoneEdit->text(),
        m_custNotesEdit->text(), m_custBalDollar->value(),
        m_custBalDinar->value(), m_custTypeCombo->currentIndex());
    if (id > 0) { loadCustomers(); m_custNameEdit->clear(); }
    else QMessageBox::warning(this, "", QString::fromUtf8("فشل الحفظ - تأكد من اسم غير مكرر"));
}

void SystemSettings::editCustomer()   {}

void SystemSettings::deleteCustomer()
{
    int row = m_customersTable->currentRow();
    if (row < 0) return;
    int id = m_customersTable->item(row, 0)->data(Qt::UserRole).toInt();
    if (QMessageBox::question(this, QString::fromUtf8("تأكيد"),
        QString::fromUtf8("هل تريد حذف هذا السجل؟")) == QMessageBox::Yes) {
        Database::deleteCustomer(id);
        loadCustomers();
    }
}

void SystemSettings::addProduct()
{
    int id = Database::addProduct(
        m_prodBarcodeEdit->text(), m_prodNameEdit->text(),
        m_prodGroupEdit->text(), m_prodTypeEdit->text(),
        m_prodCostSpin->value(), m_prodWsDolSpin->value(),
        m_prodRetDolSpin->value(), m_prodWsDinSpin->value(),
        m_prodRetDinSpin->value(), m_prodCartoonSpin->value(),
        m_prodStockSpin->value(), 0);
    if (id > 0) { loadProducts(); m_prodNameEdit->clear(); m_prodBarcodeEdit->clear(); }
    else QMessageBox::warning(this, "", QString::fromUtf8("فشل الحفظ"));
}

void SystemSettings::editProduct()   {}
void SystemSettings::deleteProduct()
{
    int row = m_productsTable->currentRow();
    if (row < 0) return;
    int id = m_productsTable->item(row, 0)->data(Qt::UserRole).toInt();
    if (QMessageBox::question(this, QString::fromUtf8("تأكيد"),
        QString::fromUtf8("هل تريد حذف هذه المادة؟")) == QMessageBox::Yes) {
        QSqlQuery q; q.prepare("DELETE FROM products WHERE id=?");
        q.addBindValue(id); q.exec();
        loadProducts();
    }
}
