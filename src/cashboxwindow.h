#ifndef CASHBOXWINDOW_H
#define CASHBOXWINDOW_H
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QHeaderView>
#include "printmanager.h"

class CashBoxWindow : public QDialog {
    Q_OBJECT
public:
    explicit CashBoxWindow(QWidget *parent = nullptr);
private slots:
    void onTypeChanged(int index);
    void onMainAccountChanged(int index);
    void calculateEquivalent();
    void updateBalance();        // recompute projected balance when amounts change
    void saveTransaction();
    void newTransaction();
    void deleteTransaction();
    void searchTransaction();
    void printReceipt();
    void exportPDF();
    void navigateFirst(); void navigatePrev();
    void navigateNext();  void navigateLast();

private:
    void setupUI();
    void applyStyles();
    void loadTransaction(int id);
    void clearForm();
    void refreshBalanceDisplay();   // fetch current balance from DB and show it

    // ── Transaction type combo ──
    QComboBox  *m_typeCombo;

    // ── Date (compact: day / month / year) ──
    QDateEdit  *m_dateDay, *m_dateMonth, *m_dateYear;

    // ── Header fields ──
    QLineEdit  *m_docNoEdit;
    QSpinBox   *m_organiserSpin;
    QLabel     *m_timeLabel;

    // ── Accounts ──
    QComboBox  *m_mainAccountCombo, *m_subAccountCombo;

    // ── Customer section (hidden unless main account = عملاء/موردون) ──
    QWidget    *m_customerWidget;
    QComboBox  *m_customerCombo;
    QLabel     *m_custBalDollarLabel, *m_custBalDinarLabel;

    // ── Amounts ──
    QLineEdit  *m_amountDollarEdit, *m_amountDinarEdit;
    QLineEdit  *m_equivDinarEdit,   *m_equivDollarEdit;
    QPushButton *m_equivBtn;

    // ── Exchange rate / cashbox ──
    QLineEdit  *m_exchangeRateEdit;
    QComboBox  *m_cashboxCombo;

    // ── Notes ──
    QLineEdit  *m_notesEdit;

    // ── Balance display (top panel) ──
    QLabel     *m_curBalDollarLabel, *m_curBalDinarLabel;
    QLabel     *m_projBalDollarLabel, *m_projBalDinarLabel;

    // ── Toolbar buttons ──
    QPushButton *m_saveBtn, *m_searchBtn, *m_deleteBtn;
    QPushButton *m_printBtn, *m_pdfBtn;
    QPushButton *m_firstBtn, *m_prevBtn, *m_nextBtn, *m_lastBtn;
    QLabel      *m_navLabel, *m_totalLabel;

    int m_currentId;
    PrintManager *m_printer;
};
#endif
