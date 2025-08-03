# Test Utilities Documentation

## Overview

This module provides a comprehensive toolkit of utility functions, formatters, and helpers for testing the Intel 8080 emulator. It supports testing for CPU instruction execution, memory setup, stack operations, and formatted debug output with optional color and verbosity flags.

---

## Responsibilities

- Provide CPU and memory setup helpers for isolated instruction testing.
- Offer color-coded and verbose debug output for arithmetic, logic, and stack operations.
- Implement reusable logging and result formatting macros.
- Enable flag checking and step-by-step opcode simulation.
- Wrap emulator/memory handling for cleaner test code.

---

## Configuration Macros

| Macro                 | Purpose                                         |
|----------------------|--------------------------------------------------|
| `ENABLE_COLOR_OUTPUT`| Enables ANSI color for test results              |
| `ENABLE_VERBOSE_DEBUG`| Enables detailed opcode/flag/stack debug output |
| `ENABLE_CPU_TESTING` | Enables test-only CPU helpers and emulation calls|

---

## Key Features

### Test Counters & Results
- `printTestResult(tag, desc, passed)`
- Tracks test count, pass/fail, and prints color-coded result.
- Resets with `resetTestCounter()`.

### Hex Formatter
- `hex(val, width)` – Returns uppercase hex string with 0x prefix.

---

## CPU Instruction Testing

### `runSingleInstruction(opcode, setupFn)`
- Runs a single instruction on a clean emulator.
- Accepts an optional lambda to configure CPU state before execution.

### `runInstructionWithMemory(opcode, addr, val, setupFn)`
- Similar to `runSingleInstruction` but includes memory preloading.

### `writeRomInstructionSequence(mem, addr, bytes)`
- Injects opcodes into memory to simulate a loaded ROM sequence.

---

## Debug Print Utilities

### General
- `printDebugResult()` – Show actual vs. expected A and CY values.
- `printOpcodeDebugBasic()` – Shows opcode, PC delta, and details.
- `printOpcodeFlags()` – Shows full flag state with expected values.

### Arithmetic & Logic
- `printArithmeticDebug()` – Debug output for ALU instructions.
- `printLogicDebug()` – Reports mismatches in flag-setting logical ops.

### Stack
- `printStackDebug()` – SP change, register/memory pair match.
- `printPopDebug()` – Confirms POP pulls correct data.
- `printSPHLDebug()` – Validates SP = HL effect.
- `printCallDebug()` – Debugs CALL behavior and stack push.
- `printXTHLDebug()` – Verifies HL <-> Stack exchange.

---

## StackInspector

```cpp
struct StackInspector {
    Memory& memory;
    uint16_t stackPointer;

    uint8_t getLowByte() const;
    uint8_t getHighByte() const;
    uint16_t readWord() const;
    std::pair<uint8_t, uint8_t> readBytePair() const;
    bool validatePair(uint8_t expectedHigh, uint8_t expectedLow) const;
    void printDebug(const std::string& label) const;
};
```

A lightweight utility for inspecting and validating stack-relative memory.

---

## Binary Utilities

- `createBinaryFile(filename, data)` – Writes a raw binary file for ROM building or verification.

---

## Design Notes

- All test utilities are **header-only**, with no external dependencies.
- Works cleanly with CPU unit test files (`cpu_*_unit_tests.cpp`).
- Designed for minimal setup, maximal feedback, and reusable debug output.

---

## Future Enhancements

- [ ] Integrate full instruction tracing log for emulator stepping
- [ ] Add snapshot/restore helpers for emulator state
- [ ] Create dynamic test scenarios with scripted opcode sequences