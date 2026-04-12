#ifndef TOTALSALESREPORT_H
#define TOTALSALESREPORT_H
#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
class TotalSalesReport : public QDialog {
    Q_OBJECT
public:
    explicit TotalSalesReport(QWidget *parent = nullptr);
private slots:
    void runReport(); void printReport();
private:
    void setupUI(); void applyStyles();
    QCheckBox *m_groupCheck, *m_productCheck, *m_customerCheck, *m_regionCheck;
    QPushButton *m_groupBtn, *m_productBtn, *m_customerBtn, *m_regionBtn;
    QComboBox *m_customerCombo;
    QLineEdit *m_customerNameEdit;
    QDateEdit *m_fromDate, *m_toDate;
    QPushButton *m_printBtn, *m_runBtn;
};
#endif
