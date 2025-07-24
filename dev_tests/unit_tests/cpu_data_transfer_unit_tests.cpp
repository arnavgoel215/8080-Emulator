// ============================================================================
// CPU Data Transfer Unit Tests
// ----------------------------------------------------------------------------
// Target Module : CPU / Emulator (Intel 8080 Emulator)
// Purpose       : Verifies data transfer instructions such as LXI, MOV, STAX,
//                 and loads them directly into CPU registers or memory addresses. 
//                 This ensures that register initialization is correct, 
//                 memory targeting works, and byte are handled properly.
// Scope         : Unit testing individual opcodes using isolated Emulator runs.
//
// Author        : Fredo  
// Date          : 7/23/25
// ============================================================================

// ====================== Defines ========================================
#define ENABLE_COLOR_OUTPUT
#define ENABLE_CPU_TESTING

// ======================= Include Files ==================================
#include "../../src/model/emulator.hpp"
#include "../support/test_utils.hpp"
#include <iostream>
#include <cassert>

// ====================== Unit Test: LXI B ===============================
// LXI B: Load immediate 16-bit value into BC
void UnitTest_LXI_BC_Load() {
    CPUState state = runSingleInstruction({0x01, 0xFE, 0xCA}); // LXI B,0xCAFE
    bool passed = (state.b == 0xCA && state.c == 0xFE);
    printTestResult("LXI B", "LXI B should load 0xCAFE → B = 0xCA, C = 0xFE", passed);
}

// ====================== Unit Test: LXI D ===============================
// LXI D: Load immediate 16-bit value into DE
void UnitTest_LXI_DE_Load() {
    CPUState state = runSingleInstruction({0x11, 0xEF, 0xBE}); // LXI D,0xBEEF
    bool passed = (state.d == 0xBE && state.e == 0xEF);
    printTestResult("LXI D", "LXI D should load 0xBEEF → D = 0xBE, E = 0xEF", passed);
}

// ====================== Unit Test: LXI H ===============================
// LXI H: Load immediate 16-bit value into HL
void UnitTest_LXI_HL_Load() {
    CPUState state = runSingleInstruction({0x21, 0x34, 0x12}); // LXI H,0x1234
    bool passed = (state.h == 0x12 && state.l == 0x34);
    printTestResult("LXI H", "LXI H should load 0x1234 → H = 0x12, L = 0x34", passed);
}

// ====================== Unit Test: LXI SP ==============================
// LXI SP: Load immediate 16-bit into stack pointer
void UnitTest_LXI_SP_Load() {
    CPUState state = runSingleInstruction({0x31, 0xAD, 0xDE}); // LXI SP,0xDEAD
    bool passed = (state.sp == 0xDEAD);
    printTestResult("LXI SP", "LXI SP should load 0xDEAD → SP = 0xDEAD", passed);
}

// ====================== Unit Test: MOV B,C =============================
// MOV B,C: Copy register C into register B
void UnitTest_MOV_BC_Copy() {
    Emulator emu = createTestEmulator();
    emu.getMemoryRef().writeRomBytes(0x0000, 0x41); // MOV B,C
    emu.getCPUStateRef().c = 0x99;

    emu.emulateCycles(1);
    bool passed = (emu.getCPUState().b == 0x99);
    printTestResult("MOV B,C", "MOV B,C should copy C = 0x99 → B = 0x99", passed);
}

// ====================== Unit Test: MOV H,L =============================
// MOV H,L: Copy register L into register H
void UnitTest_MOV_HL_Copy() {
    Emulator emu = createTestEmulator();
    emu.getMemoryRef().writeRomBytes(0x0000, 0x65); // MOV H,L
    emu.getCPUStateRef().l = 0x44;

    emu.emulateCycles(1);
    bool passed = (emu.getCPUState().h == 0x44);
    printTestResult("MOV H,L", "MOV H,L should copy L = 0x44 → H = 0x44", passed);
}

// ====================== Unit Test: MVI B ===============================
// MVI B,byte: Load immediate byte into register B
void UnitTest_MVI_B_Immediate() {
    CPUState state = runSingleInstruction({0x06, 0x77}); // MVI B,0x77
    bool passed = (state.b == 0x77);
    printTestResult("MVI B", "MVI B should load 0x77 → B = 0x77", passed);
}

// ====================== Unit Test: STAX B ==============================
// STAX B: Store A into memory address pointed by BC
void UnitTest_STAX_B_Write() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();

    cpu.b = 0x20;
    cpu.c = 0x00;   // BC = 0x2000 (RAM)
    cpu.a = 0x5A;

    mem.writeRomBytes(0x0000, 0x02); // STAX B
    emu.emulateCycles(1);

    bool passed = (mem.ReadByte(0x2000) == 0x5A);
    printTestResult("STAX B", "STAX B should store A = 0x5A → MEM[0x2000] = 0x5A", passed);
}

// ====================== Unit Test: STAX D ==============================
// STAX D: Store A into memory address pointed by DE
void UnitTest_STAX_D_Write() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();

    cpu.d = 0x20;
    cpu.e = 0x10;   // DE = 0x2010 (RAM)
    cpu.a = 0xAB;

    mem.writeRomBytes(0x0000, 0x12); // STAX D
    emu.emulateCycles(1);

    bool passed = (mem.ReadByte(0x2010) == 0xAB);
    printTestResult("STAX D", "STAX D should store A = 0xAB → MEM[0x2010] = 0xAB", passed);
}

// ====================== Unit Test: LDAX B ==============================
// LDAX B: Load A from memory address pointed by BC
void UnitTest_LDAX_B_Read() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();

    cpu.b = 0x20;
    cpu.c = 0x10;   // BC = 0x2010 (RAM)
    mem.WriteByte(0x2010, 0x42);

    mem.writeRomBytes(0x0000, 0x0A); // LDAX B
    emu.emulateCycles(1);

    bool passed = (cpu.a == 0x42);
    printTestResult("LDAX B", "LDAX B should load MEM[0x2010] = 0x42 → A = 0x42", passed);
}

// ====================== Unit Test: LDAX D ==============================
// LDAX D: Load A from memory address pointed by DE
void UnitTest_LDAX_D_Read() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();

    cpu.d = 0x20;
    cpu.e = 0x22;   // DE = 0x2022 (RAM)
    mem.WriteByte(0x2022, 0x99);

    mem.writeRomBytes(0x0000, 0x1A); // LDAX D
    emu.emulateCycles(1);

    bool passed = (cpu.a == 0x99);
    printTestResult("LDAX D", "LDAX D should load MEM[0x2022] = 0x99 → A = 0x99", passed);
}

// ========================= Entry Point =============================
int main() {
    std::cout << "\n=== Running CPU Data Transfer Unit Tests ===\n";

    // --- LXI Instructions ---
    UnitTest_LXI_BC_Load();
    UnitTest_LXI_DE_Load();
    UnitTest_LXI_HL_Load();
    UnitTest_LXI_SP_Load();

    // --- MOV Instructions ---
    UnitTest_MOV_BC_Copy();
    UnitTest_MOV_HL_Copy();

    // --- MVI Instructions ---
    UnitTest_MVI_B_Immediate();

    // --- STAX Instructions ---
    UnitTest_STAX_B_Write();
    UnitTest_STAX_D_Write();

    // --- LDAX Instructions ---
    UnitTest_LDAX_B_Read();
    UnitTest_LDAX_D_Read();

    std::cout << "\n=== Unit Tests Complete: " << testCounter - 1 << " Total ===\n";
    return 0;
}
