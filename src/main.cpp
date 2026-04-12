#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QMessageBox>
#include <QSqlQuery>

#include "mainwindow.h"
#include "database.h"
#include "arabichelper.h"

// ============================================================
// Login Dialog
// ============================================================
class LoginDialog : public QDialog
{
public:
    explicit LoginDialog(QWidget *parent = nullptr) : QDialog(parent)
    {
        setWindowTitle(QString::fromUtf8("تسجيل الدخول"));
        setLayoutDirection(Qt::RightToLeft);
        setFixedSize(340, 260);
        setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(24, 24, 24, 24);
        layout->setSpacing(14);

        QLabel *titleLbl = new QLabel(QString::fromUtf8("نظام المبيعات والمشتريات"));
        titleLbl->setFont(QFont("Tahoma", 13, QFont::Bold));
        titleLbl->setAlignment(Qt::AlignCenter);
        titleLbl->setStyleSheet("color: #003366; padding: 8px;");

        QLabel *userLbl = new QLabel(QString::fromUtf8("اسم المستخدم:"));
        userLbl->setFont(QFont("Tahoma", 10));
        m_userEdit = new QLineEdit;
        m_userEdit->setFont(QFont("Tahoma", 11));
        m_userEdit->setText("1");
        m_userEdit->setAlignment(Qt::AlignCenter);

        QLabel *passLbl = new QLabel(QString::fromUtf8("كلمة السر:"));
        passLbl->setFont(QFont("Tahoma", 10));
        m_passEdit = new QLineEdit;
        m_passEdit->setFont(QFont("Tahoma", 11));
        m_passEdit->setEchoMode(QLineEdit::Password);
        m_passEdit->setText("1");
        m_passEdit->setAlignment(Qt::AlignCenter);

        QHBoxLayout *btnLayout = new QHBoxLayout;
        QPushButton *loginBtn = new QPushButton(QString::fromUtf8("دخول"));
        QPushButton *cancelBtn = new QPushButton(QString::fromUtf8("إلغاء"));
        loginBtn->setFont(QFont("Tahoma", 11, QFont::Bold));
        loginBtn->setFixedHeight(36);
        loginBtn->setStyleSheet(
            "QPushButton { background: #003366; color: white; border-radius: 4px; }"
            "QPushButton:hover { background: #0044aa; }");
        cancelBtn->setFont(QFont("Tahoma", 11));
        cancelBtn->setFixedHeight(36);
        cancelBtn->setStyleSheet(
            "QPushButton { background: #882222; color: white; border-radius: 4px; }"
            "QPushButton:hover { background: #aa2222; }");

        btnLayout->addWidget(loginBtn);
        btnLayout->addWidget(cancelBtn);

        layout->addWidget(titleLbl);
        layout->addWidget(userLbl);
        layout->addWidget(m_userEdit);
        layout->addWidget(passLbl);
        layout->addWidget(m_passEdit);
        layout->addLayout(btnLayout);

        setStyleSheet("QDialog { background: #dde4f0; } "
                      "QLineEdit { background: white; border: 1px solid #99aacc; border-radius: 3px; padding: 4px 8px; } "
                      "QLabel { color: #003366; }");

        connect(loginBtn,  &QPushButton::clicked, this, &LoginDialog::tryLogin);
        connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
        connect(m_passEdit, &QLineEdit::returnPressed, this, &LoginDialog::tryLogin);
    }

    int loggedUserId() const { return m_userId; }

private slots:
    void tryLogin()
    {
        QSqlQuery q;
        q.prepare("SELECT id FROM operators WHERE username=? AND password=? AND active=1");
        q.addBindValue(m_userEdit->text());
        q.addBindValue(m_passEdit->text());
        q.exec();
        if (q.next()) {
            m_userId = q.value(0).toInt();
            accept();
        } else {
            QMessageBox::warning(this,
                QString::fromUtf8("خطأ"),
                QString::fromUtf8("اسم المستخدم أو كلمة السر غير صحيحة"));
            m_passEdit->clear();
            m_passEdit->setFocus();
        }
    }

private:
    QLineEdit *m_userEdit, *m_passEdit;
    int m_userId = -1;
};

// ============================================================
// main()
// ============================================================
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Arabic / RTL setup
    ArabicHelper::setupArabicUI(app);

    // Application info
    app.setApplicationName(QString::fromUtf8("نظام المبيعات والمشتريات"));
    app.setOrganizationName("AmirSystems");
    app.setApplicationVersion("1.0.0");

    // Init database (creates tables if needed)
    if (!Database::init("data.db")) {
        QMessageBox::critical(nullptr,
            QString::fromUtf8("خطأ"),
            QString::fromUtf8("تعذّر فتح قاعدة البيانات!"));
        return 1;
    }

    // Login
    LoginDialog login;
    if (login.exec() != QDialog::Accepted)
        return 0;

    // Main window
    MainWindow w;
    w.show();

    return app.exec();
}
