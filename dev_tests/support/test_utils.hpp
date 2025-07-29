// ============================================================================
// Test Utility Toolkit
// ----------------------------------------------------------------------------
// Target Module : General Testing Utility for all Modules
// Purpose       : Provides shared test helpers for logging, formatting,
//                 CPU instruction simulation, and debug output
// Scope         : Test counter, ANSI colors, flag debug, hex printing,
//                 emulator helper methods (requires ENABLE_CPU_TESTING)
//
// Author        : Fredo
// Date          : 7/24/25
// ============================================================================


// ============================== Defines ===================================
#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

// ============================== Includes ===================================
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <functional>
#include <optional>
#include "../../src/model/emulator.hpp"

// =============================== Flags Alias ===============================
// Alias for CPU flag struct
using Flags = decltype(CPUState{}.flags);

// ============ Console Output & Formatting Utilities ========================

// ========================== Test Color Output ==============================
// Defines ANSI color codes for terminal output if enabled
#ifdef ENABLE_COLOR_OUTPUT
inline const std::string GREEN = "\033[32m";
inline const std::string RED   = "\033[31m";
inline const std::string RESET = "\033[0m";
#else
inline const std::string GREEN = "";
inline const std::string RED   = "";
inline const std::string RESET = "";
#endif

// ============================ Test Counter =================================
// Global counter to number each test output
inline int testCounter = 1;

inline void resetTestCounter() {
    testCounter = 1;
}

// ============================ Hex Formatter ================================
// Returns formatted hexadecimal string with leading zeros
inline std::string hex(uint16_t val, int width = 4) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase
        << std::setw(width) << std::setfill('0') << val;
    return oss.str();
}

// ========================== Basic Test Result ==============================
// Standardized test pass/fail output with coloring and numbering
inline void printTestResult(const std::string& tag,
                            const std::string& description,
                            bool passed) {
    std::cout << "[" << tag << " | Test #" << std::dec << testCounter++ << "] "
              << description << ": "
              << (passed ? (GREEN + "[PASS]" + RESET)
                         : (RED + "[FAIL]" + RESET))
              << "\n\n";
}

// ======================== Simple Debug Snapshot ============================
// Debug print for single-byte value and CY flag check
inline void printDebugResult(const std::string& testName,
                             uint8_t actualA, bool actualCY,
                             uint8_t expectedA, bool expectedCY) {
    std::cout << "[TEST] " << testName << "\n"
              << "  Actual   → A = " << hex(actualA, 2)
              << ", CY = " << std::boolalpha << actualCY << "\n"
              << "  Expected → A = " << hex(expectedA, 2)
              << ", CY = " << std::boolalpha << expectedCY << "\n"
              << "  Result   → "
              << ((actualA == expectedA && actualCY == expectedCY)
                  ? (GREEN + "[PASS]" + RESET)
                  : (RED + "[FAIL]" + RESET))
              << "\n\n";
}

// ======================== Binary File Writer ===============================
// Creates a binary file using the provided byte buffer
inline void createBinaryFile(const std::string& filename,
                             const std::vector<uint8_t>& data) {
    std::ofstream out(filename, std::ios::binary);
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
    out.close();
}

// ============ CPU Emulation Test Utilities ================================

// ====================== CPU Instruction Helpers ============================
// Creates an emulator, loads and executes a single instruction
#ifdef ENABLE_CPU_TESTING

// ====================== Create Emulator ====================================
// Creates and returns a fresh Emulator instance with memory cleared and CPU reset.
inline Emulator createTestEmulator() {
    Emulator emu;
    emu.reset();
    return emu;
}

// ======================== Isolate Opcode ====================================
// Executes a single instruction on a clean emulator, with optional CPU state setup.
// Useful for quick isolated opcode testing.
inline CPUState runSingleInstruction(const std::vector<uint8_t>& opcode,
                                     const std::function<void(CPUState&)>& setupFn = nullptr) {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    if (setupFn) setupFn(cpu);

    for (size_t i = 0; i < opcode.size(); ++i)
        mem.writeRomBytes(static_cast<uint16_t>(0x0000 + i), opcode[i]);

    emu.emulateCycles(1);
    return emu.getCPUState();
}

// ====================== Isolate Opcode w/ Memory & CPU =======================
// Executes a single instruction with both CPU and Memory state preloaded
// Allows you to write a specific byte to a memory address before execution
// and apply a custom setup function that modifies CPU and memory as needed
inline CPUState runInstructionWithMemory(const std::vector<uint8_t>& opcode,
                                         uint16_t memAddress,
                                         uint8_t memValue,
                                         const std::function<void(CPUState&, Memory&)>& setupFn) {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    mem.WriteByte(memAddress, memValue);
    if (setupFn) setupFn(cpu, mem);

    for (size_t i = 0; i < opcode.size(); ++i)
        mem.writeRomBytes(static_cast<uint16_t>(0x0000 + i), opcode[i]);

    emu.emulateCycles(1);
    return emu.getCPUState();
}

#endif // ENABLE_CPU_TESTING

// ============ Debug Print Utilities ========================================

// ======================= CPU Arithmetic: Print Helper ======================
// Print detailed debug output for arithmetic operations
// Displays actual vs expected result, flags (Z, S, P, AC, CY), and carry info
// Useful for debugging mismatched arithmetic behavior and emulator logic
#include <optional>

void printArithmeticDebug(
    const std::string& name,
    uint8_t lhs,
    uint8_t rhs,
    bool useCarry,
    uint8_t actualResult,
    const Flags& flags,
    uint8_t expectedResult,
    bool expectedCarry,
    std::optional<bool> expectedZ = std::nullopt,
    std::optional<bool> expectedS = std::nullopt,
    std::optional<bool> expectedP = std::nullopt,
    std::optional<bool> expectedAC = std::nullopt)
{
    std::cout << "[DEBUG: " << name << "]\n";
    std::cout << "  LHS           = 0x" << std::hex << (int)lhs << "\n";
    std::cout << "  RHS           = 0x" << std::hex << (int)rhs << "\n";
    std::cout << "  Use Carry     = " << (useCarry ? "true" : "false") << "\n";
    std::cout << "  Actual Result = 0x" << std::hex << (int)actualResult << "\n";
    std::cout << "  Expected      = 0x" << std::hex << (int)expectedResult << "\n";
    std::cout << "  Carry         = " << (int)flags.cy << " (expected: " << expectedCarry << ")\n";
    std::cout << "  Flags         = Z:" << (int)flags.z
              << " S:" << (int)flags.s
              << " P:" << (int)flags.p
              << " AC:" << (int)flags.ac
              << " CY:" << (int)flags.cy << "\n";

    // Optional expected flags (if provided)
    if (expectedZ.has_value())
        std::cout << "  Expected Z    = " << expectedZ.value() << "\n";
    if (expectedS.has_value())
        std::cout << "  Expected S    = " << expectedS.value() << "\n";
    if (expectedP.has_value())
        std::cout << "  Expected P    = " << expectedP.value() << "\n";
    if (expectedAC.has_value())
        std::cout << "  Expected AC   = " << expectedAC.value() << "\n";
}


// ======================= CPU Logic: Print Helper ======================
// Print detailed debug output for logical operations and comparisons
// Reports flag correctness per bit and highlights mismatches 
// Ideal for verifying bitwise ops and flag-setting logic
void printLogicDebug(
    const std::string& name,
    uint8_t lhs,
    uint8_t rhs,
    uint8_t actualResult,
    const Flags& flags,
    uint8_t expectedResult,
    bool expectedZ,
    bool expectedS,
    bool expectedP,
    bool expectedCY,
    bool expectedAC)
{
    std::cout << "[DEBUG: " << name << "]\n";
    std::cout << "  LHS           = 0x" << std::hex << (int)lhs << "\n";
    std::cout << "  RHS           = 0x" << std::hex << (int)rhs << "\n";
    std::cout << "  Actual Result = 0x" << std::hex << (int)actualResult
              << " (expected: 0x" << (int)expectedResult << ")\n";
    std::cout << "  Flags         = Z:" << (int)flags.z << " (exp:" << expectedZ << ")"
              << " S:" << (int)flags.s << " (exp:" << expectedS << ")"
              << " P:" << (int)flags.p << " (exp:" << expectedP << ")"
              << " CY:" << (int)flags.cy << " (exp:" << expectedCY << ")"
              << " AC:" << (int)flags.ac << " (exp:" << expectedAC << ")\n";
}

// ===================== Stack Debug Helper ==================================
// Outputs detailed stack debug information in a human-readable format:
// opcode value, initial/final SP, register and memory pair values,
// and stack byte match result
inline void printStackDebug(
    const std::string& name,
    uint16_t opcode,
    uint16_t initialSP,
    uint16_t finalSP,
    std::optional<std::pair<uint8_t, uint8_t>> regPair = std::nullopt,
    std::optional<std::pair<uint8_t, uint8_t>> memPair = std::nullopt) {

#ifdef ENABLE_VERBOSE_DEBUG
    std::cout << "\n[DEBUG: " << name << "]\n";
    std::cout << "  Opcode        = " << hex(opcode, 4) << "\n";
    std::cout << "  Initial SP    = " << hex(initialSP, 4) << "\n";

    if (regPair.has_value()) {
        std::cout << "  Register Pair = " << hex(regPair->first, 2)
                  << ", " << hex(regPair->second, 2) << "\n";
    }
    if (memPair.has_value()) {
        std::cout << "  Stack Bytes   = " << hex(memPair->first, 2)
                  << ", " << hex(memPair->second, 2) << "\n";
    }
    if (regPair.has_value() && memPair.has_value()) {
        bool match = (regPair->first == memPair->first &&
                      regPair->second == memPair->second);
        std::cout << "  Stack Match   = "
                  << (match ? "[PASS]" : "[FAIL]") << "\n";
    }

    std::cout << "  Final SP      = " << hex(finalSP, 4) << "\n";
#endif
}

// ============================= CPU Stack: POP Debug Printer ===============================
// Prints detailed debug output for POP instructions.
// Verifies that the values popped from the stack match the expected register pair.
inline void printPopDebug(const std::string& label,
                           uint16_t opcode,
                           uint16_t initialSP,
                           uint16_t finalSP,
                           std::pair<uint8_t, uint8_t> expected,
                           std::pair<uint8_t, uint8_t> actual) {
    std::cout << "[DEBUG: " << label << "]\n";
    std::cout << "  Opcode        = " << hex(opcode) << "\n";
    std::cout << "  Initial SP    = " << hex(initialSP) << "\n";
    std::cout << "  Stack Bytes   = " << hex(expected.first, 2) << ", " << hex(expected.second, 2) << "\n";
    std::cout << "  Register Pair = " << hex(actual.first, 2) << ", " << hex(actual.second, 2) << "\n";
    std::cout << "  Match         = [" << ((expected == actual) ? "PASS" : "FAIL") << "]\n";
    std::cout << "  Final SP      = " << hex(finalSP) << "\n";
}

// Prints debug output for the SPHL instruction
// Confirms that SP is set to the value stored in the HL register pair.
inline void printSPHLDebug(uint16_t opcode,
                            uint16_t originalSP,
                            uint16_t resultSP) {
    std::cout << "[DEBUG: SPHL]\n";
    std::cout << "  Opcode        = " << hex(opcode) << "\n";
    std::cout << "  Original SP   = " << hex(originalSP) << "\n";
    std::cout << "  Result SP     = " << hex(resultSP) << "\n";
    std::cout << "  SP = HL       = [" << ((originalSP != resultSP) ? "PASS" : "FAIL") << "]\n";
}

// Prints debug output for the XTHL instruction
// Displays original and new values of HL and stack memory to verify correct exchange
inline void printXTHLDebug(uint16_t opcode,
                            uint16_t sp,
                            std::pair<uint8_t, uint8_t> originalHL,
                            std::pair<uint8_t, uint8_t> originalMem,
                            std::pair<uint8_t, uint8_t> newHL,
                            std::pair<uint8_t, uint8_t> newMem) {
    std::cout << "[DEBUG: XTHL]\n";
    std::cout << "  Opcode        = " << hex(opcode) << "\n";
    std::cout << "  SP            = " << hex(sp) << "\n";
    std::cout << "  Original HL   = " << hex(originalHL.first, 2) << ", " << hex(originalHL.second, 2) << "\n";
    std::cout << "  Original Mem  = " << hex(originalMem.first, 2) << ", " << hex(originalMem.second, 2) << "\n";
    std::cout << "  New HL        = " << hex(newHL.first, 2) << ", " << hex(newHL.second, 2) << "\n";
    std::cout << "  New Mem       = " << hex(newMem.first, 2) << ", " << hex(newMem.second, 2) << "\n";
    std::cout << "  Exchange      = ["
              << ((originalHL == newMem && originalMem == newHL) ? "PASS" : "FAIL")
              << "]\n";
}


// ================= StackInspector Utility Class ============================
// StackInspector: Lightweight utility to inspect stack pointer-relative memory
// Provides direct access to bytes and word values at the stack pointer
struct StackInspector {
    Memory& memory;
    uint16_t stackPointer;

    StackInspector(Memory& memRef, uint16_t sp) : memory(memRef), stackPointer(sp) {}

    uint8_t getLowByte() const { return memory.ReadByte(stackPointer); }
    uint8_t getHighByte() const { return memory.ReadByte(stackPointer + 1); }

    uint16_t readWord() const {
        return (getHighByte() << 8) | getLowByte();
    }

    std::pair<uint8_t, uint8_t> readBytePair() const {
        return {getHighByte(), getLowByte()};
    }

    bool validatePair(uint8_t expectedHigh, uint8_t expectedLow) const {
        return (getHighByte() == expectedHigh && getLowByte() == expectedLow);
    }

    void printDebug(const std::string& label) const {
#ifdef ENABLE_VERBOSE_DEBUG
        std::cout << "[DEBUG: " << label << "] SP = " << hex(stackPointer, 4)
                  << ", High = " << hex(getHighByte(), 2)
                  << ", Low = " << hex(getLowByte(), 2) << "\n";
#endif
    }
};

#endif // TEST_UTILS_HPP

