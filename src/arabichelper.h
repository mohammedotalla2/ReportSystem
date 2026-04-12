#ifndef ARABICHELPER_H
#define ARABICHELPER_H

#include <QString>
#include <QApplication>
#include <QFont>
#include <QFontDatabase>

class ArabicHelper
{
public:
    static void setupArabicUI(QApplication &app);
    static QFont arabicFont(int size = 12, bool bold = false);
    static QString arabicDate(const QDate &date);
    static QString arabicDayName(const QDate &date);
};

#endif // ARABICHELPER_H
