#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mw;
    mw.setFixedSize(700, SIZE );
    mw.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    mw.show();
    return  a.exec();
}
