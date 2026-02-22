#include "mainwindow.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qDebug() << QSqlDatabase::drivers();
    w.show();
    return a.exec();
}
