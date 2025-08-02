# ✅ Emulator Testing Strategy

## Overview

Testing verifies correctness of opcode execution, memory layout, stack behavior, and frame rendering. The emulator uses a layered approach for unit, functional, and integration testing.

---

## 🔍 Unit Testing

### Focus Areas:
- **CPU Instructions:** Arithmetic, logic, flow control, stack, rotation
- **Memory:** ROM/RAM boundary, VRAM integrity
- **Flags:** Zero, Sign, Carry, Parity, Aux Carry

### File Structure:
- `cpu_arithmetic_unit_tests.cpp`
- `cpu_stack_unit_tests.cpp`
- `cpu_logic_unit_tests.cpp`
- `cpu_flow_unit_tests.cpp`
- `memory_unit_tests.cpp`
- `romloader_unit_tests.cpp`

### Tools:
- `test_utils` macros for validation
- Snapshot/debug comparison
- Custom assert wrappers

---

## 🧪 Functional Testing

- `run_frame()` checked for accurate buffer generation
- `stepSingleInstruction()` confirms micro-step execution
- Frame buffer output visually validated through Qt

---

## 🎥 Frame Buffer Testing

- `FrameBufferTester` used for automated video output stress tests
- Simulates 60Hz frame output to confirm stability and performance

---

## 🧩 Integration Validation

- Confirmed input → signal → state update → frame render pipeline
- Full test of emulator running game loop with user input simulation

---

## 📈 Coverage Tracking

- Opcode coverage tracked against actual game ROM usage
- Snapshot validation of CPU state vs. expected values
- Frame buffer outputs visually confirmed

---

## 🚧 Future Test Plans

- Emulator state snapshots for regression testing
- CPU instruction cycle accuracy profiling
- Automated screenshot diff testing

---
