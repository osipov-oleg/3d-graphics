#include "mainwindow.h"
#include <QApplication>
#include <myglwidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    MyGLWidget w(nullptr);
    w.show();

    return a.exec();
}

