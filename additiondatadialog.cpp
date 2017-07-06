#include "additiondatadialog.h"
#include "ui_additiondatadialog.h"

AdditionDataDialog::AdditionDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdditionDataDialog)
{
    ui->setupUi(this);
}

AdditionDataDialog::~AdditionDataDialog()
{
    delete ui;
}
