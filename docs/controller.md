# Controller Module Documentation

## Overview

The `Controller` class serves as the intermediary between the **View** (`MainWindow`) and the **Model** (`Emulator`) in the MVC architecture of the Space Invaders emulator. It handles user inputs, coordinates ROM loading and emulation control, and synchronizes graphical output with the emulator's frame buffer.

---

## Responsibilities

- Connects Qt signals/slots between the `MainWindow` (view) and `Emulator` (model).
- Handles user input and converts Qt key events into game input commands.
- Controls emulation state transitions (run, pause, reset, close).
- Synchronizes frame rendering with emulator interrupts.
- Provides optional debug interfaces for CLI/testing tools.

---

## Signals & Slots

### Slots (from `MainWindow` to `Controller`)
- `onKeyEvent(int key, bool isPressed)`
- `onLoadROM(const std::string& romFilePath, bool* isValidRomPath)`
- `onToggleRun(bool* isRunning)`
- `onReset()`
- `onCloseGame()`

### Signals (from `Controller` to `MainWindow`)
- `sendframeBuffer(const frame_buffer_t* buffer)`

---

## Key Methods

### `Controller::Controller(Emulator* model, MainWindow* view, QObject* parent = nullptr)`

Initializes the Controller and connects signals/slots. Validates non-null model and view pointers and sets up the emulator frame buffer.

---

### `void onLoadROM(const std::string& romFilePath, bool* isValidRomPath)`

Attempts to load the ROM via the emulator. Updates internal state and optionally auto-starts the game. Updates `isValidRomPath` to reflect success/failure.

---

### `void onToggleRun(bool* isRunning)`

Toggles the run/pause state of the emulator. Updates internal flag and provides the result back to the caller.

---

### `void onReset()`

Resets the emulator state and reloads the most recently loaded ROM if available. Clears the frame buffer and resumes running.

---

### `void onCloseGame()`

Fully stops emulation, resets state, and clears the ROM path. Intended to be used when the user closes or exits the game.

---

### `void onKeyEvent(int key, bool isPressed)`

Converts key press/release events from Qt into game input enums (e.g., Coin, Start, Shoot, Left, Right) and updates the emulator's input state accordingly.

---

### `void runFrame()`

Handles the full rendering cycle for a single frame:
1. Emulates cycles for the first half.
2. Triggers RST 1 (mid-screen) interrupt.
3. Copies first half of the screen buffer.
4. Emulates second half.
5. Triggers RST 2 (V-Blank) interrupt.
6. Copies second half of the screen buffer.
7. Emits the completed buffer to the view.

Protected with a mutex for thread-safe execution.

---

## Debug Methods

- `void stepSingleInstruction()`: Advances the emulator by a minimal number of cycles (intended to execute one instruction).
- `CPUState getCPUStateForDebug() const`: Returns the current CPU state for debug inspection.

---

## Internal Members

- `Emulator* m_model`: The core emulation logic.
- `MainWindow* m_view`: The GUI and input event handler.
- `bool m_isRunning`: Whether the emulator is actively running.
- `std::string m_romPath`: Path to the currently loaded ROM file.
- `frame_buffer_t m_frameBuffer`: Stores pixel data for the current frame.
- `uint8_t* emulatorFrameBufferPtr`: Pointer to the emulator’s internal video memory.
- `std::mutex mutex`: Ensures thread-safe frame operations.

---

## Notes

- Uses classic Qt signal/slot macros (`SIGNAL`, `SLOT`) for better compatibility with dynamic libraries.
- Inputs are hardcoded to common keyboard mappings (`C`, `Enter`, `Space`, `A`, `D`) for Coin, Start, Shoot, Left, and Right.
- Frame rendering is tightly coupled to Space Invaders' original timing and interrupt model (RST 1 and RST 2).
