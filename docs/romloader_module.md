# ROM Loader Module Documentation

## 1. OVERVIEW
The ROM Loader module is responsible for loading the Space Invaders ROM files into memory after initialization.  
It maps each of the 4 binary files to a specific memory region within the Intel 8080’s 64KB memory space (`0x0000 – 0x1FFF`).  
Each binary file represents a segment of the game, and this module ensures accurate placement, size verification, and optional checksum verification.

This module is essential for bootstrapping the emulator with ROM data and supports additional ROM loading features beyond just Space Invaders.

## 2. PURPOSE
The ROM Loader’s purpose is to:
- Load the four 2KB Space Invaders binary files into their correct locations in memory
- Validate each file for existence and size
- Allow for optional directory override for file loading
- Support non-Space Invaders ROMs via individual file loading

| ROM Segment | Filename     | Memory Range       |
|-------------|--------------|--------------------|
| Segment 1   | `invaders.h` | `0x0000 – 0x07FF`  |
| Segment 2   | `invaders.g` | `0x0800 – 0x0FFF`  |
| Segment 3   | `invaders.f` | `0x1000 – 0x17FF`  |
| Segment 4   | `invaders.e` | `0x1800 – 0x1FFF`  |

Though primarily built for Space Invaders, the loader can be extended to support other Intel 8080-compatible ROMs.

## 3. USAGE INSTRUCTIONS

A typical usage scenario includes:

1. Initializing memory
2. Calling the ROM loader
3. Starting emulation

### Example Integration

```cpp
#include "romloader.hpp"
#include "memory.hpp"

int main() {
    Memory memory;

    bool success = LoadSpaceInvadersROM(memory);
    if (!success) {
        std::cerr << "Failed to load ROM set.\n";
        return 1;
    }

    // Emulation begins...
}
```

### Custom Directory Override

By default, the loader looks in `"src/roms/"`.  
You can override this path:

```cpp
LoadSpaceInvadersROM(memory, "roms/");
```


## 4. FUNCTION SIGNATURES

### Space Invaders Loader

```cpp
bool LoadSpaceInvadersROM(Memory& memory, const std::string& overrideDirectory = "");
```

- `memory`: The emulator memory object to load data into.
- `overrideDirectory`: (optional) Custom path to ROM files.
- **Returns**: `true` if all files load successfully, `false` if any fail.

### Individual File Loader (Internal)

```cpp
bool LoadROMFile(const std::string& filepath, Memory& memory, uint16_t targetAddr);
```

- Used for custom ROM loading
- Reads one file and places it into memory at a specified address

## 5. INTEGRATION NOTES

- ROMs are loaded in **binary mode** using `std::ifstream`.
- The loader uses `memory.writeRomBytes(...)` to bypass normal ROM write protections.
- It performs **no disassembly, decryption, or patching** of ROM content.
- It is typically invoked during the emulator’s startup or reset phase.

## 6. DEBUG MODE FEATURES (OPTIONAL)

When compiled with `-DENABLE_MEMORY_DEBUG`, the ROM Loader prints debug info.

### Features include:
- **Per-file summaries**:
  ```
  [ROM Load] src/roms/invaders.h → 0x0000 (2048 bytes)
  ```

- **Checksum Verification**:
  ```
  [ROM Debug] Checksums:
  invaders.h @ 0x0000 → checksum: 0x2C8CC
  invaders.g @ 0x0800 → checksum: 0x1965B
  ```

These features assist with debugging ROM integrity and load behavior.

## 7. ERROR HANDLING

Failures will return `false` and log the issue to `stderr`.

Common failure cases:
- Missing or unreadable file
- File size too large for target memory region
- Write error due to memory bounds
- File not found at specified directory

### Example Logs

```
[ROM Load Error] Failed to open: src/roms/invaders.h
[ROM Load Error] File "invaders.e" exceeds memory bounds at 0x1800
```

## 8. TESTING TIPS

To ensure correct ROM loading:
- Inspect memory after each file is loaded
- Use `ReadByte()` on known ROM opcode locations
- Run with checksums enabled and verify against known values
- Pair with snapshot or watchpoint debug tools to detect memory changes

## 9. CHANGE LOG

| Version | Description                                      |
|---------|--------------------------------------------------|
| v1.0    | Implemented multi-part ROM loader                |
| v1.1    | Added error handling and checksum verification   |
| v1.2    | Added optional directory override support        |
