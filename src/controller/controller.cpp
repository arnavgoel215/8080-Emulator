/**********************************************************
 * @file controller.cpp
 * 
 * @brief Implementation of the Controller class.
 * 
 * @author: Jesse
 * 
 *********************************************************/

/***************** Include files. ***********************/
#include "controller.hpp"
#include "view/mainwindow.h"
#include <QKeyEvent>

/***************** Macros and defines. ***********************/
// The VRAM size for Space Invaders is 7168 bytes.
constexpr size_t VRAM_SIZE = 7168;

/***************** Namespaces. ***********************/

/***************** Local Classes. ***********************/

/***************** Local Functions. ***********************/

/***************** Global Class Functions. ***********************/

Controller::Controller(Emulator* model, MainWindow* view)
    : m_model(model), m_view(view), m_isRunning(false)
{
    // In a fully wired application, the constructor would be the ideal
    // place to connect signals from the view to the slots in this controller.
    // For example:
    // QObject::connect(m_view, &MainWindow::runToggled, this, &Controller::onToggleRun);
    // QObject::connect(m_view, &MainWindow::resetTriggered, this, &Controller::onReset);
    // etc.
}

void Controller::onLoadROM(const std::string& romFilePath)
{
    m_isRunning = false;
    m_model->loadROM(romFilePath);
    m_model->reset();
    if (m_view) {
        // m_view->showStatusMessage("ROM loaded successfully. Press Start.");
    }
}

void Controller::onToggleRun()
{
    m_isRunning = !m_isRunning;
    if (m_view) {
        // m_view->showStatusMessage(m_isRunning ? "Emulation running." : "Emulation paused.");
    }
}

void Controller::onReset()
{
    m_isRunning = false;
    m_model->reset();
    if (m_view) {
        // m_view->showStatusMessage("Emulator reset.");
    }
}

void Controller::onKeyEvent(int key, bool isPressed)
{
    // This function maps keyboard presses from the View to the abstract
    // game inputs that the Model understands.
    GameInput input;
    switch (key)
    {
        case Qt::Key_C:
            input = GameInput::Coin;
            break;
        case Qt::Key_Return: // 'Enter' key
        case Qt::Key_Enter:
            input = GameInput::P1_Start;
            break;
        case Qt::Key_Space:
            input = GameInput::P1_Shoot;
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            input = GameInput::P1_Left;
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            input = GameInput::P1_Right;
            break;
        default:
            return; // Ignore other keys
    }
    
    // The model should have a method to set the state of these inputs.
    m_model->setInputState(input, isPressed);
}

void Controller::runFrame()
{
    if (!m_isRunning)
    {
        return;
    }

    // Emulate cycles for the first half of the screen.
    m_model->emulateCycles(CYCLES_PER_FRAME / 2);

    // Trigger the mid-screen interrupt (RST 1). This is a characteristic
    // of the original Space Invaders hardware.
    m_model->requestInterrupt(1);

    // Emulate cycles for the second half of the screen.
    m_model->emulateCycles(CYCLES_PER_FRAME / 2);

    // Trigger the V-Blank interrupt (RST 2). This signals the end of a frame.
    m_model->requestInterrupt(2);

    if (m_view) {
        // The model should provide a pointer to its internal framebuffer.
        const uint8_t* frameBuffer = m_model->getFrameBuffer();
        if (frameBuffer)
        {
            // The view should have a method to take the raw framebuffer data and render it.
            // m_view->updateGraphics(frameBuffer, VRAM_SIZE);
        }

        // The view could also have a method to display debug info.
        // CPUState state = m_model->getCPUState();
        // m_view->updateDebugInfo(state);
    }
}

// --- CLI / Debug Methods ---

void Controller::stepSingleInstruction()
{
    // Emulate just enough cycles to hopefully execute one instruction.
    // The exact number of cycles per instruction varies, but the emulator
    // core handles that. We just need to advance it.
    m_model->emulateCycles(1);
}

CPUState Controller::getCPUStateForDebug() const
{
    return m_model->getCPUState();
}
