/**********************************************************
 * @file view_main.cpp
 * 
 * @brief Sample file to run self contained Qt application GUI.
 * 
 * @author: Sergio Chavarria
 * 
 *********************************************************/

/***************** Include files. ***********************/
#include "mainwindow.h"
#include <QApplication>


/***************** Macros and defines. ***********************/

/***************** Namespaces. ***********************/

/***************** Local Classes. ***********************/

/***************** Local Functions. ***********************/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

/***************** Global Class Functions. ***********************/