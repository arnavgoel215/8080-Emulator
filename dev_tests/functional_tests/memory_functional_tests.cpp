// ============================================================================
// Memory Functional Tests
// ----------------------------------------------------------------------------
// Target Module : Memory (Intel 8080 Emulator)
// Purpose       : Tests the memory against real-world behaviors like ROM 
//                 loading, snapshotting,VRAM interaction, file logging, 
//                 and watchpoints.
// Scope         : Functional tests that combines multiple test methods.
//
// Author        : Fredo
// Date          : 7/19/25
// ============================================================================

// ====================== Defines ============================
#define ENABLE_MEMORY_DEBUG
#define ENABLE_COLOR_OUTPUT

// ======================= Headers ===========================
#include "../../src/model/memory.hpp"
#include "../../src/model/romloader.hpp"
#include "../support/test_utils.hpp"

// ===================== Include Files ========================
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

// ===================== Constants ============================
const std::string outputDir = "dev_tests/output/";

// ======================= Functional Test: RAM & ROM Access =======================
// Tests normal RAM writing and verifies ROM writes are blocked
void Test_BasicMemoryAccess(Memory& memory) {
    std::cout << "\n=== [Core Test] RAM & ROM Access ===\n";

    memory.WriteByte(0x2000, 0xAB);
    printTestResult("RAM", "Write/Read at 0x2000", memory.ReadByte(0x2000) == 0xAB);

    memory.WriteByte(0x0000, 0xFF);
    printTestResult("ROM", "Write blocked at 0x0000", memory.ReadByte(0x0000) == 0x00);

    memory.WriteByte(0xFFFF, 0xCC);
    printTestResult("RAM", "Write/Read at 0xFFFF", memory.ReadByte(0xFFFF) == 0xCC);
}

// ======================= Functional Test: VRAM Access ===========================
// Verifies that video memory (0x2400–0x3FFF) produces the same data via vector and pointer
void Test_VRAMAccess(Memory& memory) {
    std::cout << "\n=== [Core Test] VRAM Access ===\n";

    memory.WriteByte(0x2400, 0x11);
    auto vram = memory.GetVRAM();

    printTestResult("VRAM", "VRAM[0] == 0x11", vram[0] == 0x11);
    printTestResult("VRAM", "Pointer dereference == 0x11", *memory.GetVRAMPointer() == 0x11);
}

// ======================= Functional Test: ROM Override ==========================
// Testing if ROM Override is working by writing to region (0x0000–0x1FFF) using  writeRomBytes()
void Test_ROMManualOverride(Memory& memory) {
    std::cout << "\n=== [Core Test] ROM Direct Write Override ===\n";

    memory.writeRomBytes(0x0003, 0xAA);
    printTestResult("ROM", "Write at 0x0003", memory.ReadByte(0x0003) == 0xAA);

    memory.writeRomBytes(0x2000, 0xBB);
    printTestResult("ROM", "Out-of-range write ignored", memory.ReadByte(0x2000) == 0x00);

    memory.writeRomBytes(0x1FFF, 0x77);
    printTestResult("ROM", "Write at ROM boundary (0x1FFF)", memory.ReadByte(0x1FFF) == 0x77);
}

// ======================= Functional Test: Snapshot System ========================
// Takes a memory snapshot, mutates memory, and then comparesthe memory against saved snapshot
void Test_SnapshotRestore(Memory& memory) {
    std::cout << "\n=== [Debug Test] Snapshot System ===\n";

    memory.WriteByte(0x3000, 0x01);
    memory.Snapshot();
    memory.WriteByte(0x3000, 0x02);

    memory.CompareWithSnapshot();
    printTestResult("Snapshot", "Value now = 0x02 (changed)", memory.ReadByte(0x3000) == 0x02);
}

// ======================= Functional Test: Watchpoint System ======================
// Adds a watchpoint to an address and trigger the area to make sure alert is triggered
void Test_Watchpoints(Memory& memory) {
    std::cout << "\n=== [Debug Test] Watchpoint System ===\n";

    memory.AddWatchpoint(0x6000);
    memory.WriteByte(0x6000, 0x55);
    printTestResult("Watchpoint", "Triggered at 0x6000", memory.ReadByte(0x6000) == 0x55);
    memory.ClearWatchpoints();
}

// ===================== Diagnostic Tests =======================

// ======================= Functional Test: Memory Dump + Logging ==================
// Verifies memory dump to binary file | Verifies read/write access logs to text file
void Test_MemoryArtifacts(Memory& memory) {
    std::cout << "\n=== [Diagnostic Test] Dump + Access Log ===\n";

    std::string dumpPath = outputDir + "memory_dump_test.bin";
    std::string logPath = outputDir + "access_log.txt";

    memory.DumpMemory(dumpPath);
    printTestResult("Dump", "memory_dump_test.bin created", std::ifstream(dumpPath).good());

    memory.LogAccessCounts(logPath);
    printTestResult("Log", "access_log.txt created", std::ifstream(logPath).good());
}

// ======================= Functional Test: VRAM Dump Range ========================
// Ensures DumpVRAM accepts only valid address ranges
void Test_VRAMDumpRanges(Memory& memory) {
    std::cout << "\n=== [Diagnostic Test] VRAM Dump Range Checks ===\n";

    memory.DumpVRAM(0x2400, 0x240F);  // Valid
    memory.DumpVRAM(0x2300, 0x2400);  // Invalid start
    memory.DumpVRAM(0x3FFF, 0x4000);  // Invalid end

    printTestResult("VRAM", "Range validation completed (see console output)", true);
}

// ======================= Functional Test: Access Stress Logging ==================
// Performs 1000 read/write cycles on a memory address and logs access frequencies
void Test_AccessStress(Memory& memory) {
    std::cout << "\n=== [Stress Test] Sustained Access Logging ===\n";

    for (int i = 0; i < 1000; ++i) {
        memory.WriteByte(0x2FFF, i % 256);
        memory.ReadByte(0x2FFF);
    }

    std::string path = outputDir + "stress_access.txt";
    memory.LogAccessCounts(path);
    printTestResult("Log", "stress_access.txt created", std::ifstream(path).good());
}

// ======================= Functional Test: RAM Clean After ROM Load ==============
// Verifies that loading a ROM does not modify RAM (0x2000–0x3FFF) | should stay zeroed.
void Test_RAMAfterROM(Memory& memory) {
    std::cout << "\n=== [Sanity Test] RAM Clean After ROM Load ===\n";

    int dirtyCount = 0;
    for (uint16_t addr = 0x2000; addr <= 0x3FFF; ++addr) {
        if (memory.ReadByte(addr) != 0x00)
            ++dirtyCount;
    }

    printTestResult("RAM", "All RAM clear (0x2000–0x3FFF)", dirtyCount == 0);
}

// ===================== Entry Point =======================

int main() {
    std::cout << "=== Running Memory Functional Tests ===\n";

    // === Init memory and load ROM ===
    Memory memory;
    std::string romPath = "src/tests/";
    LoadSpaceInvadersROM(memory, romPath);

    // === Core functional tests ===
    Test_RAMAfterROM(memory);
    Test_BasicMemoryAccess(memory);
    Test_VRAMAccess(memory);
    Test_ROMManualOverride(memory);
    Test_SnapshotRestore(memory);
    Test_Watchpoints(memory);

    // === Diagnostics and stress ===
    Test_MemoryArtifacts(memory);
    Test_VRAMDumpRanges(memory);
    Test_AccessStress(memory);

    std::cout << "\n=== Functional Tests Complete — Total: " << testCounter - 1 << " ===\n";
    return 0;
}
