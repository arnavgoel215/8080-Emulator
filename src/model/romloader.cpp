#include "romloader.hpp"
#include "memory.hpp"          
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>             

static const std::string DEFAULT_ROM_DIR = "src/tests/"; 

// ========================= READ ROM FILE =============================

// Load ROM File
bool LoadROMFile(const std::string& filepath, Memory& memory, uint16_t targetAddr) {
    std::ifstream romFile(filepath, std::ios::binary);
    if (!romFile) {
        std::cerr << "[ROM Load Error] Failed to open: " << filepath << "\n";
        return false;
    }

    // --- Read ROM file into buffer ---
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(romFile), {});
    size_t size = buffer.size();

    // --- Check memory bounds ---
    if (targetAddr + size > Memory::MEMORY_SIZE) {
        std::cerr << "[ROM Load Error] File \"" << filepath << "\" exceeds memory bounds at 0x"
                  << std::hex << std::setw(4) << std::setfill('0') << targetAddr << "\n";
        return false;
    }

    // =================== WRITE TO MEMORY =============================

    // === Write === Write each Byte to Memory using writeROMBytes() 
    for (size_t i = 0; i < size; ++i) {
        memory.writeRomBytes(static_cast<uint16_t>(targetAddr + i), buffer[i]);
    }
    // --- DEBUG MODE ---
    #ifdef ENABLE_MEMORY_DEBUG
        std::cout << "[ROM Load] " << filepath << " → 0x"
                << std::hex << std::setw(4) << std::setfill('0') << targetAddr
                << " (" << std::dec << size << " bytes)\n";
    #endif // --- END DEBUG --- 

    return true;
}

// =================== LOAD SPACE INVADERS ROM =========================

// --- Load Space Invaders ---
bool LoadSpaceInvadersROM(Memory& memory, const std::string& overrideDirectory) {
    const std::string romDirectory = overrideDirectory.empty() ? DEFAULT_ROM_DIR : overrideDirectory;

    const std::vector<std::pair<std::string, uint16_t>> romParts = {
        {"invaders.h", 0x0000},
        {"invaders.g", 0x0800},
        {"invaders.f", 0x1000},
        {"invaders.e", 0x1800}
    };


    // --- Load the 4 ROM files ---
    for (const auto& [filename, address] : romParts) {
        std::string fullPath = romDirectory + filename;
        if (!LoadROMFile(fullPath, memory, address)) {
            return false; // exit on invalid or missing files
        }
    }
    // ================== ROM CHECKSUM VALIDATION ======================
    
    // --- DEBUG MODE ---
    #ifdef ENABLE_MEMORY_DEBUG

        // Calculates a checksum 
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
