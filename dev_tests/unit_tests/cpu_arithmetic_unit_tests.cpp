// ============================================================================
// CPU Arithmetic Unit Tests
// ----------------------------------------------------------------------------
// Target Module : CPU / Emulator (Intel 8080 Emulator)
// Purpose       : Verifies arithmetic instructions including ADD, SUB, INR,
//                 DCR, and immediate variants. Ensures register updates and
//                 flag accuracy (Z, S, P, CY, AC) are correct.
// Scope         : Unit testing individual arithmetic instructions with both
//                 register and immediate operands.
//
// Author        : Fredo  
// Date          : 7/23/25
// ============================================================================


// ====================== Defines ========================================
#define ENABLE_COLOR_OUTPUT
#define ENABLE_CPU_TESTING
#define ENABLE_VERBOSE_DEBUG

// ======================= Include Files ==================================
#include "../../src/model/emulator.hpp"
#include "../support/test_utils.hpp"
#include <iostream>
#include <cassert>

// ===================== Basic Register Addition ==========================

// ====================== Unit Test: ADD B ===============================
// ADD B: A = A + B
void UnitTest_ADD_B() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.a = 0x10;
    cpu.b = 0x22;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x80);
    emu.emulateCycles(1);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD B",
        0x10, 0x22, false, cpu.a, cpu.flags,
        0x32, false);
#endif
    printTestResult("ADD B", "A = 0x10 + B = 0x22 → A = 0x32, CY = 0", cpu.a == 0x32 && !cpu.flags.cy);
}

// ====================== Unit Test: ADD C ===============================
// ADD C: A = A + C (with carry expected)
void UnitTest_ADD_C_WithCarry() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.a = 0xF0;
    cpu.c = 0x30;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x81);
    emu.emulateCycles(1);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD C",
        0xF0, 0x30, false, cpu.a, cpu.flags,
        0x20, true);
#endif
    printTestResult("ADD C", "A = 0xF0 + C = 0x30 → A = 0x20, CY = 1", cpu.a == 0x20 && cpu.flags.cy);
}

// ====================== Unit Test: ADD A ===============================
// ADD A: A = A + A
void UnitTest_ADD_AA() {
    CPUState state = runSingleInstruction({0x87}, [](CPUState& cpu) {
        cpu.a = 0x22;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD A,A",
        0x22, 0x22, false, state.a, state.flags,
        0x44, false);
#endif
    printTestResult("ADD A,A", "A = 0x22 + A = 0x22 → A = 0x44", state.a == 0x44);
}

// ====================== Unit Test: ADD 0xFF ============================
// ADI: A = A + 0xFF (overflow with carry)
void UnitTest_ADD_FF() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();
    cpu.a = 0x02;
    mem.writeRomBytes(0x0000, 0xC6);
    mem.writeRomBytes(0x0001, 0xFF);
    emu.emulateCycles(1);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD 0xFF",
        0x02, 0xFF, false, cpu.a, cpu.flags,
        0x01, true);
#endif
    printTestResult("ADD 0xFF", "A = 0x02 + 0xFF → A = 0x01, CY = 1", cpu.a == 0x01 && cpu.flags.cy);
}

// ====================== Unit Test: ADC B ===============================
// ADC B: A = A + B + CY
void UnitTest_ADC_B() {
    CPUState state = runSingleInstruction({0x88}, [](CPUState& cpu) {
        cpu.a = 0x10; cpu.b = 0x20; cpu.flags.cy = true;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADC B",
        0x10, 0x20, true, state.a, state.flags,
        0x31, false);
#endif
    printTestResult("ADC B", "A = 0x10 + B = 0x20 + CY → A = 0x31", state.a == 0x31);
}

// ====================== Unit Test: ADI 0x10 ============================
// ADI: A = A + immediate
void UnitTest_ADI_Immediate() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();
    cpu.a = 0x22;
    mem.writeRomBytes(0x0000, 0xC6);
    mem.writeRomBytes(0x0001, 0x10);
    emu.emulateCycles(1);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADI",
        0x22, 0x10, false, cpu.a, cpu.flags,
        0x32, false);
#endif
    printTestResult("ADI", "A = 0x22 + 0x10 → A = 0x32, CY = 0", cpu.a == 0x32 && !cpu.flags.cy);
}

// ====================== Unit Test: ACI ================================
// ACI: A = A + immediate + CY
void UnitTest_ACI_Immediate() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();
    cpu.a = 0x05;
    cpu.flags.cy = true;
    mem.writeRomBytes(0x0000, 0xCE);
    mem.writeRomBytes(0x0001, 0x10);
    emu.emulateCycles(1);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ACI",
        0x05, 0x10, true, cpu.a, cpu.flags,
        0x16, false);
#endif
    printTestResult("ACI", "A = 0x05 + 0x10 + CY → A = 0x16", cpu.a == 0x16);
}

// ================= Basic Subtraction and Variants =======================

// ====================== Unit Test: SUB B ===============================
// SUB B: A = A - B
void UnitTest_SUB_B() {
    CPUState state = runSingleInstruction({0x90}, [](CPUState& cpu) {
        cpu.a = 0x50; cpu.b = 0x20;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("SUB B",
        0x50, 0x20, false, state.a, state.flags,
        0x30, false);
#endif
    printTestResult("SUB B", "A = 0x50 - B = 0x20 → A = 0x30, CY = 0", state.a == 0x30 && !state.flags.cy);
}

// ====================== Unit Test: SUB B Carry ==========================
// SUB B: A = A - B (underflow with carry)
void UnitTest_SUB_B_Carry() {
    CPUState state = runSingleInstruction({0x90}, [](CPUState& cpu) {
        cpu.a = 0x10; cpu.b = 0x30;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("SUB B Carry",
        0x10, 0x30, false, state.a, state.flags,
        0xE0, true);
#endif
    printTestResult("SUB B Carry", "A = 0x10 - B = 0x30 → A = 0xE0, CY = 1", state.a == 0xE0 && state.flags.cy);
}

// ====================== Unit Test: SUB A Zero ===========================
// SUB A: A = A - A = 0 → Z = 1
void UnitTest_SUB_A_Zero() {
    CPUState state = runSingleInstruction({0x97}, [](CPUState& cpu) {
        cpu.a = 0x44;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("SUB A",
        0x44, 0x44, false, state.a, state.flags,
        0x00, false, true);
#endif
    printTestResult("SUB A", "A = 0x44 - A = 0x44 → A = 0x00, Z = 1", state.a == 0x00 && state.flags.z);
}

// ====================== Unit Test: SBB C ================================
// SBB C: A = A - C - CY
void UnitTest_SBB_C() {
    CPUState state = runSingleInstruction({0x99}, [](CPUState& cpu) {
        cpu.a = 0x40; cpu.c = 0x10; cpu.flags.cy = true;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("SBB C",
        0x40, 0x10, true, state.a, state.flags,
        0x2F, false);
#endif
    printTestResult("SBB C", "A = 0x40 - C = 0x10 - 1 → A = 0x2F", state.a == 0x2F);
}

// ====================== Unit Test: SUI Immediate ========================
// SUI: A = A - immediate
void UnitTest_SUI_Immediate() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();
    cpu.a = 0x50;
    mem.writeRomBytes(0x0000, 0xD6);
    mem.writeRomBytes(0x0001, 0x20);
    emu.emulateCycles(1);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("SUI",
        0x50, 0x20, false, cpu.a, cpu.flags,
        0x30, false);
#endif
    printTestResult("SUI", "A = 0x50 - 0x20 → A = 0x30, CY = 0", cpu.a == 0x30 && !cpu.flags.cy);
}

// ====================== Unit Test: SUI Negative =========================
// SUI: A = A - immediate (underflow case)
void UnitTest_SUI_Negative() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();
    cpu.a = 0x50;
    mem.writeRomBytes(0x0000, 0xD6);
    mem.writeRomBytes(0x0001, 0x60);
    emu.emulateCycles(1);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("SUI Underflow",
        0x50, 0x60, false, cpu.a, cpu.flags,
        0xF0, true);
#endif
    printTestResult("SUI Underflow", "A = 0x50 - 0x60 → A = 0xF0", cpu.a == 0xF0);
}

// ====================== Unit Test: SBI Immediate ========================
// SBI: A = A - immediate - CY
void UnitTest_SBI_Immediate() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();
    cpu.a = 0x50;
    cpu.flags.cy = true;
    mem.writeRomBytes(0x0000, 0xDE);
    mem.writeRomBytes(0x0001, 0x10);
    emu.emulateCycles(1);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("SBI",
        0x50, 0x10, true, cpu.a, cpu.flags,
        0x3F, false);
#endif
    printTestResult("SBI", "A = 0x50 - 0x10 - CY → A = 0x3F", cpu.a == 0x3F);
}

// =================== INR / DCR Tests (Register and Memory) ==============

// ====================== Unit Test: INR C ===============================
// INR C: Increment register C, should set Sign
void UnitTest_INR_C() {
    CPUState state = runSingleInstruction({0x0C}, [](CPUState& cpu) {
        cpu.c = 0x7F;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("INR C",
        0x7F, 0x01, false, state.c, state.flags,
        0x80, false, false, std::nullopt, std::nullopt);
#endif
    printTestResult("INR C", "C = 0x7F + 1 → C = 0x80, S = 1, Z = 0", state.c == 0x80 && state.flags.s && !state.flags.z);
}

// ====================== Unit Test: INR A Wrap ===========================
// INR A: A = 0xFF + 1 → 0x00, check zero flag
void UnitTest_INR_A_Wrap() {
    CPUState state = runSingleInstruction({0x3C}, [](CPUState& cpu) {
        cpu.a = 0xFF;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("INR A Wrap",
        0xFF, 0x01, false, state.a, state.flags,
        0x00, false, true);
#endif
    printTestResult("INR A Wrap", "A = 0xFF + 1 → A = 0x00, Z = 1", state.a == 0x00 && state.flags.z);
}

// ====================== Unit Test: INR M ===============================
// INR M: Increment memory at HL
void UnitTest_INR_Memory() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();
    cpu.h = 0x30; cpu.l = 0x00;
    mem.WriteByte(0x3000, 0x7F);
    mem.writeRomBytes(0x0000, 0x34);
    emu.emulateCycles(1);
    uint8_t result = mem.ReadByte(0x3000);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("INR M",
        0x7F, 0x01, false, result, cpu.flags,
        0x80, false);
#endif
    printTestResult("INR M", "MEM[HL] = 0x7F + 1 → 0x80, S=1", result == 0x80 && cpu.flags.s);
}

// ====================== Unit Test: DCR D ===============================
// DCR D: Decrement register D to 0, check zero flag
void UnitTest_DCR_D() {
    CPUState state = runSingleInstruction({0x15}, [](CPUState& cpu) {
        cpu.d = 0x01;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("DCR D",
        0x01, 0x01, false, state.d, state.flags,
        0x00, false, true);
#endif
    printTestResult("DCR D", "D = 0x01 - 1 → D = 0x00, Z = 1", state.d == 0x00 && state.flags.z);
}

// ====================== Unit Test: DCR B Zero ===========================
// DCR B: B = 0x01 - 1 → 0x00, set Z flag
void UnitTest_DCR_B_Zero() {
    CPUState state = runSingleInstruction({0x05}, [](CPUState& cpu) {
        cpu.b = 0x01;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("DCR B",
        0x01, 0x01, false, state.b, state.flags,
        0x00, false, true);
#endif
    printTestResult("DCR B Zero", "B = 0x01 - 1 → B = 0x00, Z = 1", state.b == 0x00 && state.flags.z);
}

// ====================== Unit Test: DCR M ===============================
// DCR M: Decrement memory at HL to 0, set Z flag
void UnitTest_DCR_Memory() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();
    cpu.h = 0x40; cpu.l = 0x00;
    mem.WriteByte(0x4000, 0x01);
    mem.writeRomBytes(0x0000, 0x35);
    emu.emulateCycles(1);
    uint8_t result = mem.ReadByte(0x4000);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("DCR M",
        0x01, 0x01, false, result, cpu.flags,
        0x00, false, true);
#endif
    printTestResult("DCR M", "MEM[HL] = 0x01 - 1 → 0x00, Z=1", result == 0x00 && cpu.flags.z);
}

// =================== 16-bit Arithmetic (DAD) ============================

// ====================== Unit Test: DAD H ===============================
// DAD H: HL = HL + HL
void UnitTest_DAD_H() {
    CPUState state = runSingleInstruction({0x29}, [](CPUState& cpu) {
        cpu.h = 0x12;
        cpu.l = 0x34;
    });

    uint16_t hl = (state.h << 8) | state.l;
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("DAD H",
        0x12, 0x12, false, state.l, state.flags,
        0x68, false);
    std::cout << "  Computed HL     = 0x" << std::hex << hl << std::dec << "\n";
#endif
    printTestResult("DAD H", "HL = HL + HL → HL = 0x2468", hl == 0x2468);
}

// ====================== Unit Test: DAD CY ===============================
// DAD H: HL = 0xFFFF + HL → Carry
void UnitTest_DAD_Carry() {
    CPUState state = runSingleInstruction({0x29}, [](CPUState& cpu) {
        cpu.h = 0xFF;
        cpu.l = 0xFF;
    });

    uint16_t hl = (state.h << 8) | state.l;
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("DAD CY",
        0xFF, 0xFF, false, state.l, state.flags,
        0xFE, true);
    std::cout << "  Computed HL     = 0x" << std::hex << hl << std::dec << "\n";
#endif
    printTestResult("DAD CY", "DAD H with HL=0xFFFF should set CY", state.flags.cy);
}

// ====================== Unit Test: DAD B ===============================
// DAD B: HL = HL + BC
void UnitTest_DAD_B() {
    CPUState state = runSingleInstruction({0x09}, [](CPUState& cpu) {
        cpu.b = 0x01;
        cpu.c = 0x10;
        cpu.h = 0x00;
        cpu.l = 0x20;
    });

    uint16_t hl = (state.h << 8) | state.l;
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("DAD B",
        0x20, 0x01, false, state.l, state.flags,
        0x30, false);
    std::cout << "  Computed HL     = 0x" << std::hex << hl << std::dec << "\n";
#endif
    printTestResult("DAD B", "HL = HL + BC → HL = 0x0130", hl == 0x0130);
}

// ====================== Unit Test: DAD D ===============================
// DAD D: HL = HL + DE
void UnitTest_DAD_D() {
    CPUState state = runSingleInstruction({0x19}, [](CPUState& cpu) {
        cpu.d = 0x00;
        cpu.e = 0x05;
        cpu.h = 0x00;
        cpu.l = 0x05;
    });

    uint16_t hl = (state.h << 8) | state.l;
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("DAD D",
        0x05, 0x05, false, state.l, state.flags,
        0x0A, false);
    std::cout << "  Computed HL     = 0x" << std::hex << hl << std::dec << "\n";
#endif
    printTestResult("DAD D", "HL = HL + DE → HL = 0x000A", hl == 0x000A);
}

// ====================== Unit Test: DAD SP ===============================
// DAD SP: HL = HL + SP
void UnitTest_DAD_SP() {
    CPUState state = runSingleInstruction({0x39}, [](CPUState& cpu) {
        cpu.sp = 0x1234;
        cpu.h = 0x00;
        cpu.l = 0x10;
    });

    uint16_t hl = (state.h << 8) | state.l;
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("DAD SP",
        0x10, 0x12, false, state.l, state.flags,
        0x44, false);
    std::cout << "  Computed HL     = 0x" << std::hex << hl << std::dec << "\n";
#endif
    printTestResult("DAD SP", "HL = HL + SP → HL = 0x1244", hl == 0x1244);
}

// =================== CMP and CPI Instructions ===========================

// ====================== Unit Test: CMP B (Z) ===========================
// CMP B: A == B → set Z flag
void UnitTest_CMP_B_Z() {
    CPUState state = runSingleInstruction({0xB8}, [](CPUState& cpu) {
        cpu.a = 0x42;
        cpu.b = 0x42;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("CMP B (Z)",
        0x42, 0x42, false, state.a, state.flags,
        0x42, false, true);
#endif
    printTestResult("CMP B", "A = B → Z = 1", state.flags.z);
}

// ====================== Unit Test: CMP B (LT) ==========================
// CMP B: A < B → set CY, clear Z
void UnitTest_CMP_B_LT() {
    CPUState state = runSingleInstruction({0xB8}, [](CPUState& cpu) {
        cpu.a = 0x30;
        cpu.b = 0x40;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("CMP B (LT)",
        0x30, 0x40, false, state.a, state.flags,
        0x30, true, false);
#endif
    printTestResult("CMP B", "A < B → CY = 1, Z = 0", state.flags.cy && !state.flags.z);
}

// ====================== Unit Test: CMP B (GT) ==========================
// CMP B: A > B → clear CY, Z
void UnitTest_CMP_B_GT() {
    CPUState state = runSingleInstruction({0xB8}, [](CPUState& cpu) {
        cpu.a = 0x50;
        cpu.b = 0x20;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("CMP B (GT)",
        0x50, 0x20, false, state.a, state.flags,
        0x50, false, false);
#endif
    printTestResult("CMP B", "A > B → CY = 0, Z = 0", !state.flags.cy && !state.flags.z);
}

// ====================== Unit Test: CPI Sign Flip =======================
// CPI: Compare A = 0x10 and immediate = 0x80 (signed flip)
void UnitTest_CPI_SignFlip() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();
    cpu.a = 0x10;
    mem.writeRomBytes(0x0000, 0xFE);
    mem.writeRomBytes(0x0001, 0x80);
    emu.emulateCycles(1);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("CPI SignFlip",
        0x10, 0x80, false, cpu.a, cpu.flags,
        0x10, true, false, std::nullopt, true);
#endif
    printTestResult("CPI", "A = 0x10, Imm = 0x80 → CY = 1, S = 1", cpu.flags.cy && cpu.flags.s);
}

// =================== Logical AND ========================================

// ====================== Unit Test: ADD No-Carry Overflow ===================
// ADD: A = 0x7F + B = 0x01 → A = 0x80, CY = 0 (overflow, no carry)
void UnitTest_ADD_NoCarry_Overflow() {
    CPUState state = runSingleInstruction({0x80}, [](CPUState& cpu) {
        cpu.a = 0x7F; cpu.b = 0x01;
    });
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD Overflow",
        0x7F, 0x01, false, state.a, state.flags,
        0x80, false);
#endif
    printTestResult("ADD Overflow", "A = 0x7F + B = 0x01 → A = 0x80, CY = 0", state.a == 0x80 && !state.flags.cy);
}

// ====================== Unit Test: ANA B ===============================
// ANA B: A = A & B, logical AND between A and B
void UnitTest_ANA_B() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();
    cpu.a = 0xF0;
    cpu.b = 0xCC;
    mem.writeRomBytes(0x0000, 0xA0); // ANA B opcode
    emu.emulateCycles(1);
#ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ANA B",
        0xF0, 0xCC, false, cpu.a, cpu.flags,
        0xC0, false);
#endif
    printTestResult("ANA B", "A = 0xF0 & B = 0xCC → A = 0xC0", cpu.a == (0xF0 & 0xCC));
}


// ========================= Entry Point =============================
int main() {
    std::cout << "\n=== Running CPU Arithmetic Unit Tests ===\n\n";

    // === Basic Register Addition ===
    std::cout << "=== Starting Basic Register Addition Tests ===\n";
    UnitTest_ADD_B();
    UnitTest_ADD_C_WithCarry();
    UnitTest_ADD_AA();
    UnitTest_ADD_FF();
    UnitTest_ADC_B();
    UnitTest_ADI_Immediate();
    UnitTest_ACI_Immediate();
     std::cout << "=== Basic Register Addition Tests Complete ===\n\n";

    // === Basic Subtraction and Variants ===
    std::cout << "=== Starting Basic Subtraction and Variants Tests ===\n";
    UnitTest_SUB_B();
    UnitTest_SUB_B_Carry();
    UnitTest_SUB_A_Zero();
    UnitTest_SBB_C();
    UnitTest_SUI_Immediate();
    UnitTest_SUI_Negative();
    UnitTest_SBI_Immediate();
    std::cout << "=== Basic Subtraction and Variants Tests Complete ===\n\n";

    // === INR / DCR Tests (Register and Memory) ===
    std::cout << "=== Starting INR / DCR Tests (Register and Memory) Tests ===\n";
    UnitTest_INR_C();
    UnitTest_INR_A_Wrap();
    UnitTest_INR_Memory();
    UnitTest_DCR_D();
    UnitTest_DCR_B_Zero();
    UnitTest_DCR_Memory();
    std::cout << "=== INR / DCR Tests (Register and Memory) Tests Complete ===\n\n";

    // === 16-bit Arithmetic (DAD) ===
    std::cout << "=== Starting 16-bit Arithmetic (DAD) Tests ===\n";
    UnitTest_DAD_H();
    UnitTest_DAD_Carry();
    UnitTest_DAD_B();
    UnitTest_DAD_D();
    UnitTest_DAD_SP();
    std::cout << "=== 16-bit Arithmetic (DAD) Tests Complete ===\n\n";

    // === CMP and CPI Instructions ===
    std::cout << "=== Starting CMP and CPI Instructions Tests ===\n";
    UnitTest_CMP_B_Z();
    UnitTest_CMP_B_LT();
    UnitTest_CMP_B_GT();
    UnitTest_CPI_SignFlip();
    std::cout << "=== CMP and CPI Instructions Tests Complete ===\n\n";

    // === Logical AND ===
    std::cout << "=== Starting Logical AND Tests ===\n";
    UnitTest_ADD_NoCarry_Overflow();
    UnitTest_ANA_B();
    std::cout << "=== Logical AND Tests Complete ===\n\n";

    std::cout << "\n=== Unit Tests Complete: " << testCounter - 1 << " Total ===\n";
    return 0;
}
