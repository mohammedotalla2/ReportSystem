#ifndef INVENTORYREPORT_H
#define INVENTORYREPORT_H
#include <QDialog>
#include <QDateEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QListWidget>
class InventoryReport : public QDialog {
    Q_OBJECT
public:
    explicit InventoryReport(QWidget *parent = nullptr);
private slots:
    void runReport(); void printReport(); void showPrevBalances();
private:
    void setupUI(); void applyStyles();
    QRadioButton *m_groupRadio, *m_typeRadio, *m_groupTypeRadio, *m_generalRadio;
    QCheckBox *m_hideZeroCheck;
    QDateEdit *m_toDate;
    QPushButton *m_printBtn, *m_runBtn, *m_prevBalBtn;
    QListWidget *m_groupList;
};
#endif
