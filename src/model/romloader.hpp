// ============================================================================
// ROMLoader Module for the Intel 8080 Emulator
// ----------------------------------------------------------------------------
// The ROMLoader is responsible for loading the 4-part Space Invaders ROM 
// set into memory.  Each file is 2kb and must be put directly into the 
// ROM section of RAM
//
// ROM Segments:
//   - invaders.h → 0x0000
//   - invaders.g → 0x0800
//   - invaders.f → 0x1000
//   - invaders.e → 0x1800
//
// Author : Fredo
// Date   : 7/20/25
// ============================================================================

// ======================= Header Files ==============================
#ifndef ROMLOADER_HPP
#define ROMLOADER_HPP

// ======================= Includes Files =============================
#include "memory.hpp"
#include <string>

// ======================= ROM Loader ================================

// Loads the full Space Invaders ROM set into memory
// If overrideDirectory is provided, loads from that folder.
// Returns true on success, false on failure.
bool LoadSpaceInvadersROM(Memory& memory, const std::string& overrideDirectory = "");

// Loads a single ROM binary file into memory starting at target address specified 
// Performs memory boundary check and writes to ROM only
bool LoadROMFile(const std::string& filepath, Memory& memory, uint16_t targetAddr);

#endif // ROMLOADER_HPP
