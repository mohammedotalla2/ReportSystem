#ifndef CASHBOXWINDOW_H
#define CASHBOXWINDOW_H
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QSpinBox>
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
private:
    void setupUI(); void applyStyles();
    void loadTransaction(int id);
    void clearForm();
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
};
#endif
