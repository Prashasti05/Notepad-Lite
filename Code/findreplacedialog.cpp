#include "findreplacedialog.h"
#include "ui_findreplacedialog.h"

FindReplaceDialog::FindReplaceDialog(QWidget *parent)
    : QDialog(parent)//constrctor initializes baseclass QDialog
    , ui(new Ui::FindReplaceDialog) //nd allocates memorry for ui and sets it up
{
    ui->setupUi(this); //Initializes UI elements from the .ui file
}

FindReplaceDialog::~FindReplaceDialog()
{
    delete ui;
}

//find and replace-----------------------------------------
void FindReplaceDialog::on_btnFind_clicked()
{
    emit findNext(ui->lineEditFind->text());
    //emits findnext signal for(retrieves find text input by user in lineedit)
}

void FindReplaceDialog::on_btnReplace_clicked()
{
    emit replace(ui->lineEditFind->text(),ui->lineEditReplace->text());
    //emits replace signal when both find text and replacement text retrived.
}


void FindReplaceDialog::on_btnReplaceAll_clicked()
{
    emit replaceAll(ui->lineEditFind->text(),ui->lineEditReplace->text());
}

