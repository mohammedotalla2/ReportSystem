#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDate>
#include <QVariant>

class Database
{
public:
    static bool init(const QString &path = "data.db");
    static bool createTables();
    static QSqlDatabase db();

    // Settings
    static double getExchangeRate();
    static void setExchangeRate(double rate);
    static QString getCompanyName();
    static void setCompanyName(const QString &name);

    // Customers  (type: 0=زبون, 1=مجهز, 2=زبون ومجهز)
    static QSqlQuery getCustomers();
    static int addCustomer(const QString &name, const QString &region,
                           const QString &address, const QString &phone,
                           const QString &notes, double balanceDollar,
                           double balanceDinar, int type);
    static bool updateCustomer(int id, const QString &name, const QString &region,
                               const QString &address, const QString &phone,
                               const QString &notes, double balanceDollar,
                               double balanceDinar, int type = 0);
    static bool deleteCustomer(int id);

    // Product groups
    static QStringList getProductGroups();
    static void ensureProductGroup(const QString &groupName);

    // Products
    static QSqlQuery getProducts();
    static int addProduct(const QString &code, const QString &name,
                          const QString &group, const QString &type,
                          double costPrice, double costPriceDinar,
                          double wholeSaleDollar, double retailDollar,
                          double wholeSaleDinar, double retailDinar,
                          int cartoonQty, double stockQty, double stockCartons,
                          double reorderLevel, double exchangeRateAtAdd);
    static bool updateProduct(int id, const QString &name,
                              const QString &group, const QString &type,
                              double costPrice, double costPriceDinar,
                              double wholeSaleDollar, double retailDollar,
                              double wholeSaleDinar, double retailDinar,
                              double stockQty, double reorderLevel);

    // Sales
    static int createSalesInvoice(int customerId, const QDate &date,
                                  const QString &paymentType, const QString &currency,
                                  const QString &notes, double totalDollar,
                                  double totalDinar, int operatorId);
    static bool addSalesItem(int invoiceId, int productId, double qty,
                             double unitPriceDollar, double unitPriceDinar,
                             double totalDollar, double totalDinar, const QString &notes);

    // Purchases
    static int createPurchaseInvoice(int supplierId, const QDate &date,
                                     const QString &purchaseType,
                                     const QString &paymentType,
                                     const QString &currency, double exchangeRate,
                                     const QString &notes, int operatorId);
    static bool addPurchaseItem(int invoiceId, int productId, double qty,
                                double costPrice, double wholeSaleDollar,
                                double retailDollar, double wholeSaleDinar,
                                double retailDinar, const QString &notes);

    // Cash Box
    static int addCashTransaction(int type, const QDate &date, const QString &time,
                                  int operatorId, const QString &docNo,
                                  const QString &mainAccount, const QString &subAccount,
                                  double amountDollar, double amountDinar,
                                  double equivalentDinar, double equivalentDollar,
                                  double exchangeRate, const QString &direction,
                                  const QString &notes);

    // Reports
    static QSqlQuery getDailyReport(const QDate &from, const QDate &to, bool byUser, int userId = 0);
    static QSqlQuery getCustomerStatement(int customerId, const QDate &from, const QDate &to);
    static QSqlQuery getCustomerBalances(const QDate &upTo, const QString &region = QString(),
                                         const QString &type = QString(), int balanceType = 0);
    static QSqlQuery getInventoryReport(const QDate &upTo, const QString &groupBy, bool hideZero);
    static QSqlQuery getLatePayments(int days, const QString &region = QString());
    static QSqlQuery getTotalSalesReport(const QDate &from, const QDate &to,
                                          int customerId, int productId,
                                          const QString &groupBy);
    static QSqlQuery getAllCustomers(const QString &region, const QString &type);
    static QSqlQuery getOverallCustomerBalance(const QString &region, const QString &type, int balanceType);

    static double getCashBoxBalance(const QString &currency, const QDate &upTo = QDate::currentDate());
    static double getTodaySales();

private:
    static QString m_dbPath;
};

#endif // DATABASE_H
