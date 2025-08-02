# ROM Compatibility Guide

## Overview

This document describes the compatibility requirements for ROM files used in the Space Invaders emulator. It outlines supported formats, how ROMs are loaded, and known limitations or issues during ROM usage.

---

## Supported ROMs

- **Primary Target**: Space Invaders (1978, Midway)
- **Format**: Combined 4-part binary dump
  - Original files:
    - `invaders.h`
    - `invaders.g`
    - `invaders.f`
    - `invaders.e`
  - Merged together in memory order:
    - `invaders.h` → 0x0000–0x07FF
    - `invaders.g` → 0x0800–0x0FFF
    - `invaders.f` → 0x1000–0x17FF
    - `invaders.e` → 0x1800–0x1FFF
- **Expected Combined Size**: 8 KB (8192 bytes)

---

## ROM Loading Requirements

- ROMs are loaded via the `Controller::onLoadROM()` method.
- The emulator checks:
  - That the file is readable
  - That its size is exactly **8192 bytes**
- Once validated, the ROM is loaded directly into memory starting at address `0x0000`.
- Supported file extensions:
  - `.bin`
  - `.rom`
  - Raw binary dumps (no compression or archive formats)

---

## Known Compatibility Constraints

- Compatible with:
  - Official Midway Space Invaders ROMs
  - Clean binary dumps merged to 8KB
- Not compatible with:
  - Bootleg ROMs with different layouts
  - ROMs requiring decryption or special mappers
  - Compressed archives (`.zip`, `.rar`)
  - Non-Space Invaders games

---

## Verifying a Compatible ROM

To check that your ROM is compatible:
- Ensure it is **exactly 8192 bytes**:
  ```bash
  ls -l space_invaders.rom
  ```
- Use checksums to verify integrity:
  ```bash
  md5sum space_invaders.rom
  ```
- Emulator logs may provide hints:
  - `[INFO] ROM loaded successfully.`
  - `[ERROR] Invalid ROM size: expected 8192 bytes.`

---

## Troubleshooting

| Issue                    | Likely Cause                  | Solution                                |
|--------------------------|-------------------------------|-----------------------------------------|
| Emulator won't start     | Incorrect ROM size            | Ensure ROM is 8192 bytes                |
| Black screen / no output | ROM is corrupt or not loaded  | Use a verified clean ROM file           |
| Crash on startup         | Unsupported format or corrupt | Load a known-good Space Invaders ROM    |

---

## Future Compatibility Goals

- [ ] Handle `.zip` archives with auto-extract
- [ ] Add CRC/hash-based verification and feedback

---
