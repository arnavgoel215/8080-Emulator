/**********************************************************
 * @file controller.hpp
 *
 * @brief Public interface for the Controller.
 *        This class connects user actions from the View to the Model (Emulator)
 *        and drives the main emulation loop.
 *
 * @author Jesse
 * @author Sergio
 *
 *********************************************************/
#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

/***************** Include files. ***********************/
#include <string>
#include <QMutex>
#include "emulator.hpp" // Needs to know about the Emulator's public interface
#include "common_frame_cfg.h" // For frame_buffer_t type.

// QT Specific tools.
#include <QObject>

/***************** Global Classes. ***********************/
// Forward-declaration of the View's main window class to avoid including Qt headers here.
// This breaks the circular dependency between Controller and View.
class MainWindow; 

/**
 * @brief The Controller class, responsible for application logic.
 */
class Controller : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the Controller.
     * @param model A pointer to the Emulator instance.
     * @param view A pointer to the MainView instance.
     */
    explicit Controller(Emulator* model, MainWindow* view, QObject* parent = nullptr);

    // --- Main Emulation Loop ---
    /**
     * @brief Executes a single frame's worth of emulation and updates the view.
     *        This function should be called repeatedly by a timer in the View (e.g., 60Hz).
     */
    void runFrame();

    // --- CLI / Debug Methods ---
    /**
     * @brief Executes a single CPU instruction.
     *        For debugging and command-line execution.
     */
    void stepSingleInstruction();

    /**
     * @brief Gets the current CPU state for debugging.
     * @return A snapshot of the CPU's registers and flags.
     */
    CPUState getCPUStateForDebug() const;

    // --- User Action Handlers (called by the View) ---

public slots:
    /**
     * @brief Handles the user request to load a ROM file.
     * @param romFilePath The absolute path to the ROM file.
     * @param[out] isValidRomPath write true if ROM path was valid, else false.
     */
    void onLoadROM(const std::string& romFilePath, bool *isValidRomPath = nullptr);

    /**
     * @brief Handles the user request to toggle the emulation (Start/Pause).
     */
    void onToggleRun(bool *isRunning);

    /**
     * @brief Handles the user request to reset the emulation.
     */
    void onReset();

    /**
     * @brief Handles the request to close the game.
     * 
     * The ROM is unloaded from memory, and the emulator is reset.
     */
    void onCloseGame();

    /**
     * @brief Handles a keyboard event from the View.
     * @param key An integer identifying the key (e.g., from Qt::Key).
     * @param isPressed The state of the key (true for pressed, false for released).
     */
    void onKeyEvent(int key, bool isPressed);

signals:
    
    /**
     * @brief Signal used to send a full video frame.
     * 
     * @param buffer Snapshot of current video frame.
     */
    void sendframeBuffer(const frame_buffer_t *buffer);

private:
    // --- Private Members ---
    Emulator* m_model;
    MainWindow* m_view;
    bool m_isRunning;
    std::string m_romPath;
    const uint8_t* emulatorFrameBufferPtr;
    frame_buffer_t m_frameBuffer;
    QMutex mutex;

    // --- Constants ---
    // The original arcade machine had a 2MHz CPU and a 60Hz refresh rate.
    // This gives us approximately 33,333 cycles per frame.
    static constexpr int CYCLES_PER_FRAME = 33333;
};

#endif /* CONTROLLER_HPP_ */
