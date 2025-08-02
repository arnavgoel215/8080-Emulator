// ============================================================================
// CPU Logic & Control Unit Tests - CMP, CPI, CMA, CMC, CALL Instructions
// ----------------------------------------------------------------------------
// Target Module : CPU / Emulator (Intel 8080 Emulator)
// Purpose       : Validates comparison and control flow instructions:
//                 - CMP: Compare A with register/memory
//                 - CPI: Compare A with immediate value
//                 - CMA: Complement accumulator
//                 - CMC: Complement carry flag
//                 - CALL: Unconditional and conditional call instructions
// Scope         : Ensures correct flag behavior (Z, CY, S, P) and stack/PC updates
//                 for subroutine calls and logical comparisons.
//
// Author        : Fredo
// Date          : 7/30/25
// ============================================================================

//=========================== Define ==========================================
#define ENABLE_COLOR_OUTPUT
#define ENABLE_CPU_TESTING
#define ENABLE_VERBOSE_DEBUG

// ========================== Include ========================================
#include "../../src/model/emulator.hpp"
#include "../support/test_utils.hpp"
#include <iostream>
#include <cassert>

// ====================== Unit Test: CMP A ===============================
// Tests CMP A: Compares A with A → should set Z, clear CY
void UnitTest_CMP_A() {
    uint8_t initialA = 0x55;

    CPUState state = runSingleInstruction({0xBF}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP A", initialA, initialA, state.a, state.flags,
                    initialA, true, false, false, false, false);
    #endif

    bool pass = (state.flags.z == true && state.flags.cy == false);
    printTestResult("CMP A", "A == A → Z = 1, CY = 0", pass);
}

// ====================== Unit Test: CMA ===============================
// Tests CMA: Complements the accumulator bits (A = ~A)
void UnitTest_CMA() {
    uint8_t initialA = 0x0F;

    CPUState state = runSingleInstruction({0x2F}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMA", initialA, 0x00, state.a, state.flags,
                    0xF0, false, true, false, false, false);
    #endif

    printTestResult("CMA", "CMA 0x0F → A = 0xF0", state.a == 0xF0);
}

// ====================== Unit Test: CMC ===============================
// Tests CMC: Flips the carry flag (CY)
void UnitTest_CMC() {
    CPUState state = runSingleInstruction({0x3F}, [&](CPUState& cpu) {
        cpu.flags.cy = true;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    std::cout << "[DEBUG: CMC]\n"
              << "  CY Before = 1\n"
              << "  CY After  = " << state.flags.cy << "\n";
    #endif

    printTestResult("CMC", "Carry flag should be flipped: 1 → 0", state.flags.cy == false);
}

// ====================== Unit Test: CMP B ===============================
// Tests CMP B: A = 0x10, B = 0x20 → expect Z = 0, CY = 1
void UnitTest_CMP_B() {
    uint8_t initialA = 0x10;
    uint8_t operand = 0x20;

    CPUState state = runSingleInstruction({0xB8}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.b = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP B", initialA, operand, state.a, state.flags,
                    initialA, false, false, true, false, false);
    #endif

    bool pass = (state.flags.z == false && state.flags.cy == true);
    printTestResult("CMP B", "A < B → Z = 0, CY = 1", pass);
}

// ====================== Unit Test: CMP E ===============================
// Tests CMP E: A = 0x40, E = 0x30 → expect Z = 0, CY = 0
void UnitTest_CMP_E() {
    uint8_t initialA = 0x40;
    uint8_t operand = 0x30;

    CPUState state = runSingleInstruction({0xBB}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.e = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP E", initialA, operand, state.a, state.flags,
                    initialA, false, false, false, false, false);
    #endif

    bool pass = (state.flags.z == false && state.flags.cy == false);
    printTestResult("CMP E", "A > E → Z = 0, CY = 0", pass);
}

// ====================== Unit Test: CMP H ===============================
// Tests CMP H: A = H → should set Z = 1, clear CY
void UnitTest_CMP_H() {
    uint8_t initialA = 0x80;
    uint8_t operand = 0x80;

    CPUState state = runSingleInstruction({0xBC}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.h = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP H", initialA, operand, state.a, state.flags,
                    initialA, true, false, false, false, false);
    #endif

    bool pass = (state.flags.z == true && state.flags.cy == false);
    printTestResult("CMP H", "A == H → Z = 1, CY = 0", pass);
}

// ====================== Unit Test: CMP L ===============================
// Tests CMP L: A = 0x05, L = 0x10 → expect Z = 0, CY = 1
void UnitTest_CMP_L() {
    uint8_t initialA = 0x05;
    uint8_t operand = 0x10;

    CPUState state = runSingleInstruction({0xBD}, [&](CPUState& cpu) {
        cpu.a = initialA;
        cpu.l = operand;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP L", initialA, operand, state.a, state.flags,
                    initialA, false, false, true, false, false);
    #endif

    bool pass = (state.flags.z == false && state.flags.cy == true);
    printTestResult("CMP L", "A < L → Z = 0, CY = 1", pass);
}

// ====================== Unit Test: CMP M ===============================
// Tests CMP M: A = MEM[HL] → expect Z = 1, CY = 0
void UnitTest_CMP_M() {
    CPUState state = runInstructionWithMemory({0xBE}, 0x3000, 0x22, [](CPUState& cpu, Memory& mem) {
        cpu.a = 0x22;
        cpu.h = 0x30;
        cpu.l = 0x00;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CMP M", 0x22, 0x22, state.a, state.flags,
                    0x22, true, false, false, false, false);
    #endif

    bool pass = (state.flags.z == true && state.flags.cy == false);
    printTestResult("CMP M", "A == MEM[HL] → Z = 1, CY = 0", pass);
}

// ====================== Unit Test: CPI ===============================
// Tests CPI: A = 0x50, imm = 0x30 → expect CY = 0, Z = 0
void UnitTest_CPI() {
    uint8_t initialA = 0x50;
    uint8_t immediate = 0x30;

    CPUState state = runSingleInstruction({0xFE, immediate}, [&](CPUState& cpu) {
        cpu.a = initialA;
    });

    #ifdef ENABLE_VERBOSE_DEBUG
    printLogicDebug("CPI", initialA, immediate, state.a, state.flags,
                    initialA, false, false, false, false, false);
    #endif

    bool pass = (state.flags.cy == false && state.flags.z == false);
    printTestResult("CPI", "A > imm → CY = 0, Z = 0", pass);
}

// ====================== Main Entry Point ===============================
int main() {
    resetTestCounter();

    std::cout << "=== Starting CMP / CMA / CMC / CPI Tests ===\n";
    UnitTest_CMA();
    UnitTest_CMC();
    UnitTest_CMP_A();
    UnitTest_CMP_B();
    UnitTest_CMP_E();
    UnitTest_CMP_H();
    UnitTest_CMP_L();
    UnitTest_CMP_M();
    UnitTest_CPI();
    std::cout << "=== CMP / CMA / CMC / CPI Tests Complete ===\n\n";

    int totalTests = testsPassed + testsFailed;
    std::cout << "\n==============================\n";
    std::cout << " Opcode Unit Test Summary\n";
    std::cout << "------------------------------\n";
    std::cout << " Total Tests : " << totalTests << "\n";
    std::cout << GREEN << " Passed      : " << testsPassed << RESET << "\n";
    std::cout << RED   << " Failed      : " << testsFailed << RESET << "\n";
    std::cout << "==============================\n";

    return 0;
}
