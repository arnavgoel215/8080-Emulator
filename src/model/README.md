# Model Module

Contains the core components of the Intel 8080 emulator, including CPU execution, memory access, and ROM loading.

---

## Files

```
model/
├── emulator_main.cpp
├── emulator.cpp / emulator.hpp
├── memory.cpp / memory.hpp
├── romloader.cpp / romloader.hpp
├── CMakeLists.txt
```

---

## Responsibilities

- Executes decoded CPU instructions.
- Manages system memory (ROM, RAM, VRAM).
- Loads ROM segments (invaders.e–h) and validates ROM layout.
- Coordinates memory-mapped I/O and display memory writes.

---

## Related Tests

- `memory_unit_tests.cpp`
- `romloader_unit_tests.cpp`
- `cpu_*_unit_tests.cpp`
