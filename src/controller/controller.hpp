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
 * @brief An enumeration of all possible game inputs.
 * Used by the Controller to report key presses to the model.
 */
enum class GameInput
{
    Coin,
    P1_Start,
    P1_Shoot,
    P1_Left,
    P1_Right,
};

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
    void on_loadROM(const std::string& romFilePath);

    /**
     * @brief Handles the user request to toggle the emulation (Start/Pause).
     */
    void on_toggleRun();

    /**
     * @brief Handles the user request to reset the emulation.
     */
    void on_reset();

    /**
     * @brief Handles a keyboard event from the View.
     * @param key An integer identifying the key (e.g., from Qt::Key).
     * @param isPressed The state of the key (true for pressed, false for released).
     */
    void on_keyEvent(int key, bool isPressed);

    /**
     * @brief Updates the state of a game control.
     * @param input The specific game input being changed.
     * @param isPressed The state of the input (true for pressed, false for released).
     */
    void setInputState(GameInput input, bool isPressed);

    // --- Main Emulation Loop ---

    /**
     * @brief Executes a single frame's worth of emulation and updates the view.
     *        This function should be called repeatedly by a timer in the View (e.g., 60Hz).
     */
    void runFrame();
};

#endif /* CONTROLLER_HPP_ */