#include "Notepad.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//Creates an instance of QApplication
    MainWindow w; //Creates an instance of Mainwindow
    w.show(); //makes window visible on screen
    return a.exec();//Enter event loop and keep the app running
}
