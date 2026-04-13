#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QFont>
#include <QMessageBox>
#include <QSqlQuery>

#include "mainwindow.h"
#include "database.h"
#include "arabichelper.h"

// ============================================================
// Login Dialog  — professional redesign
// ============================================================
class LoginDialog : public QDialog
{
public:
    explicit LoginDialog(QWidget *parent = nullptr) : QDialog(parent)
    {
        setWindowTitle(QString::fromUtf8("تسجيل الدخول — نظام أمير"));
        setLayoutDirection(Qt::RightToLeft);
        setFixedSize(420, 530);
        setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);

        // ── Root layout (no margins — fills the whole window) ──────────
        QVBoxLayout *root = new QVBoxLayout(this);
        root->setContentsMargins(0, 0, 0, 0);
        root->setSpacing(0);

        // ══════════════════════════════════════════════════════════════
        //  TOP HEADER BAND
        // ══════════════════════════════════════════════════════════════
        QWidget *header = new QWidget;
        header->setObjectName("loginHeader");
        header->setFixedHeight(195);

        QVBoxLayout *hdrL = new QVBoxLayout(header);
        hdrL->setContentsMargins(24, 28, 24, 20);
        hdrL->setSpacing(6);
        hdrL->setAlignment(Qt::AlignCenter);

        // Decorative icon circle (emoji in a styled label)
        QLabel *iconLbl = new QLabel(QString::fromUtf8("🏢"));
        iconLbl->setObjectName("loginIcon");
        iconLbl->setFont(QFont("Segoe UI Emoji", 32));
        iconLbl->setAlignment(Qt::AlignCenter);
        iconLbl->setFixedHeight(56);

        // Horizontal divider line
        QFrame *divH = new QFrame;
        divH->setFrameShape(QFrame::HLine);
        divH->setObjectName("hdrDivider");
        divH->setFixedHeight(1);

        QLabel *titleLbl = new QLabel(QString::fromUtf8("نظام أمير للمبيعات والمشتريات"));
        titleLbl->setObjectName("loginTitle");
        titleLbl->setFont(QFont("Tahoma", 14, QFont::Bold));
        titleLbl->setAlignment(Qt::AlignCenter);

        QLabel *subLbl = new QLabel(QString::fromUtf8("إدارة متكاملة للمخزون والحسابات"));
        subLbl->setObjectName("loginSub");
        subLbl->setFont(QFont("Tahoma", 9));
        subLbl->setAlignment(Qt::AlignCenter);

        hdrL->addWidget(iconLbl);
        hdrL->addWidget(divH);
        hdrL->addWidget(titleLbl);
        hdrL->addWidget(subLbl);

        // ══════════════════════════════════════════════════════════════
        //  FORM CARD
        // ══════════════════════════════════════════════════════════════
        QWidget *card = new QWidget;
        card->setObjectName("loginCard");

        QVBoxLayout *cardL = new QVBoxLayout(card);
        cardL->setContentsMargins(40, 28, 40, 24);
        cardL->setSpacing(0);

        // Welcome text
        QLabel *welcomeLbl = new QLabel(QString::fromUtf8("أدخل بيانات الدخول للمتابعة"));
        welcomeLbl->setObjectName("welcomeTxt");
        welcomeLbl->setFont(QFont("Tahoma", 9));
        welcomeLbl->setAlignment(Qt::AlignCenter);
        cardL->addWidget(welcomeLbl);
        cardL->addSpacing(20);

        // ── Username field ──
        QLabel *userLbl = new QLabel(QString::fromUtf8("اسم المستخدم"));
        userLbl->setObjectName("fieldLbl");
        userLbl->setFont(QFont("Tahoma", 9, QFont::Bold));

        m_userEdit = new QLineEdit;
        m_userEdit->setObjectName("loginField");
        m_userEdit->setFont(QFont("Tahoma", 11));
        m_userEdit->setFixedHeight(44);
        m_userEdit->setAlignment(Qt::AlignCenter);
        m_userEdit->setPlaceholderText(QString::fromUtf8("اسم المستخدم ..."));
        m_userEdit->setText("1");

        cardL->addWidget(userLbl);
        cardL->addSpacing(5);
        cardL->addWidget(m_userEdit);
        cardL->addSpacing(14);

        // ── Password field ──
        QLabel *passLbl = new QLabel(QString::fromUtf8("كلمة السر"));
        passLbl->setObjectName("fieldLbl");
        passLbl->setFont(QFont("Tahoma", 9, QFont::Bold));

        m_passEdit = new QLineEdit;
        m_passEdit->setObjectName("loginField");
        m_passEdit->setFont(QFont("Tahoma", 11));
        m_passEdit->setFixedHeight(44);
        m_passEdit->setAlignment(Qt::AlignCenter);
        m_passEdit->setEchoMode(QLineEdit::Password);
        m_passEdit->setPlaceholderText(QString::fromUtf8("••••••••"));
        m_passEdit->setText("1");

        cardL->addWidget(passLbl);
        cardL->addSpacing(5);
        cardL->addWidget(m_passEdit);
        cardL->addSpacing(24);

        // ── Login button ──
        QPushButton *loginBtn = new QPushButton(QString::fromUtf8("تسجيل الدخول"));
        loginBtn->setObjectName("loginBtn");
        loginBtn->setFont(QFont("Tahoma", 12, QFont::Bold));
        loginBtn->setFixedHeight(48);
        loginBtn->setCursor(Qt::PointingHandCursor);
        cardL->addWidget(loginBtn);
        cardL->addSpacing(10);

        // ── Cancel button ──
        QPushButton *cancelBtn = new QPushButton(QString::fromUtf8("إلغاء"));
        cancelBtn->setObjectName("cancelBtn");
        cancelBtn->setFont(QFont("Tahoma", 10));
        cancelBtn->setFixedHeight(36);
        cancelBtn->setCursor(Qt::PointingHandCursor);
        cardL->addWidget(cancelBtn);

        cardL->addStretch();

        // ── Bottom version strip ──
        QLabel *verLbl = new QLabel(QString::fromUtf8("الإصدار 1.0.0  —  Amir Systems © 2024"));
        verLbl->setObjectName("verLbl");
        verLbl->setFont(QFont("Tahoma", 8));
        verLbl->setAlignment(Qt::AlignCenter);
        cardL->addWidget(verLbl);

        // ── Assemble root ──
        root->addWidget(header);
        root->addWidget(card, 1);

        // ══════════════════════════════════════════════════════════════
        //  STYLESHEET
        // ══════════════════════════════════════════════════════════════
        setStyleSheet(R"(
            QDialog {
                background: #f2f5fb;
            }

            /* ─── Header band ─── */
            #loginHeader {
                background: #0d2a5e;
            }
            #hdrDivider {
                background: #2a5aaa;
                border: none;
                margin: 0 30px;
            }
            #loginIcon {
                color: #ffd700;
            }
            #loginTitle {
                color: #ffffff;
            }
            #loginSub {
                color: #8ab0e0;
            }

            /* ─── Card ─── */
            #loginCard {
                background: #ffffff;
                border-top: 3px solid #1a4a9e;
            }

            /* ─── Form labels ─── */
            #welcomeTxt {
                color: #7788aa;
            }
            #fieldLbl {
                color: #334466;
            }

            /* ─── Input fields ─── */
            #loginField {
                background: #f4f7fd;
                border: 2px solid #c8d8f0;
                border-radius: 8px;
                padding: 4px 14px;
                color: #223355;
                font-size: 11pt;
            }
            #loginField:focus {
                border: 2px solid #1a4a9e;
                background: #ffffff;
            }

            /* ─── Login button ─── */
            #loginBtn {
                background: #1a4a9e;
                color: white;
                border: none;
                border-radius: 8px;
            }
            #loginBtn:hover {
                background: #2255bb;
            }
            #loginBtn:pressed {
                background: #0d2a6a;
            }

            /* ─── Cancel button ─── */
            #cancelBtn {
                background: transparent;
                color: #8899bb;
                border: 1px solid #c8d8f0;
                border-radius: 8px;
            }
            #cancelBtn:hover {
                background: #f0f4fc;
                color: #334466;
                border-color: #99aabb;
            }

            /* ─── Version label ─── */
            #verLbl {
                color: #aabbcc;
            }
        )");

        // ── Connections ──
        connect(loginBtn,  &QPushButton::clicked,       this, &LoginDialog::tryLogin);
        connect(cancelBtn, &QPushButton::clicked,       this, &QDialog::reject);
        connect(m_passEdit, &QLineEdit::returnPressed,  this, &LoginDialog::tryLogin);
        connect(m_userEdit, &QLineEdit::returnPressed,  this, [this]{ m_passEdit->setFocus(); });
    }

    int loggedUserId() const { return m_userId; }

private slots:
    void tryLogin()
    {
        QSqlQuery q;
        q.prepare("SELECT id FROM operators WHERE username=? AND password=? AND active=1");
        q.addBindValue(m_userEdit->text().trimmed());
        q.addBindValue(m_passEdit->text());
        q.exec();
        if (q.next()) {
            m_userId = q.value(0).toInt();
            accept();
        } else {
            QMessageBox::warning(this,
                QString::fromUtf8("خطأ في تسجيل الدخول"),
                QString::fromUtf8("اسم المستخدم أو كلمة السر غير صحيحة.\nالرجاء المحاولة مرة أخرى."));
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
