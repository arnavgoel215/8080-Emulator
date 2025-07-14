/**********************************************************
 * @file view.hpp
 * 
 * @brief Public interface for the main GUI window (View).
 * 
 * @author Sergio
 * 
 *********************************************************/
#ifndef VIEW_HPP_
#define VIEW_HPP_

/***************** Include files. ***********************/
#include <QMainWindow>
#include <string>
#include "../model/emulator.hpp" // Needs to know about CPUState struct

/***************** Global Classes. ***********************/

/**
 * @brief The main window of the application, derived from QMainWindow.
 */
class MainView : public QMainWindow
{
    Q_OBJECT // Required macro for Qt's signal/slot mechanism

public:
    // Constructor, destructor, and other view-specific methods...
    explicit MainView(QWidget *parent = nullptr);

public slots:
    // --- Public Slots (called by the Controller) ---

    /**
     * @brief Renders a new frame to the screen.
     * @param frameBuffer A pointer to the raw video buffer data from the model.
     * @param size The size of the buffer in bytes (should be VRAM_SIZE).
     */
    void updateGraphics(const uint8_t* frameBuffer, size_t size);

    /**
     * @brief Updates the debug display with current CPU register values.
     * @param state A const reference to the CPUState struct.
     */
    void updateDebugInfo(const CPUState& state);

    /**
     * @brief Displays a message in the application's status bar.
     * @param message The string message to display.
     */
    void showStatusMessage(const std::string& message);

signals:
    // The view will define signals here that the main application connects
    // to the Controller's on_...() methods. For example:
    // void romLoadRequested(const std::string& filePath);
    // void runToggled();
};

#endif /* VIEW_HPP_ */