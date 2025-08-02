# View Module

Handles GUI rendering, window events, video test modes, and color masking for the emulator’s display output.

---

## Files

```
view/
├── mainwindow.cpp / mainwindow.h
├── frame_buffer_tester.cpp / .h
├── common_frame_cfg.h
├── resources/
├── CMakeLists.txt
```

---

## Responsibilities

- Renders the game screen using `QPainter` and `QImage`.
- Maps Qt key events into controller signals.
- Provides debug/test UI tools like buffer testing and pause/restart control.
- Loads ROM folder via file dialog and displays error messages.

---

## Interactions

- Emits `sendRomPath()` to Controller.
- Receives `updateFrameBuffer()` signal from frame buffer tester.

---

## Related Features

- Video test mode
- Color mask overlay
- Game control UI
