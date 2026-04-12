#ifndef OVERALLBALANCEREPORT_H
#define OVERALLBALANCEREPORT_H
#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QCheckBox>
class OverallBalanceReport : public QDialog {
    Q_OBJECT
public:
    explicit OverallBalanceReport(QWidget *parent = nullptr);
private slots:
    void runReport(); void printReport();
private:
    void setupUI(); void applyStyles();
    QCheckBox *m_regionCheck, *m_typeCheck, *m_balTypeCheck;
    QPushButton *m_regionBtn, *m_typeBtn, *m_balTypeBtn;
    QComboBox *m_regionCombo, *m_typeCombo, *m_balTypeCombo;
    QDateEdit *m_toDate;
    QPushButton *m_printBtn, *m_runBtn;
};
#endif
