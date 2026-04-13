#include "printmanager.h"
#include "database.h"
#include <QTextDocument>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QPageLayout>
#include <QPageSize>

PrintManager::PrintManager(QObject *parent) : QObject(parent) {}

void PrintManager::setKonicaMinoltaPrinter(const QString &printerName) { m_konicaPrinter = printerName; }
void PrintManager::setThermalPrinter(const QString &printerName)       { m_thermalPrinter = printerName; }

// ══════════════════════════════════════════════════════════════════════════════
//  CORE RENDER HELPER
//
//  ROOT CAUSE OF THE WIDTH BUG:
//    QTextDocument needs BOTH setPageSize() AND setTextWidth() set to the
//    printer's actual paint rect — using only setTextWidth() is not enough
//    because Qt uses the page size to paginate, and if it doesn't match the
//    printer's paint rect the document renders narrower than the paper.
// ══════════════════════════════════════════════════════════════════════════════
static void renderHtmlToPrinter(const QString &html, QPrinter *p)
{
    QRectF paintRect = p->pageLayout().paintRectPixels(p->resolution());

    QTextDocument doc;
    doc.setDefaultFont(QFont("Tahoma", 10));
    doc.setPageSize(paintRect.size());     // ← FIX: must match printer paint rect
    doc.setTextWidth(paintRect.width());   // ← FIX: constrain text to exact width
    doc.setHtml(html);
    doc.print(p);
}

// ══════════════════════════════════════════════════════════════════════════════
//  SHARED CSS  — Qt QTextDocument HTML subset ONLY:
//   ✓ block styles (font, color, background-color, border, padding, margin)
//   ✓ table / th / td / tr
//   ✓ text-align, width on table cells
//   ✗ flexbox, grid, display:table-cell  ← NOT supported, never use these
//   ✗ CSS direction:rtl on <table>       ← unreliable; use dir='rtl' attribute
//   ✗ box-sizing, calc(), variables      ← not supported
//
//  RTL RULE: put  dir='rtl'  as an HTML attribute on every <table> tag.
//            The CSS direction property works on <body> but is unreliable
//            on tables in Qt's renderer.
// ══════════════════════════════════════════════════════════════════════════════
QString PrintManager::arabicHtmlHeader()
{
    return
        "<!DOCTYPE html>"
        "<html dir='rtl'>"
        "<head><meta charset='utf-8'/>"
        "<style>"
        "body  { font-family:Tahoma,Arial; font-size:10pt; margin:0; padding:6px; }"
        ".co   { font-size:16pt; font-weight:bold; color:#003366; text-align:center;"
        "        border-bottom:2px solid #003366; padding-bottom:6px; margin-bottom:8px; }"
        ".title{ font-size:13pt; font-weight:bold; color:#003366; text-align:center; margin:4px 0 8px 0; }"
        /* meta table — no borders, used for date/customer header info */
        "table.meta    { width:100%; border:none; margin-bottom:6px; }"
        "table.meta td { border:none; padding:2px 4px; font-size:10pt; }"
        /* main data table */
        "table.data    { width:100%; border-collapse:collapse; margin-top:4px; }"
        "table.data th { background-color:#003366; color:#ffffff; border:1px solid #003366;"
        "                padding:5px 3px; font-size:9pt; text-align:center; }"
        "table.data td { border:1px solid #cccccc; padding:4px 3px; font-size:9pt; text-align:center; }"
        "tr.tot td     { background-color:#ffd700; font-weight:bold; border:1px solid #aaaaaa; }"
        /* info box for receipts — uses a real table internally */
        "table.info    { width:100%; border:1px solid #003366; border-collapse:collapse; margin:8px 0; }"
        "table.info td { padding:5px 8px; font-size:10pt; border:none; }"
        ".lbl  { font-weight:bold; color:#003366; }"
        ".sig  { margin-top:28px; font-size:10pt; text-align:left; }"
        "</style></head><body>";
}

// ══════════════════════════════════════════════════════════════════════════════
//  printDocument
// ══════════════════════════════════════════════════════════════════════════════
bool PrintManager::printDocument(QWidget *parent, const QString &html, const QString &title)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15,15,15,15), QPageLayout::Millimeter);
    if (!m_konicaPrinter.isEmpty()) printer.setPrinterName(m_konicaPrinter);

    QPrintDialog dlg(&printer, parent);
    dlg.setWindowTitle(title);
    if (dlg.exec() != QDialog::Accepted) return false;
    renderHtmlToPrinter(html, &printer);
    return true;
}

// ══════════════════════════════════════════════════════════════════════════════
//  printPreview
// ══════════════════════════════════════════════════════════════════════════════
bool PrintManager::printPreview(QWidget *parent, const QString &html, const QString &title)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15,15,15,15), QPageLayout::Millimeter);
    if (!m_konicaPrinter.isEmpty()) printer.setPrinterName(m_konicaPrinter);

    QPrintPreviewDialog preview(&printer, parent);
    preview.setWindowTitle(title);
    QObject::connect(&preview, &QPrintPreviewDialog::paintRequested,
                     [html](QPrinter *p){ renderHtmlToPrinter(html, p); });
    preview.exec();
    return true;
}

// ══════════════════════════════════════════════════════════════════════════════
//  exportToPDF
// ══════════════════════════════════════════════════════════════════════════════
bool PrintManager::exportToPDF(QWidget *parent, const QString &html, const QString &title)
{
    QString fileName = QFileDialog::getSaveFileName(
        parent, QString::fromUtf8("حفظ كـ PDF"), title+".pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return false;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15,15,15,15), QPageLayout::Millimeter);
    renderHtmlToPrinter(html, &printer);

    QMessageBox::information(parent, QString::fromUtf8("تم"),
                             QString::fromUtf8("تم حفظ الملف:\n") + fileName);
    return true;
}

// ══════════════════════════════════════════════════════════════════════════════
//  THERMAL — 80 mm roll
// ══════════════════════════════════════════════════════════════════════════════
static QString thermalHtmlHeader()
{
    return
        "<!DOCTYPE html>"
        "<html dir='rtl'>"
        "<head><meta charset='utf-8'/>"
        "<style>"
        "body          { font-family:'Courier New',Tahoma; font-size:9pt; margin:0; padding:2px 3px; }"
        ".tt           { font-size:11pt; font-weight:bold; text-align:center; margin:3px 0; }"
        ".sub          { font-size:8pt; text-align:center; margin:1px 0; }"
        "hr            { border:none; border-top:1px dashed #000; margin:3px 0; }"
        "p             { margin:1px 0; font-size:9pt; }"
        "table.row     { width:100%; border:none; }"
        "table.row td  { border:none; padding:1px 0; font-size:9pt; }"
        "td.lv         { text-align:right; }"
        "td.rv         { text-align:left; }"
        "table.items   { width:100%; border-collapse:collapse; font-size:8pt; }"
        "table.items th{ border-top:1px solid #000; border-bottom:1px solid #000; padding:2px 1px; text-align:center; }"
        "table.items td{ padding:2px 1px; text-align:center; }"
        ".tot          { font-weight:bold; font-size:10pt; text-align:center; border-top:1px solid #000; padding-top:3px; margin:3px 0; }"
        ".ft           { font-size:8pt; text-align:center; margin-top:5px; }"
        "</style></head><body>";
}

bool PrintManager::printThermal(QWidget *parent, const QString &content, const QString &printerName)
{
    QPrinter printer;
    QPageSize thermalPage(QSizeF(80, 297), QPageSize::Millimeter, "Thermal80");
    printer.setPageSize(thermalPage);
    printer.setPageMargins(QMarginsF(2,2,2,2), QPageLayout::Millimeter);
    printer.setFullPage(false);

    QString pName = printerName.isEmpty() ? m_thermalPrinter : printerName;
    if (!pName.isEmpty()) printer.setPrinterName(pName);
    else {
        QPrintDialog dlg(&printer, parent);
        if (dlg.exec() != QDialog::Accepted) return false;
    }

    QString fullHtml = thermalHtmlHeader() + content + "</body></html>";

    QRectF pr = printer.pageLayout().paintRectPixels(printer.resolution());
    QTextDocument doc;
    doc.setDefaultFont(QFont("Tahoma", 9));
    doc.setPageSize(pr.size());
    doc.setTextWidth(pr.width());
    doc.setHtml(fullHtml);
    doc.print(&printer);
    return true;
}

// Supported line prefixes:
//   **text     → bold centred store title
//   ##text     → small centred subtitle
//   ---        → dashed separator
//   >>lbl|val  → two-column row
//   ==text     → total line (large, bold)
//   ~~text     → footer (small, centred)
//   (other)    → plain paragraph
bool PrintManager::printThermalReceipt(QWidget *parent,
                                        const QStringList &lines,
                                        const QString &printerName)
{
    QString content;
    for (const QString &raw : lines) {
        if (raw == "---") {
            content += "<hr/>";
        } else if (raw.startsWith("**")) {
            content += "<p class='tt'>"  + raw.mid(2) + "</p>";
        } else if (raw.startsWith("##")) {
            content += "<p class='sub'>" + raw.mid(2) + "</p>";
        } else if (raw.startsWith(">>")) {
            QStringList p = raw.mid(2).split('|');
            content += "<table class='row' dir='rtl'><tr>"
                       "<td class='lv'>" + p.value(0) + "</td>"
                       "<td class='rv'>" + p.value(1) + "</td>"
                       "</tr></table>";
        } else if (raw.startsWith("==")) {
            content += "<p class='tot'>" + raw.mid(2) + "</p>";
        } else if (raw.startsWith("~~")) {
            content += "<p class='ft'>"  + raw.mid(2) + "</p>";
        } else {
            content += "<p>" + raw + "</p>";
        }
    }
    return printThermal(parent, content, printerName);
}

// ══════════════════════════════════════════════════════════════════════════════
//  REPORT HTML GENERATORS
//  Pattern used in every report:
//    1.  arabicHtmlHeader()           — shared CSS
//    2.  <div class='co'>…</div>      — company name banner
//    3.  <div class='title'>…</div>   — report title
//    4.  <table class='meta' dir='rtl'>…</table>   — date/customer header
//    5.  <table class='data' dir='rtl'>…</table>   — data rows
//
//  The  dir='rtl'  HTML attribute on each <table> is mandatory for Qt.
//  Column widths are set as % directly on <th> — Qt honours these.
// ══════════════════════════════════════════════════════════════════════════════

// ── Sales Invoice ──────────────────────────────────────────────────────────
QString PrintManager::generateSalesInvoiceHtml(int invoiceId,
                                                const QString &companyName,
                                                double exchangeRate)
{
    QSqlQuery qi;
    qi.prepare("SELECT si.id,si.date,c.name,si.payment_type,si.currency,"
               "si.total_dollar,si.total_dinar,si.notes "
               "FROM sales_invoices si LEFT JOIN customers c ON si.customer_id=c.id WHERE si.id=?");
    qi.addBindValue(invoiceId); qi.exec(); qi.next();

    QSqlQuery qit;
    qit.prepare("SELECT p.name,sit.qty,sit.unit_price_dollar,sit.unit_price_dinar,"
                "sit.total_dollar,sit.total_dinar "
                "FROM sales_items sit LEFT JOIN products p ON sit.product_id=p.id WHERE sit.invoice_id=?");
    qit.addBindValue(invoiceId); qit.exec();

    QString h = arabicHtmlHeader();
    h += "<div class='co'>" + companyName + "</div>";
    h += "<div class='title'>فاتورة مبيعات رقم: " + QString::number(invoiceId) + "</div>";

    h += "<table class='meta' dir='rtl'>"
         "<tr><td><span class='lbl'>الزبون:</span> "     + qi.value(2).toString() + "</td>"
         "    <td><span class='lbl'>التاريخ:</span> "    + qi.value(1).toString() + "</td></tr>"
         "<tr><td><span class='lbl'>نوع الدفع:</span> "  + qi.value(3).toString() + "</td>"
         "    <td><span class='lbl'>رقم القائمة:</span> "+ QString::number(invoiceId) + "</td></tr>"
         "</table>";

    QString invCurrency = qi.value(4).toString();
    bool isDinar = (invCurrency != "$");

    if (isDinar) {
        h += "<table class='data' dir='rtl'>"
             "<tr>"
             "<th width='4%'>#</th>"
             "<th width='45%'>اسم المادة</th>"
             "<th width='15%'>الكمية</th>"
             "<th width='17%'>السعر (د)</th>"
             "<th width='19%'>المجموع (د)</th>"
             "</tr>";
    } else {
        h += "<table class='data' dir='rtl'>"
             "<tr>"
             "<th width='4%'>#</th>"
             "<th width='45%'>اسم المادة</th>"
             "<th width='15%'>الكمية</th>"
             "<th width='17%'>السعر ($)</th>"
             "<th width='19%'>المجموع ($)</th>"
             "</tr>";
    }

    int row=1; double totD=0, totDin=0;
    while (qit.next()) {
        double unitPrice = isDinar ? qit.value(3).toDouble() : qit.value(2).toDouble();
        double lineTotal = isDinar ? qit.value(5).toDouble() : qit.value(4).toDouble();
        QString fmt      = isDinar ? QString::number(unitPrice,'f',0) : QString::number(unitPrice,'f',2);
        QString fmtTot   = isDinar ? QString::number(lineTotal,'f',0) : QString::number(lineTotal,'f',2);
        h += "<tr>"
             "<td>" + QString::number(row++) + "</td>"
             "<td>" + qit.value(0).toString() + "</td>"
             "<td>" + QString::number(qit.value(1).toDouble()) + "</td>"
             "<td>" + fmt    + "</td>"
             "<td>" + fmtTot + "</td>"
             "</tr>";
        totD   += qit.value(4).toDouble();
        totDin += qit.value(5).toDouble();
    }
    if (isDinar) {
        h += "<tr class='tot'>"
             "<td colspan='4'>الإجمالي</td>"
             "<td>" + QString::number(totDin,'f',0) + " د</td>"
             "</tr></table>";
        h += "<p>المبلغ الإجمالي: " + QString::number(totDin,'f',0) + QString::fromUtf8(" دينار") + "</p>";
    } else {
        h += "<tr class='tot'>"
             "<td colspan='4'>الإجمالي</td>"
             "<td>" + QString::number(totD,'f',2) + " $</td>"
             "</tr></table>";
        h += "<p>سعر الصرف: " + QString::number(exchangeRate,'f',0) + " دينار / دولار</p>";
    }
    h += "<div class='sig'>التوقيع: _______________</div>";
    h += "</body></html>";
    return h;
}

// ── Daily Report ───────────────────────────────────────────────────────────
QString PrintManager::generateDailyReportHtml(const QDate &from, const QDate &to, bool)
{
    QSqlQuery q = Database::getDailyReport(from, to, false);

    QString h = arabicHtmlHeader();
    h += "<div class='co'>"    + Database::getCompanyName() + "</div>";
    h += "<div class='title'>التقرير اليومي</div>";

    h += "<table class='meta' dir='rtl'><tr>"
         "<td><span class='lbl'>الفترة من:</span> " + from.toString("dd/MM/yyyy") + "</td>"
         "<td><span class='lbl'>إلى:</span> "       + to.toString("dd/MM/yyyy")   + "</td>"
         "</tr></table>";

    h += "<table class='data' dir='rtl'>"
         "<tr>"
         "<th width='5%'>#</th>"
         "<th width='16%'>التاريخ</th>"
         "<th width='34%'>الزبون</th>"
         "<th width='20%'>نوع الدفع</th>"
         "<th width='13%'>المجموع $</th>"
         "<th width='12%'>المجموع د</th>"
         "</tr>";

    int row=1; double totD=0, totDin=0;
    while (q.next()) {
        h += "<tr>"
             "<td>" + QString::number(row++) + "</td>"
             "<td>" + q.value(1).toString() + "</td>"
             "<td>" + q.value(2).toString() + "</td>"
             "<td>" + q.value(3).toString() + "</td>"
             "<td>" + QString::number(q.value(4).toDouble(),'f',2) + "</td>"
             "<td>" + QString::number(q.value(5).toDouble(),'f',0) + "</td>"
             "</tr>";
        totD   += q.value(4).toDouble();
        totDin += q.value(5).toDouble();
    }
    h += "<tr class='tot'>"
         "<td colspan='4'>الإجمالي</td>"
         "<td>" + QString::number(totD,'f',2) + " $</td>"
         "<td>" + QString::number(totDin,'f',0) + " د</td>"
         "</tr></table></body></html>";
    return h;
}

// ── Customer Statement ─────────────────────────────────────────────────────
QString PrintManager::generateCustomerStatementHtml(int customerId,
                                                      const QDate &from, const QDate &to)
{
    QSqlQuery cq;
    cq.prepare("SELECT name,balance_dollar,balance_dinar FROM customers WHERE id=?");
    cq.addBindValue(customerId); cq.exec(); cq.next();

    QSqlQuery q = Database::getCustomerStatement(customerId, from, to);

    QString h = arabicHtmlHeader();
    h += "<div class='co'>" + Database::getCompanyName() + "</div>";
    h += "<div class='title'>كشف حساب زبون: " + cq.value(0).toString() + "</div>";

    h += "<table class='meta' dir='rtl'><tr>"
         "<td><span class='lbl'>من:</span> " + from.toString("dd/MM/yyyy") + "</td>"
         "<td><span class='lbl'>إلى:</span> " + to.toString("dd/MM/yyyy") + "</td>"
         "</tr></table>";

    h += "<table class='data' dir='rtl'>"
         "<tr>"
         "<th width='14%'>التاريخ</th>"
         "<th width='24%'>البيان</th>"
         "<th width='15%'>مدين $</th>"
         "<th width='15%'>دائن $</th>"
         "<th width='16%'>مدين د</th>"
         "<th width='16%'>دائن د</th>"
         "</tr>";

    while (q.next()) {
        QString type = q.value(0).toString()=="sale"
                       ? QString::fromUtf8("بيع") : QString::fromUtf8("دفع");
        h += "<tr>"
             "<td>" + q.value(1).toString() + "</td>"
             "<td>" + type + " #" + QString::number(q.value(2).toInt()) + "</td>"
             "<td>" + QString::number(q.value(3).toDouble(),'f',2) + "</td>"
             "<td>" + QString::number(q.value(4).toDouble(),'f',2) + "</td>"
             "<td>" + QString::number(q.value(5).toDouble(),'f',0) + "</td>"
             "<td>" + QString::number(q.value(6).toDouble(),'f',0) + "</td>"
             "</tr>";
    }
    h += "<tr class='tot'>"
         "<td colspan='2'>الرصيد الحالي</td>"
         "<td colspan='2'>" + QString::number(cq.value(1).toDouble(),'f',2) + " $</td>"
         "<td colspan='2'>" + QString::number(cq.value(2).toDouble(),'f',0) + " د</td>"
         "</tr></table></body></html>";
    return h;
}

// ── Inventory Report ───────────────────────────────────────────────────────
QString PrintManager::generateInventoryReportHtml(const QDate &upTo, const QString &groupBy)
{
    QSqlQuery q = Database::getInventoryReport(upTo, groupBy, true);

    QString h = arabicHtmlHeader();
    h += "<div class='co'>" + Database::getCompanyName() + "</div>";
    h += "<div class='title'>جرد أرصدة المواد</div>";
    h += "<p style='text-align:center;'>حتى تاريخ: " + upTo.toString("dd/MM/yyyy") + "</p>";

    h += "<table class='data' dir='rtl'>"
         "<tr>"
         "<th width='5%'>#</th>"
         "<th width='18%'>الباركود</th>"
         "<th width='30%'>اسم المادة</th>"
         "<th width='17%'>المجموعة</th>"
         "<th width='10%'>الرصيد</th>"
         "<th width='10%'>الكلفة</th>"
         "<th width='10%'>البيع $</th>"
         "</tr>";

    int row=1;
    while (q.next()) {
        h += "<tr>"
             "<td>" + QString::number(row++) + "</td>"
             "<td>" + q.value(1).toString() + "</td>"
             "<td>" + q.value(2).toString() + "</td>"
             "<td>" + q.value(3).toString() + "</td>"
             "<td>" + QString::number(q.value(5).toDouble(),'f',2) + "</td>"
             "<td>" + QString::number(q.value(7).toDouble(),'f',2) + "</td>"
             "<td>" + QString::number(q.value(8).toDouble(),'f',2) + "</td>"
             "</tr>";
    }
    h += "</table></body></html>";
    return h;
}

// ── Late Payments ──────────────────────────────────────────────────────────
QString PrintManager::generateLatePaymentsHtml(int days, const QString &region)
{
    QSqlQuery q = Database::getLatePayments(days, region);

    QString h = arabicHtmlHeader();
    h += "<div class='co'>" + Database::getCompanyName() + "</div>";
    h += "<div class='title'>كشف المتأخرين عن التسديد</div>";

    h += "<table class='data' dir='rtl'>"
         "<tr>"
         "<th width='5%'>#</th>"
         "<th width='30%'>اسم الزبون</th>"
         "<th width='20%'>المنطقة</th>"
         "<th width='20%'>الهاتف</th>"
         "<th width='13%'>الرصيد $</th>"
         "<th width='12%'>الرصيد د</th>"
         "</tr>";

    int row=1; double totD=0, totDin=0;
    while (q.next()) {
        h += "<tr>"
             "<td>" + QString::number(row++) + "</td>"
             "<td>" + q.value(1).toString() + "</td>"
             "<td>" + q.value(2).toString() + "</td>"
             "<td>" + q.value(3).toString() + "</td>"
             "<td>" + QString::number(q.value(4).toDouble(),'f',2) + "</td>"
             "<td>" + QString::number(q.value(5).toDouble(),'f',0) + "</td>"
             "</tr>";
        totD   += q.value(4).toDouble();
        totDin += q.value(5).toDouble();
    }
    h += "<tr class='tot'>"
         "<td colspan='4'>الإجمالي</td>"
         "<td>" + QString::number(totD,'f',2)   + " $</td>"
         "<td>" + QString::number(totDin,'f',0) + " د</td>"
         "</tr></table></body></html>";
    return h;
}

// ── All Customers ──────────────────────────────────────────────────────────
QString PrintManager::generateAllCustomersHtml(const QString &region, const QString &type)
{
    QSqlQuery q = Database::getAllCustomers(region, type);

    QString h = arabicHtmlHeader();
    h += "<div class='co'>" + Database::getCompanyName() + "</div>";
    h += "<div class='title'>تقرير جميع أسماء الزبائن</div>";

    h += "<table class='data' dir='rtl'>"
         "<tr>"
         "<th width='5%'>#</th>"
         "<th width='25%'>الاسم</th>"
         "<th width='17%'>المنطقة</th>"
         "<th width='15%'>الهاتف</th>"
         "<th width='20%'>العنوان</th>"
         "<th width='9%'>الرصيد $</th>"
         "<th width='9%'>الرصيد د</th>"
         "</tr>";

    int row=1;
    while (q.next()) {
        h += "<tr>"
             "<td>" + QString::number(row++) + "</td>"
             "<td>" + q.value(1).toString() + "</td>"
             "<td>" + q.value(2).toString() + "</td>"
             "<td>" + q.value(3).toString() + "</td>"
             "<td>" + q.value(4).toString() + "</td>"
             "<td>" + QString::number(q.value(5).toDouble(),'f',2) + "</td>"
             "<td>" + QString::number(q.value(6).toDouble(),'f',0) + "</td>"
             "</tr>";
    }
    h += "</table></body></html>";
    return h;
}

// ── Overall Balance ────────────────────────────────────────────────────────
QString PrintManager::generateOverallBalanceHtml(const QString &region, const QString &type)
{
    QSqlQuery q = Database::getOverallCustomerBalance(region, type, 0);

    QString h = arabicHtmlHeader();
    h += "<div class='co'>" + Database::getCompanyName() + "</div>";
    h += "<div class='title'>الرصيد الإجمالي للزبائن دائن ومدين</div>";

    h += "<table class='data' dir='rtl'>"
         "<tr>"
         "<th width='5%'>#</th>"
         "<th width='42%'>الاسم</th>"
         "<th width='23%'>المنطقة</th>"
         "<th width='15%'>الرصيد $</th>"
         "<th width='15%'>الرصيد د</th>"
         "</tr>";

    int row=1; double totD=0, totDin=0;
    while (q.next()) {
        h += "<tr>"
             "<td>" + QString::number(row++) + "</td>"
             "<td>" + q.value(0).toString() + "</td>"
             "<td>" + q.value(1).toString() + "</td>"
             "<td>" + QString::number(q.value(2).toDouble(),'f',2) + "</td>"
             "<td>" + QString::number(q.value(3).toDouble(),'f',0) + "</td>"
             "</tr>";
        totD   += q.value(2).toDouble();
        totDin += q.value(3).toDouble();
    }
    h += "<tr class='tot'>"
         "<td colspan='3'>الإجمالي</td>"
         "<td>" + QString::number(totD,'f',2)   + " $</td>"
         "<td>" + QString::number(totDin,'f',0) + " د</td>"
         "</tr></table></body></html>";
    return h;
}

// ── Purchase Invoice ───────────────────────────────────────────────────────
QString PrintManager::generatePurchaseInvoiceHtml(int invoiceId, const QString &companyName)
{
    QSqlQuery qi;
    qi.prepare("SELECT pi.id,pi.date,c.name,pi.purchase_type,pi.payment_type,"
               "pi.currency,pi.exchange_rate,pi.notes "
               "FROM purchase_invoices pi "
               "LEFT JOIN customers c ON pi.supplier_id=c.id WHERE pi.id=?");
    qi.addBindValue(invoiceId); qi.exec(); qi.next();

    QSqlQuery qit;
    qit.prepare("SELECT p.name,pit.qty,pit.cost_price,pit.wholesale_dollar,pit.retail_dollar "
                "FROM purchase_items pit "
                "LEFT JOIN products p ON pit.product_id=p.id WHERE pit.invoice_id=?");
    qit.addBindValue(invoiceId); qit.exec();

    QString h = arabicHtmlHeader();
    h += "<div class='co'>" + companyName + "</div>";
    h += "<div class='title'>فاتورة شراء رقم: " + QString::number(invoiceId) + "</div>";

    h += "<table class='meta' dir='rtl'>"
         "<tr><td><span class='lbl'>المجهز:</span> "    + qi.value(2).toString() + "</td>"
         "    <td><span class='lbl'>التاريخ:</span> "   + qi.value(1).toString() + "</td></tr>"
         "<tr><td><span class='lbl'>نوع الدفع:</span> " + qi.value(4).toString() + "</td>"
         "    <td><span class='lbl'>نوع الشراء:</span> "+ qi.value(3).toString() + "</td></tr>"
         "</table>";

    h += "<table class='data' dir='rtl'>"
         "<tr>"
         "<th width='5%'>#</th>"
         "<th width='37%'>اسم المادة</th>"
         "<th width='12%'>الكمية</th>"
         "<th width='15%'>كلفة $</th>"
         "<th width='16%'>جملة $</th>"
         "<th width='15%'>مفرد $</th>"
         "</tr>";

    int row=1; double totalCost=0;
    while (qit.next()) {
        double qty=qit.value(1).toDouble(), cost=qit.value(2).toDouble();
        totalCost += qty*cost;
        h += "<tr>"
             "<td>" + QString::number(row++) + "</td>"
             "<td>" + qit.value(0).toString() + "</td>"
             "<td>" + QString::number(qty) + "</td>"
             "<td>" + QString::number(cost,'f',3) + "</td>"
             "<td>" + QString::number(qit.value(3).toDouble(),'f',3) + "</td>"
             "<td>" + QString::number(qit.value(4).toDouble(),'f',3) + "</td>"
             "</tr>";
    }
    h += "<tr class='tot'>"
         "<td colspan='4'>إجمالي الكلفة</td>"
         "<td colspan='2'>" + QString::number(totalCost,'f',2) + " $</td>"
         "</tr></table></body></html>";
    return h;
}

// ── Cash Receipt ───────────────────────────────────────────────────────────
QString PrintManager::generateCashReceiptHtml(int transId, const QString &companyName)
{
    QSqlQuery q;
    q.prepare("SELECT type,date,time,operator_id,doc_no,main_account,sub_account,"
              "amount_dollar,amount_dinar,exchange_rate,direction,notes "
              "FROM cash_transactions WHERE id=?");
    q.addBindValue(transId); q.exec();
    if (!q.next()) return QString();

    static const QStringList typeNames = {
        QString::fromUtf8("صرف مصاريف عامة"),
        QString::fromUtf8("صرف ذمم عملاء"),
        QString::fromUtf8("قبض من عملاء"),
        QString::fromUtf8("إيداع في الصندوق"),
        QString::fromUtf8("سحب من الصندوق")
    };
    int idx = q.value(0).toInt();
    QString typeName = (idx>=0 && idx<typeNames.size()) ? typeNames[idx] : "";

    QString h = arabicHtmlHeader();
    h += "<div class='co'>" + companyName + "</div>";
    h += "<div class='title'>وصل صندوق رقم: " + QString::number(transId) + "</div>";

    // Info box via real <table> — layout-safe in Qt renderer
    h += "<table class='info' dir='rtl'>"
         "<tr><td colspan='2'><span class='lbl'>النوع:</span> " + typeName + "</td></tr>"
         "<tr>"
         "<td><span class='lbl'>التاريخ:</span> " + q.value(1).toString() + "</td>"
         "<td><span class='lbl'>الوقت:</span> "   + q.value(2).toString() + "</td>"
         "</tr>"
         "<tr>"
         "<td><span class='lbl'>الحساب الرئيسي:</span> " + q.value(5).toString() + "</td>"
         "<td><span class='lbl'>الحساب الفرعي:</span> "  + q.value(6).toString() + "</td>"
         "</tr>"
         "<tr>"
         "<td><span class='lbl'>المبلغ بالدولار $:</span> " +
             QString::number(q.value(7).toDouble(),'f',2) + "</td>"
         "<td><span class='lbl'>المبلغ بالدينار د:</span> " +
             QString::number(q.value(8).toDouble(),'f',0) + "</td>"
         "</tr>"
         "<tr><td colspan='2'><span class='lbl'>سعر الصرف:</span> " +
             QString::number(q.value(9).toDouble(),'f',0) + "</td></tr>";

    if (!q.value(11).toString().isEmpty())
        h += "<tr><td colspan='2'><span class='lbl'>الملاحظات:</span> " +
             q.value(11).toString() + "</td></tr>";

    h += "</table>";
    h += "<div class='sig'>التوقيع: _______________</div>";
    h += "</body></html>";
    return h;
}

// ── Total Sales ────────────────────────────────────────────────────────────
QString PrintManager::generateTotalSalesHtml(const QDate &from, const QDate &to,
                                              int customerId, const QString &groupBy)
{
    QSqlQuery q = Database::getTotalSalesReport(from, to, customerId, 0, groupBy);

    QString h = arabicHtmlHeader();
    h += "<div class='co'>" + Database::getCompanyName() + "</div>";
    h += "<div class='title'>مجموع المبيعات</div>";

    h += "<table class='meta' dir='rtl'><tr>"
         "<td><span class='lbl'>من:</span> " + from.toString("dd/MM/yyyy") + "</td>"
         "<td><span class='lbl'>إلى:</span> " + to.toString("dd/MM/yyyy") + "</td>"
         "</tr></table>";

    if (groupBy == "customer" || groupBy == "product") {
        h += "<table class='data' dir='rtl'>"
             "<tr>"
             "<th width='60%'>الاسم</th>"
             "<th width='20%'>الإجمالي $</th>"
             "<th width='20%'>الإجمالي د</th>"
             "</tr>";
        double grand=0;
        while (q.next()) {
            h += "<tr>"
                 "<td>" + q.value(0).toString() + "</td>"
                 "<td>" + QString::number(q.value(1).toDouble(),'f',2) + "</td>"
                 "<td>" + QString::number(q.value(2).toDouble(),'f',0) + "</td>"
                 "</tr>";
            grand += q.value(1).toDouble();
        }
        h += "<tr class='tot'><td>الإجمالي</td>"
             "<td>" + QString::number(grand,'f',2) + " $</td><td></td></tr>";
    } else {
        h += "<table class='data' dir='rtl'>"
             "<tr>"
             "<th width='5%'>#</th>"
             "<th width='17%'>التاريخ</th>"
             "<th width='38%'>الزبون</th>"
             "<th width='20%'>الإجمالي $</th>"
             "<th width='20%'>الإجمالي د</th>"
             "</tr>";
        int row=1; double grand=0;
        while (q.next()) {
            h += "<tr>"
                 "<td>" + QString::number(row++) + "</td>"
                 "<td>" + q.value(1).toString() + "</td>"
                 "<td>" + q.value(2).toString() + "</td>"
                 "<td>" + QString::number(q.value(3).toDouble(),'f',2) + "</td>"
                 "<td>" + QString::number(q.value(4).toDouble(),'f',0) + "</td>"
                 "</tr>";
            grand += q.value(3).toDouble();
        }
        h += "<tr class='tot'>"
             "<td colspan='3'>الإجمالي</td>"
             "<td>" + QString::number(grand,'f',2) + " $</td><td></td></tr>";
    }
    h += "</table></body></html>";
    return h;
}

// ── Legacy API ─────────────────────────────────────────────────────────────
void PrintManager::renderHtml(QPainter &painter, QPrinter &printer, const QString &html)
{
    Q_UNUSED(painter)
    renderHtmlToPrinter(html, &printer);
}
