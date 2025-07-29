/**********************************************************
 * @file controller.cpp
 *
 * @brief Implementation of the Controller class.
 *
 * @author: Jesse
 * @author Sergio
 *
 *********************************************************/

/***************** Include files. ***********************/
#include "controller.hpp"
#include "mainwindow.h"

// Qt tools.
#include <QKeyEvent> // For Qt Key enumrations.

/***************** Macros and defines. ***********************/
// The VRAM size for Space Invaders is 7168 bytes.
constexpr size_t VRAM_SIZE = 7168;

/***************** Namespaces. ***********************/

/***************** Local Classes. ***********************/

/***************** Local Functions. ***********************/

/***************** Global Class Functions. ***********************/

Controller::Controller(Emulator* model, MainWindow* view, QObject* parent)
    : QObject{parent}, m_model(model), m_view(view), m_isRunning(false), m_romPath("")
{
    // For some reason dynamic libraries struggle with the new signal/slot syntax,
    // The older SIGNAL/SLOT macros are more lenient and allow detection of
    // the interfaces as long as the class is derived from QObject.

    // Key events (View -> Controller).
    connect(view, SIGNAL(sendKeySignal(int,bool)), this, SLOT(onKeyEvent(int,bool)));

    // Frame buffer events (Controller -> View).
    connect(this, SIGNAL(sendframeBuffer(const frame_buffer_t*)), view, SLOT(on_frameBufferReceived(const frame_buffer_t*)));

    // ROM Load events (View -> Controller).
    connect(view, SIGNAL(sendRomPath(const std::string,bool*)), this, SLOT(onLoadROM(const std::string,bool*)));

    // Reset emulation (View -> Controller).
    connect(view, SIGNAL(sendResetSignal()), this, SLOT(onReset()));

    // Close Game (View -> Controller).
    connect(view, SIGNAL(sendCloseGameSignal()), this, SLOT(onCloseGame()));

    // Toggle Run (View -> Controller).
    connect(view, SIGNAL(sendToggleRunSignal()), this, SLOT(onToggleRun()));
}

void Controller::onLoadROM(const std::string& romFilePath, bool *isValidRomPath)
{
    m_isRunning = false;
    if (true == m_model->loadROM(romFilePath))
    {
        if (m_view) {
            // m_view->showStatusMessage("ROM loaded successfully. Press Start.");
        }

        if (nullptr != isValidRomPath)
        {
            *isValidRomPath = true;
        }

        // Store copy of path.
        m_romPath = romFilePath;
    }
    else
    {
        if (nullptr != isValidRomPath)
        {
            *isValidRomPath = false;
        }
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
    if ("" != m_romPath)
    {
        m_model->loadROM(m_romPath);
        
        if (m_view) {
            // m_view->showStatusMessage("Game reset.");
        }
    }
}

void Controller::onCloseGame()
{
    m_isRunning = false;
    m_model->reset();
    m_romPath = ""; // Clear out temporal ROM path.
    if (m_view) 
    {
        // m_view->showStatusMessage("Game closed.");
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
