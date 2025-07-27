#include "controller.hpp"
#include "emulator.hpp"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Emulator model;
    Controller controller(&model, &w);

    w.show();
    return a.exec();
}
