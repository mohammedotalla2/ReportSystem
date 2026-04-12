// ========== dailyreport.h ==========
#ifndef DAILYREPORT_H
#define DAILYREPORT_H
#include <QDialog>
#include <QDateEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
class DailyReport : public QDialog {
    Q_OBJECT
public:
    explicit DailyReport(QWidget *parent = nullptr);
private slots:
    void runDetailed(); void runSummary(); void printReport();
private:
    void setupUI(); void applyStyles();
    QDateEdit *m_fromDate, *m_toDate;
    QPushButton *m_detailBtn, *m_summaryBtn, *m_printBtn;
    QCheckBox *m_userCheck;
    QLabel *m_titleLabel;
};
#endif
