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
#include <QApplication>  //for exiting app

#include"findreplacedialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setCentralWidget(ui->textEdit); //makes text edit,the main content area

    connect(ui->textEdit,&QTextEdit::currentCharFormatChanged,this,[=](const QTextCharFormat &format){
    ui->actionbold->setChecked(format.fontWeight()==QFont::Bold);
    ui->actionItalic->setChecked(format.fontItalic());
    });

    //connect(ui->btnTextcolor, &QAction::triggered, this ,&MainWindow::on_btnTextcolor_triggered);  //for textclr
   //connect(ui->actionBackground_color, &QAction::triggered, this ,&MainWindow::on_actionBackground_color_triggered);

   //date
   dateLabel =new QLabel(this);
   QString formattedDate= QDate::currentDate().toString("dddd,MMMM d, yyyy");        //ddd-day; MMMM-mnth; d-date; yyyy-year

   dateLabel->setText("📅" +formattedDate);
   dateLabel->setStyleSheet("font-weight:bold; font-size:14px; padding:5px;");

   statusBar()->addPermanentWidget(dateLabel);
}

MainWindow::~MainWindow()
{
    delete ui; //frees allocated memory for UI element
}


void MainWindow::on_actionNew_triggered() //Clears curr doc n resets file path.
{
    File_path=""; //resets curr file path to empty
    ui->textEdit->setText("");//clears the text editor
}


void MainWindow::on_actionOpen_triggered()//Opens an existing text file nd disply its content.
{
    QString file_name= QFileDialog::getOpenFileName(this,"Open the File");    //opens file dialog to open txt file
    QFile file(file_name);                                                //creates QFile obj wid selected file & stores selected file path.
    File_path=file_name;
    if(!file.open(QFile::ReadOnly|QFile::Text)){                       //Tries to open file in read-only text mode. If fails, show error.
        QMessageBox::warning(this,"Error Box","File Not Open");
        return;
    }
    QTextStream in(&file);     //else creates text stream from file
    QString text = in.readAll();  //Reads all file content and displays it in txt editor.
    ui->textEdit->setHtml(text);   //opens formatted text
    file.close();
}

void MainWindow::on_actionSave_triggered()
{
    QFile file(File_path);  // Create QFile with stored path
    if(!file.open(QFile::WriteOnly|QFile::Text)){         //tries to open the existing file in write mode.if fails returns error
        QMessageBox::warning(this,"Error Box","File Not Open");
        return;
    }
    QTextStream out(&file);            // Create output stream
    QString text = ui->textEdit->toHtml(); //gets curr formatted text(with font color ,B,I ) from editor
    out<<text;               //and writes to file
    file.flush();
    file.close();
}


void MainWindow::on_actionSave_as_triggered()
{
    QString file_name= QFileDialog::getSaveFileName(this,"Open the File"); // Show save dialog
    QFile file(file_name);                          // Create QFile object
    File_path=file_name;    // Update file path

    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"Error Box","File Not Open"); // Try to open for writing,but error
        return;
    }
    QTextStream out(&file);                //output stream
    QString text = ui->textEdit->toHtml();  //get curr formatted text
    out<<text;                            //write to file
    file.flush();
    file.close();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();  //cuts selected txt to clipborad
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();    //copy selected txt to clipborad
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();     // Paste from clipboard into QTextEdit
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

 // Opens a print dialog and sends content to the printer
void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;                         // Create printer object
    printer.setPrinterName("Printer name");   // Set printer name (can be ignored or customized)
    QPrintDialog dialog(&printer,this);       //create print dialog

    if(dialog.exec()==QDialog::Rejected)   //if user cancels
     return;

    ui->textEdit->print(&printer);        // Print the text content

}

// Opens a font dialog and applies selected font
void MainWindow::on_actionFont_triggered()
{
    bool ok;                          // Status of selection
    QFont font=QFontDialog::getFont(&ok,this);//show font picker dialog
    if(ok){
        ui->textEdit->setFont(font);     //apply selected font
    }
    else{                         //do nothing if user cancels
        return;
    }
}

// Shows an About box with app information
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

    QMessageBox::about(this,"About Notepad Lite",about_text);     // Show message box
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();   //closes the application
}
     // Opens color picker to choose and apply text color
void MainWindow::on_btnTextcolor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Choose Text Color");   //opens color dialog
    if(color.isValid()){
        ui->textEdit->setTextColor(color);    // Apply selected color to text
    }
}
        // Opens color picker for background color
void MainWindow::on_actionBackground_color_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose BG Color");  //opens color dialog

    if (color.isValid()) {          //if user selected a valid color
        QString style = QString("QTextEdit { background-color: %1; }").arg(color.name());
        ui->textEdit->setStyleSheet(style);
    }
}

void MainWindow::on_actionbold_toggled(bool checked)
{
    QTextCursor cursor = ui->textEdit->textCursor(); //get curr cursor(posn where user is typing or selecting)
    QTextCharFormat format;      //create a txt format obj to do char formattng
    format.setFontWeight(checked ? QFont::Bold : QFont::Normal); //set font wt to bold/normal

    if (cursor.hasSelection()){ //check if any txt is selected
        cursor.mergeCharFormat(format); //apply bold formatting to selected txt only
    }
    else{
        ui->textEdit->mergeCurrentCharFormat(format);// No selection: set format for new txt typed from now on
    }
}

void MainWindow::on_actionItalic_toggled(bool checked)
{
    QTextCursor cursor = ui->textEdit->textCursor(); //get curr cursor
    QTextCharFormat format;     //create a txt format obj
    format.setFontItalic(checked);// Set the italic style/normal

    if (cursor.hasSelection()){  //check if any txt is selected
        cursor.mergeCharFormat(format);  //apply italic formatting to selected txt only
    }
    else{
       QTextCharFormat currentFormat = ui->textEdit->currentCharFormat();  // Get the curr char format
        currentFormat.setFontItalic(checked);           // Modify it to set or unset italic
        ui->textEdit->setCurrentCharFormat(currentFormat);  // No selection: set format for new txt typed from now on
    }
}

//--------------------------------darktheme------------------------------------

void MainWindow::on_actionToggleDarkMode_toggled(bool checked)
{
    if (checked) {           // If the dark mode toggle is ON
        QFile file(":/img/darktheme.qss.txt");    // Create QFile obj to access stylesheet file stored in Qt resources
        if (file.open(QFile::ReadOnly | QFile::Text)) {   // Open the file in read-only text mode
            qApp->setStyleSheet(file.readAll());    // Read entire stylesheet content n apply it to applcation using qApp
        }    
    } else {        // If the dark mode toggle is OFF
        qApp->setStyleSheet("");  // Clear stylesheet to revert to default
    }
}


//---------------------------------------------FIND AND REPLACE CODE---------------

void MainWindow::on_actionFindReplace_triggered()
{
    if (!findDialog) {      // If FindReplaceDialog has not been created yet
        findDialog = new FindReplaceDialog(this); // Create new instance of dialog nd make MainWindow its parent


        //-----------------------find code------------------------------------
        connect(findDialog, &FindReplaceDialog::findNext, this, [=](const QString &text) {
            QTextDocument *doc = ui->textEdit->document();    // Get the QTextDocument
            QTextCursor cursor = ui->textEdit->textCursor();  // Get the current cursor

            cursor.setPosition(cursor.selectionEnd());// Move cursor to end of curr selection to avoid finding same word again

            QTextCursor found = doc->find(text, cursor);  // Search from curr cursor position

            if (!found.isNull()) {  // If search found something
                ui->textEdit->setTextCursor(found);   // move cursor to found location

                QTextCursor restartCursor(doc);    // new restart cursor from beginning of doc
                QTextCursor wrapped = doc->find(text, restartCursor);  //search again from top

                if (!wrapped.isNull()) {      // If text is found
                    ui->textEdit->setTextCursor(wrapped);   // Move cursor to that found loc

                    QTextCursor advance = wrapped;   // Create a new cursor (copy of the wrapped one)
                    advance.setPosition(wrapped.selectionEnd());  // Move it to end of found word
                    ui->textEdit->setTextCursor(advance);   // Set this new position

                } else {   // Show message if nothing was found
                    QMessageBox::information(this, "Find", "Text not found.");
                }
            }
        });


        //-----------------------replace code------------------------------------

        connect(findDialog, &FindReplaceDialog::replace, this, [=](const QString &findText, const QString &replaceText) {
            QTextDocument *doc = ui->textEdit->document();   // Get the QTextDocument
            QTextCursor cursor = doc->find(findText);    // Search for first occu of findText

            if(!cursor.isNull()){     // If found
                cursor.beginEditBlock(); // Begin a grouped undo command
                cursor.insertText(replaceText);   // Replace the found text
                cursor.endEditBlock();      //end the grouped undo command
                ui->textEdit->setTextCursor(cursor); // Move the cursor to replaced text
            }
            else{
                QMessageBox::information(this,"Replace", "Text not found");
            }
        });


        //-----------------------replaceAll code------------------------------------
       connect(findDialog, &FindReplaceDialog::replaceAll, this, [=](const QString &findText, const QString &replaceText) {
            QTextDocument *doc = ui->textEdit->document();    // Get the QTextDocument
            QTextCursor cursor(doc);        // Create a cursor starting at beginning of doc
            int count = 0; // Count how many replacements are made

            while (!cursor.isNull() && !cursor.atEnd()) {  // Loop until cursor reaches end
                cursor = doc->find(findText, cursor);  // Search for the next occurrence
                if (!cursor.isNull()) {       // If found
                    cursor.insertText(replaceText);      //replace it
                    count++;    //increment the counter
                }
            }
            QMessageBox::information(this, "Replace All", QString("Replaced %1 occurrence(s).").arg(count));   // Show how many replacements were made
        });
    }

    findDialog->show();    // Show the FindReplaceDialog window
    findDialog->raise();   // Raise it above other windows
    findDialog->activateWindow();   // Focus the dialog so user can start typing
}






