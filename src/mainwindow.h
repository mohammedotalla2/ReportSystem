#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QDate>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
void onBarcodeEntered(); // الدالة الجديدة
    void openSales();
    void openPurchases();
    void openCashBox();
    void openReports();
    void openSystemSettings();
    void updateDateTime();
    void openDailyReport();
    void openCustomerStatement();
    void openCustomerBalances();
    void openInventoryReport();
    void showTodaySalesDetails();
    
   

private:
    void setupUI();
    void applyStyles();

    QLabel *m_dayLabel;
    QLabel *m_dateLabel;
    QLabel *m_todaySalesLabel;
    QLabel *m_companyLabel;
    QLabel *m_trialLabel;

    QTimer *m_timer;

    // Center panel - main buttons
    QPushButton *m_salesBtn;
    QPushButton *m_cashBoxBtn;
    QPushButton *m_reportsBtn;
    QPushButton *m_settingsBtn;

    // Left panel shortcuts
    QPushButton *m_dailyReportBtn;
    QPushButton *m_customerStatementBtn;
    QPushButton *m_customerBalancesBtn;
    QPushButton *m_inventoryBtn;
    QPushButton *m_closeBtn;

    // Right panel
    QPushButton *m_salesDetailsBtn;

    // Bottom sales entry
    QComboBox *m_barcodeCombo;
    QLineEdit *m_productNameEdit;
    QLineEdit *m_priceUsdEdit;
    QLineEdit *m_priceDinarEdit;
    QLineEdit *m_qtyEdit;
    QLineEdit *m_costEdit;
    QComboBox *m_customerCombo;
    QLineEdit *m_balDollarEdit;
    QLineEdit *m_balDinarEdit;
    QComboBox *m_productSearchCombo;

};

#endif // MAINWINDOW_H
