#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include <cstdint>
#include <string>

//--- DEBUG MODE ---  Enables DEBUG MODE 
#ifdef ENABLE_MEMORY_DEBUG
#include <unordered_map>
#include <unordered_set>
#include <vector>
#endif

//Declares and Manages Memory 
class Memory {
public:
    static const size_t MEMORY_SIZE = 0x10000; // Creates 64KB Memory
    static constexpr uint16_t VRAM_START = 0x2400; // QT: VRAM Access
    static constexpr uint16_t VRAM_END   = 0x3FFF; // QT: VRAM Access

    // =============== Constructor =================================
    Memory();  //Constructor 

    // =============== Core Memory Access ==========================
    uint8_t ReadByte(uint16_t address) const; // Read Memory
    void WriteByte(uint16_t address, uint8_t value); // Normal: Write to  Memory
    void writeRomBytes(uint16_t address, uint8_t value); // RomLoader Only:  Write to Memory

    // =============== VRAM access =================================
    std::vector<uint8_t> GetVRAM() const;  //Copy of VRAM
    const uint8_t* GetVRAMPointer() const; // Direct access 

#ifdef ENABLE_MEMORY_DEBUG
    // ================ DEBUG Tools ================================

    //  Snapshots & Comparison 
    void Snapshot(); // Snapshots the memory
    void CompareWithSnapshot() const; // Compares memory with snapshot

    // Dumping and Logging
    void DumpMemory(const std::string& filename) const; // Dumps memory 
    void DumpRegion(uint16_t start, uint16_t end) const; //Dumps a region of the RAM 
    void LogAccessCounts(const std::string& filename) const; // Logs how often each memory address was accessed
    
    // Video RAM Dump
    void DumpVRAM(uint16_t start, uint16_t end) const;

    // Create and Clear Watchpoints
    void AddWatchpoint(uint16_t address); // addes a watch point at a specific address
    void ClearWatchpoints(); // Clears your watch points

#endif

private:
    // Memory Storge
    std::array<uint8_t, MEMORY_SIZE> mem{}; // The Main Memory - 64KB for  ROM, RAM, VRAM

#ifdef ENABLE_MEMORY_DEBUG
    // ===============  DEBUG TOOLS =========================================
    // Setup States
    std::vector<uint8_t> snapshot;   // snapshots memory 
    mutable std::unordered_map<uint16_t, int> readCounts; // Tracks read Counts
    mutable std::unordered_map<uint16_t, int> writeCounts; // Tracks write counts
    mutable std::unordered_set<uint16_t> watchpoints; // Memory addresses being watched 
#endif
};

#endif 
