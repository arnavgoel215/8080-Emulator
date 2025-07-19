#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_ROM_triggered()
{
    // TODO.
}


void MainWindow::on_action_Re_Start_Game_triggered()
{
    // TODO.
}


void MainWindow::on_actionPause_Game_triggered()
{
    // TODO.
}


void MainWindow::on_actionInsert_Coin_triggered()
{
    // TODO.
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    /* TODO: This is placeholder code just to display a simple image. */
    QPainter painter(this);

    auto image = QImage(":/images/frames/frame1.png");
    auto scaled_image = image.scaled(QSize(width(), height() - 22), Qt::IgnoreAspectRatio, Qt::FastTransformation);

    painter.drawImage(UPPER_CORNER, scaled_image);
}
