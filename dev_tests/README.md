# Dev Tests — Intel 8080 Emulator

This directory contains all development tests for the Intel 8080 emulator project.  
Tests are grouped by type to support modular, layered validation from unit-level to system-level.

---

## Directory Structure

```
dev_tests/
├── output/                    # Output for logs, snapshots, and dumps
│   └── output.txt
│
├── support/                   # Shared test utilities and debug macros
│   └── test_utils.hpp
│
├── unit_tests/                # Unit tests for each module and opcode class
│   ├── cpu_a_opcodes_test.cpp
│   ├── cpu_arithmetic_unit_tests.cpp
│   ├── cpu_c_opcodes_tests.cpp
│   ├── cpu_d_opcodes_tests.cpp
│   ├── cpu_data_transfer_unit_tests.cpp
│   ├── cpu_flags_unit_tests.cpp
│   ├── cpu_h_opcodes_tests.cpp
│   ├── cpu_logic_unit_tests.cpp
│   ├── cpu_mov_opcodes_tests.cpp
│   ├── cpu_si_opcodes_tests.cpp
│   ├── cpu_stack_unit_tests.cpp
│   ├── io_unit_tests.cpp
│   ├── memory_unit_tests.cpp
│   └── romloader_unit_tests.cpp
```

---

## Test Categories

### `unit_tests/`
- Tests each module in isolation with fine-grained, opcode-by-opcode validation.
- Coverage includes:
  - CPU instructions grouped by register/opcode prefix
  - I/O operations
  - Memory module behaviors (ROM lockout, VRAM bounds)
  - ROM loader mechanics

### `output/`
- Contains test logs, output text dumps, or visual debug info.
- Files like `cpu_data_tests` and `output.txt` record result history.

### `support/`
- Reusable tools for testing across all files.
- Includes colorized output, test counters, and hex converters via `test_utils.hpp`.

---

## Build & Run Instructions

Example (compile a test manually):

```bash
g++ -std=c++17 -DENABLE_COLOR_OUTPUT \
    dev_tests/unit_tests/cpu_stack_unit_tests.cpp \
    src/model/emulator.cpp src/model/memory.cpp \
    -o dev_tests/output/cpu_stack_tests
```

Then run:

```bash
./dev_tests/output/cpu_stack_tests
```

---

##  Notes

- Many test files are opcode-specific for traceability.
- Grouped by register families (A, C, D, H, MOV, SI).
- `test_utils.hpp` is shared and imported across all files.
- Future additions:
  - `functional_tests/` for high-level module behavior
  - `integration_tests/` to validate module-to-module workflows
  - `system_level/` for full emulator regression tests

---
