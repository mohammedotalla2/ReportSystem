#ifndef SALESWINDOW_H
#define SALESWINDOW_H

#include <QDialog>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QList>
#include "printmanager.h"

struct WaitedInvoiceItem {
    int    productId;
    QString productName;
    QString unit;
    double qty;
    double unitPrice;
    double total;
    QString notes;
};

struct WaitedInvoice {
    int    customerId;
    QString customerName;
    QString address;
    QString phone;
    QDate  date;
    QString paymentType;
    QString currency;
    QString invoiceType;
    double exchangeRate;
    QString receiver;
    QString notes;
    double receivedDollar;
    double receivedDinar;
    double discount;
    QList<WaitedInvoiceItem> items;
};

class SalesWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SalesWindow(QWidget *parent = nullptr);

private slots:
    /* --- Action Slots --- */
    void addItem();
    void editItem();
    void deleteItem();
    void saveInvoice();
    void newInvoice();
    void searchInvoice();
    void deleteInvoice();
    void printInvoice();
    void printThermal();
    void exportPDF();
    void printPreparationList();
    void showCustomerStatement();
    void addToWait();
    void retrieveFromWait();

    /* --- Logic & Calculation Slots --- */
    void onProductSelected(int index);
    void onCustomerSelected(int index);
    void onQtyChanged();
    void onCurrencyChanged();
    void onInvoiceTypeChanged();
    void updateTotals();

    /* --- Navigation --- */
    void navigateFirst();
    void navigatePrev();
    void navigateNext();
    void navigateLast();

private:
    void setupUI();
    void applyStyles();
    void loadCustomers();
    void loadProducts();
    void loadInvoice(int id);
    void clearForm();
    
    /* --- Helper Methods --- */
    void updateDayName();
    void refreshInvoiceCount();
    void renumberRows();
    void repriceRow(int row, int productId);
    void updateProductInfoLabels(int productId);
    double getProductPrice(int productId);

    /* ── Header ── */
    QSpinBox    *m_invoiceNoSpin;
    QComboBox   *m_customerCombo;
    QDateEdit   *m_dateEdit;
    QDateEdit   *m_monthEdit;
    QDateEdit   *m_yearEdit;
    QLabel      *m_dayLabel;
    QComboBox   *m_paymentTypeCombo;
    QComboBox   *m_currencyCombo;
    QComboBox   *m_invoiceTypeCombo;
    QLineEdit   *m_exchangeRateEdit;
    QLineEdit   *m_receiverEdit;
    QLineEdit   *m_addressEdit;
    QLineEdit   *m_phoneEdit;
    QLineEdit   *m_notesEdit;
    QDateEdit   *m_dueDateEdit;
    QLabel      *m_timeLabel;
    QLabel      *m_operatorLabel;
    QComboBox   *m_driverCombo;
    QLineEdit   *m_carNoEdit;

    /* ── Right product-selection panel ── */
    QComboBox    *m_groupCombo;
    QLineEdit    *m_partNameEdit;
    QTableWidget *m_productList;

    /* ── Current item entry row ── */
    QComboBox   *m_barcodeCombo;
    QComboBox   *m_productCombo;
    QComboBox   *m_unitCombo;
    QLineEdit   *m_qtyEdit;
    QLineEdit   *m_unitPriceEdit;
    QLineEdit   *m_totalEdit;
    QLabel      *m_stockLabel;
    QLabel      *m_itemSeqLabel;
    QPushButton *m_editItemBtn;

    /* ── Items table ── */
    QTableWidget *m_itemsTable;

    /* ── Summary & Details ── */
    QLabel *m_totalDollarLabel;
    QLabel *m_totalDinarLabel;
    QLabel *m_balDollarLabel;
    QLabel *m_balDinarLabel;
    QLabel *m_currentBalDollarLabel;
    QLabel *m_currentBalDinarLabel;
    QLabel *m_itemStockBalLabel;
    QLabel *m_lastPriceDollarLabel;
    QLabel *m_lastPriceDinarLabel;

    QLineEdit *m_receivedDollarEdit;
    QLineEdit *m_receivedDinarEdit;
    QLineEdit *m_discountDinarEdit;

    /* ── Toolbar ── */
    QPushButton *m_saveBtn;
    QPushButton *m_newBtn;
    QPushButton *m_deleteBtn;
    QPushButton *m_searchBtn;
    QPushButton *m_printBtn;
    QPushButton *m_pdfBtn;
    QPushButton *m_thermalBtn;
    QPushButton *m_prepListBtn;
    QPushButton *m_customerStatBtn;
    QPushButton *m_productMoveBtn;
    
    QPushButton *m_firstBtn;
    QPushButton *m_prevBtn;
    QPushButton *m_nextBtn;
    QPushButton *m_lastBtn;
    
    QLabel      *m_navLabel;
    QLabel      *m_totalCountLabel;
    QSpinBox    *m_copiesSpinBox;
    QCheckBox   *m_printedCheck;

    int m_currentInvoiceId;
    int m_totalInvoices;
    PrintManager *m_printer;
    QList<WaitedInvoice> m_waitList;
};

#endif // SALESWINDOW_H