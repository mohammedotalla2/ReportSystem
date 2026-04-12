#ifndef LATEDPAYMENTS_H
#define LATEDPAYMENTS_H
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
class LatedPayments : public QDialog {
    Q_OBJECT
public:
    explicit LatedPayments(QWidget *parent = nullptr);
private slots:
    void runReport(); void printReport();
private:
    void setupUI(); void applyStyles();
    QCheckBox *m_regionCheck;
    QPushButton *m_regionBtn;
    QComboBox *m_regionCombo;
    QLineEdit *m_daysEdit;
    QPushButton *m_daysBtn, *m_printBtn, *m_runBtn;
};
#endif
