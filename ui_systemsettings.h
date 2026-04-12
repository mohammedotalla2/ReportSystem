/********************************************************************************
** Form generated from reading UI file 'systemsettings.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSTEMSETTINGS_H
#define UI_SYSTEMSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SystemSettings
{
public:
    QVBoxLayout *rootLayout;
    QTabWidget *tabWidget;
    QWidget *tabGeneral;
    QGridLayout *settingsGrid;
    QLabel *lblCompanyName;
    QLineEdit *txtCompanyName;
    QLabel *lblExchRate;
    QDoubleSpinBox *spnExchRate;
    QLabel *label;
    QLineEdit *txtCashboxDollar;
    QLabel *label1;
    QLineEdit *txtCashboxDinar;
    QSpacerItem *spacerItem;
    QWidget *tabCustomers;
    QVBoxLayout *custTabLayout;
    QGroupBox *grpCustForm;
    QGridLayout *custFormGrid;
    QLabel *label2;
    QLineEdit *txtCustName;
    QLabel *label3;
    QLineEdit *txtCustRegion;
    QLabel *label4;
    QLineEdit *txtCustPhone;
    QLabel *label5;
    QLineEdit *txtCustAddress;
    QLabel *label6;
    QDoubleSpinBox *spnCustBalDollar;
    QLabel *label7;
    QDoubleSpinBox *spnCustBalDinar;
    QLabel *label8;
    QComboBox *cmbCustType;
    QLabel *label9;
    QLineEdit *txtCustNotes;
    QHBoxLayout *hboxLayout;
    QPushButton *btnSaveCust;
    QPushButton *btnNewCust;
    QSpacerItem *spacerItem1;
    QPushButton *btnDeleteCust;
    QTableWidget *tblCustomers;
    QWidget *tabProducts;
    QVBoxLayout *prodTabLayout;
    QGroupBox *grpProdForm;
    QGridLayout *prodFormGrid;
    QLabel *label10;
    QLineEdit *txtBarcode;
    QLabel *label11;
    QLineEdit *txtProdName;
    QLabel *label12;
    QLineEdit *txtProdGroup;
    QLabel *label13;
    QLineEdit *txtProdType;
    QLabel *label14;
    QDoubleSpinBox *spnCost;
    QLabel *label15;
    QSpinBox *spnCartoon;
    QLabel *label16;
    QDoubleSpinBox *spnWsDollar;
    QLabel *label17;
    QDoubleSpinBox *spnRetDollar;
    QLabel *label18;
    QDoubleSpinBox *spnWsDinar;
    QLabel *label19;
    QDoubleSpinBox *spnRetDinar;
    QLabel *label20;
    QDoubleSpinBox *spnStock;
    QHBoxLayout *hboxLayout1;
    QPushButton *btnSaveProd;
    QPushButton *btnNewProd;
    QSpacerItem *spacerItem2;
    QPushButton *btnDeleteProd;
    QTableWidget *tblProducts;
    QHBoxLayout *hboxLayout2;
    QPushButton *btnSave;
    QSpacerItem *spacerItem3;
    QPushButton *btnClose;

    void setupUi(QDialog *SystemSettings)
    {
        if (SystemSettings->objectName().isEmpty())
            SystemSettings->setObjectName(QString::fromUtf8("SystemSettings"));
        SystemSettings->resize(900, 650);
        SystemSettings->setLayoutDirection(Qt::RightToLeft);
        rootLayout = new QVBoxLayout(SystemSettings);
#ifndef Q_OS_MAC
        rootLayout->setSpacing(6);
#endif
        rootLayout->setContentsMargins(8, 8, 8, 8);
        rootLayout->setObjectName(QString::fromUtf8("rootLayout"));
        tabWidget = new QTabWidget(SystemSettings);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setLayoutDirection(Qt::RightToLeft);
        tabGeneral = new QWidget();
        tabGeneral->setObjectName(QString::fromUtf8("tabGeneral"));
        settingsGrid = new QGridLayout(tabGeneral);
        settingsGrid->setSpacing(8);
        settingsGrid->setContentsMargins(12, 12, 12, 12);
        settingsGrid->setObjectName(QString::fromUtf8("settingsGrid"));
        lblCompanyName = new QLabel(tabGeneral);
        lblCompanyName->setObjectName(QString::fromUtf8("lblCompanyName"));

        settingsGrid->addWidget(lblCompanyName, 0, 1, 1, 1);

        txtCompanyName = new QLineEdit(tabGeneral);
        txtCompanyName->setObjectName(QString::fromUtf8("txtCompanyName"));
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(12);
        font.setBold(true);
        txtCompanyName->setFont(font);

        settingsGrid->addWidget(txtCompanyName, 0, 0, 1, 1);

        lblExchRate = new QLabel(tabGeneral);
        lblExchRate->setObjectName(QString::fromUtf8("lblExchRate"));

        settingsGrid->addWidget(lblExchRate, 1, 1, 1, 1);

        spnExchRate = new QDoubleSpinBox(tabGeneral);
        spnExchRate->setObjectName(QString::fromUtf8("spnExchRate"));
        spnExchRate->setMinimum(1.000000000000000);
        spnExchRate->setMaximum(99999.000000000000000);
        spnExchRate->setDecimals(0);
        spnExchRate->setValue(1450.000000000000000);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(13);
        font1.setBold(true);
        spnExchRate->setFont(font1);

        settingsGrid->addWidget(spnExchRate, 1, 0, 1, 1);

        label = new QLabel(tabGeneral);
        label->setObjectName(QString::fromUtf8("label"));

        settingsGrid->addWidget(label, 2, 1, 1, 1);

        txtCashboxDollar = new QLineEdit(tabGeneral);
        txtCashboxDollar->setObjectName(QString::fromUtf8("txtCashboxDollar"));

        settingsGrid->addWidget(txtCashboxDollar, 2, 0, 1, 1);

        label1 = new QLabel(tabGeneral);
        label1->setObjectName(QString::fromUtf8("label1"));

        settingsGrid->addWidget(label1, 3, 1, 1, 1);

        txtCashboxDinar = new QLineEdit(tabGeneral);
        txtCashboxDinar->setObjectName(QString::fromUtf8("txtCashboxDinar"));

        settingsGrid->addWidget(txtCashboxDinar, 3, 0, 1, 1);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        settingsGrid->addItem(spacerItem, 4, 0, 1, 2);

        tabWidget->addTab(tabGeneral, QString());
        tabCustomers = new QWidget();
        tabCustomers->setObjectName(QString::fromUtf8("tabCustomers"));
        custTabLayout = new QVBoxLayout(tabCustomers);
        custTabLayout->setSpacing(4);
        custTabLayout->setContentsMargins(6, 6, 6, 6);
        custTabLayout->setObjectName(QString::fromUtf8("custTabLayout"));
        grpCustForm = new QGroupBox(tabCustomers);
        grpCustForm->setObjectName(QString::fromUtf8("grpCustForm"));
        custFormGrid = new QGridLayout(grpCustForm);
        custFormGrid->setSpacing(6);
        custFormGrid->setObjectName(QString::fromUtf8("custFormGrid"));
        label2 = new QLabel(grpCustForm);
        label2->setObjectName(QString::fromUtf8("label2"));

        custFormGrid->addWidget(label2, 0, 3, 1, 1);

        txtCustName = new QLineEdit(grpCustForm);
        txtCustName->setObjectName(QString::fromUtf8("txtCustName"));

        custFormGrid->addWidget(txtCustName, 0, 2, 1, 1);

        label3 = new QLabel(grpCustForm);
        label3->setObjectName(QString::fromUtf8("label3"));

        custFormGrid->addWidget(label3, 0, 1, 1, 1);

        txtCustRegion = new QLineEdit(grpCustForm);
        txtCustRegion->setObjectName(QString::fromUtf8("txtCustRegion"));

        custFormGrid->addWidget(txtCustRegion, 0, 0, 1, 1);

        label4 = new QLabel(grpCustForm);
        label4->setObjectName(QString::fromUtf8("label4"));

        custFormGrid->addWidget(label4, 1, 3, 1, 1);

        txtCustPhone = new QLineEdit(grpCustForm);
        txtCustPhone->setObjectName(QString::fromUtf8("txtCustPhone"));

        custFormGrid->addWidget(txtCustPhone, 1, 2, 1, 1);

        label5 = new QLabel(grpCustForm);
        label5->setObjectName(QString::fromUtf8("label5"));

        custFormGrid->addWidget(label5, 1, 1, 1, 1);

        txtCustAddress = new QLineEdit(grpCustForm);
        txtCustAddress->setObjectName(QString::fromUtf8("txtCustAddress"));

        custFormGrid->addWidget(txtCustAddress, 1, 0, 1, 1);

        label6 = new QLabel(grpCustForm);
        label6->setObjectName(QString::fromUtf8("label6"));

        custFormGrid->addWidget(label6, 2, 3, 1, 1);

        spnCustBalDollar = new QDoubleSpinBox(grpCustForm);
        spnCustBalDollar->setObjectName(QString::fromUtf8("spnCustBalDollar"));
        spnCustBalDollar->setMinimum(-999999.000000000000000);
        spnCustBalDollar->setMaximum(999999.000000000000000);
        spnCustBalDollar->setDecimals(2);

        custFormGrid->addWidget(spnCustBalDollar, 2, 2, 1, 1);

        label7 = new QLabel(grpCustForm);
        label7->setObjectName(QString::fromUtf8("label7"));

        custFormGrid->addWidget(label7, 2, 1, 1, 1);

        spnCustBalDinar = new QDoubleSpinBox(grpCustForm);
        spnCustBalDinar->setObjectName(QString::fromUtf8("spnCustBalDinar"));
        spnCustBalDinar->setMinimum(-999999999.000000000000000);
        spnCustBalDinar->setMaximum(999999999.000000000000000);
        spnCustBalDinar->setDecimals(0);

        custFormGrid->addWidget(spnCustBalDinar, 2, 0, 1, 1);

        label8 = new QLabel(grpCustForm);
        label8->setObjectName(QString::fromUtf8("label8"));

        custFormGrid->addWidget(label8, 3, 3, 1, 1);

        cmbCustType = new QComboBox(grpCustForm);
        cmbCustType->addItem(QString());
        cmbCustType->addItem(QString());
        cmbCustType->setObjectName(QString::fromUtf8("cmbCustType"));

        custFormGrid->addWidget(cmbCustType, 3, 2, 1, 1);

        label9 = new QLabel(grpCustForm);
        label9->setObjectName(QString::fromUtf8("label9"));

        custFormGrid->addWidget(label9, 3, 1, 1, 1);

        txtCustNotes = new QLineEdit(grpCustForm);
        txtCustNotes->setObjectName(QString::fromUtf8("txtCustNotes"));

        custFormGrid->addWidget(txtCustNotes, 3, 0, 1, 1);


        custTabLayout->addWidget(grpCustForm);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        btnSaveCust = new QPushButton(tabCustomers);
        btnSaveCust->setObjectName(QString::fromUtf8("btnSaveCust"));
        btnSaveCust->setMinimumHeight(32);

        hboxLayout->addWidget(btnSaveCust);

        btnNewCust = new QPushButton(tabCustomers);
        btnNewCust->setObjectName(QString::fromUtf8("btnNewCust"));
        btnNewCust->setMinimumHeight(32);

        hboxLayout->addWidget(btnNewCust);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem1);

        btnDeleteCust = new QPushButton(tabCustomers);
        btnDeleteCust->setObjectName(QString::fromUtf8("btnDeleteCust"));
        btnDeleteCust->setMinimumHeight(32);

        hboxLayout->addWidget(btnDeleteCust);


        custTabLayout->addLayout(hboxLayout);

        tblCustomers = new QTableWidget(tabCustomers);
        if (tblCustomers->columnCount() < 6)
            tblCustomers->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblCustomers->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblCustomers->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tblCustomers->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tblCustomers->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tblCustomers->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tblCustomers->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tblCustomers->setObjectName(QString::fromUtf8("tblCustomers"));
        tblCustomers->setSelectionBehavior(QAbstractItemView::SelectRows);

        custTabLayout->addWidget(tblCustomers);

        tabWidget->addTab(tabCustomers, QString());
        tabProducts = new QWidget();
        tabProducts->setObjectName(QString::fromUtf8("tabProducts"));
        prodTabLayout = new QVBoxLayout(tabProducts);
        prodTabLayout->setSpacing(4);
        prodTabLayout->setContentsMargins(6, 6, 6, 6);
        prodTabLayout->setObjectName(QString::fromUtf8("prodTabLayout"));
        grpProdForm = new QGroupBox(tabProducts);
        grpProdForm->setObjectName(QString::fromUtf8("grpProdForm"));
        prodFormGrid = new QGridLayout(grpProdForm);
        prodFormGrid->setSpacing(6);
        prodFormGrid->setObjectName(QString::fromUtf8("prodFormGrid"));
        label10 = new QLabel(grpProdForm);
        label10->setObjectName(QString::fromUtf8("label10"));

        prodFormGrid->addWidget(label10, 0, 5, 1, 1);

        txtBarcode = new QLineEdit(grpProdForm);
        txtBarcode->setObjectName(QString::fromUtf8("txtBarcode"));

        prodFormGrid->addWidget(txtBarcode, 0, 4, 1, 1);

        label11 = new QLabel(grpProdForm);
        label11->setObjectName(QString::fromUtf8("label11"));

        prodFormGrid->addWidget(label11, 0, 3, 1, 1);

        txtProdName = new QLineEdit(grpProdForm);
        txtProdName->setObjectName(QString::fromUtf8("txtProdName"));

        prodFormGrid->addWidget(txtProdName, 0, 0, 1, 3);

        label12 = new QLabel(grpProdForm);
        label12->setObjectName(QString::fromUtf8("label12"));

        prodFormGrid->addWidget(label12, 1, 5, 1, 1);

        txtProdGroup = new QLineEdit(grpProdForm);
        txtProdGroup->setObjectName(QString::fromUtf8("txtProdGroup"));

        prodFormGrid->addWidget(txtProdGroup, 1, 4, 1, 1);

        label13 = new QLabel(grpProdForm);
        label13->setObjectName(QString::fromUtf8("label13"));

        prodFormGrid->addWidget(label13, 1, 3, 1, 1);

        txtProdType = new QLineEdit(grpProdForm);
        txtProdType->setObjectName(QString::fromUtf8("txtProdType"));

        prodFormGrid->addWidget(txtProdType, 1, 2, 1, 1);

        label14 = new QLabel(grpProdForm);
        label14->setObjectName(QString::fromUtf8("label14"));

        prodFormGrid->addWidget(label14, 2, 5, 1, 1);

        spnCost = new QDoubleSpinBox(grpProdForm);
        spnCost->setObjectName(QString::fromUtf8("spnCost"));
        spnCost->setMaximum(999999.000000000000000);
        spnCost->setDecimals(3);

        prodFormGrid->addWidget(spnCost, 2, 4, 1, 1);

        label15 = new QLabel(grpProdForm);
        label15->setObjectName(QString::fromUtf8("label15"));

        prodFormGrid->addWidget(label15, 2, 3, 1, 1);

        spnCartoon = new QSpinBox(grpProdForm);
        spnCartoon->setObjectName(QString::fromUtf8("spnCartoon"));
        spnCartoon->setMinimum(1);
        spnCartoon->setMaximum(9999);
        spnCartoon->setValue(1);

        prodFormGrid->addWidget(spnCartoon, 2, 2, 1, 1);

        label16 = new QLabel(grpProdForm);
        label16->setObjectName(QString::fromUtf8("label16"));

        prodFormGrid->addWidget(label16, 3, 5, 1, 1);

        spnWsDollar = new QDoubleSpinBox(grpProdForm);
        spnWsDollar->setObjectName(QString::fromUtf8("spnWsDollar"));
        spnWsDollar->setMaximum(999999.000000000000000);
        spnWsDollar->setDecimals(3);

        prodFormGrid->addWidget(spnWsDollar, 3, 4, 1, 1);

        label17 = new QLabel(grpProdForm);
        label17->setObjectName(QString::fromUtf8("label17"));

        prodFormGrid->addWidget(label17, 3, 3, 1, 1);

        spnRetDollar = new QDoubleSpinBox(grpProdForm);
        spnRetDollar->setObjectName(QString::fromUtf8("spnRetDollar"));
        spnRetDollar->setMaximum(999999.000000000000000);
        spnRetDollar->setDecimals(3);

        prodFormGrid->addWidget(spnRetDollar, 3, 2, 1, 1);

        label18 = new QLabel(grpProdForm);
        label18->setObjectName(QString::fromUtf8("label18"));

        prodFormGrid->addWidget(label18, 4, 5, 1, 1);

        spnWsDinar = new QDoubleSpinBox(grpProdForm);
        spnWsDinar->setObjectName(QString::fromUtf8("spnWsDinar"));
        spnWsDinar->setMaximum(9999999.000000000000000);
        spnWsDinar->setDecimals(0);

        prodFormGrid->addWidget(spnWsDinar, 4, 4, 1, 1);

        label19 = new QLabel(grpProdForm);
        label19->setObjectName(QString::fromUtf8("label19"));

        prodFormGrid->addWidget(label19, 4, 3, 1, 1);

        spnRetDinar = new QDoubleSpinBox(grpProdForm);
        spnRetDinar->setObjectName(QString::fromUtf8("spnRetDinar"));
        spnRetDinar->setMaximum(9999999.000000000000000);
        spnRetDinar->setDecimals(0);

        prodFormGrid->addWidget(spnRetDinar, 4, 2, 1, 1);

        label20 = new QLabel(grpProdForm);
        label20->setObjectName(QString::fromUtf8("label20"));

        prodFormGrid->addWidget(label20, 5, 5, 1, 1);

        spnStock = new QDoubleSpinBox(grpProdForm);
        spnStock->setObjectName(QString::fromUtf8("spnStock"));
        spnStock->setMinimum(-999999.000000000000000);
        spnStock->setMaximum(999999.000000000000000);
        spnStock->setDecimals(2);

        prodFormGrid->addWidget(spnStock, 5, 4, 1, 1);


        prodTabLayout->addWidget(grpProdForm);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        btnSaveProd = new QPushButton(tabProducts);
        btnSaveProd->setObjectName(QString::fromUtf8("btnSaveProd"));
        btnSaveProd->setMinimumHeight(32);

        hboxLayout1->addWidget(btnSaveProd);

        btnNewProd = new QPushButton(tabProducts);
        btnNewProd->setObjectName(QString::fromUtf8("btnNewProd"));
        btnNewProd->setMinimumHeight(32);

        hboxLayout1->addWidget(btnNewProd);

        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem2);

        btnDeleteProd = new QPushButton(tabProducts);
        btnDeleteProd->setObjectName(QString::fromUtf8("btnDeleteProd"));
        btnDeleteProd->setMinimumHeight(32);

        hboxLayout1->addWidget(btnDeleteProd);


        prodTabLayout->addLayout(hboxLayout1);

        tblProducts = new QTableWidget(tabProducts);
        if (tblProducts->columnCount() < 7)
            tblProducts->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tblProducts->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tblProducts->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tblProducts->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tblProducts->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tblProducts->setHorizontalHeaderItem(4, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tblProducts->setHorizontalHeaderItem(5, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tblProducts->setHorizontalHeaderItem(6, __qtablewidgetitem12);
        tblProducts->setObjectName(QString::fromUtf8("tblProducts"));
        tblProducts->setSelectionBehavior(QAbstractItemView::SelectRows);

        prodTabLayout->addWidget(tblProducts);

        tabWidget->addTab(tabProducts, QString());

        rootLayout->addWidget(tabWidget);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        btnSave = new QPushButton(SystemSettings);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setMinimumHeight(36);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Tahoma"));
        font2.setPointSize(11);
        font2.setBold(true);
        btnSave->setFont(font2);

        hboxLayout2->addWidget(btnSave);

        spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem3);

        btnClose = new QPushButton(SystemSettings);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));
        btnClose->setMinimumHeight(36);
        btnClose->setFont(font2);

        hboxLayout2->addWidget(btnClose);


        rootLayout->addLayout(hboxLayout2);


        retranslateUi(SystemSettings);

        QMetaObject::connectSlotsByName(SystemSettings);
    } // setupUi

    void retranslateUi(QDialog *SystemSettings)
    {
        SystemSettings->setWindowTitle(QCoreApplication::translate("SystemSettings", "\330\243\330\271\330\257\330\247\330\257 \330\247\331\204\331\206\330\270\330\247\331\205", nullptr));
        lblCompanyName->setText(QCoreApplication::translate("SystemSettings", "\330\247\330\263\331\205 \330\247\331\204\330\264\330\261\331\203\330\251 / \330\247\331\204\331\205\330\255\331\204:", nullptr));
        lblExchRate->setText(QCoreApplication::translate("SystemSettings", "\330\263\330\271\330\261 \330\265\330\261\331\201 \330\247\331\204\330\257\331\210\331\204\330\247\330\261:", nullptr));
        label->setText(QCoreApplication::translate("SystemSettings", "\330\261\330\265\331\212\330\257 \330\247\331\204\330\265\331\206\330\257\331\210\331\202 \330\247\331\204\330\243\331\210\331\204\331\212 $:", nullptr));
        txtCashboxDollar->setText(QCoreApplication::translate("SystemSettings", "0", nullptr));
        label1->setText(QCoreApplication::translate("SystemSettings", "\330\261\330\265\331\212\330\257 \330\247\331\204\330\265\331\206\330\257\331\210\331\202 \330\247\331\204\330\243\331\210\331\204\331\212 \330\257\331\212\331\206\330\247\330\261:", nullptr));
        txtCashboxDinar->setText(QCoreApplication::translate("SystemSettings", "0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabGeneral), QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\245\330\271\330\257\330\247\330\257\330\247\330\252 \330\247\331\204\330\271\330\247\331\205\330\251", nullptr));
        grpCustForm->setTitle(QCoreApplication::translate("SystemSettings", "\330\250\331\212\330\247\331\206\330\247\330\252 \330\247\331\204\330\264\330\256\330\265 / \330\247\331\204\330\264\330\261\331\203\330\251", nullptr));
        label2->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\247\330\263\331\205:", nullptr));
        label3->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\205\331\206\330\267\331\202\330\251:", nullptr));
        label4->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\207\330\247\330\252\331\201:", nullptr));
        label5->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\271\331\206\331\210\330\247\331\206:", nullptr));
        label6->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\261\330\265\331\212\330\257 $:", nullptr));
        label7->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\261\330\265\331\212\330\257 \330\257\331\212\331\206\330\247\330\261:", nullptr));
        label8->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\206\331\210\330\271:", nullptr));
        cmbCustType->setItemText(0, QCoreApplication::translate("SystemSettings", "\330\264\330\256\330\265", nullptr));
        cmbCustType->setItemText(1, QCoreApplication::translate("SystemSettings", "\330\264\330\261\331\203\330\251", nullptr));

        label9->setText(QCoreApplication::translate("SystemSettings", "\331\205\331\204\330\247\330\255\330\270\330\247\330\252:", nullptr));
        btnSaveCust->setText(QCoreApplication::translate("SystemSettings", "\360\237\222\276 \330\255\331\201\330\270", nullptr));
        btnNewCust->setText(QCoreApplication::translate("SystemSettings", "+ \330\254\330\257\331\212\330\257", nullptr));
        btnDeleteCust->setText(QCoreApplication::translate("SystemSettings", "\342\234\226 \330\255\330\260\331\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tblCustomers->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\247\330\263\331\205", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tblCustomers->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\205\331\206\330\267\331\202\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tblCustomers->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\207\330\247\330\252\331\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tblCustomers->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\261\330\265\331\212\330\257 $", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tblCustomers->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\261\330\265\331\212\330\257 \330\257", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tblCustomers->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\206\331\210\330\271", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabCustomers), QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\243\330\264\330\256\330\247\330\265 \331\210\330\247\331\204\330\264\330\261\331\203\330\247\330\252", nullptr));
        grpProdForm->setTitle(QCoreApplication::translate("SystemSettings", "\330\250\331\212\330\247\331\206\330\247\330\252 \330\247\331\204\331\205\330\247\330\257\330\251", nullptr));
        label10->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\250\330\247\330\261\331\203\331\210\330\257:", nullptr));
        label11->setText(QCoreApplication::translate("SystemSettings", "\330\247\330\263\331\205 \330\247\331\204\331\205\330\247\330\257\330\251:", nullptr));
        label12->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\205\330\254\331\205\331\210\330\271\330\251:", nullptr));
        label13->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\206\331\210\330\271:", nullptr));
        label14->setText(QCoreApplication::translate("SystemSettings", "\330\263\330\271\330\261 \330\247\331\204\331\203\331\204\331\201\330\251:", nullptr));
        label15->setText(QCoreApplication::translate("SystemSettings", "\330\252\330\271\330\250\330\246\330\251 \330\247\331\204\331\203\330\261\330\252\331\210\331\206:", nullptr));
        label16->setText(QCoreApplication::translate("SystemSettings", "\330\254\331\205\331\204\330\251 $:", nullptr));
        label17->setText(QCoreApplication::translate("SystemSettings", "\331\205\331\201\330\261\330\257 $:", nullptr));
        label18->setText(QCoreApplication::translate("SystemSettings", "\330\254\331\205\331\204\330\251 \330\257:", nullptr));
        label19->setText(QCoreApplication::translate("SystemSettings", "\331\205\331\201\330\261\330\257 \330\257:", nullptr));
        label20->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\261\330\265\331\212\330\257 \330\247\331\204\330\243\331\210\331\204\331\212:", nullptr));
        btnSaveProd->setText(QCoreApplication::translate("SystemSettings", "\360\237\222\276 \330\255\331\201\330\270", nullptr));
        btnNewProd->setText(QCoreApplication::translate("SystemSettings", "+ \330\254\330\257\331\212\330\257", nullptr));
        btnDeleteProd->setText(QCoreApplication::translate("SystemSettings", "\342\234\226 \330\255\330\260\331\201", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tblProducts->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\250\330\247\330\261\331\203\331\210\330\257", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tblProducts->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("SystemSettings", "\330\247\330\263\331\205 \330\247\331\204\331\205\330\247\330\257\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tblProducts->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\205\330\254\331\205\331\210\330\271\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tblProducts->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\203\331\204\331\201\330\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tblProducts->horizontalHeaderItem(4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("SystemSettings", "\330\254\331\205\331\204\330\251 $", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tblProducts->horizontalHeaderItem(5);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("SystemSettings", "\331\205\331\201\330\261\330\257 $", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tblProducts->horizontalHeaderItem(6);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("SystemSettings", "\330\247\331\204\330\261\330\265\331\212\330\257", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabProducts), QCoreApplication::translate("SystemSettings", "\330\247\331\204\331\205\331\210\330\247\330\257", nullptr));
        btnSave->setText(QCoreApplication::translate("SystemSettings", "\360\237\222\276 \330\255\331\201\330\270 \330\247\331\204\330\245\330\271\330\257\330\247\330\257\330\247\330\252", nullptr));
        btnClose->setText(QCoreApplication::translate("SystemSettings", "\342\234\226 \330\245\330\272\331\204\330\247\331\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SystemSettings: public Ui_SystemSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEMSETTINGS_H
