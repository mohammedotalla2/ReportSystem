#ifndef ADVANCEDSALESWINDOW_H
#define ADVANCEDSALESWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QSpinBox>
#include <QListWidget>
#include "printmanager.h"

// ── Waiting-system data structures (stored in RAM) ──────────────────────────

struct WaitingItem {
    int     productId;
    QString barcode;
    QString productName;
    QString pricingType;   // مفرد  or  جملة
    double  qty;
    double  priceDollar;
    double  priceDinar;
    double  totalDollar;
    double  totalDinar;
    QString notes;
};

struct WaitingInvoice {
    int     customerId;
    QString customerName;
    QString paymentType;
    QString currency;
    QString notes;
    QDate   date;
    double  exchangeRate;
    QList<WaitingItem> items;
};

// ────────────────────────────────────────────────────────────────────────────

class AdvancedSalesWindow : public QDialog
{
    Q_OBJECT
public:
    explicit AdvancedSalesWindow(QWidget *parent = nullptr);

private slots:
    void onCustomerSelected(int index);
    void onProductComboSelected(int index);
    void onQtyChanged();
    void onCurrencyChanged();
    void onGroupClicked(QListWidgetItem *item);
    void onProductSearchChanged(const QString &text);
    void onProductTableDoubleClicked(int row, int col);
    void onItemTableChanged(QTableWidgetItem *item);
    void onReceivedChanged();
    void addItem();
    void deleteItem();
    void saveInvoice();
    void waitInvoice();
    void retrieveInvoice();
    void printInvoice();
    void printThermalSlot();
    void exportPDFSlot();
    void showCustomerStatement();
    void navigateFirst();
    void navigateLast();
    void updateTotals();
    void clearForm();

private:
    void setupUI();
    void applyStyles();
    void loadCustomers();
    void loadGroups();
    void loadProducts(const QString &group, const QString &search);
    void addRowToTable(int productId, const QString &barcode,
                       const QString &name, const QString &pricingType,
                       double qty, double priceDollar, double priceDinar,
                       double totalDollar, double totalDinar,
                       const QString &notes);
    void loadInvoice(int id);
    void recalcRowTotal(int row);
    void applyCurrentPriceToEntryRow(int productId);

    // ── invoice-table column indices ──
    enum Col {
        C_NUM    = 0,
        C_NAME   = 1,   // اسم المادة
        C_TYPE   = 2,   // نوع التسعير
        C_QTY    = 3,   // الكمية
        C_PRICE  = 4,   // السعر
        C_TOTAL  = 5,   // المبلغ الكلي
        C_NOTES  = 6,   // ملاحظات
        C_PID    = 7,   // product_id  (hidden)
        C_PDOL   = 8,   // price $     (hidden)
        C_PDIN   = 9,   // price IQD   (hidden)
        C_BARCODE= 10   // barcode     (hidden)
    };

    // ── Header ──────────────────────────────────────────────────────────────
    QSpinBox    *m_invoiceNoSpin;
    QComboBox   *m_customerCombo;
    QDateEdit   *m_dateEdit;
    QLabel      *m_dayLabel;
    QComboBox   *m_paymentTypeCombo;
    QComboBox   *m_currencyCombo;
    QComboBox   *m_invoiceTypeCombo;
    QLineEdit   *m_exchangeRateEdit;
    QLineEdit   *m_notesEdit;
    QLabel      *m_timeLabel;
    QLabel      *m_operatorLabel;

    // ── Customer info (read-only) ────────────────────────────────────────
    QLabel      *m_balDollarLabel;
    QLabel      *m_balDinarLabel;
    QLineEdit   *m_addressEdit;
    QLineEdit   *m_phoneEdit;

    // ── Item-entry row ───────────────────────────────────────────────────
    QComboBox   *m_barcodeCombo;
    QComboBox   *m_productCombo;
    QComboBox   *m_entryPricingCombo;
    QLineEdit   *m_qtyEdit;
    QLineEdit   *m_unitPriceEdit;
    QLineEdit   *m_totalEdit;
    QLabel      *m_stockLabel;

    // ── Invoice table ────────────────────────────────────────────────────
    QTableWidget *m_itemsTable;

    // ── Summary / accounting ─────────────────────────────────────────────
    QLabel      *m_totalDollarLabel;
    QLabel      *m_totalDinarLabel;
    QLineEdit   *m_receivedDollarEdit;
    QLineEdit   *m_receivedDinarEdit;
    QLabel      *m_changeLabel;
    QLabel      *m_currentBalDollarLabel;
    QLabel      *m_currentBalDinarLabel;

    // ── Right panel ──────────────────────────────────────────────────────
    QListWidget  *m_groupsList;
    QLineEdit    *m_productSearchEdit;
    QTableWidget *m_productTable;   // cols: name, stock, price

    // ── Toolbar ──────────────────────────────────────────────────────────
    QPushButton *m_saveBtn;
    QPushButton *m_deleteBtn;
    QPushButton *m_printBtn;
    QPushButton *m_thermalBtn;
    QPushButton *m_pdfBtn;
    QPushButton *m_waitBtn;
    QPushButton *m_retrieveBtn;
    QPushButton *m_customerStatBtn;
    QPushButton *m_prepListBtn;
    QSpinBox    *m_copiesSpinBox;
    QPushButton *m_firstBtn;
    QPushButton *m_lastBtn;
    QLabel      *m_navLabel;

    // ── State ────────────────────────────────────────────────────────────
    int     m_currentInvoiceId;
    int     m_currentCustomerId;
    double  m_customerBalDollar;
    double  m_customerBalDinar;
    bool    m_blockSignals;

    QList<WaitingInvoice> m_waitingInvoices;
    PrintManager *m_printer;
};

#endif // ADVANCEDSALESWINDOW_H
