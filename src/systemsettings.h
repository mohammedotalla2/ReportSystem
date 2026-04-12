#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H
#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QEvent>

class SystemSettings : public QDialog {
    Q_OBJECT
public:
    explicit SystemSettings(QWidget *parent = nullptr);
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
    void saveSettings();
    // Customer slots
    void addCustomer();
    void editCustomer();
    void deleteCustomer();
    void loadCustomers();
    void clearCustomerForm();
    void searchCustomer();
    void onCustomerCellClicked(int row, int col);
    // Product slots
    void addProduct();
    void editProduct();
    void deleteProduct();
    void loadProducts();
    void clearProductForm();
    void generateBarcode();
    void onCostDollarChanged(double val);
    void onWsDollarChanged(double val);
    void onRetDollarChanged(double val);
    void loadProductGroups();
    void onTabChanged(int index);
    void updateNextProductId();
    void updateNextCustomerId();
private:
    void setupUI();
    void applyStyles();
    void installFocusClear(QWidget *w);
    void installEnterNav(QList<QWidget*> &order);
    // Helpers
    static QLabel *makeLabel(const QString &text);

    // Settings tab
    QLineEdit      *m_companyNameEdit;
    QDoubleSpinBox *m_exchangeRateSpin;
    QLineEdit      *m_cashboxDollarEdit, *m_cashboxDinarEdit;

    // Customers tab
    QTableWidget   *m_customersTable;
    QLabel         *m_custIdLabel;
    QLineEdit      *m_custNameEdit, *m_custRegionEdit, *m_custPhoneEdit;
    QLineEdit      *m_custAddressEdit, *m_custNotesEdit, *m_custSearchEdit;
    QDoubleSpinBox *m_custBalDollar, *m_custBalDinar;
    QComboBox      *m_custTypeCombo;
    int             m_editingCustomerId;

    // Products tab
    QTableWidget   *m_productsTable;
    QLabel         *m_prodIdLabel;
    QLineEdit      *m_prodBarcodeEdit, *m_prodNameEdit, *m_prodTypeEdit;
    QComboBox      *m_prodGroupCombo;
    QDoubleSpinBox *m_prodCostSpin,    *m_prodCostDinSpin;
    QDoubleSpinBox *m_prodWsDolSpin,   *m_prodRetDolSpin;
    QDoubleSpinBox *m_prodWsDinSpin,   *m_prodRetDinSpin;
    QDoubleSpinBox *m_prodStockSpin,   *m_prodReorderSpin;
    QSpinBox       *m_prodCartoonSpin;
    int             m_editingProductId;

    QPushButton *m_saveBtn, *m_closeBtn;
    QTabWidget  *m_tabs;

    // Tab-order lists for Enter-key navigation
    QList<QWidget*> m_prodTabOrder;
    QList<QWidget*> m_custTabOrder;
};
#endif
