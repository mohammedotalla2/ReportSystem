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
#include "printmanager.h"

class SalesWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SalesWindow(QWidget *parent = nullptr);

private slots:
    void onProductSelected(int index);
    void onCustomerSelected(int index);
    void onQtyChanged();
    void addItem();
    void deleteItem();
    void saveInvoice();
    void printInvoice();
    void printThermal();
    void exportPDF();
    void printPreparationList();
    void showCustomerStatement();
    void navigateFirst();
    void navigateLast();
    void updateTotals();

private:
    void setupUI();
    void applyStyles();
    void loadCustomers();
    void loadProducts();
    void loadInvoice(int id);
    void clearForm();
    

    /* ── Header ── */
    QSpinBox    *m_invoiceNoSpin;
    QComboBox   *m_customerCombo;
    QDateEdit   *m_dateEdit;
    QLabel      *m_dayLabel;
    QComboBox   *m_paymentTypeCombo;
    QComboBox   *m_currencyCombo;
    QComboBox   *m_invoiceTypeCombo;
    QLineEdit   *m_exchangeRateEdit;
    QLineEdit   *m_receiverEdit;
    QLineEdit   *m_addressEdit;
    QLineEdit   *m_phoneEdit;
    QLineEdit   *m_notesEdit;
    QLabel      *m_timeLabel;
    QLabel      *m_operatorLabel;
    QComboBox   *m_driverCombo;
    QLineEdit   *m_carNoEdit;

    /* ── Right product-selection panel ── */
    QComboBox    *m_groupCombo;
    QLineEdit    *m_partNameEdit;
    QTableWidget *m_productList;

    /* ── Current item entry row ── */
    QComboBox  *m_barcodeCombo;
    QComboBox  *m_productCombo;
    QLineEdit  *m_qtyEdit;
    QComboBox  *m_unitCombo;
    QLineEdit  *m_unitPriceEdit;
    QLineEdit  *m_totalEdit;
    QLabel     *m_stockLabel;

    /* ── Items table ── */
    QTableWidget *m_itemsTable;

    /* ── Summary labels ── */
    QLabel *m_totalDollarLabel;
    QLabel *m_totalDinarLabel;
    QLabel *m_balDollarLabel;
    QLabel *m_balDinarLabel;
    QLabel *m_currentBalDollarLabel;
    QLabel *m_currentBalDinarLabel;

    /* ── Toolbar ── */
    QPushButton *m_saveBtn;
    QPushButton *m_deleteBtn;
    QPushButton *m_searchBtn;
    QPushButton *m_printBtn;
    QPushButton *m_pdfBtn;
    QPushButton *m_thermalBtn;
    QPushButton *m_prepListBtn;
    QPushButton *m_customerStatBtn;
    QPushButton *m_productMoveBtn;
    QPushButton *m_firstBtn;
    QPushButton *m_lastBtn;
    QLabel      *m_navLabel;
    QLabel      *m_totalCountLabel;
    QSpinBox    *m_copiesSpinBox;

    int m_currentInvoiceId;
    int m_totalInvoices;
    PrintManager *m_printer;
};

#endif // SALESWINDOW_H
