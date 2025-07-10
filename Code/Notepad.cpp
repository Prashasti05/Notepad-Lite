#include "Notepad.h"
#include "ui_Notepad.h"
#include "form.h"

#include <QFile>
#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextStream>
#include <QMessageBox>
#include <QFontDialog>
#include <QFont>
#include <QColorDialog>
#include <QColor>
#include <QPalette>

#include<QLabel>
#include<QDate>
#include <QApplication>  

#include"findreplacedialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->textEdit); 

    connect(ui->textEdit,&QTextEdit::currentCharFormatChanged,this,[=](const QTextCharFormat &format){
    ui->actionbold->setChecked(format.fontWeight()==QFont::Bold);
    ui->actionItalic->setChecked(format.fontItalic());
    });

    
   dateLabel =new QLabel(this);
   QString formattedDate= QDate::currentDate().toString("dddd,MMMM d, yyyy");        //dddd-day; MMMM-mnth; d-date; yyyy-year

   dateLabel->setText("📅" +formattedDate);
   dateLabel->setStyleSheet("font-weight:bold; font-size:14px; padding:5px;");

   statusBar()->addPermanentWidget(dateLabel);
}

MainWindow::~MainWindow()
{
    delete ui; 
}


void MainWindow::on_actionNew_triggered() 
{
    File_path=""; 
    ui->textEdit->setText("");
}

void MainWindow::on_actionOpen_triggered()
{
    QString file_name= QFileDialog::getOpenFileName(this,"Open the File");    
    QFile file(file_name);                                                
    File_path=file_name;
    if(!file.open(QFile::ReadOnly|QFile::Text)){                       
        QMessageBox::warning(this,"Error Box","File Not Open");
        return;
    }
    QTextStream in(&file);     
    QString text = in.readAll();  
    ui->textEdit->setHtml(text);   
    file.close();
}

void MainWindow::on_actionSave_triggered()
{
    QFile file(File_path);  
    if(!file.open(QFile::WriteOnly|QFile::Text)){        
        QMessageBox::warning(this,"Error Box","File Not Open");
        return;
    }
    QTextStream out(&file);            
    QString text = ui->textEdit->toHtml(); 
    out<<text;               
    file.flush();
    file.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    QString file_name= QFileDialog::getSaveFileName(this,"Open the File"); 
    QFile file(file_name);                          
    File_path=file_name;    

    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"Error Box","File Not Open"); 
        return;
    }
    QTextStream out(&file);                
    QString text = ui->textEdit->toHtml();  
    out<<text;                            
    file.flush();
    file.close();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut(); 
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();    
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();     
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;                         
    printer.setPrinterName("Printer name");   
    QPrintDialog dialog(&printer,this);      

    if(dialog.exec()==QDialog::Rejected)   
     return;

    ui->textEdit->print(&printer);       
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;                          
    QFont font=QFontDialog::getFont(&ok,this);
    if(ok){
        ui->textEdit->setFont(font);     
    }
    else{                        
        return;
    }
}

void MainWindow::on_actionAbout_Notepad_triggered()
{
    QString about_text;
    about_text += " Notepad++ Lite (Built with Qt)\n";
    about_text += "Version: 1.0.0\n";
    about_text += "Author: Prashasti Paul\n";
    about_text += "Created with using Qt and C++\n\n";
    about_text += "Features:\n";
    about_text += "• Dark/Light Theme Toggle\n";
    about_text += "• Font customization\n";
    about_text += "• Simple and clean UI\n";
    about_text += "• File editing and formatting\n\n";
    about_text += "(C) 2025 Notepad Lite. All rights reserved.";

    QMessageBox::about(this,"About Notepad Lite",about_text);     
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();  
}
    
void MainWindow::on_btnTextcolor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Choose Text Color");    
    if(color.isValid()){
        ui->textEdit->setTextColor(color);    
    }
}

void MainWindow::on_actionBackground_color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose BG Color");  

    if (color.isValid()) {          
        QString style = QString("QTextEdit { background-color: %1; }").arg(color.name());
        ui->textEdit->setStyleSheet(style);
    }
}

void MainWindow::on_actionbold_toggled(bool checked)
{
    QTextCursor cursor = ui->textEdit->textCursor(); 
    QTextCharFormat format;      
    format.setFontWeight(checked ? QFont::Bold : QFont::Normal); 

    if (cursor.hasSelection()){ 
        cursor.mergeCharFormat(format); 
    }
    else{
        ui->textEdit->mergeCurrentCharFormat(format);
    }
}

void MainWindow::on_actionItalic_toggled(bool checked)
{
    QTextCursor cursor = ui->textEdit->textCursor(); 
    QTextCharFormat format;     
    format.setFontItalic(checked);

    if (cursor.hasSelection()){  
        cursor.mergeCharFormat(format);  
    }
    else{
       QTextCharFormat currentFormat = ui->textEdit->currentCharFormat();  
        currentFormat.setFontItalic(checked);          
        ui->textEdit->setCurrentCharFormat(currentFormat); 
    }
}

void MainWindow::on_actionToggleDarkMode_toggled(bool checked)
{
    if (checked) {           
        QFile file(":/img/darktheme.qss.txt");    
        if (file.open(QFile::ReadOnly | QFile::Text)) {   
            qApp->setStyleSheet(file.readAll());  
        }    
    } else {        
        qApp->setStyleSheet("");  
    }
}

void MainWindow::on_actionFindReplace_triggered()
{
    if (!findDialog) {      
        findDialog = new FindReplaceDialog(this); 

        //--find code--
        connect(findDialog, &FindReplaceDialog::findNext, this, [=](const QString &text) {
            QTextDocument *doc = ui->textEdit->document();   
            QTextCursor cursor = ui->textEdit->textCursor();  

            cursor.setPosition(cursor.selectionEnd());

            QTextCursor found = doc->find(text, cursor); 

            if (!found.isNull()) {  
                ui->textEdit->setTextCursor(found);   

                QTextCursor restartCursor(doc);   
                QTextCursor wrapped = doc->find(text, restartCursor);  

                if (!wrapped.isNull()) {      
                    ui->textEdit->setTextCursor(wrapped);  

                    QTextCursor advance = wrapped;   
                    advance.setPosition(wrapped.selectionEnd());  
                    ui->textEdit->setTextCursor(advance);   

                } else {   
                    QMessageBox::information(this, "Find", "Text not found.");
                }
            }
        });

        //--replace code---
        
        connect(findDialog, &FindReplaceDialog::replace, this, [=](const QString &findText, const QString &replaceText) {
            QTextDocument *doc = ui->textEdit->document();   
            QTextCursor cursor = doc->find(findText);    

            if(!cursor.isNull()){     
                cursor.beginEditBlock(); 
                cursor.insertText(replaceText);   
                cursor.endEditBlock();      
                ui->textEdit->setTextCursor(cursor); 
            }
            else{
                QMessageBox::information(this,"Replace", "Text not found");
            }
        });

        //---replaceAll code----
        
       connect(findDialog, &FindReplaceDialog::replaceAll, this, [=](const QString &findText, const QString &replaceText) {
            QTextDocument *doc = ui->textEdit->document();    
            QTextCursor cursor(doc);        
            int count = 0; 

            while (!cursor.isNull() && !cursor.atEnd()) {  
                cursor = doc->find(findText, cursor); 
                if (!cursor.isNull()) {      
                    cursor.insertText(replaceText);     
                    count++;    
                }
            }
            QMessageBox::information(this, "Replace All", QString("Replaced %1 occurrence(s).").arg(count));   
        });
    }

    findDialog->show();    
    findDialog->raise();   
    findDialog->activateWindow();  
}






