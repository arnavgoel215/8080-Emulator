/**********************************************************
 * @file controller.hpp
 * 
 * @brief Public interface for the Controller.
 *        This class connects user actions from the View to the Model (Emulator)
 *        and drives the main emulation loop.
 * 
 * @author Jesse
 * 
 *********************************************************/
#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

/***************** Include files. ***********************/
#include <string>
#include "../model/emulator.hpp" // Needs to know about the Emulator's public interface

/***************** Global Classes. ***********************/
// Forward-declaration of the View's main window class to avoid including Qt headers here.
// This breaks the circular dependency between Controller and View.
class MainWindow; 

/**
 * @brief The Controller class, responsible for application logic.
 */
class Controller
{
public:
    /**
     * @brief Constructs the Controller.
     * @param model A pointer to the Emulator instance.
     * @param view A pointer to the MainView instance.
     */
    Controller(Emulator* model, MainWindow* view);

    // --- User Action Handlers (called by the View) ---

    /**
     * @brief Handles the user request to load a ROM file.
     * @param romFilePath The absolute path to the ROM file.
     */
    void onLoadROM(const std::string& romFilePath);

    /**
     * @brief Handles the user request to toggle the emulation (Start/Pause).
     */
    void onToggleRun();

    /**
     * @brief Handles the user request to reset the emulation.
     */
    void onReset();

    /**
     * @brief Handles a keyboard event from the View.
     * @param key An integer identifying the key (e.g., from Qt::Key).
     * @param isPressed The state of the key (true for pressed, false for released).
     */
    void onKeyEvent(int key, bool isPressed);

    // --- Main Emulation Loop ---

    /**
     * @brief Executes a single frame's worth of emulation and updates the view.
     *        This function should be called repeatedly by a timer in the View (e.g., 60Hz).
     */
    void runFrame();

private:
    /**
     * @brief Updates the state of a game control.
     * @param input The specific game input being changed.
     * @param isPressed The state of the input (true for pressed, false for released).
     */
    void setInputState(GameInput input, bool isPressed);

    Emulator* m_model;
    MainWindow* m_view;
    bool m_isRunning;
};

#endif /* CONTROLLER_HPP_ */
