// ============================================================================
// CPU Arithmetic & Logical Unit Tests - ADC, ADD, ANA, ANI Instructions
// ----------------------------------------------------------------------------
// Target Module : CPU / Emulator (Intel 8080 Emulator)
// Purpose       : Validates arithmetic and logical instruction behavior.
//                 - ADC: Add register/memory/immediate to A with carry.
//                 - ADD: Add register/memory to A (no carry).
//                 - ANA: Logical AND of A with register/memory.
//                 - ANI: Logical AND of A with immediate.
// Scope         : Confirms correctness of result and flags (Z, S, P, CY, AC)
//                 across all operand types (register, memory, immediate).
//
// Author        : Fredo
// Date          : 7/30/25
// ============================================================================


// ===================== Defines ==============================================
#define ENABLE_COLOR_OUTPUT
#define ENABLE_CPU_TESTING
#define ENABLE_VERBOSE_DEBUG

// ==================== Includes ================================================
#include "../../src/model/emulator.hpp"
#include "../support/test_utils.hpp"
#include <iostream>
#include <cassert>

// ====================== Unit Test: ADC B ===============================
// Tests the ADC B instruction: Adds B and carry flag to A and checks result
// ADC B: A = A + B + CY
void UnitTest_ADC_B() {
    uint8_t initialA = 0x20;
    uint8_t operand  = 0x22;

    CPUState state = runSingleInstruction({0x88}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.b = operand;
        cpu.flags.cy = 1;
    });

    uint8_t expectedA = 0x43;
    bool expectedCY = false;

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADC B", initialA, operand, true,
                         state.a, state.flags,
                         expectedA, expectedCY);
    #endif

    printTestResult("ADC B", "A = 0x20 + B = 0x22 + CY = 1 → A = 0x43", state.a == expectedA);
}

// ====================== Unit Test: ADC C ===============================
// Tests the ADC C instruction: Adds C and carry flag to A and verifies flags
void UnitTest_ADC_C() {
    uint8_t initialA = 0xFF;
    uint8_t operand  = 0x01;

    CPUState state = runSingleInstruction({0x89}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.c = operand;
        cpu.flags.cy = 1;
    });

    uint8_t expectedA = 0x01;
    bool expectedCY = true;

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADC C", initialA, operand, true,
                         state.a, state.flags,
                         expectedA, expectedCY);
    #endif

    printTestResult("ADC C", "A = 0xFF + 0x01 + CY = 1 → A = 0x01", state.a == expectedA);
}

// ====================== Unit Test: ADC D ===============================
// Tests the ADC D instruction: Adds D and carry to A with zero carry case
void UnitTest_ADC_D() {
    uint8_t initialA = 0x10;
    uint8_t operand  = 0x10;

    CPUState state = runSingleInstruction({0x8A}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.d = operand;
        cpu.flags.cy = 0;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADC D", initialA, operand, false,
                         state.a, state.flags,
                         0x20, false);
    #endif

    printTestResult("ADC D", "A = 0x10 + D = 0x10 + CY = 0 → A = 0x20", state.a == 0x20);
}

// ====================== Unit Test: ADC A ===============================
// Tests the ADC A instruction: Adds A + A + carry, checks for overflow
void UnitTest_ADC_A() {
    uint8_t initialA = 0x80;

    CPUState state = runSingleInstruction({0x8F}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.flags.cy = 1;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADC A", initialA, initialA, true,
                         state.a, state.flags,
                         0x01, true);
    #endif

    printTestResult("ADC A", "A = 0x80 + A = 0x80 + CY = 1 → A = 0x01", state.a == 0x01);
}

// ====================== Unit Test: ADC E ===============================
// Tests the ADC E instruction: Verifies addition of E and carry to A
void UnitTest_ADC_E() {
    uint8_t initialA = 0x11;
    uint8_t operand  = 0x22;

    CPUState state = runSingleInstruction({0x8B}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.e = operand;
        cpu.flags.cy = 1;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADC E", initialA, operand, true,
                         state.a, state.flags,
                         0x34, false);
    #endif

    printTestResult("ADC E", "A = 0x11 + E = 0x22 + CY = 1 → A = 0x34", state.a == 0x34);
}

// ====================== Unit Test: ADC H ===============================
// Tests the ADC H instruction: Adds H and carry to A, carry is zero
void UnitTest_ADC_H() {
    uint8_t initialA = 0x10;
    uint8_t operand  = 0x10;

    CPUState state = runSingleInstruction({0x8C}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.h = operand;
        cpu.flags.cy = 0;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADC H", initialA, operand, false,
                         state.a, state.flags,
                         0x20, false);
    #endif

    printTestResult("ADC H", "A = 0x10 + H = 0x10 + CY = 0 → A = 0x20", state.a == 0x20);
}

// ====================== Unit Test: ADC L ===============================
// Tests the ADC L instruction: Adds L and carry to A, result should wrap
void UnitTest_ADC_L() {
    uint8_t initialA = 0x01;
    uint8_t operand  = 0xFE;

    CPUState state = runSingleInstruction({0x8D}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.l = operand;
        cpu.flags.cy = 1;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADC L", initialA, operand, true,
                         state.a, state.flags,
                         0x00, true);
    #endif

    printTestResult("ADC L", "A = 0x01 + L = 0xFE + CY = 1 → A = 0x00", state.a == 0x00);
}

// ====================== Unit Test: ADC M ===============================
// Tests ADC M: Adds memory[HL] and carry to A, verifies final result
void UnitTest_ADC_M() {
    CPUState state = runInstructionWithMemory({0x8E}, 0x2200, 0x10, [](CPUState& cpu, Memory& mem) {
        cpu.a = 0x10;
        cpu.h = 0x22;
        cpu.l = 0x00;
        cpu.flags.cy = 1;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADC M", 0x10, 0x10, true,
                         state.a, state.flags,
                         0x21, false);
    #endif

    printTestResult("ADC M", "A = 0x10 + MEM[HL] = 0x10 + CY = 1 → A = 0x21", state.a == 0x21);
}

// ====================== Unit Test: ADI Immediate =======================
// Tests ADI instruction: Adds immediate 8-bit value to A, carry is cleared
void UnitTest_ADI_Immediate() {
    uint8_t initialA = 0x22;
    uint8_t immediate = 0x11;

    CPUState state = runSingleInstruction({0xC6, immediate}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADI", initialA, immediate, false,
                         state.a, state.flags,
                         0x33, false);
    #endif

    printTestResult("ADI", "A = 0x22 + 0x11 → A = 0x33", state.a == 0x33);
}

// ====================== Unit Test: ADD B ===============================
// Tests the ADD B instruction: Adds B to A and checks result and flags
void UnitTest_ADD_B() {
    uint8_t initialA = 0x10;
    uint8_t operand = 0x10;

    CPUState state = runSingleInstruction({0x80}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.b = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD B", initialA, operand, false,
                         state.a, state.flags,
                         0x20, false);
    #endif

    printTestResult("ADD B", "A = 0x10 + B = 0x10 → A = 0x20", state.a == 0x20);
}

// ====================== Unit Test: ADD C ===============================
// Tests the ADD C instruction: A + C should wrap around with carry flag set
void UnitTest_ADD_C() {
    uint8_t initialA = 0xF0;
    uint8_t operand = 0x10;

    CPUState state = runSingleInstruction({0x81}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.c = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD C", initialA, operand, false,
                         state.a, state.flags,
                         0x00, true);
    #endif

    printTestResult("ADD C", "A = 0xF0 + C = 0x10 → A = 0x00", state.a == 0x00);
}

// ====================== Unit Test: ADD D ===============================
// Tests the ADD D instruction: Adds D to A, checks result and flag logic
void UnitTest_ADD_D() {
    uint8_t initialA = 0x22;
    uint8_t operand = 0x33;

    CPUState state = runSingleInstruction({0x82}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.d = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD D", initialA, operand, false,
                         state.a, state.flags,
                         0x55, false);
    #endif

    printTestResult("ADD D", "A = 0x22 + D = 0x33 → A = 0x55", state.a == 0x55);
}

// ====================== Unit Test: ADD E ===============================
// Tests the ADD E instruction: Simple addition of E to A with no overflow
void UnitTest_ADD_E() {
    uint8_t initialA = 0x01;
    uint8_t operand = 0x01;

    CPUState state = runSingleInstruction({0x83}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.e = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD E", initialA, operand, false,
                         state.a, state.flags,
                         0x02, false);
    #endif

    printTestResult("ADD E", "A = 0x01 + E = 0x01 → A = 0x02", state.a == 0x02);
}

// ====================== Unit Test: ADD H ===============================
// Tests the ADD H instruction: Adds H to A, result flips sign bit
void UnitTest_ADD_H() {
    uint8_t initialA = 0x7F;
    uint8_t operand = 0x01;

    CPUState state = runSingleInstruction({0x84}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.h = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD H", initialA, operand, false,
                         state.a, state.flags,
                         0x80, false);
    #endif

    printTestResult("ADD H", "A = 0x7F + H = 0x01 → A = 0x80", state.a == 0x80);
}

// ====================== Unit Test: ADD L ===============================
// Tests the ADD L instruction: Adds L to A, verifying standard behavior
void UnitTest_ADD_L() {
    uint8_t initialA = 0x0A;
    uint8_t operand = 0x0A;

    CPUState state = runSingleInstruction({0x85}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.l = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD L", initialA, operand, false,
                         state.a, state.flags,
                         0x14, false);
    #endif

    printTestResult("ADD L", "A = 0x0A + L = 0x0A → A = 0x14", state.a == 0x14);
}

// ====================== Unit Test: ADD M ===============================
// Tests the ADD M instruction: Adds memory[HL] to A and checks result and flags
void UnitTest_ADD_M() {
    CPUState state = runInstructionWithMemory({0x86}, 0x3000, 0x01, [](CPUState& cpu, Memory& mem) {
        cpu.a = 0x01;
        cpu.h = 0x30;
        cpu.l = 0x00;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printArithmeticDebug("ADD M", 0x01, 0x01, false,
                         state.a, state.flags,
                         0x02, false);
    #endif

    printTestResult("ADD M", "A = 0x01 + MEM[HL] = 0x01 → A = 0x02", state.a == 0x02);
}

// ====================== Unit Test: ANA A ===============================
// Tests ANA A: A & A should result in A itself, CY cleared
void UnitTest_ANA_A() {
    uint8_t initialA = 0xF0;

    CPUState state = runSingleInstruction({0xA7}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA A", initialA, initialA, state.a, state.flags,
                    0xF0, false, true, false, false, true);
    #endif

    printTestResult("ANA A", "A = 0xF0 & A = 0xF0 → A = 0xF0", state.a == 0xF0);
}

// ====================== Unit Test: ANA B ===============================
// Tests ANA B: Logical AND between A and B, result should be zero
void UnitTest_ANA_B() {
    uint8_t initialA = 0xF0;
    uint8_t operand  = 0x0F;

    CPUState state = runSingleInstruction({0xA0}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.b = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA B", initialA, operand, state.a, state.flags,
                    0x00, true, false, false, false, true);
    #endif

    printTestResult("ANA B", "A = 0xF0 & B = 0x0F → A = 0x00", state.a == 0x00);
}

// ====================== Unit Test: ANA C ===============================
// Tests ANA C: A & C where A and C have same value, all bits preserved
void UnitTest_ANA_C() {
    uint8_t initialA = 0x0F;
    uint8_t operand  = 0x0F;

    CPUState state = runSingleInstruction({0xA1}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.c = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA C", initialA, operand, state.a, state.flags,
                    0x0F, false, false, true, false, true);
    #endif

    printTestResult("ANA C", "A = 0x0F & C = 0x0F → A = 0x0F", state.a == 0x0F);
}

// ====================== Unit Test: ANA D ===============================
// Tests ANA D: Logical AND between A and D, verifies result and flags
void UnitTest_ANA_D() {
    uint8_t initialA = 0xAA;
    uint8_t operand  = 0x0F;

    CPUState state = runSingleInstruction({0xA2}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.d = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA D", initialA, operand, state.a, state.flags,
                    0x0A, false, false, true, false, true);
    #endif

    printTestResult("ANA D", "A = 0xAA & D = 0x0F → A = 0x0A", state.a == 0x0A);
}

// ====================== Unit Test: ANA E ===============================
// Tests ANA E: A & E results in zero, CY flag should be cleared
void UnitTest_ANA_E() {
    uint8_t initialA = 0xAA;
    uint8_t operand  = 0x55;

    CPUState state = runSingleInstruction({0xA3}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.e = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA E", initialA, operand, state.a, state.flags,
                    0x00, true, false, false, false, true);
    #endif

    printTestResult("ANA E", "A = 0xAA & E = 0x55 → A = 0x00", state.a == 0x00);
}

// ====================== Unit Test: ANA H ===============================
// Tests ANA H: Full bit overlap between A and H, expect same value
void UnitTest_ANA_H() {
    uint8_t initialA = 0xF0;
    uint8_t operand  = 0xF0;

    CPUState state = runSingleInstruction({0xA4}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.h = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA H", initialA, operand, state.a, state.flags,
                    0xF0, false, true, false, false, true);
    #endif

    printTestResult("ANA H", "A = 0xF0 & H = 0xF0 → A = 0xF0", state.a == 0xF0);
}

// ====================== Unit Test: ANA L ===============================
// Tests ANA L: Partial overlap between A and L bits, result should be masked
void UnitTest_ANA_L() {
    uint8_t initialA = 0x3C;
    uint8_t operand  = 0x0F;

    CPUState state = runSingleInstruction({0xA5}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.l = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA L", initialA, operand, state.a, state.flags,
                    0x0C, false, false, true, false, true);
    #endif

    printTestResult("ANA L", "A = 0x3C & L = 0x0F → A = 0x0C", state.a == 0x0C);
}

// ====================== Unit Test: ANA M ===============================
// Tests ANA M: A & Memory[HL], ensures correct bitwise AND and flags
void UnitTest_ANA_M() {
    CPUState state = runInstructionWithMemory({0xA6}, 0x4000, 0xF0, [](CPUState& cpu, Memory& mem) {
        cpu.a = 0x0F;
        cpu.h = 0x40;
        cpu.l = 0x00;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("ANA M", 0x0F, 0xF0, state.a, state.flags,
                    0x00, true, false, false, false, true);
    #endif

    printTestResult("ANA M", "A = 0x0F & MEM[HL] = 0xF0 → A = 0x00", state.a == 0x00);
}

// ====================== Unit Test: ANI Immediate =======================
// Tests ANI instruction: A & immediate value, verifies result and flag logic.
void UnitTest_ANI_Immediate() {
    uint8_t initialA  = 0xF0;
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


// ====================== Main Entry Point ===============================
// Executes all unit tests for ADC, ADD, ANA, and ANI instructions
// Displays grouped summaries and overall pass/fail count
int main() {
#ifdef ENABLE_CPU_TESTING
    resetTestCounter();

// === ADC (Add with Carry) Instructions ===
std::cout << "=== Starting ADC Tests ===\n";
UnitTest_ADC_A();
UnitTest_ADC_B();
UnitTest_ADC_C();
UnitTest_ADC_D();
UnitTest_ADC_E();
UnitTest_ADC_H();
UnitTest_ADC_L();
UnitTest_ADC_M();
UnitTest_ADI_Immediate();
std::cout << "=== ADC Tests Complete ===\n\n";

// === ADD (Add without Carry) Instructions ===
std::cout << "=== Starting ADD Tests ===\n";
UnitTest_ADD_B();
UnitTest_ADD_C();
UnitTest_ADD_D();
UnitTest_ADD_E();
UnitTest_ADD_H();
UnitTest_ADD_L();
UnitTest_ADD_M();
std::cout << "=== ADD Tests Complete ===\n\n";

// === ANA (Logical AND with Register) ===
std::cout << "=== Starting ANA Tests ===\n";
UnitTest_ANA_A();
UnitTest_ANA_B();
UnitTest_ANA_C();
UnitTest_ANA_D();
UnitTest_ANA_E();
UnitTest_ANA_H();
UnitTest_ANA_L();
UnitTest_ANA_M();
UnitTest_ANI_Immediate();
std::cout << "=== ANA Tests Complete ===\n\n";



int totalTests = testsPassed + testsFailed;
    std::cout << "\n==============================\n";
    std::cout << " Opcode Unit Test Summary\n";
    std::cout << "------------------------------\n";
    std::cout << " Total Tests : " << totalTests << "\n";
    std::cout << GREEN << " Passed      : " << testsPassed << RESET << "\n";
    std::cout << RED   << " Failed      : " << testsFailed << RESET << "\n";
    std::cout << "==============================\n";
#else
    std::cout << "CPU testing is disabled. Define ENABLE_CPU_TESTING to run tests.\n";
#endif
    return 0;
}