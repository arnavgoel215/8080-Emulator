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
#include <QFileDialog> // For loading ROM path.
#include <QMessageBox> // For displaying failed attempt to load ROM.
#include <QTimer> // For single shot 'C' key presses.

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
    currentRenderedImage = QImage(":/resources/frame1.png");

    // Color mask used to set up screen colors.
    colorMask = QImage(":/resources/color_mask.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLoad_ROM_triggered()
{
    // Opens selection dialog, but only to select a folder, and not a file.
    QString selectedDirectory = QFileDialog::getExistingDirectory(
        this,
        "Select the folder where invaders.e/.f/.g/.h are",
        QDir::currentPath(),
        QFileDialog::ShowDirsOnly
    );

    if (false == selectedDirectory.isEmpty())
    {
        bool validPath = false;
        emit sendRomPath(selectedDirectory.toStdString(), &validPath);

        if (true == validPath)
        {
            // Enable Game options, disable test options.
            ui->menuDebug->setDisabled(true);
            ui->menuGame->setDisabled(false);
            ui->actionClose_ROM->setDisabled(false);
            ui->actionLoad_ROM->setDisabled(true);

            romIsLoaded = true;
        }
        else
        {
            QMessageBox errorBox;
            errorBox.warning(
                nullptr,
                "ROM file not found in selected folder",
                "Please select a folder where the ROM files are included which are:\n"
                "invaders.e, invaders.f, invaders.g, and invaders.h"
            );
        }
    }
}

void MainWindow::on_actionClose_ROM_triggered()
{
    // Close game, and reset emulator.
    emit sendCloseGameSignal();

    // Disable Game options, and enable test options.
    ui->menuDebug->setDisabled(false);
    ui->menuGame->setDisabled(true);
    ui->actionClose_ROM->setDisabled(true);
    ui->actionLoad_ROM->setDisabled(false);

    // Disable any other game actions.
    romIsLoaded = false;
}

void MainWindow::on_action_Re_Start_Game_triggered()
{
    // Reset emulator.
    emit sendResetSignal();
}

void MainWindow::on_actionPause_Game_triggered()
{
    // Toggle pause.
    emit sendToggleRunSignal();
}

void MainWindow::on_actionInsert_Coin_triggered()
{
    // Emulate a press of the 'C' key for 300 ms.
    // We use 300 ms just to simulate a normal key pulse.
    // NOTE: It might not be the most optimal to hardcode the 'C' press.
    // In the future it might be better to abstract all the game
    // actions before sending them to the controller.
    pulseKey(Qt::Key_C, KEY_PULSE_TIME_MS, ui->actionInsert_Coin);
}

void MainWindow::on_actionP1_Start_triggered()
{
    // Simulate pulsing enter key.
    pulseKey(Qt::Key_Enter, KEY_PULSE_TIME_MS, ui->actionP1_Start);
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
    QSize scaledSize = QSize(width(), height() - MENU_BAR_HEIGHT);
    QImage scaledImage = currentRenderedImage.scaled(scaledSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    QImage scaledMask = colorMask.scaled(scaledSize, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    // Update monochrome image.
    painter.drawImage(UPPER_LEFT_CORNER, scaledImage);

    // Multiply the mask to pain the white pixels with all the colors from the mask.
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    painter.drawImage(UPPER_LEFT_CORNER, scaledMask);
}

void MainWindow::on_frameBufferReceived(const frame_buffer_t *buffer)
{
    if (nullptr == buffer)
    {
        return;
    }

    // Directly translate the contents of the frame buffer to a bitmap.
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Block any key events until game is fully loaded.
    if (false == romIsLoaded)
    {
        return;
    }

    // Avoid repeated events when holding the input.
    if (false == event->isAutoRepeat())
    {
        // Catch UI shortcuts.
        switch(event->key())
        {
            // Pause Game.
            case Qt::Key_P:
            {
                emit sendToggleRunSignal();
                break;
            }
            // (Re)Start Game.
            case Qt::Key_R:
            {
                emit sendResetSignal();
            }
            // Any other keys are redirected to controller.
            default:
            {
                emit sendKeySignal(event->key(), true);
            }
        }

        qDebug() << event->text() << "has been pressed";
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // Block any key events until game is fully loaded.
    if (false == romIsLoaded)
    {
        return;
    }

    // Avoid repeated events when holding the input.
    if (false == event->isAutoRepeat())
    {
        emit sendKeySignal(event->key(), false);
        qDebug() << event->text() << "has been released";
    }
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

void MainWindow::pulseKey(int key, unsigned int milliseconds, QAction *blockAction)
{
    // Create a temporary timer that will generate a callback
    // that will release a key state.
    QTimer *releaseKeyTimer = new QTimer;
    releaseKeyTimer->setSingleShot(true);
    releaseKeyTimer->setInterval(milliseconds);

    // Set key state first.
    emit sendKeySignal(key, true);

    // Temporarily disable option.
    blockAction->setDisabled(true);

    // Call up single shot timer to release key later.
    connect(releaseKeyTimer, &QTimer::timeout, this, [=](){
        emit sendKeySignal(key, false);

        // Reenable menu option.
        blockAction->setDisabled(false);

        // Deallocate timer.
        delete releaseKeyTimer;
    });
    releaseKeyTimer->start();
}
