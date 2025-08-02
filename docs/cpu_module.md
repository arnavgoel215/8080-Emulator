# CPU Documentation

## Overview

This file documents the CPU instruction decoding and execution logic in the emulator. The CPU component simulates the Intel 8080 microprocessor and is responsible for fetching, decoding, and executing all machine instructions.

---

## Responsibilities

- Decode and execute Intel 8080 instructions
- Update CPU state including registers, program counter, and flags
- Handle branching, arithmetic, logic, and stack operations
- Implement interrupt and reset logic
- Provide debug views of the current CPU state

---

## Instruction Execution

- `executeInstruction()`: Fetches and executes the next opcode at `state.pc`
- `emulateCycles(int)`: Executes a fixed number of instructions (not true timing cycles)
- Instructions are handled in categorized `op_*` functions
- MOV opcodes are decoded via bitmasking for compact handling

---

## Register Operations

- `get_reg(uint8_t code)`: Returns the value of a register or memory (M)
- `set_reg(uint8_t code, uint8_t val)`: Writes to the specified register or memory

---

## Instruction Categories

- **Data Transfer**: MOV, MVI, LXI, STAX, LDAX, SHLD, LHLD, XCHG
- **Arithmetic**: ADD, ADC, SUB, SBB, INR, DCR, DAD, ADI, SUI, SBI, ACI
- **Logical**: ANA, ORA, XRA, CMP, CMA, STC, CMC
- **Branching**: JMP, CALL, RET, conditional jumps and calls, RST
- **Stack**: PUSH, POP, SPHL, XTHL
- **Control**: NOP, HLT, EI

---

## Stack & Interrupts

- `requestInterrupt(uint8_t)`: Pushes PC to stack and jumps to interrupt vector
- Stack uses memory and grows downward
- Flags are preserved via PUSH/POP PSW

---

## Flags

- `setFlags(uint8_t result)`: Updates Z, S, P based on result
- AC and CY are set explicitly in arithmetic ops
- Flags are used for conditional jumps and returns

---

## Helper Functions

- `reset()`: Resets all CPU state and memory
- `hl()`: Returns the combined 16-bit HL register value
- `getCPUState()`: Returns the current CPU state for debug or test use

---

## Design Notes

- Each opcode is mapped directly to its corresponding `op_*()` implementation
- Instruction lengths vary; `state.pc` is advanced accordingly
- Unimplemented opcodes print an error and skip forward
