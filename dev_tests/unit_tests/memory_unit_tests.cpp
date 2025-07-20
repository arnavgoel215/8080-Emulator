// ============================================================================
// Memory Unit Tests
// ----------------------------------------------------------------------------
// Target Module : Memory (Intel 8080 Emulator)
// Purpose       : Verifies isolated behavior of memory read/write operations,
//                 ROM protection, VRAM access, debug features (snapshot, log).
// Scope         : Unit testing individual methods with minimal external setup.
//
// Author        : Fredo  
// Date          : 7/19/25
// ============================================================================

// ====================== Defines ========================================
#define ENABLE_MEMORY_DEBUG
#define ENABLE_COLOR_OUTPUT


// ======================= Include Files ==================================
#include "../../src/model/memory.hpp"
#include "../support/test_utils.hpp"
#include <cassert>
#include <iostream>

// =================== Unit Test: RAM Zero on Init ====================
// RAM should be zero-initialized on startup
void UnitTest_RAMZeroOnInit() {
    Memory memory;
    bool clean = true;
    for (uint16_t addr = 0x2000; addr < 0x2400; ++addr) {
        if (memory.ReadByte(addr) != 0x00) {
            clean = false;
            break;
        }
    }
    printTestResult("Unit", "RAM zeroed on init (0x2000–0x23FF)", clean);
}

// ======================= Unit Test: ROM Protection =======================
// Writing to ROM with WriteByte() should be ignored
void UnitTest_ROMWriteBlocked() {
    Memory memory;
    memory.WriteByte(0x0005, 0xAA);  // ROM address
    bool result = (memory.ReadByte(0x0005) == 0x00);
    printTestResult("Unit", "ROM write blocked at 0x0005", result);
}

// ======================= Unit Test: ROM Boundary Block ========================
// Ensure WriteByte() cannot write to last ROM address (0x1FFF)
void UnitTest_ROMBoundaryBlocked() {
    Memory memory;
    memory.WriteByte(0x1FFF, 0xFF);
    bool result = (memory.ReadByte(0x1FFF) == 0x00);
    printTestResult("Unit", "ROM boundary write blocked at 0x1FFF", result);
}

// =================== Unit Test: Write to 0xFFFF ====================
// Testing if last addressable memory is writable
void UnitTest_WriteAtFFFF() {
    Memory memory;
    memory.WriteByte(0xFFFF, 0x7E);
    bool result = (memory.ReadByte(0xFFFF) == 0x7E);
    printTestResult("Unit", "Write/read at 0xFFFF", result);
}

// ======================= Unit Test: ROM Override ========================
// writeRomBytes() should allow writing directly to ROM
void UnitTest_ROMOverrideAllowed() {
    Memory memory;
    memory.writeRomBytes(0x0010, 0xBB);
    bool result = (memory.ReadByte(0x0010) == 0xBB);
    printTestResult("Unit", "ROM override allowed at 0x0010", result);
}

// ======================= Unit Test: RAM Access ==========================
// RAM (0x2000 and up) should be fully writable
void UnitTest_RAMWriteRead() {
    Memory memory;
    memory.WriteByte(0x2000, 0x42);
    bool result = (memory.ReadByte(0x2000) == 0x42);
    printTestResult("Unit", "RAM read/write at 0x2000", result);
}

// =================== Unit Test: ROM Override Out-of-Range ====================
// writeRomBytes() should not allow be allowed to write past 0x1FFF
void UnitTest_ROMOverrideOutOfRange() {
    Memory memory;
    memory.writeRomBytes(0x2000, 0xAB);  // Illegal ROM address
    bool result = (memory.ReadByte(0x2000) == 0x00);  // Should not be written
    printTestResult("Unit", "ROM override blocked at 0x2000", result);
}


// ======================= Unit Test: VRAM Access =========================
// Writing to VRAM should show that value in GetVRAM()
void UnitTest_VRAMReflectsWrite() {
    Memory memory;
    memory.WriteByte(0x2400, 0x33);  // First byte of VRAM
    auto vram = memory.GetVRAM();
    bool result = (vram.size() == 0x3FFF - 0x2400 + 1 && vram[0] == 0x33);
    printTestResult("Unit", "VRAM reflects write at 0x2400", result);
}

// ======================= Unit Test: GetVRAMPointer ======================
// Pointer returned by GetVRAMPointer() should return correct memory address
void UnitTest_VRAMPointerCorrect() {
    Memory memory;
    memory.WriteByte(0x2400, 0x77);
    const uint8_t* ptr = memory.GetVRAMPointer();
    bool result = (ptr[0] == 0x77);
    printTestResult("Unit", "VRAM pointer valid at 0x2400", result);
}

// =================== Unit Test: VRAM Size Correct ====================
// Confirming VRAM vector is exactly (0x3FFF - 0x2400 + 1) bytes long
void UnitTest_VRAMSizeCheck() {
    Memory memory;
    auto vram = memory.GetVRAM();
    bool result = (vram.size() == 0x3FFF - 0x2400 + 1);
    printTestResult("Unit", "VRAM vector size correct", result);
}


#ifdef ENABLE_MEMORY_DEBUG
// =================== Unit Test: Snapshot Match ==========================
// Test Snapshot: Should match if memory is unchanged
void UnitTest_SnapshotNoChange() {
    Memory memory;
    memory.WriteByte(0x3000, 0x01);
    memory.Snapshot();
    bool result = true;
    for (uint16_t addr = 0x3000; addr <= 0x3000; ++addr) {
        result &= (memory.ReadByte(addr) == 0x01);
    }
    printTestResult("Unit", "Snapshot matches current memory", result);
}

// =================== Unit Test: Watchpoint Trigger =====================
// Test Watchpoint: Should trigger and new memory value should be unchanged
void UnitTest_WatchpointTracking() {
    Memory memory;
    memory.AddWatchpoint(0x6000);
    memory.WriteByte(0x6000, 0x12);
    uint8_t val = memory.ReadByte(0x6000);
    memory.ClearWatchpoints();
    bool result = (val == 0x12);
    printTestResult("Unit", "Watchpoint read/write works", result);
}
#endif

// ======================= Entry Point ============================
int main() {
    std::cout << "=== Running Memory Unit Tests ===\n";

    // === Core Memory Tests ===
    UnitTest_ROMWriteBlocked();
    UnitTest_ROMBoundaryBlocked();
    UnitTest_ROMOverrideAllowed();
    UnitTest_ROMOverrideOutOfRange();
    UnitTest_RAMWriteRead();
    UnitTest_RAMZeroOnInit();
    UnitTest_WriteAtFFFF();

    // === VRAM Tests ===
    UnitTest_VRAMReflectsWrite();
    UnitTest_VRAMSizeCheck();
    UnitTest_VRAMPointerCorrect();

    // === Debug Tests ===
#ifdef ENABLE_MEMORY_DEBUG
    UnitTest_SnapshotNoChange();
    UnitTest_WatchpointTracking();
#endif

    std::cout << "\n=== Unit Tests Complete: " << testCounter - 1 << " Total ===\n";
    return 0;
}
