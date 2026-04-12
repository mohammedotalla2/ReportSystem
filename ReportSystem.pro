QT += core gui widgets printsupport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = ReportSystem
TEMPLATE = app
CONFIG  += c++11

# UTF-8 source encoding — use correct flag per compiler
msvc {
    QMAKE_CXXFLAGS += /utf-8
}
gcc|g++|mingw {
    QMAKE_CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8
}

win32 {
    VERSION                  = 1.0.0.0
    QMAKE_TARGET_COMPANY     = "Amir Systems"
    QMAKE_TARGET_PRODUCT     = "Sales And Purchase System"
    QMAKE_TARGET_DESCRIPTION = "Sales And Purchase Management"
    QMAKE_TARGET_COPYRIGHT   = "2026"
}

SOURCES += \
    src/main.cpp \
    src/arabichelper.cpp \
    src/database.cpp \
    src/printmanager.cpp \
    src/mainwindow.cpp \
    src/saleswindow.cpp \
    src/purchasewindow.cpp \
    src/cashboxwindow.cpp \
    src/reportswindow.cpp \
    src/systemsettings.cpp \
    src/dailyreport.cpp \
    src/customerstatement.cpp \
    src/customerbalances.cpp \
    src/inventoryreport.cpp \
    src/latedpayments.cpp \
    src/totalsalesreport.cpp \
    src/allcustomersreport.cpp \
    src/overallbalancereport.cpp

HEADERS += \
    src/arabichelper.h \
    src/database.h \
    src/printmanager.h \
    src/mainwindow.h \
    src/saleswindow.h \
    src/purchasewindow.h \
    src/cashboxwindow.h \
    src/reportswindow.h \
    src/systemsettings.h \
    src/dailyreport.h \
    src/customerstatement.h \
    src/customerbalances.h \
    src/inventoryreport.h \
    src/latedpayments.h \
    src/totalsalesreport.h \
    src/allcustomersreport.h \
    src/overallbalancereport.h

FORMS += \
    forms/mainwindow.ui \
    forms/saleswindow.ui \
    forms/purchasewindow.ui \
    forms/cashboxwindow.ui \
    forms/reportswindow.ui \
    forms/systemsettings.ui \
    forms/dailyreport.ui \
    forms/customerstatement.ui \
    forms/customerbalances.ui \
    forms/inventoryreport.ui \
    forms/latedpayments.ui \
    forms/totalsalesreport.ui \
    forms/allcustomersreport.ui \
    forms/overallbalancereport.ui

RESOURCES += resources/resources.qrc
