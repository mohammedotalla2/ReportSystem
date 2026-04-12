#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

QString Database::m_dbPath = "data.db";

bool Database::init(const QString &path)
{
    m_dbPath = path;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    if (!db.open()) {
        qDebug() << "DB Error:" << db.lastError().text();
        return false;
    }
    return createTables();
}

bool Database::createTables()
{
    QSqlQuery q;

    // Settings
    q.exec("CREATE TABLE IF NOT EXISTS settings ("
           "key TEXT PRIMARY KEY, value TEXT)");
    q.exec("INSERT OR IGNORE INTO settings VALUES ('exchange_rate', '1450')");
    q.exec("INSERT OR IGNORE INTO settings VALUES ('company_name', 'Sales System')");

    // Customers (type: 0=person, 1=company)
    q.exec("CREATE TABLE IF NOT EXISTS customers ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "name TEXT NOT NULL,"
           "region TEXT,"
           "address TEXT,"
           "phone TEXT,"
           "notes TEXT,"
           "balance_dollar REAL DEFAULT 0,"
           "balance_dinar REAL DEFAULT 0,"
           "type INTEGER DEFAULT 0,"
           "created_at DATE DEFAULT CURRENT_DATE)");

    // Products
    q.exec("CREATE TABLE IF NOT EXISTS products ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "barcode TEXT UNIQUE,"
           "name TEXT NOT NULL,"
           "product_group TEXT,"
           "product_type TEXT,"
           "cost_price REAL DEFAULT 0,"
           "wholesale_dollar REAL DEFAULT 0,"
           "retail_dollar REAL DEFAULT 0,"
           "wholesale_dinar REAL DEFAULT 0,"
           "retail_dinar REAL DEFAULT 0,"
           "cartoon_qty INTEGER DEFAULT 1,"
           "stock_qty REAL DEFAULT 0,"
           "stock_cartons REAL DEFAULT 0,"
           "created_at DATE DEFAULT CURRENT_DATE)");

    // Sales invoices
    q.exec("CREATE TABLE IF NOT EXISTS sales_invoices ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "customer_id INTEGER,"
           "date DATE,"
           "payment_type TEXT,"
           "currency TEXT,"
           "notes TEXT,"
           "total_dollar REAL DEFAULT 0,"
           "total_dinar REAL DEFAULT 0,"
           "exchange_rate REAL DEFAULT 1450,"
           "operator_id INTEGER DEFAULT 1,"
           "due_date DATE,"
           "FOREIGN KEY(customer_id) REFERENCES customers(id))");

    // Sales items
    q.exec("CREATE TABLE IF NOT EXISTS sales_items ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "invoice_id INTEGER,"
           "product_id INTEGER,"
           "qty REAL,"
           "unit_price_dollar REAL DEFAULT 0,"
           "unit_price_dinar REAL DEFAULT 0,"
           "total_dollar REAL DEFAULT 0,"
           "total_dinar REAL DEFAULT 0,"
           "notes TEXT,"
           "FOREIGN KEY(invoice_id) REFERENCES sales_invoices(id))");

    // Purchase invoices
    q.exec("CREATE TABLE IF NOT EXISTS purchase_invoices ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "supplier_id INTEGER,"
           "invoice_no TEXT,"
           "date DATE,"
           "purchase_type TEXT,"
           "payment_type TEXT,"
           "currency TEXT,"
           "exchange_rate REAL DEFAULT 1450,"
           "notes TEXT,"
           "total_dollar REAL DEFAULT 0,"
           "total_dinar REAL DEFAULT 0,"
           "operator_id INTEGER DEFAULT 1,"
           "FOREIGN KEY(supplier_id) REFERENCES customers(id))");

    // Purchase items
    q.exec("CREATE TABLE IF NOT EXISTS purchase_items ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "invoice_id INTEGER,"
           "product_id INTEGER,"
           "qty REAL,"
           "cost_price REAL DEFAULT 0,"
           "wholesale_dollar REAL DEFAULT 0,"
           "retail_dollar REAL DEFAULT 0,"
           "wholesale_dinar REAL DEFAULT 0,"
           "retail_dinar REAL DEFAULT 0,"
           "notes TEXT,"
           "FOREIGN KEY(invoice_id) REFERENCES purchase_invoices(id))");

    // Cash transactions
    q.exec("CREATE TABLE IF NOT EXISTS cash_transactions ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "type INTEGER,"
           "date DATE,"
           "time TEXT,"
           "operator_id INTEGER DEFAULT 1,"
           "doc_no TEXT,"
           "main_account TEXT,"
           "sub_account TEXT,"
           "amount_dollar REAL DEFAULT 0,"
           "amount_dinar REAL DEFAULT 0,"
           "equiv_dinar REAL DEFAULT 0,"
           "equiv_dollar REAL DEFAULT 0,"
           "exchange_rate REAL DEFAULT 1450,"
           "direction TEXT,"
           "notes TEXT,"
           "purchase_list TEXT)");

    // Operators / users
    q.exec("CREATE TABLE IF NOT EXISTS operators ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "name TEXT,"
           "username TEXT UNIQUE,"
           "password TEXT,"
           "active INTEGER DEFAULT 1)");
    q.exec("INSERT OR IGNORE INTO operators (id,name,username,password) VALUES (1,'Admin','1','1')");

    // Product groups (dynamic categories)
    q.exec("CREATE TABLE IF NOT EXISTS product_groups ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "name TEXT UNIQUE NOT NULL)");

    // Migrate existing tables: add new columns if they don't exist
    q.exec("ALTER TABLE products ADD COLUMN reorder_level REAL DEFAULT 0");
    q.exec("ALTER TABLE products ADD COLUMN cost_price_dinar REAL DEFAULT 0");
    q.exec("ALTER TABLE products ADD COLUMN exchange_rate_at_add REAL DEFAULT 1450");
    // customer type column already supports integer; 2 = زبون ومجهز (no schema change needed)

    if (q.lastError().isValid()) {
        // ALTER TABLE errors are expected if columns already exist – clear error
        QSqlQuery check;
        check.exec("SELECT reorder_level FROM products LIMIT 1");
        if (check.lastError().isValid()) {
            qDebug() << "Table creation error:" << check.lastError().text();
            return false;
        }
    }
    return true;
}

QSqlDatabase Database::db()
{
    return QSqlDatabase::database();
}

double Database::getExchangeRate()
{
    QSqlQuery q;
    q.exec("SELECT value FROM settings WHERE key='exchange_rate'");
    if (q.next()) return q.value(0).toDouble();
    return 1450;
}

void Database::setExchangeRate(double rate)
{
    QSqlQuery q;
    q.prepare("UPDATE settings SET value=? WHERE key='exchange_rate'");
    q.addBindValue(rate);
    q.exec();
}

QString Database::getCompanyName()
{
    QSqlQuery q;
    q.exec("SELECT value FROM settings WHERE key='company_name'");
    if (q.next()) return q.value(0).toString();
    return QString::fromUtf8("نظام الحسابات");
}

void Database::setCompanyName(const QString &name)
{
    QSqlQuery q;
    q.prepare("UPDATE settings SET value=? WHERE key='company_name'");
    q.addBindValue(name);
    q.exec();
}

QSqlQuery Database::getCustomers()
{
    QSqlQuery q;
    q.exec("SELECT id, name, region, address, phone, notes, balance_dollar, balance_dinar, type FROM customers ORDER BY name");
    return q;
}

int Database::addCustomer(const QString &name, const QString &region,
                           const QString &address, const QString &phone,
                           const QString &notes, double balanceDollar,
                           double balanceDinar, int type)
{
    QSqlQuery q;
    q.prepare("INSERT INTO customers (name,region,address,phone,notes,balance_dollar,balance_dinar,type) "
              "VALUES (?,?,?,?,?,?,?,?)");
    q.addBindValue(name); q.addBindValue(region); q.addBindValue(address);
    q.addBindValue(phone); q.addBindValue(notes); q.addBindValue(balanceDollar);
    q.addBindValue(balanceDinar); q.addBindValue(type);
    if (q.exec()) return q.lastInsertId().toInt();
    return -1;
}

bool Database::updateCustomer(int id, const QString &name, const QString &region,
                               const QString &address, const QString &phone,
                               const QString &notes, double balanceDollar,
                               double balanceDinar, int type)
{
    QSqlQuery q;
    q.prepare("UPDATE customers SET name=?,region=?,address=?,phone=?,notes=?,"
              "balance_dollar=?,balance_dinar=?,type=? WHERE id=?");
    q.addBindValue(name); q.addBindValue(region); q.addBindValue(address);
    q.addBindValue(phone); q.addBindValue(notes); q.addBindValue(balanceDollar);
    q.addBindValue(balanceDinar); q.addBindValue(type); q.addBindValue(id);
    return q.exec();
}

bool Database::deleteCustomer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM customers WHERE id=?");
    q.addBindValue(id);
    return q.exec();
}

QSqlQuery Database::getProducts()
{
    QSqlQuery q;
    q.exec("SELECT id, barcode, name, product_group, product_type, cost_price, "
           "wholesale_dollar, retail_dollar, wholesale_dinar, retail_dinar, "
           "cartoon_qty, stock_qty, stock_cartons FROM products ORDER BY name");
    return q;
}

int Database::addProduct(const QString &code, const QString &name,
                          const QString &group, const QString &type,
                          double costPrice, double costPriceDinar,
                          double wholeSaleDollar, double retailDollar,
                          double wholeSaleDinar, double retailDinar,
                          int cartoonQty, double stockQty, double stockCartons,
                          double reorderLevel, double exchangeRateAtAdd)
{
    QSqlQuery q;
    q.prepare("INSERT INTO products (barcode,name,product_group,product_type,cost_price,"
              "cost_price_dinar,wholesale_dollar,retail_dollar,wholesale_dinar,retail_dinar,"
              "cartoon_qty,stock_qty,stock_cartons,reorder_level,exchange_rate_at_add) "
              "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    q.addBindValue(code);          q.addBindValue(name);
    q.addBindValue(group);         q.addBindValue(type);
    q.addBindValue(costPrice);     q.addBindValue(costPriceDinar);
    q.addBindValue(wholeSaleDollar); q.addBindValue(retailDollar);
    q.addBindValue(wholeSaleDinar);  q.addBindValue(retailDinar);
    q.addBindValue(cartoonQty);    q.addBindValue(stockQty);
    q.addBindValue(stockCartons);  q.addBindValue(reorderLevel);
    q.addBindValue(exchangeRateAtAdd);
    if (q.exec()) return q.lastInsertId().toInt();
    qDebug() << "addProduct error:" << q.lastError().text();
    return -1;
}

bool Database::updateProduct(int id, const QString &name,
                              const QString &group, const QString &type,
                              double costPrice, double costPriceDinar,
                              double wholeSaleDollar, double retailDollar,
                              double wholeSaleDinar, double retailDinar,
                              double stockQty, double reorderLevel)
{
    QSqlQuery q;
    q.prepare("UPDATE products SET name=?,product_group=?,product_type=?,cost_price=?,"
              "cost_price_dinar=?,wholesale_dollar=?,retail_dollar=?,"
              "wholesale_dinar=?,retail_dinar=?,stock_qty=?,reorder_level=? WHERE id=?");
    q.addBindValue(name);          q.addBindValue(group);
    q.addBindValue(type);          q.addBindValue(costPrice);
    q.addBindValue(costPriceDinar); q.addBindValue(wholeSaleDollar);
    q.addBindValue(retailDollar);  q.addBindValue(wholeSaleDinar);
    q.addBindValue(retailDinar);   q.addBindValue(stockQty);
    q.addBindValue(reorderLevel);  q.addBindValue(id);
    return q.exec();
}

QStringList Database::getProductGroups()
{
    QStringList list;
    QSqlQuery q("SELECT name FROM product_groups ORDER BY name");
    while (q.next()) list << q.value(0).toString();
    return list;
}

void Database::ensureProductGroup(const QString &groupName)
{
    if (groupName.trimmed().isEmpty()) return;
    QSqlQuery q;
    q.prepare("INSERT OR IGNORE INTO product_groups (name) VALUES (?)");
    q.addBindValue(groupName.trimmed());
    q.exec();
}

int Database::createSalesInvoice(int customerId, const QDate &date,
                                  const QString &paymentType, const QString &currency,
                                  const QString &notes, double totalDollar,
                                  double totalDinar, int operatorId)
{
    QSqlQuery q;
    q.prepare("INSERT INTO sales_invoices (customer_id,date,payment_type,currency,notes,"
              "total_dollar,total_dinar,operator_id,exchange_rate) VALUES (?,?,?,?,?,?,?,?,?)");
    q.addBindValue(customerId); q.addBindValue(date.toString("yyyy-MM-dd"));
    q.addBindValue(paymentType); q.addBindValue(currency); q.addBindValue(notes);
    q.addBindValue(totalDollar); q.addBindValue(totalDinar); q.addBindValue(operatorId);
    q.addBindValue(getExchangeRate());
    if (q.exec()) return q.lastInsertId().toInt();
    return -1;
}

bool Database::addSalesItem(int invoiceId, int productId, double qty,
                             double unitPriceDollar, double unitPriceDinar,
                             double totalDollar, double totalDinar, const QString &notes)
{
    QSqlQuery q;
    q.prepare("INSERT INTO sales_items (invoice_id,product_id,qty,unit_price_dollar,"
              "unit_price_dinar,total_dollar,total_dinar,notes) VALUES (?,?,?,?,?,?,?,?)");
    q.addBindValue(invoiceId); q.addBindValue(productId); q.addBindValue(qty);
    q.addBindValue(unitPriceDollar); q.addBindValue(unitPriceDinar);
    q.addBindValue(totalDollar); q.addBindValue(totalDinar); q.addBindValue(notes);
    // Update stock
    if (q.exec()) {
        QSqlQuery upd;
        upd.prepare("UPDATE products SET stock_qty = stock_qty - ? WHERE id=?");
        upd.addBindValue(qty); upd.addBindValue(productId);
        upd.exec();
        return true;
    }
    return false;
}

int Database::createPurchaseInvoice(int supplierId, const QDate &date,
                                     const QString &purchaseType,
                                     const QString &paymentType,
                                     const QString &currency, double exchangeRate,
                                     const QString &notes, int operatorId)
{
    QSqlQuery q;
    q.prepare("INSERT INTO purchase_invoices (supplier_id,date,purchase_type,payment_type,"
              "currency,exchange_rate,notes,operator_id) VALUES (?,?,?,?,?,?,?,?)");
    q.addBindValue(supplierId); q.addBindValue(date.toString("yyyy-MM-dd"));
    q.addBindValue(purchaseType); q.addBindValue(paymentType); q.addBindValue(currency);
    q.addBindValue(exchangeRate); q.addBindValue(notes); q.addBindValue(operatorId);
    if (q.exec()) return q.lastInsertId().toInt();
    return -1;
}

bool Database::addPurchaseItem(int invoiceId, int productId, double qty,
                                double costPrice, double wholeSaleDollar,
                                double retailDollar, double wholeSaleDinar,
                                double retailDinar, const QString &notes)
{
    QSqlQuery q;
    q.prepare("INSERT INTO purchase_items (invoice_id,product_id,qty,cost_price,"
              "wholesale_dollar,retail_dollar,wholesale_dinar,retail_dinar,notes) "
              "VALUES (?,?,?,?,?,?,?,?,?)");
    q.addBindValue(invoiceId); q.addBindValue(productId); q.addBindValue(qty);
    q.addBindValue(costPrice); q.addBindValue(wholeSaleDollar); q.addBindValue(retailDollar);
    q.addBindValue(wholeSaleDinar); q.addBindValue(retailDinar); q.addBindValue(notes);
    if (q.exec()) {
        QSqlQuery upd;
        upd.prepare("UPDATE products SET stock_qty = stock_qty + ?, cost_price=?, "
                    "wholesale_dollar=?, retail_dollar=?, wholesale_dinar=?, retail_dinar=? WHERE id=?");
        upd.addBindValue(qty); upd.addBindValue(costPrice); upd.addBindValue(wholeSaleDollar);
        upd.addBindValue(retailDollar); upd.addBindValue(wholeSaleDinar); upd.addBindValue(retailDinar);
        upd.addBindValue(productId); upd.exec();
        return true;
    }
    return false;
}

int Database::addCashTransaction(int type, const QDate &date, const QString &time,
                                  int operatorId, const QString &docNo,
                                  const QString &mainAccount, const QString &subAccount,
                                  double amountDollar, double amountDinar,
                                  double equivalentDinar, double equivalentDollar,
                                  double exchangeRate, const QString &direction,
                                  const QString &notes)
{
    QSqlQuery q;
    q.prepare("INSERT INTO cash_transactions (type,date,time,operator_id,doc_no,main_account,"
              "sub_account,amount_dollar,amount_dinar,equiv_dinar,equiv_dollar,exchange_rate,"
              "direction,notes) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    q.addBindValue(type); q.addBindValue(date.toString("yyyy-MM-dd")); q.addBindValue(time);
    q.addBindValue(operatorId); q.addBindValue(docNo); q.addBindValue(mainAccount);
    q.addBindValue(subAccount); q.addBindValue(amountDollar); q.addBindValue(amountDinar);
    q.addBindValue(equivalentDinar); q.addBindValue(equivalentDollar); q.addBindValue(exchangeRate);
    q.addBindValue(direction); q.addBindValue(notes);
    if (q.exec()) return q.lastInsertId().toInt();
    return -1;
}

QSqlQuery Database::getDailyReport(const QDate &from, const QDate &to, bool byUser, int userId)
{
    QSqlQuery q;
    QString sql = "SELECT si.id, si.date, c.name as customer, si.payment_type, "
                  "si.total_dollar, si.total_dinar, si.currency, si.operator_id "
                  "FROM sales_invoices si LEFT JOIN customers c ON si.customer_id=c.id "
                  "WHERE si.date BETWEEN ? AND ?";
    if (byUser && userId > 0) sql += " AND si.operator_id=" + QString::number(userId);
    sql += " ORDER BY si.date, si.id";
    q.prepare(sql);
    q.addBindValue(from.toString("yyyy-MM-dd"));
    q.addBindValue(to.toString("yyyy-MM-dd"));
    q.exec();
    return q;
}

QSqlQuery Database::getCustomerStatement(int customerId, const QDate &from, const QDate &to)
{
    QSqlQuery q;
    q.prepare("SELECT 'sale' as type, si.date, si.id as doc_no, "
              "si.total_dollar as debit_dollar, 0 as credit_dollar, "
              "si.total_dinar as debit_dinar, 0 as credit_dinar "
              "FROM sales_invoices si WHERE si.customer_id=? AND si.date BETWEEN ? AND ? "
              "UNION ALL "
              "SELECT 'payment' as type, ct.date, ct.id as doc_no, "
              "0 as debit_dollar, ct.amount_dollar as credit_dollar, "
              "0 as debit_dinar, ct.amount_dinar as credit_dinar "
              "FROM cash_transactions ct WHERE ct.sub_account=? AND ct.date BETWEEN ? AND ? AND ct.type=2 "
              "ORDER BY date");
    QString cid = QString::number(customerId);
    q.addBindValue(customerId);
    q.addBindValue(from.toString("yyyy-MM-dd"));
    q.addBindValue(to.toString("yyyy-MM-dd"));
    q.addBindValue(cid);
    q.addBindValue(from.toString("yyyy-MM-dd"));
    q.addBindValue(to.toString("yyyy-MM-dd"));
    q.exec();
    return q;
}

QSqlQuery Database::getCustomerBalances(const QDate &upTo, const QString &region,
                                         const QString &type, int balanceType)
{
    QSqlQuery q;
    QString sql = "SELECT id, name, region, balance_dollar, balance_dinar, type FROM customers WHERE 1=1";
    if (!region.isEmpty()) sql += " AND region='" + region + "'";
    if (!type.isEmpty()) sql += " AND type='" + type + "'";
    if (balanceType == 1) sql += " AND balance_dollar > 0 OR balance_dinar > 0"; // debit
    else if (balanceType == 2) sql += " AND balance_dollar < 0 OR balance_dinar < 0"; // credit
    sql += " ORDER BY name";
    q.exec(sql);
    return q;
}

QSqlQuery Database::getInventoryReport(const QDate &upTo, const QString &groupBy, bool hideZero)
{
    QSqlQuery q;
    QString sql = "SELECT id, barcode, name, product_group, product_type, "
                  "stock_qty, cartoon_qty, cost_price, wholesale_dollar, retail_dollar "
                  "FROM products WHERE 1=1";
    if (hideZero) sql += " AND stock_qty != 0";
    if (groupBy == "group") sql += " ORDER BY product_group, name";
    else if (groupBy == "type") sql += " ORDER BY product_type, name";
    else sql += " ORDER BY name";
    q.exec(sql);
    return q;
}

QSqlQuery Database::getLatePayments(int days, const QString &region)
{
    QSqlQuery q;
    QString sql = QString("SELECT c.id, c.name, c.region, c.phone, "
                          "c.balance_dollar, c.balance_dinar "
                          "FROM customers c "
                          "WHERE (c.balance_dollar > 0 OR c.balance_dinar > 0)");
    if (!region.isEmpty()) sql += " AND c.region='" + region + "'";
    sql += " ORDER BY c.name";
    q.exec(sql);
    return q;
}

QSqlQuery Database::getTotalSalesReport(const QDate &from, const QDate &to,
                                          int customerId, int productId,
                                          const QString &groupBy)
{
    QSqlQuery q;
    QString sql;
    if (groupBy == "customer") {
        sql = "SELECT c.name, SUM(si.total_dollar) as total_dollar, SUM(si.total_dinar) as total_dinar "
              "FROM sales_invoices si LEFT JOIN customers c ON si.customer_id=c.id "
              "WHERE si.date BETWEEN ? AND ?";
        if (customerId > 0) sql += " AND si.customer_id=" + QString::number(customerId);
        sql += " GROUP BY c.id ORDER BY total_dollar DESC";
    } else if (groupBy == "product") {
        sql = "SELECT p.name, SUM(sit.qty) as total_qty, SUM(sit.total_dollar) as total_dollar "
              "FROM sales_items sit LEFT JOIN products p ON sit.product_id=p.id "
              "LEFT JOIN sales_invoices si ON sit.invoice_id=si.id "
              "WHERE si.date BETWEEN ? AND ?";
        if (productId > 0) sql += " AND sit.product_id=" + QString::number(productId);
        sql += " GROUP BY p.id ORDER BY total_dollar DESC";
    } else {
        sql = "SELECT si.id, si.date, c.name, si.total_dollar, si.total_dinar "
              "FROM sales_invoices si LEFT JOIN customers c ON si.customer_id=c.id "
              "WHERE si.date BETWEEN ? AND ?";
        if (customerId > 0) sql += " AND si.customer_id=" + QString::number(customerId);
        sql += " ORDER BY si.date";
    }
    q.prepare(sql);
    q.addBindValue(from.toString("yyyy-MM-dd"));
    q.addBindValue(to.toString("yyyy-MM-dd"));
    q.exec();
    return q;
}

QSqlQuery Database::getAllCustomers(const QString &region, const QString &type)
{
    QSqlQuery q;
    QString sql = "SELECT id, name, region, phone, address, balance_dollar, balance_dinar FROM customers WHERE 1=1";
    if (!region.isEmpty()) sql += " AND region='" + region + "'";
    if (!type.isEmpty()) sql += " AND type='" + type + "'";
    sql += " ORDER BY name";
    q.exec(sql);
    return q;
}

QSqlQuery Database::getOverallCustomerBalance(const QString &region, const QString &type, int balanceType)
{
    QSqlQuery q;
    QString sql = "SELECT name, region, balance_dollar, balance_dinar FROM customers WHERE 1=1";
    if (!region.isEmpty()) sql += " AND region='" + region + "'";
    if (!type.isEmpty()) sql += " AND type=" + type;
    if (balanceType == 1) sql += " AND balance_dollar > 0";
    else if (balanceType == 2) sql += " AND balance_dollar < 0";
    sql += " ORDER BY name";
    q.exec(sql);
    return q;
}

double Database::getCashBoxBalance(const QString &currency, const QDate &upTo)
{
    QSqlQuery q;
    if (currency == "$") {
        q.prepare("SELECT SUM(CASE WHEN type=2 THEN amount_dollar ELSE -amount_dollar END) "
                  "FROM cash_transactions WHERE date <= ?");
    } else {
        q.prepare("SELECT SUM(CASE WHEN type=2 THEN amount_dinar ELSE -amount_dinar END) "
                  "FROM cash_transactions WHERE date <= ?");
    }
    q.addBindValue(upTo.toString("yyyy-MM-dd"));
    q.exec();
    if (q.next()) return q.value(0).toDouble();
    return 0;
}

double Database::getTodaySales()
{
    QSqlQuery q;
    q.prepare("SELECT COALESCE(SUM(total_dollar), 0) FROM sales_invoices WHERE date=?");
    q.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    q.exec();
    if (q.next()) return q.value(0).toDouble();
    return 0;
}
