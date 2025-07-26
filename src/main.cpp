#include "controller/controller.hpp"
#include "model/emulator.hpp"
#include "view/mainwindow.h"

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
