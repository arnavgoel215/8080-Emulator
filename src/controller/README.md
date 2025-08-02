# Controller Module

Handles input processing, player controls, and emulator state toggles. It acts as the communication bridge between the GUI (View) and the emulator core (Model).

---

## Files

```
controller/
├── controller.cpp
├── controller.hpp
├── CMakeLists.txt
```

---

## Responsibilities

- Translates user input (from `MainWindow`) into emulator-recognizable signals.
- Maintains controller state for inputs like Coin, Fire, Player Start.
- Emits and receives Qt signals for game control (reset, pause, keypresses).

---

## Interfaces

- **Signals**: sendKeySignal(), sendResetSignal(), sendToggleRunSignal()
- **Slots**: receive key events, GUI toggles

---

## Related Tests

- `dev_tests/unit_tests/io_unit_tests.cpp`
