/**********************************************************
 * @file mainwindow.h
 * 
 * @brief Public class for MainWindow.
 * Holds all the common interfaces for the GUI user inputs
 * and graphical outputs.
 * 
 * @author Sergio Chavarria
 * 
 *********************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/***************** Include files. ***********************/

// Project includes.
#include <QMainWindow>
#include "frame_buffer_tester.h"

// Qt tools includes.
#include <QPainter>
#include <QImage>
#include <QElapsedTimer>

/***************** Namespaces. ***********************/
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


/***************** Global Classes. ***********************/

/**
 * @brief MainWindow class. Defines the GUI application interfaces.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /***************** Private Class Functions. ***********************/
    
    /**
     * @brief Default constructor for QWidget.
     * @param parent Widget parent class.
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    /**
     * @brief Paint Window override event
     * Called when updates to the main window
     * graphics are required, such as resize
     * events, and direct calls to ui->update();
     * 
     * @param event Paint Event data.
     */
    void paintEvent(QPaintEvent *event) override;

private slots:

    /***************** Private Slot Functions. ***********************/

    /**
     * @brief Slot for Load ROM action.
     * 
     * This slot is called whenever the user
     * pressed the 'Load ROM' option in the 
     * menu bar.
     * 
     * The slot should call the required interface
     * to load the ROM file into the emulator.
     */
    void on_actionLoad_ROM_triggered();

    /**
     * @brief Slot for (Re)Start Game.
     * 
     * This slot is called whenever the user
     * pressed the '(Re)Start Game' option in the 
     * menu bar.
     * 
     * The slot should call the emulator interface
     * to start or restar the game.
     */
    void on_action_Re_Start_Game_triggered();

    /**
     * @brief Slot for Pause Game.
     * 
     * This slot is called whenever the user
     * pressed the 'Pause Game' option in the 
     * menu bar.
     * 
     * The slot should call the emulator to pause
     * or resume the game.
     */
    void on_actionPause_Game_triggered();

    /**
     * @brief Slot for Insert Coin.
     * 
     * This slot is called whenever the user
     * pressed the 'Insert Coin' option in the 
     * menu bar.
     * 
     * The slot should call the emulator to simulate
     * a coin insertion into the arcade machine.
     */
    void on_actionInsert_Coin_triggered();

    /**
     * @brief Slot for Run Video Test.
     * 
     * This slot is called whenever the user
     * pressed the 'Run Video Test' option in the 
     * menu bar.
     * 
     * This slot is used for debugging purposes,
     * starting the frame video tests. Not part
     * of the actual emulation.
     */
    void on_actionRun_Video_Test_triggered();

    /**
     * @brief Slot for receiving frame buffers.
     * 
     * Custom slot used as an open interface to receive
     * new video frame buffers.
     * 
     * This slot is open ended, so that it can be signaled
     * by a testing application, or the real emulation class.
     * 
     * @param buffer Video buffer for single frame.
     */
    void on_frameBufferReceived(const frame_buffer_t *buffer);


private:

    /***************** Private class functions. ***********************/

    /**
     * @brief Auxiliary function to calculate current FPS.
     * 
     * Prints into the qDebug console the approximated
     * frames per second measured between calls to 
     * on_frameBufferReceived.
     */
    void calculateFPS(void);


    /***************** Private class elements. ***********************/

    /**
     * @brief Pointer to UI interfaces.
     */
    Ui::MainWindow *ui;

    /**
     * @brief Currently rendered image in the UI.
     * 
     * To emulate the video transitions, this image must
     * be updated by the frame buffer processing.
     */
    QImage currentRenderedImage;

    /**
     * @brief Internal flag for video testing.
     * 
     * This flag is set when video test mode is active.
     */
    bool bufferTestModeActive = false;

    /**
     * @brief Local pointer to frame buffer tester.
     * 
     * This class contains the test methods used to run
     * the frame buffer at an approximate of 60 FPS.
     */
    frame_buffer_tester::FrameBufferTester *bufferTester = nullptr;

    /**
     * @brief Auxiliary timer for FPS calculation.
     * 
     * Used to display the FPS calculation in the qDebug terminal.
     */
    QElapsedTimer fpsTimer;
};
#endif // MAINWINDOW_H
