# نظام المبيعات والمشتريات
## Sales & Purchase Management System
### Qt C++ — Windows 7/8/10/11

---

## متطلبات البناء / Build Requirements

| Tool | Version |
|------|---------|
| Qt   | 5.6 – 5.15 (recommended: **Qt 5.15.2**) |
| Compiler | MSVC 2017/2019 **or** MinGW 7.3+ |
| SQLite | bundled with Qt (QSQLITE driver) |

---

## بناء المشروع / Build Steps

### Using Qt Creator (recommended)
1. Open **Qt Creator**
2. File → Open File or Project → select `ReportSystem.pro`
3. Select kit: **Desktop Qt 5.15.2 MSVC2019 64bit**
   (or MinGW 64-bit for open-source Qt)
4. Click **Build** ▶ **Build Project**
5. Click **Run** ▶

### Using command line (MinGW)
```bat
cd ReportSystem
qmake ReportSystem.pro
mingw32-make -j4
```

### Using command line (MSVC)
```bat
cd ReportSystem
qmake ReportSystem.pro
nmake
```

---

## تشغيل البرنامج / Running

- Default login: username `1` / password `1`
- Database file `data.db` is created automatically beside the executable
- All data is stored in SQLite — no server required

---

## الطباعة / Printing

### Konica Minolta (A4 laser)
- Go to **أعداد النظام** → printer settings
- Or select the Konica Minolta printer in any print dialog
- All reports support A4 portrait printing

### Thermal Printer (80 mm receipt)
- Use the **الطابعة الشريطية** button in the sales window
- Set the thermal printer name in settings
- Optimised for 80 mm roll width

### PDF Export
- Every window with a **PDF** button can export directly to PDF
- No additional library needed (uses Qt's built-in PDF printer)

---

## هيكل الملفات / File Structure

```
ReportSystem/
│
├── ReportSystem.pro          Qt project file
├── README.md                 This file
│
├── src/
│   ├── main.cpp              Entry point + Login dialog
│   ├── arabichelper.h/.cpp   RTL / Arabic font setup
│   ├── database.h/.cpp       SQLite database layer
│   ├── printmanager.h/.cpp   Konica + Thermal + PDF printing
│   │
│   ├── mainwindow.h/.cpp     Main window  (الواجهة الرئيسية)
│   ├── saleswindow.h/.cpp    Sales        (المبيعات)
│   ├── purchasewindow.h/.cpp Purchases    (المشتريات)
│   ├── cashboxwindow.h/.cpp  Cash box     (الصندوق)
│   ├── reportswindow.h/.cpp  Reports menu (التقارير)
│   ├── systemsettings.h/.cpp Settings     (أعداد النظام)
│   │
│   ├── dailyreport.h/.cpp          التقرير اليومي
│   ├── customerstatement.h/.cpp    كشف حساب زبون
│   ├── customerbalances.h/.cpp     ارصدة الزبائن
│   ├── inventoryreport.h/.cpp      جرد أرصدة المواد
│   ├── latedpayments.h/.cpp        كشف المتأخرين
│   ├── totalsalesreport.h/.cpp     مجموع المبيعات
│   ├── allcustomersreport.h/.cpp   تقرير أسماء الزبائن
│   └── overallbalancereport.h/.cpp الرصيد الإجمالي
│
└── resources/
    └── resources.qrc         Qt resources
```

---

## ميزات النظام / Features

| Module | Features |
|--------|----------|
| **Sales** | Dual currency ($ + IQD), carton/piece units, customer balance tracking, thermal receipt, PDF, preparation list |
| **Purchases** | Local/import, supplier management, auto stock update, cost price tracking |
| **Cash Box** | Receive/pay customers, general expenses, equivalent calculation, receipt printing |
| **Reports** | 6 customer reports, 6 material reports, 6 cash reports, 5 profit reports |
| **Settings** | Exchange rate, company name, customers CRUD, products CRUD |
| **Printing** | Konica Minolta A4 full preview, 80mm thermal, PDF export |

---

## ملاحظات تقنية / Technical Notes

- Fully **RTL** (Right-to-Left) Arabic interface
- Uses **Tahoma** font (included in Windows)
- SQLite database — single file, no installation
- Compatible with **Windows 7 SP1** and later
- Qt 5.6 minimum for Windows 7 compatibility
  (Qt 5.x dropped Windows 7 support from 5.15.3+;
   use 5.15.2 for guaranteed Win7 support)

---

## دعم / Support

Built by: **Amir Systems** — 2026
