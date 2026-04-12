#ifndef REPORTSWINDOW_H
#define REPORTSWINDOW_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QTabBar>
#include <QLabel>

class ReportsWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ReportsWindow(QWidget *parent = nullptr);

private slots:
    void onCategoryChanged(int index);
    void onReportSelected(QListWidgetItem *item);
    void openReport();
    void closeWindow();

private:
    void setupUI();
    void applyStyles();
    void populateReports(const QString &category);

    QListWidget *m_reportsList;
    QTabBar *m_categoryTabs;
    QPushButton *m_openBtn;
    QPushButton *m_closeBtn;
};

#endif // REPORTSWINDOW_H
