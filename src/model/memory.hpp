// ============================================================
// Memory Module for the Intel 8080 
// ------------------------------------------------------------
//
// Emulates the memory of the Intel 8080
// Creates 64KB of emulated RAM divide into 4 sections:
// ROM: 0x0000 – 0x1FFF | (READ ONLY - Write Protected)
// Working RAM: 0x2000 – 0x23FF | CPU Functions
// Video RAM: 0x2400 – 0x3FFF | Video RAM for QT
// Expansion / Stack:0x4000 – 0xFFFF | Emulation Core and I/O
//
// Provides memory access for: RAM/ROM access, VRAM, Debugging
// Author: Fredo | Date: 7/19/25
// ============================================================


// ====================== Define Files ====================================
#ifndef MEMORY_HPP
#define MEMORY_HPP

// ======================= Include Files ===================================
#include <array>
#include <cstdint>
#include <string>
#include <vector>

// ====================== ENABLE MEMORY DEBUG =============================
//--- DEBUG MODE ---  Enables DEBUG MODE 
#ifdef ENABLE_MEMORY_DEBUG
#include <unordered_map>
#include <unordered_set>

#endif // --- END DEBUG -- 

// ================== Memory Class ========================================
// Declares and Manages Memory 
class Memory {
public:
    static const size_t MEMORY_SIZE = 0x10000; // Creates 64KB Memory
    static constexpr uint16_t VRAM_START = 0x2400; // QT: VRAM Access
    static constexpr uint16_t VRAM_END   = 0x3FFF; // QT: VRAM Access

    // =============== Constructor ================================
    Memory();   

    // =============== Core Memory Access ==========================
    // === Main Memory === 
    // Used to Read, Write and Write the ROM to memory
    uint8_t ReadByte(uint16_t address) const; 
    void WriteByte(uint16_t address, uint8_t value); 
    void writeRomBytes(uint16_t address, uint8_t value); 
    void Clear();

    // === VRAM access ===
    // Copy of VRAM for QT | Direct Read only access to VRAM via pointer
    std::vector<uint8_t> GetVRAM() const;  
    const uint8_t* GetVRAMPointer() const; 

#ifdef ENABLE_MEMORY_DEBUG
    // ================ DEBUG Tools ================================
    //  === Snapshots & Comparison ===
    // Used to Snapshot memory | Compares snapshots to Memory
    void Snapshot(); 
    void CompareWithSnapshot() const; 

    // === Dumping and Logging ===
    // Used for Dumping Memory, Dumping a region of memory, and creating an access log
    void DumpMemory(const std::string& filename) const; 
    void DumpRegion(uint16_t start, uint16_t end) const; 
    void LogAccessCounts(const std::string& filename) const; 
    
    // === Video RAM Dump ===
    void DumpVRAM(uint16_t start, uint16_t end) const;

    // === Create and Clear Watchpoints ===
    // Adds a watch point at specific address | Clears watchpoints
    void AddWatchpoint(uint16_t address); 
    void ClearWatchpoints(); 

#endif

private:
    // === Memory Storge ===
    // The Main Memory - 64KB for  ROM, RAM, VRAM
    std::array<uint8_t, MEMORY_SIZE> mem{}; 

#ifdef ENABLE_MEMORY_DEBUG
    // ===============  DEBUG TOOLS =========================================
    // === Setup States ===
    // Sets up snapshot, readCounts, writeCounts, and watchpoint States
    std::vector<uint8_t> snapshot;   
    mutable std::unordered_map<uint16_t, int> readCounts; 
    mutable std::unordered_map<uint16_t, int> writeCounts; 
    mutable std::unordered_set<uint16_t> watchpoints; 
#endif
};

#endif // memory.hpp
