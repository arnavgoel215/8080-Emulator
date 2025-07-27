// ============================================================
// ROM Loader Module for the Intel 8080 Emulator
// ------------------------------------------------------------
//
// Loads the 4-part Space Invaders ROM files into emulated memory.
// Files:
//   - invaders.h → 0x0000
//   - invaders.g → 0x0800
//   - invaders.f → 0x1000
//   - invaders.e → 0x1800
//
// Handles ROM file I/O, memory bounds checking,
// debug outputs, and checksum validation.
//
// Author: Fredo | Date: 7/20/25
// ============================================================


// ================= Header Files ===========================
#include "romloader.hpp"
#include "memory.hpp"

// ================= Include Files ===========================
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <filesystem>


// ================= Constants ===============================
static const std::string DEFAULT_ROM_DIR = "src/tests/";


// =================== Load ROM File =========================
// Reads a ROM file and loads it into emulator memory
bool LoadROMFile(const std::string& filepath, Memory& memory, uint16_t targetAddr) {
    std::ifstream romFile(filepath, std::ios::binary);
    if (!romFile) {
        std::cerr << "[ROM Load Error] Failed to open: " << filepath << "\n";
        return false;
    }

    // === Read file into buffer ===
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(romFile), {});
    size_t size = buffer.size();

    // === Memory bounds check ===
    if (targetAddr + size > Memory::MEMORY_SIZE) {
        std::cerr << "[ROM Load Error] File \"" << filepath 
                  << "\" exceeds memory bounds at 0x"
                  << std::hex << std::setw(4) << std::setfill('0') << targetAddr << "\n";
        return false;
    }

    // === Write to ROM Section ===
    for (size_t i = 0; i < size; ++i) {
        memory.writeRomBytes(static_cast<uint16_t>(targetAddr + i), buffer[i]);
    }

    // --- DEBUG MODE --- 
    // Prints each Byte of the ROM write
    #ifdef ENABLE_MEMORY_DEBUG
    std::cout << "[ROM Load] " << filepath << " → 0x"
              << std::hex << std::setw(4) << std::setfill('0') << targetAddr
              << " (" << std::dec << size << " bytes)\n";
    #endif // --- END DEBUG ---

    return true;
}


// ================= Load Space Invaders =====================
// Loads the full 4-part Space Invaders ROM set


// into the appropriate memory regions. Optionally prints
// ROM checksums for validation if DEBUG mode is enabled.
//
// Returns: true if all parts loaded successfully.
//
// ===========================================================
bool LoadSpaceInvadersROM(Memory& memory, const std::string& overrideDirectory) {
    const std::string romDirectory = overrideDirectory.empty() ? DEFAULT_ROM_DIR : overrideDirectory;

    // === ROM Parts: Store Files ===
    const std::vector<std::pair<std::string, uint16_t>> romParts = {
        {"invaders.h", 0x0000},
        {"invaders.g", 0x0800},
        {"invaders.f", 0x1000},
        {"invaders.e", 0x1800}
    };

    // === Load each ROM file ===
    // Loads the Space Invaders files | Aborts on error 
    for (const auto& [filename, address] : romParts) {
        std::filesystem::path fullPath = romDirectory;
        fullPath /= filename;
        if (!LoadROMFile(fullPath.string(), memory, address)) {
            return false;
        }
    }

    // ================= ROM Checksum Validation =============

    // --- DEBUG MODE --- 
    // Print checksum of each ROM segment
    #ifdef ENABLE_MEMORY_DEBUG
    std::cout << "\n[ROM Debug] Checksums:\n";
    for (const auto& [filename, baseAddr] : romParts) {
        uint32_t checksum = 0;
        for (int i = 0; i < 0x0800; ++i) {
            checksum += memory.ReadByte(baseAddr + i);
        }

        std::cout << "  " << filename << " @ 0x"
                  << std::hex << std::setw(4) << std::setfill('0') << baseAddr
                  << " → checksum: 0x" << std::uppercase << checksum << "\n";
    }
    std::cout << std::dec;
    #endif // --- END DEBUG ---

    return true;
}
