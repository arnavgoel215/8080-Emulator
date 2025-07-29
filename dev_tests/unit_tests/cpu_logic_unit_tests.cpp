// ============================================================================
// CPU Logic Unit Tests
// ----------------------------------------------------------------------------
// Target Module : CPU / Emulator (Intel 8080 Emulator)
// Purpose       : Verifies logic instructions including ANA, ORA, XRA, CMA,
//                 CMP, CPI, and immediate variants. Ensures correct results
//                 and flag behavior (Z, S, P, CY, AC).
// Scope         : Unit testing logical operations with both register and
//                 immediate operands.
//
// Author        : Fredo
// Date          : 7/24/25
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

// =================== ANA (Logical AND with Register) =========================

// ====================== Unit Test: ANA B ===============================
// ANA B: Perform logical AND between A and B and verify result and flags
void UnitTest_ANA_B() {
    uint8_t initialA = 0xF0;
    uint8_t operand = 0x0F;

    CPUState state = runSingleInstruction({0xA0}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.b = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA B", initialA, operand, state.a, state.flags,
                    0x00, true, false, false, false, true);
#endif

    printTestResult("ANA B", "A = 0xF0 & 0x0F → A = 0x00", state.a == 0x00);
}

// ====================== Unit Test: ANA C ===============================
// ANA C: Perform logical AND between A and C and verify result and flags
void UnitTest_ANA_C() {
    uint8_t initialA = 0xFF;
    uint8_t operand = 0x0F;

    CPUState state = runSingleInstruction({0xA1}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.c = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA C", initialA, operand, state.a, state.flags,
                    0x0F, false, false, true, false, true);
#endif

    printTestResult("ANA C", "A = 0xFF & 0x0F → A = 0x0F", state.a == 0x0F);
}

// ====================== Unit Test: ANA D ===============================
// ANA D: Perform logical AND between A and D and verify result and flags
void UnitTest_ANA_D() {
    uint8_t initialA = 0xCC;
    uint8_t operand = 0xF0;

    CPUState state = runSingleInstruction({0xA2}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.d = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA D", initialA, operand, state.a, state.flags,
                    0xC0, false, true, false, false, true);
#endif

    printTestResult("ANA D", "A = 0xCC & 0xF0 → A = 0xC0", state.a == 0xC0);
}

// ====================== Unit Test: ANA E ===============================
// ANA E: Perform logical AND between A and E and verify result and flags
void UnitTest_ANA_E() {
    uint8_t initialA = 0xAA;
    uint8_t operand = 0x55;

    CPUState state = runSingleInstruction({0xA3}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.e = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA E", initialA, operand, state.a, state.flags,
                    0x00, true, false, false, false, true);
#endif

    printTestResult("ANA E", "A = 0xAA & 0x55 → A = 0x00", state.a == 0x00);
}

// ====================== Unit Test: ANA H ===============================
// ANA H: Perform logical AND between A and H and verify result and flags
void UnitTest_ANA_H() {
    uint8_t initialA = 0xF0;
    uint8_t operand = 0xF0;

    CPUState state = runSingleInstruction({0xA4}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.h = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA H", initialA, operand, state.a, state.flags,
                    0xF0, false, true, false, false, true);
#endif

    printTestResult("ANA H", "A = 0xF0 & 0xF0 → A = 0xF0", state.a == 0xF0);
}

// ====================== Unit Test: ANA L ===============================
// ANA L: Perform logical AND between A and L and verify result and flags
void UnitTest_ANA_L() {
    uint8_t initialA = 0xF0;
    uint8_t operand = 0x0F;

    CPUState state = runSingleInstruction({0xA5}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.l = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA L", initialA, operand, state.a, state.flags,
                    0x00, true, false, false, false, true);
#endif

    printTestResult("ANA L", "A = 0xF0 & 0x0F → A = 0x00", state.a == 0x00);
}


// =================== ORA (Logical OR with Register) =========================

// ====================== Unit Test: ORA B ===============================
// ORA B: Perform logical OR between A and B and verify result and flags
void UnitTest_ORA_B() {
    uint8_t initialA = 0xF0;
    uint8_t operand = 0x0F;

    CPUState state = runSingleInstruction({0xB0}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.b = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ORA B", initialA, operand, state.a, state.flags,
                    0xFF, false, true, false, false, false);
#endif

    printTestResult("ORA B", "A = 0xF0 | 0x0F → A = 0xFF", state.a == 0xFF);
}

// ====================== Unit Test: ORA C ===============================
// ORA C: Perform logical OR between A and C and verify result and flags
void UnitTest_ORA_C() {
    uint8_t initialA = 0x00;
    uint8_t operand = 0x00;

    CPUState state = runSingleInstruction({0xB1}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.c = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ORA C", initialA, operand, state.a, state.flags,
                    0x00, true, false, false, false, false);
#endif

    printTestResult("ORA C", "A = 0x00 | 0x00 → A = 0x00", state.a == 0x00);
}

// ====================== Unit Test: ORA D ===============================
// ORA D: Perform logical OR between A and D and verify result and flags
void UnitTest_ORA_D() {
    uint8_t initialA = 0x08;
    uint8_t operand = 0x02;

    CPUState state = runSingleInstruction({0xB2}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.d = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ORA D", initialA, operand, state.a, state.flags,
                    0x0A, false, false, false, false, false);
#endif

    printTestResult("ORA D", "A = 0x08 | 0x02 → A = 0x0A", state.a == 0x0A);
}

// ====================== Unit Test: ORA E ===============================
// ORA E: Perform logical OR between A and E and verify result and flags
void UnitTest_ORA_E() {
    uint8_t initialA = 0x00;
    uint8_t operand = 0xAA;

    CPUState state = runSingleInstruction({0xB3}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.e = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ORA E", initialA, operand, state.a, state.flags,
                    0xAA, false, true, false, false, false);
#endif

    printTestResult("ORA E", "A = 0x00 | 0xAA → A = 0xAA", state.a == 0xAA);
}

// ====================== Unit Test: ORA H ===============================
// ORA H: Perform logical OR between A and H and verify result and flags
void UnitTest_ORA_H() {
    uint8_t initialA = 0x11;
    uint8_t operand = 0x22;

    CPUState state = runSingleInstruction({0xB4}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.h = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ORA H", initialA, operand, state.a, state.flags,
                    0x33, false, false, false, false, false);
#endif

    printTestResult("ORA H", "A = 0x11 | 0x22 → A = 0x33", state.a == 0x33);
}

// ====================== Unit Test: ORA L ===============================
// ORA L: Perform logical OR between A and L and verify result and flags
void UnitTest_ORA_L() {
    uint8_t initialA = 0x0A;
    uint8_t operand = 0x01;

    CPUState state = runSingleInstruction({0xB5}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.l = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ORA L", initialA, operand, state.a, state.flags,
                    0x0B, false, false, false, false, false);
#endif

    printTestResult("ORA L", "A = 0x0A | 0x01 → A = 0x0B", state.a == 0x0B);
}

// =================== XRA (Logical XOR with Register) =========================

// ====================== Unit Test: XRA B ===============================
// XRA B: Perform logical XOR between A and B and verify result and flags
void UnitTest_XRA_B() {
    uint8_t initialA = 0xF0;
    uint8_t operand = 0x0F;

    CPUState state = runSingleInstruction({0xA8}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.b = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("XRA B", initialA, operand, state.a, state.flags,
                    0xFF, false, true, false, false, false);
#endif

    printTestResult("XRA B", "A = 0xF0 ^ 0x0F → A = 0xFF", state.a == 0xFF);
}

// ====================== Unit Test: XRA C ===============================
// XRA C: Perform logical XOR between A and C and verify result and flags
void UnitTest_XRA_C() {
    uint8_t initialA = 0xAA;
    uint8_t operand = 0xAA;

    CPUState state = runSingleInstruction({0xA9}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.c = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("XRA C", initialA, operand, state.a, state.flags,
                    0x00, true, false, false, false, false);
#endif

    printTestResult("XRA C", "A = 0xAA ^ 0xAA → A = 0x00", state.a == 0x00);
}

// ====================== Unit Test: XRA D ===============================
// XRA D: Perform logical XOR between A and D and verify result and flags
void UnitTest_XRA_D() {
    uint8_t initialA = 0x0F;
    uint8_t operand = 0xF0;

    CPUState state = runSingleInstruction({0xAA}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.d = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("XRA D", initialA, operand, state.a, state.flags,
                    0xFF, false, true, false, false, false);
#endif

    printTestResult("XRA D", "A = 0x0F ^ 0xF0 → A = 0xFF", state.a == 0xFF);
}

// ====================== Unit Test: XRA E ===============================
// XRA E: Perform logical XOR between A and E and verify result and flags
void UnitTest_XRA_E() {
    uint8_t initialA = 0x55;
    uint8_t operand = 0xAA;

    CPUState state = runSingleInstruction({0xAB}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.e = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("XRA E", initialA, operand, state.a, state.flags,
                    0xFF, false, true, false, false, false);
#endif

    printTestResult("XRA E", "A = 0x55 ^ 0xAA → A = 0xFF", state.a == 0xFF);
}

// ====================== Unit Test: XRA H ===============================
// XRA H: Perform logical XOR between A and H and verify result and flags
void UnitTest_XRA_H() {
    uint8_t initialA = 0x0F;
    uint8_t operand = 0xF0;

    CPUState state = runSingleInstruction({0xAC}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.h = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("XRA H", initialA, operand, state.a, state.flags,
                    0xFF, false, true, false, false, false);
#endif

    printTestResult("XRA H", "A = 0x0F ^ 0xF0 → A = 0xFF", state.a == 0xFF);
}

// ====================== Unit Test: XRA L ===============================
// XRA L: Perform logical XOR between A and L and verify result and flags
void UnitTest_XRA_L() {
    uint8_t initialA = 0x0F;
    uint8_t operand = 0x0F;

    CPUState state = runSingleInstruction({0xAD}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.l = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("XRA L", initialA, operand, state.a, state.flags,
                    0x00, true, false, false, false, false);
#endif

    printTestResult("XRA L", "A = 0x0F ^ 0x0F → A = 0x00", state.a == 0x00);
}

// =================== CMP (Compare Register) =========================

// ====================== Unit Test: CMP B ===============================
// CMP B: Compare A with B and verify flags (no change to A)
void UnitTest_CMP_B() {
    uint8_t initialA = 0x42;
    uint8_t operand = 0x42;

    CPUState state = runSingleInstruction({0xB8}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.b = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP B", initialA, operand, state.a, state.flags,
                    0x42, true, false, false, false, false);
#endif

    printTestResult("CMP B", "A = B → Z = 1", state.flags.z == true);
}

// ====================== Unit Test: CMP C ===============================
// CMP C: A < C should set CY = 1 and Z = 0
void UnitTest_CMP_C() {
    uint8_t initialA = 0x30;
    uint8_t operand = 0x40;

    CPUState state = runSingleInstruction({0xB9}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.c = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP C", initialA, operand, state.a, state.flags,
                    0x30, false, false, true, false, false);
#endif

    bool pass = (state.flags.cy == true && state.flags.z == false);
    printTestResult("CMP C", "A < C → CY = 1, Z = 0", pass);
}

// ====================== Unit Test: CMP D ===============================
// CMP D: A > D should clear CY and Z flags
void UnitTest_CMP_D() {
    uint8_t initialA = 0x50;
    uint8_t operand = 0x20;

    CPUState state = runSingleInstruction({0xBA}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.d = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP D", initialA, operand, state.a, state.flags,
                    0x50, false, false, false, false, false);
#endif

    bool pass = (state.flags.cy == false && state.flags.z == false);
    printTestResult("CMP D", "A > D → CY = 0, Z = 0", pass);
}

// ====================== Unit Test: CMP E ===============================
// CMP E: A < E, expect CY = 1
void UnitTest_CMP_E() {
    uint8_t initialA = 0x10;
    uint8_t operand = 0x80;

    CPUState state = runSingleInstruction({0xBB}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.e = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP E", initialA, operand, state.a, state.flags,
                    0x10, false, false, true, false, false);
#endif

    bool pass = (state.flags.cy == true);
    printTestResult("CMP E", "A < E → CY = 1", pass);
}

// ====================== Unit Test: CMP H ===============================
// CMP H: A == H, expect Z = 1
void UnitTest_CMP_H() {
    uint8_t initialA = 0xAA;
    uint8_t operand = 0xAA;

    CPUState state = runSingleInstruction({0xBC}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.h = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP H", initialA, operand, state.a, state.flags,
                    0xAA, true, false, false, false, false);
#endif

    printTestResult("CMP H", "A = H → Z = 1", state.flags.z == true);
}

// ====================== Unit Test: CMP L ===============================
// CMP L: A > L, expect CY = 0, Z = 0
void UnitTest_CMP_L() {
    uint8_t initialA = 0xF0;
    uint8_t operand = 0x0F;

    CPUState state = runSingleInstruction({0xBD}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.l = operand;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP L", initialA, operand, state.a, state.flags,
                    0xF0, false, false, false, false, false);
#endif

    bool pass = (state.flags.cy == false && state.flags.z == false);
    printTestResult("CMP L", "A > L → CY = 0, Z = 0", pass);
}

// ====================== Unit Test: CMA ===============================
// CMA: Complement A (flip all bits in accumulator)
void UnitTest_CMA() {
    CPUState state = runSingleInstruction({0x2F}, [](CPUState& cpu) {
        cpu.a = 0x0F;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMA", 0x0F, 0x00, state.a, state.flags,
                    0xF0, false, false, false, false, false);
#endif

    printTestResult("CMA", "CMA 0x0F → A = 0xF0", state.a == 0xF0);
}

// ====================== Unit Test: ANI Immediate ===============================
// ANI: Logical AND with immediate 0x0F, expect A = A & 0x0F
void UnitTest_ANI_Immediate() {
    uint8_t initialA = 0xF0;
    uint8_t immediate = 0x0F;

    CPUState state = runSingleInstruction({0xE6, immediate}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANI", initialA, immediate, state.a, state.flags,
                    0x00, true, false, false, false, true);
#endif

    printTestResult("ANI", "A = 0xF0 & 0x0F → A = 0x00", state.a == 0x00);
}
/*
// ====================== Unit Test: ORI Immediate ===============================
// ORI: Logical OR with immediate 0x0F, expect A = A | 0x0F
void UnitTest_ORI_Immediate() {
    uint8_t initialA = 0xF0;
    uint8_t immediate = 0x0F;

    CPUState state = runSingleInstruction({0xF6, immediate}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ORI", initialA, immediate, state.a, state.flags,
                    0xFF, false, true, false, false, false);
#endif

    printTestResult("ORI", "A = 0xF0 | 0x0F → A = 0xFF", state.a == 0xFF);
}
*/

// ====================== Unit Test: ORI Immediate ===============================
// ORI: Logical OR between A and immediate value 0x0F → A = A | 0x0F
void UnitTest_ORI_Immediate() {
    uint8_t initialA = 0xF0;
    uint8_t immediate = 0x0F;

    // Opcode 0xF6 is ORI d8
    CPUState state = runSingleInstruction({0xF6, immediate}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ORI",
        initialA,               // LHS (A)
        immediate,              // RHS (immediate)
        state.a,                // Actual A result
        state.flags,            // Actual flags
        0xFF,                   // Expected A = 0xF0 | 0x0F = 0xFF
        false,                  // Z: not zero
        true,                   // S: sign bit is 1
        true,                   // P: even parity (8 bits set)
        false,                  // CY: should be cleared
        false                   // AC: should be cleared
    );
#endif

    bool pass = (state.a == 0xFF &&
                 state.flags.z == false &&
                 state.flags.s == true &&
                 state.flags.p == true &&
                 state.flags.cy == false &&
                 state.flags.ac == false);

    printTestResult("ORI", "A = 0xF0 | 0x0F → A = 0xFF", pass);
}


// ====================== Unit Test: XRI Immediate ===============================
// XRI: Logical XOR with immediate 0xFF, expect A = A ^ 0xFF
void UnitTest_XRI_Immediate() {
    uint8_t initialA = 0x55;
    uint8_t immediate = 0xFF;

    CPUState state = runSingleInstruction({0xEE, immediate}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("XRI", initialA, immediate, state.a, state.flags,
                    0xAA, true, true, true, false, false);
#endif

    printTestResult("XRI", "A = 0x55 ^ 0xFF → A = 0xAA", state.a == 0xAA);
}

// ====================== Unit Test: CPI Immediate ===============================
// CPI: Compare A with immediate 0x10
void UnitTest_CPI_Immediate()
{
    uint8_t initialA = 0x20;
    uint8_t immediate = 0x10;

    CPUState state = runSingleInstruction({0xFE, immediate}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CPI Greater", initialA, immediate, state.a, state.flags,
                    0x20, false, false, false, false, false);
#endif

    bool pass = (state.flags.cy == false && state.flags.z == false);
    printTestResult("CPI", "A = 0x20, Imm = 0x10 → A > Imm", pass);
}


// ====================== Memory-Based Logical Ops =========================

// ====================== Unit Test: ANA M ===============================
// ANA M: A = A & MEM[HL]
void UnitTest_ANA_M() {
    CPUState state = runInstructionWithMemory({0xA6}, 0x3000, 0x0F, [](CPUState& cpu, Memory& mem) {
        cpu.a = 0xF0;
        cpu.h = 0x30;
        cpu.l = 0x00;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA M", 0xF0, 0x0F, state.a, state.flags,
                    0x00, true, false, false, false, true);
#endif

    printTestResult("ANA M", "A = 0xF0 & MEM[HL] → A = 0x00", state.a == 0x00);
}

// ====================== Unit Test: ORA M ===============================
// ORA M: A = A | MEM[HL]
void UnitTest_ORA_M() {
    CPUState state = runInstructionWithMemory({0xB6}, 0x4000, 0x0F, [](CPUState& cpu, Memory& mem) {
        cpu.a = 0xF0;
        cpu.h = 0x40;
        cpu.l = 0x00;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ORA M", 0xF0, 0x0F, state.a, state.flags,
                    0xFF, false, true, false, false, false);
#endif

    printTestResult("ORA M", "A = 0xF0 | MEM[HL] → A = 0xFF", state.a == 0xFF);
}

// ====================== Unit Test: XRA M ===============================
// XRA M: A = A ^ MEM[HL]
void UnitTest_XRA_M() {
    CPUState state = runInstructionWithMemory({0xAE}, 0x2200, 0xFF, [](CPUState& cpu, Memory& mem) {
        cpu.a = 0x55;
        cpu.h = 0x22;
        cpu.l = 0x00;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("XRA M", 0x55, 0xFF, state.a, state.flags,
                    0xAA, false, true, false, false, false);
#endif

    printTestResult("XRA M", "A = 0x55 ^ MEM[HL] → A = 0xAA", state.a == 0xAA);
}

// ====================== Unit Test: CMP M ===============================
// CMP M: Compare A with MEM[HL]
void UnitTest_CMP_M() {
    CPUState state = runInstructionWithMemory({0xBE}, 0x2000, 0x10, [](CPUState& cpu, Memory& mem) {
        cpu.a = 0x10;
        cpu.h = 0x20;
        cpu.l = 0x00;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP M", 0x10, 0x10, state.a, state.flags,
                    0x10, true, false, false, false, false);
#endif

    bool pass = (state.flags.z == true && state.flags.cy == false);
    printTestResult("CMP M", "A = MEM[HL] → Z = 1", pass);
}

// ====================== CPI (Compare Immediate Variations) ==============

// ====================== Unit Test: CPI Equal ===============================
// CPI: Compare A with immediate value, expect Z = 1 if equal
void UnitTest_CPI_Equal() {
    uint8_t initialA = 0x10;
    uint8_t immediate = 0x10;

    Emulator emu;
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    cpu.a = initialA;
    mem.writeRomBytes(0x0000, 0xFE);      // CPI opcode
    mem.writeRomBytes(0x0001, immediate); // operand = 0x10
    emu.emulateCycles(1);

    CPUState& state = cpu;

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CPI Equal", initialA, immediate, state.a, state.flags,
                    initialA, true, false, false, false, false);
#endif

    bool pass = (state.flags.z == true && state.flags.cy == false);
    printTestResult("CPI Equal", "A = Imm → Z = 1, CY = 0", pass);
}


// ====================== Unit Test: CPI Less ===============================
// CPI: Compare A < immediate value, expect CY = 1, Z = 0
void UnitTest_CPI_Less() {
    uint8_t initialA = 0x10;
    uint8_t immediate = 0x20;

    CPUState state = runSingleInstruction({0xFE, immediate}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CPI Less", initialA, immediate, state.a, state.flags,
                    initialA, false, false, false, true, false);
#endif

    bool pass = (state.flags.z == false && state.flags.cy == true);
    printTestResult("CPI Less", "A < Imm → CY = 1, Z = 0", pass);
}


// ====================== Unit Test: CPI Greater ===============================
// CPI: Compare A with immediate 0x10 (A > immediate)
// Expected: CY = 0, Z = 0
void UnitTest_CPI_Greater() {
    uint8_t initialA = 0x30;
    uint8_t immediate = 0x10;

    Emulator emu;
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    cpu.a = initialA;
    mem.writeRomBytes(0x0000, 0xFE);  // CPI
    mem.writeRomBytes(0x0001, immediate);  // operand = 0x10

    emu.emulateCycles(1);
    CPUState state = cpu;

#ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CPI Greater", initialA, immediate, state.a, state.flags,
                    initialA, false, false, false, false, false);
#endif

    bool pass = (state.flags.z == false && state.flags.cy == false);
    printTestResult("CPI Greater", "A > Imm → CY = 0, Z = 0", pass);
}


// =================== Entry Point ===========================================
int main() {
    std::cout << "=== Running CPU Logical Unit Tests ===\n";

    // === ANA (Logical AND with Register) ===
    std::cout << "=== Starting ANA (Logical AND with Register) Tests ===\n";
    UnitTest_ANA_B();
    UnitTest_ANA_C();
    UnitTest_ANA_D();
    UnitTest_ANA_E();
    UnitTest_ANA_H();
    UnitTest_ANA_L();
    std::cout << "=== ANA (Logical AND with Register) Tests Complete ===\n\n";

    // === ORA (Logical OR with Register) ===
    std::cout << "=== Starting ORA (Logical OR with Register) Tests ===\n";
    UnitTest_ORA_B();
    UnitTest_ORA_C();
    UnitTest_ORA_D();
    UnitTest_ORA_E();
    UnitTest_ORA_H();
    UnitTest_ORA_L();
    std::cout << "=== ORA (Logical OR with Register) Tests Complete ===\n\n";

    // === XRA (Logical XOR with Register) ===
    std::cout << "=== XRA (Logical XOR with Register) Tests ===\n";
    UnitTest_XRA_B();
    UnitTest_XRA_C();
    UnitTest_XRA_D();
    UnitTest_XRA_E();
    UnitTest_XRA_H();
    UnitTest_XRA_L();
    std::cout << "=== XRA (Logical XOR with Register) Tests Complete ===\n\n";

    // === CMP (Compare Register) ===
    std::cout << "=== Starting CMP (Compare Register) Tests ===\n\n";
    UnitTest_CMP_B();
    UnitTest_CMP_C();
    UnitTest_CMP_D();
    UnitTest_CMP_E();
    UnitTest_CMP_H();
    UnitTest_CMP_L();
    std::cout << "=== CMP (Compare Register) Tests Complete ===\n\n";

    // === CMA / Immediate Logic Ops ===
   std::cout << "=== Starting CMA / Immediate Logic Ops Tests ===\n\n";
    UnitTest_CMA();
    UnitTest_ANI_Immediate();
    UnitTest_ORI_Immediate();
    UnitTest_XRI_Immediate();
    UnitTest_CPI_Immediate();
    std::cout << "=== CMA / Immediate Logic Ops Tests Complete ===\n\n";

    // === Memory-Based Logical Ops === 
    UnitTest_ANA_M();
    UnitTest_ORA_M();
    UnitTest_XRA_M();
    UnitTest_CMP_M();

    // === CPI (Compare Immediate Variations) ===
     std::cout << "=== Starting CPI (Compare Immediate Variations) Tests ===\n\n";
    UnitTest_CPI_Equal();
    UnitTest_CPI_Less();
    UnitTest_CPI_Greater();
    std::cout << "=== CPI (Compare Immediate Variations) Complete ===\n\n";

    std::cout << "\n=== Unit Tests Complete: " << testCounter - 1 << " Total ===\n";
    return 0;
}
