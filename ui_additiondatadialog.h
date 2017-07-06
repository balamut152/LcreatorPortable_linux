/********************************************************************************
** Form generated from reading UI file 'additiondatadialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDITIONDATADIALOG_H
#define UI_ADDITIONDATADIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdditionDataDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *beginLE;
    QLineEdit *endLE;
    QLineEdit *periodLE;
    QLabel *bornLabel;
    QLineEdit *bornLE;
    QLabel *periodLabel;
    QLabel *statLabel;
    QLabel *beginLabel;
    QLineEdit *statLE;
    QLabel *endLabel;

    void setupUi(QDialog *AdditionDataDialog)
    {
        if (AdditionDataDialog->objectName().isEmpty())
            AdditionDataDialog->setObjectName(QStringLiteral("AdditionDataDialog"));
        AdditionDataDialog->resize(445, 329);
        buttonBox = new QDialogButtonBox(AdditionDataDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(330, 30, 81, 61));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(AdditionDataDialog);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(30, 30, 251, 221));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        beginLE = new QLineEdit(gridLayoutWidget);
        beginLE->setObjectName(QStringLiteral("beginLE"));

        gridLayout->addWidget(beginLE, 3, 2, 1, 1);

        endLE = new QLineEdit(gridLayoutWidget);
        endLE->setObjectName(QStringLiteral("endLE"));

        gridLayout->addWidget(endLE, 4, 2, 1, 1);

        periodLE = new QLineEdit(gridLayoutWidget);
        periodLE->setObjectName(QStringLiteral("periodLE"));

        gridLayout->addWidget(periodLE, 2, 2, 1, 1);

        bornLabel = new QLabel(gridLayoutWidget);
        bornLabel->setObjectName(QStringLiteral("bornLabel"));
        QFont font;
        font.setFamily(QStringLiteral("Times New Roman"));
        font.setPointSize(14);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        bornLabel->setFont(font);
        bornLabel->setTextFormat(Qt::PlainText);

        gridLayout->addWidget(bornLabel, 0, 0, 1, 1);

        bornLE = new QLineEdit(gridLayoutWidget);
        bornLE->setObjectName(QStringLiteral("bornLE"));

        gridLayout->addWidget(bornLE, 0, 2, 1, 1);

        periodLabel = new QLabel(gridLayoutWidget);
        periodLabel->setObjectName(QStringLiteral("periodLabel"));
        periodLabel->setFont(font);
        periodLabel->setTextFormat(Qt::PlainText);

        gridLayout->addWidget(periodLabel, 2, 0, 1, 1);

        statLabel = new QLabel(gridLayoutWidget);
        statLabel->setObjectName(QStringLiteral("statLabel"));
        statLabel->setFont(font);
        statLabel->setTextFormat(Qt::PlainText);

        gridLayout->addWidget(statLabel, 1, 0, 1, 1);

        beginLabel = new QLabel(gridLayoutWidget);
        beginLabel->setObjectName(QStringLiteral("beginLabel"));
        beginLabel->setFont(font);
        beginLabel->setTextFormat(Qt::PlainText);

        gridLayout->addWidget(beginLabel, 3, 0, 1, 1);

        statLE = new QLineEdit(gridLayoutWidget);
        statLE->setObjectName(QStringLiteral("statLE"));

        gridLayout->addWidget(statLE, 1, 2, 1, 1);

        endLabel = new QLabel(gridLayoutWidget);
        endLabel->setObjectName(QStringLiteral("endLabel"));
        endLabel->setFont(font);
        endLabel->setTextFormat(Qt::PlainText);

        gridLayout->addWidget(endLabel, 4, 0, 1, 1);

        QWidget::setTabOrder(bornLE, statLE);
        QWidget::setTabOrder(statLE, periodLE);
        QWidget::setTabOrder(periodLE, beginLE);
        QWidget::setTabOrder(beginLE, endLE);

        retranslateUi(AdditionDataDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AdditionDataDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AdditionDataDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AdditionDataDialog);
    } // setupUi

    void retranslateUi(QDialog *AdditionDataDialog)
    {
        AdditionDataDialog->setWindowTitle(QString());
        bornLabel->setText(QApplication::translate("AdditionDataDialog", "\320\223\320\276\320\264 \321\200\320\276\320\266\320\264\320\265\320\275\320\270\321\217:", Q_NULLPTR));
        periodLabel->setText(QApplication::translate("AdditionDataDialog", "\320\241\321\200\320\276\320\272:", Q_NULLPTR));
        statLabel->setText(QApplication::translate("AdditionDataDialog", "\320\241\321\202\320\260\321\202\321\214\321\217:", Q_NULLPTR));
        beginLabel->setText(QApplication::translate("AdditionDataDialog", "\320\235\320\260\321\207\320\260\320\273\320\276 \321\201\321\200\320\276\320\272\320\260:", Q_NULLPTR));
        endLabel->setText(QApplication::translate("AdditionDataDialog", "\320\232\320\276\320\275\320\265\321\206 \321\201\321\200\320\276\320\272\320\260:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AdditionDataDialog: public Ui_AdditionDataDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDITIONDATADIALOG_H
