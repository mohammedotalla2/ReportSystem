#ifndef PURCHASEWINDOW_H
#define PURCHASEWINDOW_H
#include <QDialog>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QSpinBox>
#include "printmanager.h"

class PurchaseWindow : public QDialog {
    Q_OBJECT
public:
    explicit PurchaseWindow(QWidget *parent = nullptr);
private slots:
    void onProductSelected(int index);
    void onSupplierChanged(int index);
    void onCurrencyChanged();
    void addItem(); void deleteItem();
    void saveInvoice(); void clearForm();
    void printInvoice(); void exportPDF();
    void calculateTotals();
    void navigateFirst(); void navigatePrev();
    void navigateNext(); void navigateLast();
private:
    void setupUI(); void applyStyles();
    void loadSuppliers(); void loadProducts();

    QSpinBox *m_invoiceNoSpin;
    QComboBox *m_supplierCombo;
    QDateEdit *m_dateEdit;
    QComboBox *m_purchaseTypeCombo, *m_payTypeCombo, *m_currencyCombo;
    QComboBox *m_payMechCombo;
    QLineEdit *m_exchangeRateEdit, *m_invoiceRefEdit, *m_notesEdit;

    QComboBox *m_barcodeCombo, *m_productCombo;
    QLineEdit *m_qtyEdit, *m_costPriceEdit;
    QLineEdit *m_wholesaleDollarEdit, *m_retailDollarEdit;
    QLineEdit *m_wholesaleDinarEdit, *m_retailDinarEdit;
    QLineEdit *m_totalDinarEdit, *m_totalDollarEdit;
    QLabel *m_stockLabel;
    QLabel *m_lastPurchasePriceLabel;
    QTableWidget *m_itemsTable;

    // Summary row
    QLineEdit *m_grandTotalDollarEdit, *m_grandTotalDinarEdit;
    QLineEdit *m_discountEdit;
    QLabel *m_supplierBalLabel;
    QLabel *m_supplierBalDinarLabel;

    QPushButton *m_saveBtn, *m_deleteBtn, *m_searchBtn;
    QPushButton *m_printBtn, *m_pdfBtn;
    QPushButton *m_firstBtn, *m_prevBtn, *m_nextBtn, *m_lastBtn;
    QPushButton *m_totalExpensesBtn;
    QPushButton *m_closeBtn;
    QPushButton *m_supplierStatBtn;
    QLabel *m_navLabel, *m_totalCountLabel;
    QLabel *m_expensesRatioLabel;
    QSpinBox *m_organiserSpin;
    QLabel *m_timeLabel;

    int m_currentInvoiceId;
    PrintManager *m_printer;
};
#endif
