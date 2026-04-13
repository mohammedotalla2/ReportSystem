#ifndef CASHBOXWINDOW_H
#define CASHBOXWINDOW_H
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include "printmanager.h"

class CashBoxWindow : public QDialog {
    Q_OBJECT
public:
    explicit CashBoxWindow(QWidget *parent = nullptr);
private slots:
    void onTypeChanged();
    void onMainAccountChanged(int index);
    void calculateEquivalent();
    void saveTransaction();
    void newTransaction();
    void deleteTransaction();
    void searchTransaction();
    void printReceipt();
    void exportPDF();
    void navigateFirst(); void navigatePrev();
    void navigateNext(); void navigateLast();
    void receiveFromCustomer();
    void payToCustomer();

    void searchDailyReport();
    void searchVaultBalance(const QString &currency);
    void searchExchangeRates();
    void searchAccountsLedger();
    void searchDiscounts();

private:
    void setupUI();
    void applyStyles();
    void loadTransaction(int id);
    void clearForm();

    QWidget* createTab1DailyReport();
    QWidget* createTab2VaultBalance();
    QWidget* createTab3ExchangeRates();
    QWidget* createTab4AccountsLedger();
    QWidget* createTab5Discounts();

    // Tab widget
    QTabWidget *m_tabs;

    // ── Tab 0: existing transaction entry ──
    // Header
    QComboBox *m_typeCombo;
    QLabel *m_timeLabel;
    QSpinBox *m_organiserSpin;
    QDateEdit *m_dateDay, *m_dateMonth, *m_dateYear;
    QLineEdit *m_docNoEdit;
    // Accounts
    QComboBox *m_mainAccountCombo, *m_subAccountCombo;
    // Balances
    QLineEdit *m_prevBalDollarEdit, *m_prevBalDinarEdit;
    QLineEdit *m_prevCustDollarEdit, *m_prevCustDinarEdit;
    // Amounts
    QLineEdit *m_amountDollarEdit, *m_amountDinarEdit;
    QLineEdit *m_equivDinarEdit, *m_equivDollarEdit;
    QPushButton *m_equivBtn;
    // Direction/cashbox
    QComboBox *m_cashboxCombo, *m_directionCombo;
    QLineEdit *m_exchangeRateEdit;
    QComboBox *m_purchaseListCombo;
    QLineEdit *m_notesEdit, *m_organiserEdit;
    // Navigation/toolbar
    QPushButton *m_saveBtn, *m_editBtn, *m_undoBtn, *m_searchBtn;
    QPushButton *m_deleteBtn, *m_printBtn, *m_pdfBtn;
    QPushButton *m_firstBtn, *m_prevBtn, *m_nextBtn, *m_lastBtn;
    QPushButton *m_receiveBtn, *m_payBtn;
    QLabel *m_navLabel, *m_totalLabel;
    int m_currentId;
    PrintManager *m_printer;

    // Tab 1 - Daily Report
    QDateEdit    *m_drFromDate, *m_drToDate;
    QLineEdit    *m_drCustomerEdit;
    QTableWidget *m_drTable;
    QLabel       *m_drNetDollar, *m_drNetDinar, *m_drTotalDollar, *m_drTotalDinar;

    // Tab 2 - Vault Balance
    QDateEdit    *m_vbFromDate, *m_vbToDate;
    QTableWidget *m_vbTable;

    // Tab 3 - Exchange Rates
    QDateEdit    *m_erFromDate, *m_erToDate;
    QTableWidget *m_erTable;

    // Tab 4 - Accounts Ledger
    QTabWidget   *m_alSubTabs;
    QTableWidget *m_alReceiptsTable, *m_alPaymentsTable;
    QLabel       *m_alReceiptsTotal, *m_alPaymentsTotal;

    // Tab 5 - Discounts
    QDateEdit    *m_discFromDate, *m_discToDate;
    QTableWidget *m_discTable;
};
#endif
