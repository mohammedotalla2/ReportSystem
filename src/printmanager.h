#ifndef PRINTMANAGER_H
#define PRINTMANAGER_H

#include <QString>
#include <QWidget>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QSqlQuery>
#include <QDate>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QFileDialog>

class PrintManager : public QObject
{
    Q_OBJECT
public:
    explicit PrintManager(QObject *parent = nullptr);

    // Print to Konica Minolta (full page laser printer)
    bool printDocument(QWidget *parent, const QString &html, const QString &title);
    bool printPreview(QWidget *parent, const QString &html, const QString &title);
    bool exportToPDF(QWidget *parent, const QString &html, const QString &title);

    // Print to thermal printer (80mm)
    bool printThermal(QWidget *parent, const QString &content, const QString &printerName = QString());
    bool printThermalReceipt(QWidget *parent, const QStringList &lines, const QString &printerName = QString());

    // Report HTML generators
    static QString generateSalesInvoiceHtml(int invoiceId, const QString &companyName, double exchangeRate);
    static QString generatePurchaseInvoiceHtml(int invoiceId, const QString &companyName);
    static QString generateCashReceiptHtml(int transId, const QString &companyName);
    static QString generateDailyReportHtml(const QDate &from, const QDate &to, bool detailed);
    static QString generateCustomerStatementHtml(int customerId, const QDate &from, const QDate &to);
    static QString generateInventoryReportHtml(const QDate &upTo, const QString &groupBy);
    static QString generateLatePaymentsHtml(int days, const QString &region);
    static QString generateTotalSalesHtml(const QDate &from, const QDate &to, int customerId, const QString &groupBy);
    static QString generateAllCustomersHtml(const QString &region, const QString &type);
    static QString generateOverallBalanceHtml(const QString &region, const QString &type);

    static QString arabicHtmlHeader();

    void setKonicaMinoltaPrinter(const QString &printerName);
    void setThermalPrinter(const QString &printerName);

private:
    QString m_konicaPrinter;
    QString m_thermalPrinter;

    void renderHtml(QPainter &painter, QPrinter &printer, const QString &html);
};

#endif // PRINTMANAGER_H
