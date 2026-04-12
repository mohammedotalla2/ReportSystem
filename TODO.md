# TODO — Advanced Sales Page (صفحة المبيعات المتقدمة)

Branch: `claude/advanced-sales-page-kbCWc`

## Tasks

- [x] Create `src/advancedsaleswindow.h`
      Header with WaitingInvoice/WaitingItem structs, all widget members, slot declarations.

- [ ] Create `src/advancedsaleswindow.cpp`
      Full implementation:
      - Group-filter list (right panel)
      - Smart search by name and barcode
      - Products table with stock display
      - Customer data load on selection (balance $, balance IQD, phone, address)
      - Currency switching with instant price conversion for all rows
      - Per-row pricing type (مفرد / جملة) via delegate
      - Direct editing of qty, price, notes in invoice table with live total update
      - Waiting system: save invoice to RAM (Wait button) / restore (Retrieve button)
      - Received-amount fields ($ and IQD) with change/shortfall calculation
      - Accounting on save: cash-box update for cash invoices, customer-balance update for credit invoices
      - Print 1 or 2 copies, thermal receipt, PDF export
      - Preparation list button
      - Customer statement quick-access button

- [ ] Update `ReportSystem.pro`
      Add `src/advancedsaleswindow.cpp` to SOURCES and `src/advancedsaleswindow.h` to HEADERS.

- [ ] Update `src/mainwindow.h`
      Add `openAdvancedSales` slot and `m_advancedSalesBtn` member.

- [ ] Update `src/mainwindow.cpp`
      Add Advanced Sales button to the centre panel and wire it to `openAdvancedSales`.

- [ ] Commit and push to `claude/advanced-sales-page-kbCWc`
