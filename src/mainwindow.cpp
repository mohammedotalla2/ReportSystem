#include "mainwindow.h"
#include "saleswindow.h"
#include "purchasewindow.h"
#include "cashboxwindow.h"
#include "reportswindow.h"
#include "systemsettings.h"
#include "dailyreport.h"
#include "customerstatement.h"
#include "customerbalances.h"
#include "inventoryreport.h"
#include "database.h"
#include "arabichelper.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFont>
#include <QDate>
#include <QTimer>
#include <QFrame>
#include <QSizePolicy>
#include <QApplication>
#include <QLabel>
#include <QGroupBox>
#include <QSpacerItem>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(QString::fromUtf8("نظام المبيعات والمشتريات"));
    setMinimumSize(1100, 650);
    resize(1366, 720);
    setLayoutDirection(Qt::RightToLeft);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    m_timer->start(60000);

    setupUI();
    applyStyles();
    updateDateTime();
}

MainWindow::~MainWindow() {}


void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ===== 1. LEFT PANEL (shortcuts) =====
    QWidget *leftPanel = new QWidget;
    leftPanel->setFixedWidth(185);
    leftPanel->setObjectName("leftPanel");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(8, 20, 8, 10);
    leftLayout->setSpacing(8);

    auto makeLeftBtn = [](const QString &text, bool isClose = false) {
        QPushButton *btn = new QPushButton(text);
        btn->setFixedHeight(38);
        btn->setFont(QFont("Tahoma", 10));
        // السطر السحري: يجبر الزر على معاملة المحتوى من اليمين لليسار
    btn->setLayoutDirection(Qt::RightToLeft); 
    
    // سطر إضافي لضمان المحاذاة لليمين داخل الزر
    btn->setStyleSheet("text-align: right; padding-right: 15px;");
        if (isClose) btn->setObjectName("closeBtn");
        else btn->setObjectName("leftBtn");
        return btn;
    };

    m_dailyReportBtn = makeLeftBtn(QString::fromUtf8("التقرير اليومي"));
    m_customerStatementBtn = makeLeftBtn(QString::fromUtf8("كشف حساب زبون"));
    m_customerBalancesBtn = makeLeftBtn(QString::fromUtf8("ارصدة الزبائن"));
    m_inventoryBtn = makeLeftBtn(QString::fromUtf8("جرد ارصدة المواد"));
    m_closeBtn = makeLeftBtn(QString::fromUtf8("اغلاق النظام"), true);

    leftLayout->addWidget(m_dailyReportBtn);
    leftLayout->addWidget(m_customerStatementBtn);
    leftLayout->addWidget(m_customerBalancesBtn);
    leftLayout->addWidget(m_inventoryBtn);
    leftLayout->addStretch();
    leftLayout->addWidget(m_closeBtn);

    // ===== 2. BOTTOM PANEL (Product & Customer Details) - تعريفها أولاً لتجنب خطأ الـ Scope =====
    QGroupBox *productBox = new QGroupBox;
    productBox->setObjectName("productBox");
    productBox->setLayoutDirection(Qt::RightToLeft);
    QGridLayout *prodLayout = new QGridLayout(productBox);
    prodLayout->setContentsMargins(15, 10, 15, 10);
    prodLayout->setSpacing(8);

    auto makeHeader = [](const QString &text) {
        QLabel *l = new QLabel(text);
        l->setAlignment(Qt::AlignCenter);
        l->setFont(QFont("Tahoma", 10, QFont::Bold));
        l->setStyleSheet("color: #003366;");
        return l;
    };

    // حقول الباركود والبحث
    prodLayout->addWidget(makeHeader(QString::fromUtf8("الباركود")), 0, 0);
    m_barcodeCombo = new QComboBox;
    m_barcodeCombo->setEditable(true);
    prodLayout->addWidget(m_barcodeCombo, 1, 0);

    prodLayout->addWidget(makeHeader(QString::fromUtf8("اسم المادة (بحث)")), 0, 1, 1, 3);
    m_productSearchCombo = new QComboBox;
    m_productSearchCombo->setEditable(true);
    prodLayout->addWidget(m_productSearchCombo, 1, 1, 1, 3);

    // تفاصيل المادة
    m_productNameEdit = new QLineEdit; m_productNameEdit->setReadOnly(true); m_productNameEdit->setObjectName("infoField");
    m_priceUsdEdit = new QLineEdit; m_priceUsdEdit->setReadOnly(true); m_priceUsdEdit->setObjectName("infoField");
    m_priceDinarEdit = new QLineEdit; m_priceDinarEdit->setReadOnly(true); m_priceDinarEdit->setObjectName("infoField");
    m_qtyEdit = new QLineEdit; m_qtyEdit->setReadOnly(true); m_qtyEdit->setObjectName("infoField");

    prodLayout->addWidget(makeHeader(QString::fromUtf8("اسم المادة")), 2, 0);
    prodLayout->addWidget(m_productNameEdit, 3, 0);
    prodLayout->addWidget(makeHeader(QString::fromUtf8("السعر $")), 2, 1);
    prodLayout->addWidget(m_priceUsdEdit, 3, 1);
    prodLayout->addWidget(makeHeader(QString::fromUtf8("السعر د")), 2, 2);
    prodLayout->addWidget(m_priceDinarEdit, 3, 2);
    prodLayout->addWidget(makeHeader(QString::fromUtf8("العدد")), 2, 3);
    prodLayout->addWidget(m_qtyEdit, 3, 3);

    // الكلفة وبيانات الزبون
    m_costEdit = new QLineEdit; m_costEdit->setReadOnly(true); m_costEdit->setObjectName("infoField");
    prodLayout->addWidget(makeHeader(QString::fromUtf8("الكلفة")), 4, 0);
    prodLayout->addWidget(m_costEdit, 5, 0);

    m_customerCombo = new QComboBox; m_customerCombo->setEditable(true);
    m_balDollarEdit = new QLineEdit; m_balDollarEdit->setReadOnly(true); m_balDollarEdit->setObjectName("infoField");
    m_balDinarEdit = new QLineEdit; m_balDinarEdit->setReadOnly(true); m_balDinarEdit->setObjectName("infoField");

    prodLayout->addWidget(makeHeader(QString::fromUtf8("الزبون")), 6, 0, 1, 2);
    prodLayout->addWidget(m_customerCombo, 7, 0, 1, 2);
    prodLayout->addWidget(makeHeader(QString::fromUtf8("الرصيد $")), 6, 2);
    prodLayout->addWidget(m_balDollarEdit, 7, 2);
    prodLayout->addWidget(makeHeader(QString::fromUtf8("الرصيد دينار")), 6, 3);
    prodLayout->addWidget(m_balDinarEdit, 7, 3);

    // ===== 3. CENTER PANEL (Main Buttons Layout) =====
    QWidget *centerPanel = new QWidget;
    centerPanel->setObjectName("centerPanel");
    QVBoxLayout *centerLayout = new QVBoxLayout(centerPanel);
    centerLayout->setContentsMargins(10, 10, 10, 10);
    centerLayout->setSpacing(15);

    m_companyLabel = new QLabel(QString::fromUtf8("نظام الحسابات"));
    m_companyLabel->setObjectName("companyLabel");
    m_companyLabel->setAlignment(Qt::AlignCenter);
    m_companyLabel->setFont(QFont("Tahoma", 18, QFont::Bold));
    centerLayout->addWidget(m_companyLabel);

    centerLayout->addStretch(1); // دفع العناصر للوسط

auto makeBigBtn = [](const QString &text, const QString &objName) {
    QPushButton *btn = new QPushButton(text);
    btn->setObjectName(objName);
    btn->setFont(QFont("Tahoma", 16, QFont::Bold));
    btn->setMinimumSize(180, 95);
    
    // التنسيق الجديد: يحافظ على اللون الأصلي لكل زر مع إضافة لمسة البروز (3D)
    btn->setStyleSheet(QString(R"(
        #%1 {
            border: 2px outset rgba(255, 255, 255, 0.8);
            border-radius: 12px;
            padding: 10px;
        }
        #%1:hover {
            background-color: rgba(255, 255, 255, 0.1);
            border: 2px solid white;
        }
        #%1:pressed {
            border: 2px inset rgba(0, 0, 0, 0.2);
        }
    )").arg(objName));
    
    return btn;
};

    m_salesBtn = makeBigBtn(QString::fromUtf8("المبيعات"), "salesBtn");
    QPushButton *purchaseBtn = makeBigBtn(QString::fromUtf8("المشتريات"), "purchaseBtn");
    m_cashBoxBtn = makeBigBtn(QString::fromUtf8("الصندوق"), "cashBtn");
    m_reportsBtn = makeBigBtn(QString::fromUtf8("التقارير"), "reportsBtn");
    m_settingsBtn = makeBigBtn(QString::fromUtf8("أعداد النظام"), "settingsBtn");

    // ترتيب الصفوف (2-1-2)
    QHBoxLayout *topRow = new QHBoxLayout; topRow->setSpacing(40);
    topRow->addWidget(m_salesBtn); topRow->addWidget(purchaseBtn);
    centerLayout->addLayout(topRow);

    QHBoxLayout *midRow = new QHBoxLayout;
    midRow->addWidget(m_cashBoxBtn, 0, Qt::AlignCenter);
    centerLayout->addLayout(midRow);

    QHBoxLayout *botRow = new QHBoxLayout; botRow->setSpacing(40);
    botRow->addWidget(m_reportsBtn); botRow->addWidget(m_settingsBtn);
    centerLayout->addLayout(botRow);

    centerLayout->addStretch(2); // فراغ كبير قبل صندوق الباركود
    centerLayout->addWidget(productBox);

    // ===== 4. RIGHT PANEL =====
    QWidget *rightPanel = new QWidget;
    rightPanel->setFixedWidth(170);
    rightPanel->setObjectName("rightPanel");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(8, 20, 8, 10);
    
    m_dayLabel = new QLabel; m_dayLabel->setObjectName("dayLabel");
    m_dayLabel->setAlignment(Qt::AlignCenter);
    m_dateLabel = new QLabel; m_dateLabel->setObjectName("dateLabel");
    m_dateLabel->setAlignment(Qt::AlignCenter);

    m_todaySalesLabel = new QLabel("0");
    m_todaySalesLabel->setObjectName("todaySalesValue");
    m_todaySalesLabel->setAlignment(Qt::AlignCenter);

    m_salesDetailsBtn = new QPushButton(QString::fromUtf8("تفاصيل"));
    m_salesDetailsBtn->setObjectName("detailsBtn");

    rightLayout->addWidget(m_dayLabel);
    rightLayout->addWidget(m_dateLabel);
    rightLayout->addSpacing(20);
    rightLayout->addWidget(new QLabel(QString::fromUtf8("مستحقي مبيعات اليوم")));
    rightLayout->addWidget(m_todaySalesLabel);
    rightLayout->addWidget(m_salesDetailsBtn);
    rightLayout->addStretch();

    // Final Assembly
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(centerPanel, 1);
    mainLayout->addWidget(rightPanel);

    // Database Load & Connect
    QSqlQuery cq = Database::getCustomers();
    while (cq.next()) m_customerCombo->addItem(cq.value(1).toString(), cq.value(0));
    QSqlQuery pq = Database::getProducts();
    while (pq.next()) m_barcodeCombo->addItem(pq.value(1).toString(), pq.value(0));

    // Load productSearchCombo with product names (barcodeCombo already has barcodes)
    QSqlQuery psq = Database::getProducts();
    while (psq.next())
        m_productSearchCombo->addItem(psq.value(2).toString(), psq.value(0));

    connect(m_salesBtn,       &QPushButton::clicked, this, &MainWindow::openSales);
    connect(purchaseBtn,      &QPushButton::clicked, this, &MainWindow::openPurchases);
    connect(m_cashBoxBtn,     &QPushButton::clicked, this, &MainWindow::openCashBox);
    connect(m_settingsBtn,    &QPushButton::clicked, this, &MainWindow::openSystemSettings);
    connect(m_salesDetailsBtn,&QPushButton::clicked, this, &MainWindow::showTodaySalesDetails);
    connect(m_closeBtn,       &QPushButton::clicked, qApp, &QApplication::quit);

    // Barcode combo: search on Enter key OR when a product is selected from dropdown
    connect(m_barcodeCombo->lineEdit(), &QLineEdit::returnPressed,
            this, &MainWindow::onBarcodeEntered);
    connect(m_barcodeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int idx) {
        if (idx < 0) return;
        int pid = m_barcodeCombo->itemData(idx).toInt();
        if (pid <= 0) return;
        QSqlQuery q;
        q.prepare("SELECT name, retail_dinar, retail_dollar, cost_price, stock_qty "
                  "FROM products WHERE id=?");
        q.addBindValue(pid); q.exec();
        if (q.next()) {
            m_productNameEdit->setText(q.value(0).toString());
            m_priceDinarEdit->setText(q.value(1).toString());
            m_priceUsdEdit->setText(q.value(2).toString());
            m_costEdit->setText(q.value(3).toString());
            m_qtyEdit->setText(q.value(4).toString());
        }
    });

    // Product name combo: selecting an item fills all product detail fields
    connect(m_productSearchCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int idx) {
        if (idx < 0) return;
        int pid = m_productSearchCombo->itemData(idx).toInt();
        if (pid <= 0) return;
        QSqlQuery q;
        q.prepare("SELECT name, retail_dinar, retail_dollar, cost_price, stock_qty, barcode "
                  "FROM products WHERE id=?");
        q.addBindValue(pid); q.exec();
        if (q.next()) {
            m_productNameEdit->setText(q.value(0).toString());
            m_priceDinarEdit->setText(q.value(1).toString());
            m_priceUsdEdit->setText(q.value(2).toString());
            m_costEdit->setText(q.value(3).toString());
            m_qtyEdit->setText(q.value(4).toString());
            m_barcodeCombo->setCurrentText(q.value(5).toString());
        }
    });

    // Customer combo: selecting a customer fills balance fields
    connect(m_customerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int idx) {
        if (idx < 0) return;
        int cid = m_customerCombo->itemData(idx).toInt();
        if (cid <= 0) {
            m_balDollarEdit->clear();
            m_balDinarEdit->clear();
            return;
        }
        QSqlQuery q;
        q.prepare("SELECT balance_dollar, balance_dinar FROM customers WHERE id=?");
        q.addBindValue(cid); q.exec();
        if (q.next()) {
            m_balDollarEdit->setText(QString::number(q.value(0).toDouble(), 'f', 2));
            m_balDinarEdit->setText(QString::number(q.value(1).toDouble(), 'f', 0));
        }
    });
    // Also trigger on free-text edit (user types customer name)
    connect(m_customerCombo->lineEdit(), &QLineEdit::textEdited,
            this, [this](const QString &text) {
        if (text.trimmed().isEmpty()) {
            m_balDollarEdit->clear();
            m_balDinarEdit->clear();
            return;
        }
        int idx = m_customerCombo->findText(text, Qt::MatchContains);
        if (idx >= 0 && m_customerCombo->currentIndex() != idx)
            m_customerCombo->setCurrentIndex(idx);
    });

    // Buttons under construction — show notice instead of opening the window
    auto uc = [this]() {
        QMessageBox::information(this,
            QString::fromUtf8("تنبيه"),
            QString::fromUtf8("هذا الحقل لا يزال تحت الإنشاء"));
    };
    connect(m_reportsBtn,           &QPushButton::clicked, this, uc);
    connect(m_dailyReportBtn,       &QPushButton::clicked, this, uc);
    connect(m_customerStatementBtn, &QPushButton::clicked, this, uc);
    connect(m_customerBalancesBtn,  &QPushButton::clicked, this, uc);
    connect(m_inventoryBtn,         &QPushButton::clicked, this, uc);
}

void MainWindow::applyStyles()
{
    // Gradient background matching the screenshot
    setStyleSheet(R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #7ecac3, stop:0.3 #9dd5d0,
                stop:0.6 #7baad5, stop:1 #8870b5);
        }
        #leftPanel {
            background: rgba(255,255,255,0.15);
            border-right: 1px solid rgba(255,255,255,0.3);
        }
#leftBtn {
    background: rgba(200,220,255,0.7);
    border: 1px solid rgba(100,150,220,0.5);
    border-radius: 4px;
    color: #003366;
    font-weight: bold;
    font-size: 10pt; /* حجم الخط المناسب */
    
    /* المحاذاة جهة اليمين */
    text-align: right; 
    
    /* إضافة مسافة من اليمين لبداية النص ومسافة من اليسار للنهاية */
    padding-right: 15px; 
    padding-left: 5px;
}

#leftBtn:hover {
    background: rgba(180,210,255,0.9);
    border: 1px solid #003366;
}
        #closeBtn:hover { background: #cc0000; }
        #centerPanel { background: transparent; }
        #trialLabel {
            color: #FFD700;
            background: rgba(0,0,80,0.3);
            padding: 8px;
            border: 2px solid #FFD700;
            border-radius: 6px;
        }
        #companyLabel {
            color: #FFD700;
            background: rgba(0,0,80,0.2);
            padding: 4px 20px;
            border-radius: 4px;
        }

        
        #salesBtn {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #e8f4e8,stop:1 #c8e8c8);
            color: #003300; border: 2px solid #66aa66; border-radius: 8px;
        }
        #salesBtn:hover { background: #c0e8c0; }
        #cashBtn {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #e8f0ff,stop:1 #c8d8ff);
            color: #003366; border: 2px solid #6688cc; border-radius: 8px;
        }
        #reportsBtn {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #fff0e8,stop:1 #ffd8c0);
            color: #330000; border: 2px solid #cc8855; border-radius: 8px;
        }
        #settingsBtn {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #f0e8ff,stop:1 #d8c8ff);
            color: #220033; border: 2px solid #9966cc; border-radius: 8px;
        }
        #productBox {
            background: rgba(200,220,255,0.6);
            border: 1px solid rgba(100,150,220,0.4);
            border-radius: 6px;
        }
        #infoField {
            background: #e8f4ff;
            border: 1px solid #99bbdd;
            border-radius: 3px;
            color: #003366;
            font-weight: bold;
        }
        #rightPanel {
            background: rgba(255,255,255,0.12);
            border-left: 1px solid rgba(255,255,255,0.3);
        }
        #dayLabel { color: #FFD700; font-weight: bold; }
        #dateLabel {
            color: #333;
            background: #f0f8ff;
            border: 1px solid #aac;
            border-radius: 3px;
            padding: 4px;
        }
        #todaySalesLbl { color: #FFD700; font-size: 9pt; }
        #todaySalesValue {
            color: #003366;
            background: #f0f8ff;
            border: 1px solid #99bbdd;
            padding: 6px;
            border-radius: 3px;
        }
        #detailsBtn {
            background: #d0e8ff;
            border: 1px solid #99bbdd;
            border-radius: 3px;
            color: #003366;
        }
        QComboBox {
            background: #ffffc0;
            border: 1px solid #aaa860;
            border-radius: 3px;
            padding: 2px 4px;
        }
        QLabel { color: #003366; }
    )");
}

void MainWindow::updateDateTime()
{
    QDate today = QDate::currentDate();
    QStringList days = {QString::fromUtf8("الأحد"), QString::fromUtf8("الاثنين"),
                        QString::fromUtf8("الثلاثاء"), QString::fromUtf8("الأربعاء"),
                        QString::fromUtf8("الخميس"), QString::fromUtf8("الجمعة"),
                        QString::fromUtf8("السبت")};
    m_dayLabel->setText(days[today.dayOfWeek() % 7]);
    m_dateLabel->setText(today.toString("dd/MM/yyyy"));
    m_todaySalesLabel->setText(QString::number(Database::getTodaySales(), 'f', 2));
}
void MainWindow::onBarcodeEntered()
{
    QString barcode = m_barcodeCombo->currentText().trimmed();
    if (barcode.isEmpty()) return;

    QSqlQuery q;
    // تم تعديل أسماء الحقول لتطابق جدول products في قاعدة بياناتك
    q.prepare("SELECT name, retail_dinar, retail_dollar, cost_price, stock_qty "
              "FROM products WHERE barcode = ?");
    q.addBindValue(barcode);

    if (q.exec()) {
        if (q.next()) {
            // تعبئة البيانات في الواجهة
            m_productNameEdit->setText(q.value(0).toString());     // name
            m_priceDinarEdit->setText(q.value(1).toString());      // retail_dinar
            m_priceUsdEdit->setText(q.value(2).toString());        // retail_dollar
            m_costEdit->setText(q.value(3).toString());           // cost_price
            m_qtyEdit->setText(q.value(4).toString());            // stock_qty

            // تجهيز الحقل للباركود التالي
            m_barcodeCombo->clearEditText();
            m_barcodeCombo->setFocus();
        } else {
            // تنبيه في حالة عدم وجود الباركود
            QMessageBox::warning(this, "تنبيه", "الباركود (" + barcode + ") غير موجود في قاعدة البيانات.");
            m_barcodeCombo->setFocus();
        }
    } else {
        // طباعة الخطأ في شاشة الـ Output للتصحيح
        qDebug() << "SQL Error:" << q.lastError().text();
    }
}


void MainWindow::openSales()     { SalesWindow *w = new SalesWindow(this); w->show(); }
void MainWindow::openPurchases() { PurchaseWindow *w = new PurchaseWindow(this); w->show(); }
void MainWindow::openCashBox()   { CashBoxWindow *w = new CashBoxWindow(this); w->show(); }
void MainWindow::openReports()   { ReportsWindow *w = new ReportsWindow(this); w->show(); }
void MainWindow::openSystemSettings() { SystemSettings *w = new SystemSettings(this); w->exec(); }
void MainWindow::openDailyReport()    { DailyReport *w = new DailyReport(this); w->show(); }
void MainWindow::openCustomerStatement() { CustomerStatement *w = new CustomerStatement(this); w->show(); }
void MainWindow::openCustomerBalances()  { CustomerBalances *w = new CustomerBalances(this); w->show(); }
void MainWindow::openInventoryReport()   { InventoryReport *w = new InventoryReport(this); w->show(); }
void MainWindow::showTodaySalesDetails() { openDailyReport(); }
