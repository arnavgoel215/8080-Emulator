// ============================================================================
// ROMLoader Unit Tests
// ----------------------------------------------------------------------------
// Target Module : ROMLoader (Intel 8080 Emulator)
// Purpose       : Validates file loading logic, memory boundary enforcement,
//                 ROM region restrictions, and I/O integrity.
// Scope         : Unit tests for LoadROMFile behavior (isolated from execution).
//
// Author        : Fredo
// Date          : 7/21/25
// ============================================================================

// ====================== Defines ========================================
#define ENABLE_COLOR_OUTPUT

// ====================== Header Files ===================================
#include "../../src/model/memory.hpp"
#include "../../src/model/romloader.hpp"
#include "../support/test_utils.hpp"

// ====================== Includes =======================================
#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>

// =================== Unit Test: ROM Fills Up to ROM Boundary =============
// Verifies loading large ROM that ends exactly at the ROM limit (0x1FFF)
void UnitTest_ROMLoadPartialROMBoundary() {
    Memory memory;
    const std::string filename = "rom_partial_boundary.bin";
    std::vector<uint8_t> data(0x0800, 0xAB);  // 2KB block from 0x1800 to 0x1FFF
    createBinaryFile(filename, data);

    bool loaded = LoadROMFile(filename, memory, 0x1800);
    bool verified = memory.ReadByte(0x1800) == 0xAB && memory.ReadByte(0x1FFF) == 0xAB;

    printTestResult("Unit", "ROM load up to ROM boundary (0x1FFF)", loaded && verified);
    std::remove(filename.c_str());
}

// =================== Unit Test: Basic ROM Load ===========================
// Loads a small binary into ROM at 0x0000 and checks contents
void UnitTest_ROMLoadBasic() {
    Memory memory;
    const std::string filename = "rom_basic.bin";
    createBinaryFile(filename, {0xAA, 0xBB, 0xCC});

    bool loaded = LoadROMFile(filename, memory, 0x0000);
    bool verified = memory.ReadByte(0x0000) == 0xAA && memory.ReadByte(0x0002) == 0xCC;

    printTestResult("Unit", "Basic ROM load at 0x0000", loaded && verified);
    std::remove(filename.c_str());
}

// =================== Unit Test: Load with Offset =========================
// Verifies file loads correctly at a non-zero ROM start address
void UnitTest_ROMLoadOffset() {
    Memory memory;
    const std::string filename = "rom_offset.bin";
    createBinaryFile(filename, {0xDE, 0xAD});

    bool loaded = LoadROMFile(filename, memory, 0x0100);
    bool verified = memory.ReadByte(0x0100) == 0xDE && memory.ReadByte(0x0101) == 0xAD;

    printTestResult("Unit", "ROM load at 0x0100", loaded && verified);
    std::remove(filename.c_str());
}

// =================== Unit Test: Exceeds Memory ===========================
// File too large to fit in memory should fail to load
void UnitTest_ROMLoadExceedsMemory() {
    Memory memory;
    const std::string filename = "rom_too_large.bin";
    std::vector<uint8_t> data(Memory::MEMORY_SIZE + 1, 0xFF);
    createBinaryFile(filename, data);

    bool loaded = LoadROMFile(filename, memory, 0x0000);
    printTestResult("Unit", "ROM too large blocked", !loaded);
    std::remove(filename.c_str());
}

// =================== Unit Test: ROM Near End =============================
// Attempt to load a file that starts too close to memory limit
void UnitTest_ROMLoadNearBoundary() {
    Memory memory;
    const std::string filename = "rom_end_fail.bin";
    createBinaryFile(filename, {0xAA, 0xBB});

    bool loaded = LoadROMFile(filename, memory, Memory::MEMORY_SIZE - 1);
    printTestResult("Unit", "ROM blocked at end of memory", !loaded);
    std::remove(filename.c_str());
}

// =================== Unit Test: Load at Invalid ROM Address ==============
// Validate that loading to 0xFFFF (RAM region) is blocked
void UnitTest_ROMLoadAtLastByte() {
    Memory memory;
    const std::string filename = "rom_invalid_boundary.bin";
    createBinaryFile(filename, {0xEE});

    LoadROMFile(filename, memory, 0xFFFF);  
    bool writeBlocked = memory.ReadByte(0xFFFF) == 0x00;

    printTestResult("Unit", "ROM not loaded to RAM region (0xFFFF)", writeBlocked);
    std::remove(filename.c_str());
}


// =================== Unit Test: Empty File ================================
// Loading an empty file should succeed but change nothing
void UnitTest_ROMLoadEmptyFile() {
    Memory memory;
    const std::string filename = "empty_rom.bin";
    createBinaryFile(filename, {});  // No data

    bool loaded = LoadROMFile(filename, memory, 0x0000);
    bool untouched = memory.ReadByte(0x0000) == 0x00;

    printTestResult("Unit", "Empty file loads with no changes to memory", loaded && untouched);
    std::remove(filename.c_str());
}

// =================== Unit Test: File Not Found ============================
// Missing file should be handled cleanly
void UnitTest_ROMFileNotFound() {
    Memory memory;
    bool loaded = LoadROMFile("no_such_file.rom", memory, 0x0000);
    printTestResult("Unit", "Missing ROM file fails to load", !loaded);
}

// =================== Unit Test: ROM Overwrite ============================
// Loading two ROMs into the same address — second should overwrite first
void UnitTest_ROMOverwrite() {
    Memory memory;

    const std::string fileA = "rom_A.bin";
    const std::string fileB = "rom_B.bin";

    createBinaryFile(fileA, {0x11});
    createBinaryFile(fileB, {0x99});

    LoadROMFile(fileA, memory, 0x0000);
    LoadROMFile(fileB, memory, 0x0000);

    bool overwritten = memory.ReadByte(0x0000) == 0x99;
    printTestResult("Unit", "Second ROM overwrites first at 0x0000", overwritten);

    std::remove(fileA.c_str());
    std::remove(fileB.c_str());
}

// =================== Unit Test: Memory Write to ROM Region ==============
// Ensures direct WriteByte to ROM region (e.g., 0x1000) does not modify memory
void UnitTest_MemoryWriteToROMRegion() {
    Memory memory;

    memory.WriteByte(0x1000, 0xAB);  
    bool blocked = memory.ReadByte(0x1000) != 0xAB;

    printTestResult("Unit", "Direct write to ROM region blocked", blocked);
}

// =================== Entry Point =========================================
int main() {
    std::cout << "=== Running ROMLoader Unit Tests ===\n";

    UnitTest_ROMLoadPartialROMBoundary();
    UnitTest_ROMLoadBasic();
    UnitTest_ROMLoadOffset();
    UnitTest_ROMLoadExceedsMemory();
    UnitTest_ROMLoadNearBoundary();
    UnitTest_ROMLoadAtLastByte();
    UnitTest_ROMLoadEmptyFile();
    UnitTest_ROMFileNotFound();
    UnitTest_ROMOverwrite();
    UnitTest_MemoryWriteToROMRegion();

    std::cout << "\n=== Unit Tests Complete: " << testCounter - 1 << " Total ===\n";
    return 0;
}
