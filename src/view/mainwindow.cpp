/**********************************************************
 * @file mainwindow.cpp
 * 
 * @brief Public class for MainWindow.
 * Holds all the common interfaces for the GUI user inputs
 * and graphical outputs.
 * 
 * @author Sergio Chavarria
 * 
 *********************************************************/

/***************** Include files. ***********************/

// Project includes.
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "frame_buffer_tester.h"

// Qt tools includes.
#include <QDebug>

/***************** Macros and defines. ***********************/

/***************** Namespaces. ***********************/

/***************** Local Classes. ***********************/

/***************** Local Functions. ***********************/

/***************** Global Class Functions. ***********************/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Default initial image. Example frame of the game.
    currentRenderedImage = QImage(":/images/frames/frame1.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_ROM_triggered()
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

void MainWindow::on_actionRun_Video_Test_triggered()
{
    // Toggle buffer test mode.
    bufferTestModeActive ^= true;

    if (true == bufferTestModeActive)
    {
        // Toggle the UI text to stop the video test next time 
        // it is clicked.
        ui->actionRun_Video_Test->setText("Stop Video Test");

        // Disable all the game options.
        ui->menuFile->setDisabled(true);
        ui->menuGame->setDisabled(true);

        this->setWindowTitle("Space Invaders (Test Video Mode)");

        // Create new buffer tester object, that runs independently
        // of this class.
        bufferTester = new frame_buffer_tester::FrameBufferTester();

        // Connect the buffer received signal to the new created class.
        // The new object will generate frame buffers at a rate of 60 Hz.
        connect(
            bufferTester, &frame_buffer_tester::FrameBufferTester::updateFrameBuffer,
            this, &MainWindow::on_frameBufferReceived
        );
    }
    else
    {
        // Restore original action label.
        ui->actionRun_Video_Test->setText("Run Video Test");

        // Enable back all the game options.
        ui->menuFile->setDisabled(false);
        ui->menuGame->setDisabled(false);

        // Restor window title.
        this->setWindowTitle("Space Invaders");

        // Clean up all data related to bufferTester. 
        if (nullptr != bufferTester)
        {
            disconnect(
                bufferTester, &frame_buffer_tester::FrameBufferTester::updateFrameBuffer,
                this, &MainWindow::on_frameBufferReceived
            );

            delete bufferTester;
            bufferTester = nullptr;
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    // Get the height of the menu bar.
    // We get these values in order to not paint over the menu bar, and
    // always keep the graphics tied to the same corner, even when
    // the window is resized.
    static const int MENU_BAR_HEIGHT = this->ui->menuBar->geometry().height();
    static const QPoint UPPER_LEFT_CORNER = QPoint(0, MENU_BAR_HEIGHT);

    QPainter painter(this);
    
    // Scale the image on every paint event. The pixel resolution
    // of the game stays the same, but it's scaled up to any size of the screen.
    // NOTE: We use FastTransformation to avoid bilinear interpolation,
    // otherwise the pixels would look all fuzzy.
    auto scaled_image = currentRenderedImage.scaled(QSize(width(), height() - MENU_BAR_HEIGHT), Qt::IgnoreAspectRatio, Qt::FastTransformation);

    // Update image.
    painter.drawImage(UPPER_LEFT_CORNER, scaled_image);
}

void MainWindow::on_frameBufferReceived(const frame_buffer_t *buffer)
{
    // Direclty translate the contents of the frame buffer to a bitmap.
    // NOTE: The good thing is that the frame data from the original
    // space invaders game runs at 8 pixels per byte, exactly the same
    // as the bitmap structure.
    // The height and width values are swapped when processing the buffer.
    QBitmap bitmap = QBitmap::fromData(QSize(FRAME_HEIGHT, FRAME_WIDTH), buffer->begin(), QImage::Format_MonoLSB);

    // Rotate 90 degrees to the left. The original arcade machine
    // has its CRT tilted 90 degrees to the right, so we neeed to 
    // compensate for it.
    auto newImage = bitmap.toImage().transformed(QTransform().rotate(-90));

    // PNG monochrome images have 0 mapped to white, and 1 to black,
    // so we need to invert the image.
    newImage.invertPixels(QImage::InvertRgb);

    // Update currently rendered image reference.
    currentRenderedImage = newImage;

    // Debug print FPS.
    calculateFPS();

    // Update UI with painted graphics.
    this->update();
}

void MainWindow::calculateFPS(void)
{
    // Get the period from the last time this was called.
    qint64 interFramePeriod = fpsTimer.nsecsElapsed();

    // Restart timer for next frame.
    fpsTimer.start();

    // Calculate and print estimated FPS.
    double fpsEstimation = (1 / (double)interFramePeriod) * 1000000000;
    qDebug() << "FPS: " << QString::number(fpsEstimation);
}


