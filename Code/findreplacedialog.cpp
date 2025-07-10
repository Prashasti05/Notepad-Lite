#include "findreplacedialog.h"
#include "ui_findreplacedialog.h"

FindReplaceDialog::FindReplaceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindReplaceDialog) 
{
    ui->setupUi(this);
}

FindReplaceDialog::~FindReplaceDialog()
{
    delete ui;
}

void FindReplaceDialog::on_btnFind_clicked()
{
    emit findNext(ui->lineEditFind->text());
}

void FindReplaceDialog::on_btnReplace_clicked()
{
    emit replace(ui->lineEditFind->text(),ui->lineEditReplace->text());
}

void FindReplaceDialog::on_btnReplaceAll_clicked()
{
    emit replaceAll(ui->lineEditFind->text(),ui->lineEditReplace->text());
}

