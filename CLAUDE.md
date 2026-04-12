# CLAUDE.md — ReportSystem

This file provides guidance for AI assistants working in this codebase.

---

## Project Overview

**ReportSystem** is a Windows desktop accounting and point-of-sale application built with C++/Qt5. It is an offline-first, single-executable system that manages sales, purchases, inventory, cash transactions, and financial reporting for a small business. The UI is fully Arabic (RTL), targeting Iraqi businesses with dual-currency support (USD and IQD).

- **Version:** 1.0.0.0 — Company: Amir Systems
- **Target Platform:** Windows 7–11 (64-bit)
- **Language/Framework:** C++11, Qt 5.15.2
- **Build System:** qmake
- **Database:** SQLite 3.x (embedded, single-file `data.db`)
- **UI Language:** Arabic (Right-to-Left)

---

## Repository Structure

```
ReportSystem/
├── src/                    C++ source and header files (15 modules)
├── forms/                  Qt Designer UI definition files (14 .ui XML files)
├── resources/              Qt resource files (images, icons via .qrc)
├── release/                Compiled output (binaries, object files, data.db)
├── ReportSystem.pro        qmake project configuration
├── deploy_windows.bat      Windows deployment packaging script
├── Makefile*               Generated build files (do not edit manually)
├── README.md               Bilingual (Arabic/English) user documentation
└── CLAUDE.md               This file
```

### Source Modules (`src/`)

| File | Purpose |
|------|---------|
| `main.cpp` | Application entry point; `LoginDialog` class, DB init, Arabic locale setup |
| `mainwindow.h/.cpp` | Main dashboard with navigation to all modules |
| `database.h/.cpp` | **Central data layer** — all SQL operations as static methods |
| `saleswindow.h/.cpp` | Sales invoice entry, viewing, printing (largest file: ~1013 lines) |
| `purchasewindow.h/.cpp` | Purchase invoice entry from local/import suppliers |
| `cashboxwindow.h/.cpp` | Cash receive/pay operations with dual-currency equivalence |
| `reportswindow.h/.cpp` | Report category menu/router |
| `systemsettings.h/.cpp` | Exchange rate, customer and product CRUD settings UI |
| `printmanager.h/.cpp` | Centralized HTML-to-printer/PDF engine |
| `arabichelper.h/.cpp` | Static utility for RTL layout and Arabic locale setup |
| `dailyreport.h/.cpp` | Sales summary by date range and operator |
| `customerstatement.h/.cpp` | Per-customer transaction history |
| `customerbalances.h/.cpp` | All customer balances with filters |
| `inventoryreport.h/.cpp` | Stock levels by group/type |
| `latedpayments.h/.cpp` | Overdue accounts |
| `totalsalesreport.h/.cpp` | Aggregated sales by customer/product/group |
| `allcustomersreport.h/.cpp` | Customer directory with filtering |
| `overallbalancereport.h/.cpp` | Consolidated customer balances |

---

## Architecture

### Pattern
MVC-style separation with no explicit controller layer:
- **Model:** `Database` class (static methods, SQLite via Qt's QSQLITE driver)
- **View/Controller:** QMainWindow and QDialog subclasses, one per feature area
- **Print Engine:** `PrintManager` (generates HTML → QTextDocument → QPrinter)

### Key Design Decisions
- **Static Database class:** All data access goes through `Database::methodName()`. No ORM, no repository pattern — direct SQL with Qt's `QSqlQuery`.
- **No network layer:** This is a fully offline desktop app. There are no HTTP endpoints, REST APIs, or sockets.
- **Parent-based memory management:** Qt's parent-child ownership handles all widget lifetimes. No manual `delete` needed for widgets parented to a window.
- **Dual-currency core:** Every monetary value has both a USD (`_dollar`) and IQD (`_dinar`) column. Exchange rate stored in `settings` table (default: 1450 IQD/USD).

---

## Database Schema

SQLite database auto-created as `data.db` beside the executable on first run via `Database::init()`.

### Tables

```sql
settings          -- key/value config (exchange_rate, company_name)
operators         -- user accounts (id, name, username UNIQUE, password, active)
customers         -- customers & suppliers (name, region, balance_dollar, balance_dinar, type)
products          -- catalog (barcode UNIQUE, name, group, type, cost/wholesale/retail prices, stock)
sales_invoices    -- sale transaction headers (customer_id FK, date, payment_type, currency, totals)
sales_items       -- sale line items (invoice_id FK, product_id FK, qty, unit_price_*)
purchase_invoices -- purchase headers (supplier_id FK, date, purchase_type local/import)
purchase_items    -- purchase line items with cost and pricing tiers
cash_transactions -- cash box entries (type, amounts dual-currency, main/sub accounts, direction)
```

### Default seed data (inserted on init)
- Operator: `id=1, username="1", password="1"`
- Setting: `exchange_rate = 1450`
- Setting: `company_name = "Sales System"`

### Important Database Rules
- Always use prepared statements: `.prepare(sql)` + `.addBindValue(val)` — never string concatenation for user input.
- `addSalesItem()` auto-updates `products.stock_qty`.
- `addPurchaseItem()` auto-updates stock AND product prices (cost, wholesale, retail).
- Foreign keys are declared but SQLite FK enforcement must be enabled per-connection with `PRAGMA foreign_keys = ON` if needed.

---

## Build Instructions

### Prerequisites
- Qt 5.15.2 (with SQL and PrintSupport modules)
- MSVC 2017/2019 64-bit **or** MinGW 7.3+ 64-bit
- Windows 7 or later (build and target)

### Build via Qt Creator (recommended)
1. Open `ReportSystem.pro` in Qt Creator
2. Select kit: *Desktop Qt 5.15.2 MSVC2019 64bit* or *MinGW 64-bit*
3. Build → Build Project (`Ctrl+B`)
4. Run (`Ctrl+R`) — login with `1` / `1`

### Build via Command Line
```bash
# MinGW
qmake ReportSystem.pro
mingw32-make -j4

# MSVC
qmake ReportSystem.pro
nmake
```

### Deployment (Windows standalone package)
```bat
deploy_windows.bat
```
This copies the release exe to `Deploy/`, runs `windeployqt`, and bundles the SQLite driver DLL. The result is a self-contained folder that runs without Qt installed.

### Qt Project File Notes (`ReportSystem.pro`)
- Qt modules: `core gui widgets printsupport sql`
- MSVC flag: `/utf-8` for correct Arabic string encoding
- GCC flag: `-finput-charset=utf-8 -fexec-charset=utf-8`
- Windows RC metadata: version, company, product name embedded

---

## Code Conventions

### Naming
- **Private members:** `m_` prefix (e.g., `m_customerCombo`, `m_totalLabel`)
- **Methods/variables:** camelCase
- **Slots:** `onEventName()` or descriptive verb (e.g., `onSaveClicked()`)
- **Classes:** PascalCase, one class per header/source pair

### UI Construction
- Hybrid: `.ui` Designer files for complex forms + programmatic layout for dynamic content
- Lambda helpers used in `mainwindow.cpp` (e.g., `makeLeftBtn`, `makeHeader`) to reduce repetition
- Layout classes: `QGridLayout`, `QVBoxLayout`, `QHBoxLayout`
- Styling: inline `setStyleSheet()` strings (color `#003366` for headers, `#dde4f0` for backgrounds)

### Arabic/RTL Requirements
- **Every** new window or dialog must call `setLayoutDirection(Qt::RightToLeft)`
- Use `ArabicHelper::setupArabicUI(widget)` for consistent locale/font setup
- Font: **Tahoma** — used throughout for Arabic text rendering
- Print HTML must include `dir='rtl'` on table/div elements

### Signal/Slot Pattern
```cpp
// Preferred modern syntax
connect(button, &QPushButton::clicked, this, &MyClass::onSaveClicked);
connect(combo, &QComboBox::currentIndexChanged, this, [this](int idx){ ... });
```

### SQL Queries
```cpp
// Always use prepared statements
QSqlQuery q;
q.prepare("INSERT INTO customers (name, region) VALUES (?, ?)");
q.addBindValue(name);
q.addBindValue(region);
q.exec();
```

### Printing
- All print output is generated as HTML strings by `PrintManager` static methods
- Use `QTextDocument` + `QPrinter` pipeline — do not use `QPainter` directly
- For thermal (80mm) printer: set page width explicitly, use narrow CSS
- For A4 laser: standard Qt page size enum

---

## Key Workflows

### Adding a New Report
1. Create `newreport.h` and `newreport.cpp` in `src/` inheriting `QDialog`
2. Add a corresponding `newreport.ui` form in `forms/` (or build layout programmatically)
3. Add a `Database::getNewReportData()` static method in `database.h/.cpp`
4. Register the new report in `ReportsWindow` — add a button/entry to the appropriate category tab
5. Add files to `ReportSystem.pro` under `SOURCES`, `HEADERS`, and `FORMS`
6. Apply RTL layout and Tahoma font via `ArabicHelper::setupArabicUI()`

### Adding a New Database Table
1. Add `CREATE TABLE IF NOT EXISTS` statement in `Database::init()` in `database.cpp`
2. Add CRUD methods to `Database` class (static, returning `bool`/`int`/`QList<T>`)
3. Use prepared statements for all INSERT/UPDATE/DELETE

### Adding a New Setting
1. Add `INSERT OR IGNORE INTO settings VALUES ('key', 'default')` in `Database::init()`
2. Add getter/setter static methods to `Database`:
   ```cpp
   static QString getNewSetting();
   static void setNewSetting(const QString &value);
   ```
3. Wire up UI in `SystemSettings` dialog

### Modifying Pricing/Currency Logic
- Exchange rate lives in `settings` table — always fetch via `Database::getExchangeRate()`, never hardcode `1450`
- Both `_dollar` and `_dinar` columns must be kept in sync when saving monetary values

---

## Testing

**There is no automated test suite.** Testing is entirely manual.

When making changes:
- Build and run the application
- Log in with credentials `1` / `1`
- Exercise the affected module manually
- Verify Arabic text renders correctly (no mojibake/squares)
- Verify print preview output if touching `PrintManager`
- Check that `data.db` is created correctly on first run (delete it to test fresh init)

---

## Common Pitfalls

1. **UTF-8 encoding:** Source files must be saved as UTF-8. Arabic string literals are embedded directly in `.cpp` files. Wrong encoding produces garbled text.
2. **RTL missing:** Forgetting `setLayoutDirection(Qt::RightToLeft)` causes left-to-right layout for Arabic UIs.
3. **SQL string building:** Never concatenate user input into SQL strings — always use `addBindValue()`.
4. **Tahoma font:** If Tahoma is not installed on the build machine, Arabic text may not render. It is a standard Windows font; ensure it is present.
5. **windeployqt:** Running the exe directly from the `release/` folder without deploying may fail due to missing Qt DLLs. Use `deploy_windows.bat` for a runnable package.
6. **data.db path:** The database is created relative to the executable's directory at runtime. Do not hardcode absolute paths.
7. **Exchange rate:** Always read the rate from the database — do not cache it in member variables across long-lived windows.

---

## Default Credentials

| Field | Value |
|-------|-------|
| Username | `1` |
| Password | `1` |

Change via System Settings → Operators tab after first login.

---

## File Encoding & Qt Pro Registration

When adding any new `.cpp`, `.h`, or `.ui` file, it **must** be registered in `ReportSystem.pro`:
```
SOURCES  += src/newfile.cpp
HEADERS  += src/newfile.h
FORMS    += forms/newfile.ui   # if applicable
```
Run `qmake` after editing `.pro` to regenerate Makefiles.
