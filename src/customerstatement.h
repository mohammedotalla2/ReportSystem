#ifndef CUSTOMERSTATEMENT_H
#define CUSTOMERSTATEMENT_H
#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
class CustomerStatement : public QDialog {
    Q_OBJECT
public:
    explicit CustomerStatement(QWidget *parent = nullptr);
private slots:
    void runDetailedDollar(); void runSummaryDollar();
    void runDetailedDinar(); void runSummaryDinar();
    void printReport();
private:
    void setupUI(); void applyStyles();
    QComboBox *m_customerCombo;
    QDateEdit *m_fromDate, *m_toDate;
    QPushButton *m_printBtn;
};
#endif
