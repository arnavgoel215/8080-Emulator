# Dev Tests — Intel 8080 Emulator

This directory contains all development tests for the Intel 8080 emulator project. Tests are grouped by purpose and scope to support a modular, layered approach to validation.

---

##  Directory Structure

```
dev_tests/
├── functional_tests/          # High-level behavior tests of individual components
│   ├── io_functional_tests.cpp
│   ├── memory_functional_tests.cpp
│   └── romloader_functional_tests.cpp
│
├── integration_tests/        
│   └── component_links/       # Integration Tests between linked modules
│       └── romloader_to_memory_tests.cpp
│
│   ├── multi_component_flows/ # Integration testing between Multiple modules (3 or higher)
│   └── system_level/          # Integration testing for End-to-end system-level validation
├── manual_tests/              # Developer-driven manual tests
├── output/                    # Output for logs, dumps, and snapshots
│
├── support/                   # Shared utilities for testing
│   └── test_utils.hpp
│
├── test_data/                 # ROMs, input files, and any other needed data
│
├── unit_tests/                # Isolated, low-level behavior tests 
│   ├── io_unit_tests.cpp
│   ├── memory_unit_tests.cpp
│   └── romloader_unit_tests.cpp
```

---

##  Test Categories

### `functional_tests/`
- Tests high-level behavior of **single components** (e.g., memory, I/O, ROM loader).
- Includes realistic usage flows like VRAM access or file-based ROM loading.

###  `integration_tests/`
- Focuses on **cross-module behavior**, such as verifying the ROM loader loads into protected memory.
- Currently organized under `component_links/`.

###  `unit_tests/`
- Isolated, **method-level verification** for memory, ROMLoader, and I/O subsystems.
- Validates low-level invariants like ROM protection or VRAM pointer correctness.

###  `manual_tests/`, `multi_component_flows/`, and `system_level/` *(planned)*
- These folders will support interactive testing, full emulator flows, and top-down regression checks.

---

##  Utilities

###  `test_utils.hpp`
- Provides consistent output formatting, test numbering, and ANSI color support.
- Shared across all test files. Use `printTestResult(...)` and `hex(...)` utilities.

---

##  Build and Run

To compile a test:
```bash
g++ -std=c++17 -DENABLE_MEMORY_DEBUG -DENABLE_COLOR_OUTPUT \
    dev_tests/functional_tests/memory_functional_tests.cpp \
    src/model/memory.cpp -o dev_tests/output/memory_tests
```

Then run:
```bash
.dev_tests/output/memory_tests  
```

---

