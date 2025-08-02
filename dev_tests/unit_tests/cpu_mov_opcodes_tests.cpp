// =============================================================================
// CPU MOV Instruction Unit Tests
// ----------------------------------------------------------------------------
// Target Module : CPU / Emulator (Intel 8080 Emulator)
// Purpose       : Verifies correct execution of all MOV instructions (MOV A–M)
//                 between registers and memory, ensuring correct data transfer.
// Scope         : Covers opcodes 0x40–0x7F (excluding HLT at 0x76)
//
// Author        : Fredo
// Date          : 7/30/25
// =============================================================================

// ===================== Defines ===============================================
#define ENABLE_COLOR_OUTPUT
#define ENABLE_CPU_TESTING
#define ENABLE_VERBOSE_DEBUG

// ===================== Includes ==============================================
#include "../../src/model/emulator.hpp"
#include "../support/test_utils.hpp"
#include <iostream>
#include <vector>
#include <string>

// ===================== MOV Opcode Table ======================================
struct MovTestEntry {
    uint8_t opcode;
    std::string dest;
    std::string src;
};

const std::vector<MovTestEntry> movTests = {
    {0x7F, "A", "A"}, {0x78, "A", "B"}, {0x79, "A", "C"}, {0x7A, "A", "D"},
    {0x7B, "A", "E"}, {0x7C, "A", "H"}, {0x7D, "A", "L"}, {0x7E, "A", "M"},
    {0x47, "B", "A"}, {0x40, "B", "B"}, {0x41, "B", "C"}, {0x42, "B", "D"},
    {0x43, "B", "E"}, {0x44, "B", "H"}, {0x45, "B", "L"}, {0x46, "B", "M"},
    {0x4F, "C", "A"}, {0x48, "C", "B"}, {0x49, "C", "C"}, {0x4A, "C", "D"},
    {0x4B, "C", "E"}, {0x4C, "C", "H"}, {0x4D, "C", "L"}, {0x4E, "C", "M"},
    {0x57, "D", "A"}, {0x50, "D", "B"}, {0x51, "D", "C"}, {0x52, "D", "D"},
    {0x53, "D", "E"}, {0x54, "D", "H"}, {0x55, "D", "L"}, {0x56, "D", "M"},
    {0x5F, "E", "A"}, {0x58, "E", "B"}, {0x59, "E", "C"}, {0x5A, "E", "D"},
    {0x5B, "E", "E"}, {0x5C, "E", "H"}, {0x5D, "E", "L"}, {0x5E, "E", "M"},
    {0x67, "H", "A"}, {0x60, "H", "B"}, {0x61, "H", "C"}, {0x62, "H", "D"},
    {0x63, "H", "E"}, {0x64, "H", "H"}, {0x65, "H", "L"}, {0x66, "H", "M"},
    {0x6F, "L", "A"}, {0x68, "L", "B"}, {0x69, "L", "C"}, {0x6A, "L", "D"},
    {0x6B, "L", "E"}, {0x6C, "L", "H"}, {0x6D, "L", "L"}, {0x6E, "L", "M"},
    {0x77, "M", "A"}, {0x70, "M", "B"}, {0x71, "M", "C"}, {0x72, "M", "D"},
    {0x73, "M", "E"}, {0x74, "M", "H"}, {0x75, "M", "L"}
};

// ===================== Helper Functions ======================================
// Sets the specified register in CPUState to a given value
// Used to prepare test setup before instruction execution
void setRegister(CPUState& cpu, const std::string& reg, uint8_t val) {
    if      (reg == "A") cpu.a = val;
    else if (reg == "B") cpu.b = val;
    else if (reg == "C") cpu.c = val;
    else if (reg == "D") cpu.d = val;
    else if (reg == "E") cpu.e = val;
    else if (reg == "H") cpu.h = val;
    else if (reg == "L") cpu.l = val;
    // Memory handled separately
}

// Returns the value of the specified register in CPUState
// Used to validate instruction results after execution
uint8_t getRegister(const CPUState& cpu, const std::string& reg) {
    if      (reg == "A") return cpu.a;
    else if (reg == "B") return cpu.b;
    else if (reg == "C") return cpu.c;
    else if (reg == "D") return cpu.d;
    else if (reg == "E") return cpu.e;
    else if (reg == "H") return cpu.h;
    else if (reg == "L") return cpu.l;
    return 0x00; // Default for "M"
}

// ===================== Unit Test: All MOV Opcodes ============================
// Executes all MOV opcode tests across register and memory combinations
// Verifies that MOV <dest>, <src> instructions copy the correct value
void UnitTest_MOV_All() {
    std::cout << "\n=== Running MOV Instruction Unit Tests ===\n";
    uint16_t memAddr = 0x2400;

    for (const auto& test : movTests) {
        Emulator emu;
        CPUState& cpu = emu.getCPUStateRef();
        Memory& mem   = emu.getMemoryRef();

        cpu.pc = 0x0000;
        cpu.sp = 0x4000;

        // Setup HL for memory operations
        if (test.dest == "M" || test.src == "M") {
            cpu.h = memAddr >> 8;
            cpu.l = memAddr & 0xFF;
        }

        mem.writeRomBytes(0x0000, test.opcode);
        uint8_t testVal = 0x3C;

        if (test.src != "M") {
            setRegister(cpu, test.src, testVal);
        } else {
            mem.WriteByte(memAddr, testVal);
        }

        emu.emulateCycles(1);

        uint8_t result = (test.dest == "M") ? mem.ReadByte(memAddr)
                                            : getRegister(cpu, test.dest);

        #ifdef ENABLE_VERBOSE_DEBUG
        std::cout << "[DEBUG: MOV " << test.dest << ", " << test.src << "]\n";
        std::cout << "  Opcode         = 0x" << std::hex << (int)test.opcode << "\n";
        std::cout << "  Expected Value = 0x3C\n";
        std::cout << "  Actual Value   = 0x" << std::hex << (int)result << "\n";
        #endif

        std::string label = "MOV " + test.dest + ", " + test.src;
        printTestResult(label, "Transfer 0x3C", result == testVal);
    }
}

// ===================== Main Entry Point ======================================
int main() {
#ifdef ENABLE_CPU_TESTING
    resetTestCounter();
    UnitTest_MOV_All();

    std::cout << "\n==============================\n";
    std::cout << " MOV Instruction Unit Test Summary\n";
    std::cout << "------------------------------\n";
    std::cout << " Total Tests : " << testsPassed + testsFailed << "\n";
    std::cout << GREEN << " Passed      : " << testsPassed << RESET << "\n";
    std::cout << RED   << " Failed      : " << testsFailed << RESET << "\n";
    std::cout << "==============================\n";
#else
    std::cout << "CPU testing is disabled. Define ENABLE_CPU_TESTING to run tests.\n";
#endif
    return 0;
}
