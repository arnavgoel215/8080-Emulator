# MEMORY MODULE DOCUMENTATION

## 1. OVERVIEW
The Memory module emulates the Intel 8080 processor’s 64KB of memory space.  
It supports modular byte-level access, so the emulator’s CPU, Graphics, and I/O have access to ROM, RAM, VRAM, and expansion zones.  
The memory module will interact with all areas of the emulator and serve as the main area for code execution, data storage, and graphical rendering.

## 2. PURPOSE
The memory module replicates the physical memory used in conjunction with the original Intel 8080 CPU.  
It interacts with all other components of the emulator. Our goal is to accurately reproduce the timing, speed, and interactions of the original hardware.

Key features:
- Facilitate controlled access
- Protect writable memory regions
- Provide read-only ROM integrity
- Integrate with other emulated hardware components
- Provide access to VRAM for smooth Qt-based graphics
- Provide debugging tools to support snapshots, memory inspection, and developer testing

## 3. ARCHITECTURE OVERVIEW

| Region             | Address Range       | Access Type | Used By                |
|--------------------|---------------------|-------------|------------------------|
| ROM Code           | 0x0000 – 0x1FFF     | Read-only   | ROMLoader (startup)    |
| Work RAM           | 0x2000 – 0x23FF     | Read/Write  | CPU + test routines    |
| Video RAM (VRAM)   | 0x2400 – 0x3FFF     | Read/Write  | Qt GUI / Framebuffer   |
| Expansion/Stack    | 0x4000 – 0xFFFF     | Read/Write  | Emulator core / I/O    |

The ROM section is read-only during runtime and may only be written using `writeRomBytes()` during initialization.

## 4. USAGE INSTRUCTIONS

### 4.1 Constructor

Initializes all 64KB to `0x00`. If `ENABLE_MEMORY_DEBUG` is defined, the constructor will also initialize debug counters, watchpoints, and snapshot buffers.

```cpp
Memory();
```

### 4.2 Memory Core Reading & Writing

Working RAM is located at `0x2000 – 0x23FF` and used for standard CPU operations. Write protection is enforced for addresses below `0x2000`.

```cpp
uint8_t ReadByte(uint16_t address) const;
void WriteByte(uint16_t address, uint8_t value);
```

### 4.3 Writing the ROM

The ROMLoader module is the only component allowed to write to memory below `0x2000` using:

```cpp
void writeRomBytes(uint16_t address, uint8_t value);
```

This bypass is only available during emulator startup and cannot write past `0x2000`.

### 4.4 Video RAM Access

Video RAM is in the range `0x2400 – 0x3FFF`. Qt can access this via:

```cpp
std::vector<uint8_t> GetVRAM() const;             // Returns a copy of VRAM
const uint8_t* GetVRAMPointer() const;            // Returns pointer to VRAM (read-only)
```

## 5. DEBUG FEATURES (`ENABLE_MEMORY_DEBUG`)

The debug mode unlocks tools to inspect, track, and compare memory states. Enable via `-DENABLE_MEMORY_DEBUG`.

### 5.1 Full Memory Dump

Writes the full 64KB memory to a binary file.

```cpp
void DumpMemory(const std::string& filename) const;
```

### 5.2 Dump Memory Range

Dumps a range of RAM from `start` to `end`adresses. Output is formatted in hex and printed to console.

```cpp
void DumpRegion(uint16_t start, uint16_t end) const;
```

### 5.3 Take a Snapshot

Stores a snapshot of current memory state. Later, compare this to the live RAM to detect changes.

```cpp
void Snapshot();
void CompareWithSnapshot() const;
```

### 5.4 Watch an Address in Memory

Adds a watchpoint to track memory reads or writes at a specific address.

```cpp
void AddWatchpoint(uint16_t address);
void ClearWatchpoints();
```

### 5.5 Log Access Counters

Logs the number of reads and writes to each address, exporting a summary to file.

```cpp
void LogAccessCounts(const std::string& filename) const;
```

### 5.6 VRAM Dump

Allows you to dump the contents of the VRAM section (`0x2400 – 0x3FFF`) using a specific start and end address.  
Only addresses within the VRAM range are valid.

```cpp
void DumpVRAM(uint16_t start, uint16_t end) const;
```

## 6. FUNCTION SIGNATURES (QUICK REF)

```cpp
// Access RAM
uint8_t ReadByte(uint16_t address) const;
void WriteByte(uint16_t address, uint8_t value);
void writeRomBytes(uint16_t address, uint8_t value);

// Access VRAM
std::vector<uint8_t> GetVRAM() const;
const uint8_t* GetVRAMPointer() const;

#ifdef ENABLE_MEMORY_DEBUG
// Debug Tools
void Snapshot();
void CompareWithSnapshot() const;
void DumpMemory(const std::string& filename) const;
void DumpRegion(uint16_t start, uint16_t end) const;
void DumpVRAM(uint16_t start, uint16_t end) const;
void AddWatchpoint(uint16_t address);
void ClearWatchpoints();
void LogAccessCounts(const std::string& filename) const;
#endif
```

## 7. INTEGRATION NOTES

- ROMLoader populates ROM bytes on startup (`0x0000 – 0x1FFF`)
- CPU fetches and manipulates memory (`0x2000 – 0x23FF` and `0x4000 – 0xFFFF`)
- ROM write protection enforced except via loader bypass
- VRAM is accessed via (`0x2400 – 0x3FFF`)
- Debug tools are optional and incur no runtime cost when disabled

## 8. USAGE EXAMPLES

```cpp
Memory memory;

memory.WriteByte(0x2000, 0xAB); // Write to RAM
uint8_t val = memory.ReadByte(0x2000); // Read from RAM

memory.writeRomBytes(0x0004, 0x3E); // Direct ROM write via loader

std::vector<uint8_t> frame = memory.GetVRAM(); // Safe VRAM copy
const uint8_t* pixels = memory.GetVRAMPointer(); // Raw framebuffer pointer
```

## 9. MEMORY MODULE TESTED

Tests include:
- RAM pollution and core integrity
- ROM loader verification
- VRAM access and bounds checking
- Snapshot and memory comparison tests
- Watchpoint validation
- Dump & log functionality
- Manual and boundary ROM writes (`0x0000`, `0x1FFF`, `0xFFFF`)
- Stress tests on access counters

## 10. CHANGE LOG

| Version | Features Added                                              |
|---------|-------------------------------------------------------------|
| v1.0    | Created Core Memory: Read/Write Implementation              |
| v1.1    | Enforced ROM write protection                               |
| v1.2    | Added Snapshot and Watchpoint tools                         |
| v1.3    | Added Access counters and VRAM dump                         |
| v1.4    | Added testing suite, debug toggle, and VRAM pointer access  |
