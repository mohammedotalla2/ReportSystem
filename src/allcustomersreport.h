#ifndef ALLCUSTOMERSREPORT_H
#define ALLCUSTOMERSREPORT_H
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
class AllCustomersReport : public QDialog {
    Q_OBJECT
public:
    explicit AllCustomersReport(QWidget *parent = nullptr);
private slots:
    void runByRegion(); void runByType();
    void runByRegionAndType(); void runGeneral();
    void printReport();
private:
    void setupUI(); void applyStyles();
    QComboBox *m_regionCombo;
    QPushButton *m_printBtn, *m_runBtn;
    QPushButton *m_regionBtn, *m_typeBtn, *m_bothBtn, *m_generalBtn;
};
#endif
