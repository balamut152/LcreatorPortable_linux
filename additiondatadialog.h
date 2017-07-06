#ifndef ADDITIONDATADIALOG_H
#define ADDITIONDATADIALOG_H
#include <QtGui>
#include <QtWidgets>
#include <QDialog>
#include "ui_additiondatadialog.h"
class AdditionDataDialog : public QDialog, public Ui::AdditionDataDialog{
Q_OBJECT
public:
AdditionDataDialog(QWidget *parent = 0) : QDialog(parent){
    setupUi(this);
}
};
#endif // ADDITIONDATADIALOG_H
