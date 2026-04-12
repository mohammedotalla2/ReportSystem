#include "reportswindow.h"
#include "dailyreport.h"
#include "customerstatement.h"
#include "customerbalances.h"
#include "inventoryreport.h"
#include "latedpayments.h"
#include "totalsalesreport.h"
#include "allcustomersreport.h"
#include "overallbalancereport.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QFont>
#include <QListWidgetItem>
#include <QTabBar>
#include <QLabel>
#include <QFrame>
#include <QSizePolicy>

ReportsWindow::ReportsWindow(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("التقارير"));
    setLayoutDirection(Qt::RightToLeft);
    setFixedSize(520, 580);
    setupUI();
    applyStyles();
    populateReports(QString::fromUtf8("زبائن"));
}

void ReportsWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(6);

    // Report list
    m_reportsList = new QListWidget;
    m_reportsList->setObjectName("reportsList");
    m_reportsList->setFont(QFont("Tahoma", 11));
    m_reportsList->setLayoutDirection(Qt::RightToLeft);

    mainLayout->addWidget(m_reportsList, 1);

    // Category tabs at bottom
    QWidget *tabWidget = new QWidget;
    tabWidget->setObjectName("tabWidget");
    QHBoxLayout *tabLayout = new QHBoxLayout(tabWidget);
    tabLayout->setContentsMargins(4, 4, 4, 4);
    tabLayout->setSpacing(6);

    auto makeTabBtn = [this](const QString &text, bool selected = false) {
        QPushButton *btn = new QPushButton(text);
        btn->setFont(QFont("Tahoma", 11, QFont::Bold));
        btn->setFixedHeight(36);
        btn->setCheckable(true);
        btn->setChecked(selected);
        btn->setObjectName(selected ? "tabBtnSelected" : "tabBtn");
        return btn;
    };

    QPushButton *custBtn = makeTabBtn(QString::fromUtf8("الزبائن"), true);
    QPushButton *matBtn = makeTabBtn(QString::fromUtf8("المواد"));
    QPushButton *cashBtn = makeTabBtn(QString::fromUtf8("الصندوق"));
    QPushButton *profBtn = makeTabBtn(QString::fromUtf8("الارباح"));
    QPushButton *delBtn = makeTabBtn(QString::fromUtf8("المحذوفات"));

    tabLayout->addWidget(custBtn);
    tabLayout->addWidget(matBtn);
    tabLayout->addWidget(cashBtn);
    tabLayout->addWidget(profBtn);
    tabLayout->addWidget(delBtn);

    mainLayout->addWidget(tabWidget);

    // Close button
    QHBoxLayout *btnLayout = new QHBoxLayout;
    m_closeBtn = new QPushButton;
    m_closeBtn->setObjectName("closeBtn");
    m_closeBtn->setFixedSize(40, 36);
    m_closeBtn->setFont(QFont("Tahoma", 14));
    m_closeBtn->setText("✖");
    btnLayout->addWidget(m_closeBtn);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    // Connect category buttons
    connect(custBtn, &QPushButton::clicked, [=]() { populateReports(QString::fromUtf8("زبائن")); });
    connect(matBtn, &QPushButton::clicked, [=]() { populateReports(QString::fromUtf8("مواد")); });
    connect(cashBtn, &QPushButton::clicked, [=]() { populateReports(QString::fromUtf8("صندوق")); });
    connect(profBtn, &QPushButton::clicked, [=]() { populateReports(QString::fromUtf8("أرباح")); });
    connect(delBtn, &QPushButton::clicked, [=]() { populateReports(QString::fromUtf8("محذوفات")); });
    connect(m_closeBtn, &QPushButton::clicked, this, &ReportsWindow::close);
    connect(m_reportsList, &QListWidget::itemDoubleClicked, this, &ReportsWindow::onReportSelected);
}

void ReportsWindow::applyStyles()
{
    setStyleSheet(R"(
        QDialog { background: #e8eef4; }
        #reportsList {
            background: #c8e8f0;
            border: 1px solid #88aabb;
            font-size: 12pt;
            color: #003366;
        }
        #reportsList::item {
            padding: 8px 12px;
            border-bottom: 1px solid #aaccd0;
        }
        #reportsList::item:selected {
            background: #8b0000;
            color: white;
        }
        #reportsList::item:hover {
            background: #a0c8d8;
        }
        #tabWidget {
            background: #8b0000;
            border-radius: 4px;
            padding: 4px;
        }
        #tabBtn, #tabBtnSelected {
            background: #c8d8e8;
            color: #003366;
            border: 1px solid #88aacc;
            border-radius: 4px;
            font-weight: bold;
        }
        #tabBtn:hover { background: #b0c8e0; }
        #tabBtnSelected, #tabBtn:checked { background: #f0f8ff; color: #003366; }
        #closeBtn {
            background: #c8d8e8;
            border: 1px solid #88aacc;
            border-radius: 4px;
            color: #003366;
        }
    )");
}

void ReportsWindow::populateReports(const QString &category)
{
    m_reportsList->clear();

    QStringList customerReports = {
        QString::fromUtf8("كشف بالقوائم المستحقة"),
        QString::fromUtf8("مجموع المبيعات حسب الزبائن"),
        QString::fromUtf8("المتأخرين عن التسديد"),
        QString::fromUtf8("كشف حساب زبون معين"),
        QString::fromUtf8("الرصيد الأجمالي للزبائن دائن ومدين"),
        QString::fromUtf8("تقرير أسماء جميع الزبائن")
    };

    QStringList materialReports = {
        QString::fromUtf8("جرد أرصدة المواد"),
        QString::fromUtf8("كشف تحليل المشتريات"),
        QString::fromUtf8("كشف تحليل المبيعات"),
        QString::fromUtf8("تقرير حركة مادة معينة"),
        QString::fromUtf8("تقرير المواد الأكثر مبيعاً"),
        QString::fromUtf8("تقرير المواد الراكدة")
    };

    QStringList cashReports = {
        QString::fromUtf8("التقرير اليومي"),
        QString::fromUtf8("كشف رصيد الصندوق"),
        QString::fromUtf8("كشف المقبوضات"),
        QString::fromUtf8("كشف المدفوعات"),
        QString::fromUtf8("تقرير المصاريف العامة"),
        QString::fromUtf8("كشف حركة الصندوق")
    };

    QStringList profitReports = {
        QString::fromUtf8("تقرير رأس المال"),
        QString::fromUtf8("مجموع الأرباح حسب الزبائن"),
        QString::fromUtf8("مجموع الأرباح حسب المواد"),
        QString::fromUtf8("كشف تحليل الأرباح"),
        QString::fromUtf8("تقرير الأرباح الشهرية")
    };

    QStringList *reports = nullptr;
    if (category == QString::fromUtf8("زبائن")) reports = &customerReports;
    else if (category == QString::fromUtf8("مواد")) reports = &materialReports;
    else if (category == QString::fromUtf8("صندوق")) reports = &cashReports;
    else if (category == QString::fromUtf8("أرباح")) reports = &profitReports;

    if (!reports) return;

    for (const QString &rep : *reports) {
        QListWidgetItem *item = new QListWidgetItem(rep);
        item->setFont(QFont("Tahoma", 11));
        m_reportsList->addItem(item);
    }

    // Select first by default
    if (m_reportsList->count() > 0)
        m_reportsList->setCurrentRow(1); // select "مجموع المبيعات" like in screenshot
}

void ReportsWindow::onReportSelected(QListWidgetItem *item)
{
    QString report = item->text();

    if (report == QString::fromUtf8("التقرير اليومي")) {
        DailyReport *w = new DailyReport(this); w->show();
    } else if (report == QString::fromUtf8("كشف حساب زبون معين")) {
        CustomerStatement *w = new CustomerStatement(this); w->show();
    } else if (report == QString::fromUtf8("الرصيد الأجمالي للزبائن دائن ومدين")) {
        OverallBalanceReport *w = new OverallBalanceReport(this); w->show();
    } else if (report == QString::fromUtf8("تقرير أسماء جميع الزبائن")) {
        AllCustomersReport *w = new AllCustomersReport(this); w->show();
    } else if (report == QString::fromUtf8("ارصدة الزبائن") ||
               report == QString::fromUtf8("كشف بالقوائم المستحقة")) {
        CustomerBalances *w = new CustomerBalances(this); w->show();
    } else if (report == QString::fromUtf8("المتأخرين عن التسديد")) {
        LatedPayments *w = new LatedPayments(this); w->show();
    } else if (report == QString::fromUtf8("مجموع المبيعات حسب الزبائن")) {
        TotalSalesReport *w = new TotalSalesReport(this); w->show();
    } else if (report == QString::fromUtf8("جرد أرصدة المواد")) {
        InventoryReport *w = new InventoryReport(this); w->show();
    }
    // More can be added
}

void ReportsWindow::onCategoryChanged(int index) {}
void ReportsWindow::openReport() {}
void ReportsWindow::closeWindow() { close(); }
