#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QDialog>
#include<QString>

namespace Ui {
class FindReplaceDialog;
}

class FindReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindReplaceDialog(QWidget *parent = nullptr);
    ~FindReplaceDialog();

signals:
    void findNext(const QString &text);
    void replace(const QString &text,const QString &replacement);
    void replaceAll(const QString &text, const QString &replacement);

private slots:
    void on_btnFind_clicked();
    void on_btnReplace_clicked();
    void on_btnReplaceAll_clicked();

private:
    Ui::FindReplaceDialog *ui;
};

#endif // FINDREPLACEDIALOG_H
