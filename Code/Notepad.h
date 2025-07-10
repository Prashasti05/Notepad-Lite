#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include<QTextEdit>
#include<QMenuBar>
#include<QMenu>
#include<QAction>
#include<QColorDialog>
#include<QToolBar>
#include<QLabel>   //to show labels like current date

#include "findreplacedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow; 
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT 

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_actionUndo_triggered();

    void on_actionPrint_triggered();

    void on_actionFont_triggered();

    void on_actionAbout_Notepad_triggered();

    void on_actionExit_triggered();

    void on_btnTextcolor_triggered();

    void on_actionBackground_color_triggered();

    void on_actionToggleDarkMode_toggled(bool checked);

    void on_actionFind_triggered();

    void on_actionFindReplace_triggered();

    void on_actionbold_toggled(bool checked);

    void on_actionItalic_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QString File_path;

    QLabel *dateLabel; 
    FindReplaceDialog *findDialog = nullptr;
};
#endif // NOTEPAD_H
