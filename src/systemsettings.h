#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H
#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>

class SystemSettings : public QDialog {
    Q_OBJECT
public:
    explicit SystemSettings(QWidget *parent = nullptr);
private slots:
    void saveSettings();
    void addCustomer(); void editCustomer(); void deleteCustomer();
    void addProduct(); void editProduct(); void deleteProduct();
    void loadCustomers(); void loadProducts();
private:
    void setupUI(); void applyStyles();
    // Settings tab
    QLineEdit *m_companyNameEdit;
    QDoubleSpinBox *m_exchangeRateSpin;
    QLineEdit *m_cashboxDollarEdit, *m_cashboxDinarEdit;
    // Customers tab
    QTableWidget *m_customersTable;
    QLineEdit *m_custNameEdit, *m_custRegionEdit, *m_custPhoneEdit;
    QLineEdit *m_custAddressEdit, *m_custNotesEdit;
    QDoubleSpinBox *m_custBalDollar, *m_custBalDinar;
    QComboBox *m_custTypeCombo;
    // Products tab
    QTableWidget *m_productsTable;
    QLineEdit *m_prodBarcodeEdit, *m_prodNameEdit, *m_prodGroupEdit, *m_prodTypeEdit;
    QDoubleSpinBox *m_prodCostSpin, *m_prodWsDolSpin, *m_prodRetDolSpin;
    QDoubleSpinBox *m_prodWsDinSpin, *m_prodRetDinSpin, *m_prodStockSpin;
    QSpinBox *m_prodCartoonSpin;

    QPushButton *m_saveBtn, *m_closeBtn;
    QTabWidget *m_tabs;
};
#endif
