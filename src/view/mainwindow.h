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
#include <QKeyEvent>

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

    /***************** Public Class Functions. ***********************/
    
    /**
     * @brief Default constructor for QWidget.
     * @param parent Widget parent class.
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    /***************** Override Event Handlers. ***********************/

    /**
     * @brief Paint Window override event
     * Called when updates to the main window
     * graphics are required, such as resize
     * events, and direct calls to ui->update();
     * 
     * @param event Paint Event data.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Override event for key press event.
     * 
     * Any keyboard key press.
     * 
     * @param QKeyEvent event info.
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief Override event for key release event.
     * 
     * Any keyboard key release.
     * 
     * @param QKeyEvent event info.
     */
    void keyReleaseEvent(QKeyEvent *event) override;

public slots:

    /***************** Public Slot Functions. ***********************/

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
     * @brief Player 1 Start Button triggered.
     *
     * This slot is called when the user clicks
     * the 'P1 Start' menu option
     */
    void on_actionP1_Start_triggered();

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
     * @brief Slot for Closing the ROM.
     * 
     * This slot is called from the menu bar
     * option 'Close ROM', under the File parent
     * menu.
     * 
     * This will in turn call the controller module
     * to clear out the ROM and reset the emulator.
     */
    void on_actionClose_ROM_triggered();

signals:
    /***************** Public Signals. ***********************/

    /**
     * @brief External key signal.
     * 
     * Used to send key events to external object. This signal
     * must be connected with the controller module to
     * receive any incoming key events.
     * 
     * @param key Key integer enumeration.
     * @param isPreessed true for pressed state, else false.
     */
    void sendKeySignal(int key, bool isPressed);

    /**
     * @brief Send the path for the ROM files.
     * 
     * After the ROM file path is selected in the GUI, this
     * signal sends the path string to the controller module.
     * 
     * @param romFilePath ROM path string.
     * @param[out] isValidRomPath The receiver of the signal writes
     *                            true if the path is valid, and
     *                            the ROM was loaded successfully.
     */
    void sendRomPath(const std::string& romFilePath, bool *isValidRomPath);

    /**
     * @brief Send Pause/Run toggle signal.
     */
    void sendToggleRunSignal(void);

    /**
     * @brief Send Reset signal.
     * 
     * Reset reinitializes the game and reloads the game
     * from a clean state.
     */
    void sendResetSignal(void);

    /**
     * @brief Send Close signal.
     * 
     * Unloads the ROM from memory, and resets the emulator state.
     */
    void sendCloseGameSignal(void);

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

    /**
     * @brief Auxiliary function to create pulsed key events.
     * 
     * This function creates a key press event, and after X milliseconds
     * it will clear it.
     * 
     * It does so by creating a callback on the timer with a lambda closure
     * with the local context.
     */
    void pulseKey(int key, unsigned int milliseconds, QAction *blockAction);

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
     * @brief Color mask image reference.
     * 
     * The arcade machine uses a color mask that's fixed for every
     * pixel. The monochrome image is then multiplied by the mask
     * to give colors to the screen.
     */
    QImage colorMask;

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

    /**
     * @brief Auxiliary flag to set if ROM is succesfully loaded.
     * 
     * The flag is used to either enable and disable the game menu options,
     * and block key events when the game is not active.
     */
    bool romIsLoaded = false;

    /**
     * @brief Simulated key pulse time.
     */
    static constexpr unsigned int KEY_PULSE_TIME_MS = 300;
};
#endif // MAINWINDOW_H
