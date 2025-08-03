# Emulator I/O System Documentation

## Overview

This file documents the input/output (I/O) system for the emulator, which interfaces with the game’s control ports and emulates hardware interactions such as the shift register and input bits.

---

## Responsibilities

- Emulate input ports IN0–IN3
- Emulate output ports OUT2–OUT6
- Maintain and shift internal register values
- Route game inputs from controller to port state

---

## Input Ports (IN)

- `IN0` (0x00): Unused
- `IN1` (0x01): Coin, P1 Start, Shoot, Left, Right
- `IN2` (0x02): Player 2 controls (not implemented)
- `IN3` (0x03): Shift register result (`(shift_register >> (8 - shift_offset)) & 0xFF`)

### `uint8_t io_read(InPortNum port)`
Reads a value from the selected input port. Handles coin/start/shoot/shift register inputs.

---

## Output Ports (OUT)

- `OUT2` (0x02): Shift register offset (`state.shift_offset`)
- `OUT3` (0x03): Sound effects group 1 (not fully implemented)
- `OUT4` (0x04): Shift register data (loads data into high byte)
- `OUT5` (0x05): Sound effects group 2 (not fully implemented)
- `OUT6` (0x06): Watchdog reset (prints stub message)

### `void io_write(OutPortNum port, uint8_t val)`
Writes to the selected output port and performs the associated action.

---

## Input State Management

### `void setInputState(GameInput input, bool isPressed)`
Updates bits in `state.port_in_1` for gameplay buttons like:
- Coin
- P1 Start
- Shoot
- Move Left / Right

---

## Shift Register

- Internal register simulates arcade hardware behavior
- Data is shifted on OUT4 write and masked on IN3 read
- Shift offset is controlled by OUT2

---

## Design Notes

- All I/O is memory-mapped and port-indexed by opcode
- Sound and watchdog are stubbed for debug feedback
- Uses structured enums for clarity: `InPortNum`, `OutPortNum`, `GameInput`
