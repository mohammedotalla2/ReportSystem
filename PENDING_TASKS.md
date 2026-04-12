# Pending Tasks — POS System Settings Window

## Status Legend
- [x] Done
- [ ] Pending

---

## 1. Database Layer
- [x] Add `product_groups` table
- [x] Add `reorder_level`, `cost_price_dinar`, `exchange_rate_at_add` columns to `products`
- [x] Update `addProduct()` / `updateProduct()` signatures
- [x] Add `getProductGroups()` and `ensureProductGroup()` methods
- [x] Update `updateCustomer()` to include `type` parameter
- [x] Support customer type 2 (زبون ومجهز) alongside 0=زبون, 1=مجهز

---

## 2. systemsettings.h
- [x] New members: `m_prodGroupCombo`, `m_prodReorderSpin`, `m_prodCostDinSpin`
- [x] New members: `m_prodIdLabel`, `m_custIdLabel`
- [x] New members: `m_custSearchEdit`, editing-state IDs
- [x] New slots declared: `generateBarcode`, `onCostDollarChanged`, `onWsDollarChanged`,
      `onRetDollarChanged`, `loadProductGroups`, `clearProductForm`, `clearCustomerForm`,
      `searchCustomer`, `onTabChanged`, `updateNextProductId`, `updateNextCustomerId`
- [x] `eventFilter` override declared for Enter-key navigation and focus-clear

---

## 3. systemsettings.cpp  ← **MAIN PENDING WORK**

### 3a. General / Infrastructure
- [ ] Include `<QScrollArea>`, `<QKeyEvent>`, `<QTimer>`, `<QFrame>`, `<QDateTime>`
- [ ] Constructor: initialise `m_editingCustomerId = -1`, `m_editingProductId = -1`
- [ ] `eventFilter`: Enter key → focus next widget in tab-order list
- [ ] `eventFilter`: FocusIn on spinbox with value 0 → `selectAll()`
- [ ] `installFocusClear(w)` helper: installs eventFilter on any widget
- [ ] `installEnterNav(list)` helper: installs eventFilter on every widget in list
- [ ] `makeLabel(text)` static helper: right-aligned Tahoma label

### 3b. RTL Layout Fix (affects ALL tabs)
- [ ] In QGridLayout with `Qt::RightToLeft`: column 0 = rightmost
- [ ] Labels must go to **lower** column indices (col 0, col 2)
- [ ] Inputs must go to **higher** column indices (col 1, col 3)
- [ ] Apply this fix to every `addWidget(label/input, row, col)` call in all tabs

### 3c. Tab 2 — Customers (أعداد النظام → الأشخاص والشركات)
- [ ] Auto-ID label (`m_custIdLabel`) showing next available customer ID
- [ ] Customer type combo: 3 options — زبون / مجهز / زبون ومجهز
- [ ] Separate balance fields: `m_custBalDollar` ($) and `m_custBalDinar` (IQD)
- [ ] `GotFocus` / FocusIn: clear zero values on all spinboxes
- [ ] `clearCustomerForm()`: clear all fields, reset type to 0, refresh next ID
- [ ] `addCustomer()`: save to DB then call `clearCustomerForm()` + `loadCustomers()`
- [ ] `editCustomer()`: implement (load selected row → form, set `m_editingCustomerId`)
- [ ] `onCustomerCellClicked(row,col)`: load row data into form for editing
- [ ] `updateNextCustomerId()`: query `SELECT COALESCE(MAX(id),0)+1 FROM customers`
- [ ] `searchCustomer()`: filter `m_customersTable` rows by `m_custSearchEdit` text
- [ ] Buttons at bottom-left: **حفظ · رجوع · إلغاء · بحث · إغلاق**
  - حفظ  → `addCustomer()` / `editCustomer()`
  - رجوع / إلغاء → `clearCustomerForm()`
  - بحث  → `searchCustomer()`
  - إغلاق → `QDialog::accept()`
- [ ] Enter-key navigation order:
      Name → Region → Phone → Address → Notes → BalDollar → BalDinar → Type → Save

### 3d. Tab 3 — Products (المواد)
- [ ] Auto-focus barcode field when switching to this tab (`onTabChanged`)
- [ ] Auto-ID label (`m_prodIdLabel`) showing next available product ID
- [ ] "إنشاء باركود" button next to barcode field → `generateBarcode()`
  - Generates 12-digit barcode: prefix `8964` + zero-padded next product ID
- [ ] After barcode entered (returnPressed) → auto-focus name field
- [ ] `m_prodGroupCombo`: editable QComboBox, loaded from `product_groups` table
- [ ] `GotFocus` / FocusIn: clear zero values on all spinboxes
- [ ] Real-time currency conversion (using `m_exchangeRateSpin` value):
  - `onCostDollarChanged(v)` → `m_prodCostDinSpin = v × rate`
  - `onWsDollarChanged(v)`   → `m_prodWsDinSpin  = v × rate`
  - `onRetDollarChanged(v)`  → `m_prodRetDinSpin  = v × rate`
- [ ] `m_prodReorderSpin`: reorder level (حد إعادة الطلب)
- [ ] `clearProductForm()`: clear all fields, refocus barcode, refresh next ID
- [ ] `addProduct()`: call `Database::ensureProductGroup()`, then `Database::addProduct()`
      with all new params incl. `costPriceDinar`, `reorderLevel`, `exchangeRateAtAdd`
- [ ] `editProduct()`: implement (load selected row → form, set `m_editingProductId`)
- [ ] `onProductCellClicked(row,col)` (or table selectionChanged): load row → form
- [ ] `deleteProduct()`: confirm → delete → reload
- [ ] `updateNextProductId()`: query `SELECT COALESCE(MAX(id),0)+1 FROM products`
- [ ] `loadProductGroups()`: populate `m_prodGroupCombo` from `Database::getProductGroups()`
- [ ] Buttons at bottom: **حفظ · جديد · تعديل · حذف** (with icons 💾 + ✏ ✖)
- [ ] Enter-key navigation order:
      Barcode → Name → Group → Type → CostDollar → CostDinar →
      WsDollar → WsDinar → RetDollar → RetDinar → Cartoon → Reorder → Stock → Save

### 3e. Styles / Layout
- [ ] Form inside `QScrollArea` (so form can scroll but buttons stay fixed)
- [ ] Button bar in a fixed-height `QFrame` at the very bottom of each tab
- [ ] Consistent button style across all tabs (objectName `"actionBtn"`)
- [ ] Existing `applyStyles()` stylesheet — extend to cover new widgets

---

## 4. Final Steps
- [ ] Verify no compile errors (check all callers of old `addProduct`/`updateProduct` signatures)
- [ ] Commit `systemsettings.cpp` to branch `claude/laughing-fermat-d9lkk`
- [ ] Push to remote
