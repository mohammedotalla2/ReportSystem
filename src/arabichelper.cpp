#include "arabichelper.h"
#include <QLocale>
#include <QDate>

void ArabicHelper::setupArabicUI(QApplication &app)
{
    app.setLayoutDirection(Qt::RightToLeft);
    QLocale::setDefault(QLocale(QLocale::Arabic, QLocale::Iraq));

    QFont font;
    font.setFamily("Tahoma");
    font.setPointSize(10);
    app.setFont(font);
}

QFont ArabicHelper::arabicFont(int size, bool bold)
{
    QFont font;
    font.setFamily("Tahoma");
    font.setPointSize(size);
    font.setBold(bold);
    return font;
}

QString ArabicHelper::arabicDate(const QDate &date)
{
    return date.toString("dd/MM/yyyy");
}

QString ArabicHelper::arabicDayName(const QDate &date)
{
    QStringList days = {
        QString::fromUtf8("الأحد"),
        QString::fromUtf8("الاثنين"),
        QString::fromUtf8("الثلاثاء"),
        QString::fromUtf8("الأربعاء"),
        QString::fromUtf8("الخميس"),
        QString::fromUtf8("الجمعة"),
        QString::fromUtf8("السبت")
    };
    return days[date.dayOfWeek() % 7];
}
