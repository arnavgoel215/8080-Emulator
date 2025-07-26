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
using Flags = decltype(CPUState{}.flags);


// ========================== Test Color Output ==============================
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
inline int testCounter = 1;

inline void resetTestCounter() {
    testCounter = 1;
}

// ============================ Hex Formatter ================================
inline std::string hex(uint16_t val, int width = 4) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase
        << std::setw(width) << std::setfill('0') << val;
    return oss.str();
}

// ========================== Basic Test Result ==============================
inline void printTestResult(const std::string& tag,
                            const std::string& description,
                            bool passed)
{
    std::cout << "[" << tag << " | Test #" << std::dec << testCounter++ << "] "
              << description << ": "
              << (passed ? (GREEN + "[PASS]" + RESET)
                         : (RED + "[FAIL]" + RESET))
              << "\n";
}

// ======================== Simple Debug Snapshot ============================
inline void printDebugResult(const std::string& testName,
                             uint8_t actualA, bool actualCY,
                             uint8_t expectedA, bool expectedCY)
{
    std::cout << "[TEST] " << testName << "\n"
              << "  Actual   → A = " << hex(actualA, 2)
              << ", CY = " << std::boolalpha << actualCY << "\n"
              << "  Expected → A = " << hex(expectedA, 2)
              << ", CY = " << std::boolalpha << expectedCY << "\n"
              << "  Result   → " << ((actualA == expectedA && actualCY == expectedCY)
                                     ? (GREEN + "[PASS]" + RESET)
                                     : (RED + "[FAIL]" + RESET))
              << "\n\n";
}

// ======================== Binary File Writer ===============================
inline void createBinaryFile(const std::string& filename,
                             const std::vector<uint8_t>& data)
{
    std::ofstream out(filename, std::ios::binary);
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
    out.close();
}

// ====================== CPU Instruction Helpers ============================
#ifdef ENABLE_CPU_TESTING

inline Emulator createTestEmulator() {
    Emulator emu;
    emu.reset();
    return emu;
}

inline CPUState runSingleInstruction(const std::vector<uint8_t>& opcode,
                                     const std::function<void(CPUState&)>& setupFn = nullptr)
{
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    if (setupFn) setupFn(cpu);

    for (size_t i = 0; i < opcode.size(); ++i)
        mem.writeRomBytes(static_cast<uint16_t>(0x0000 + i), opcode[i]);

    emu.emulateCycles(1);
    return emu.getCPUState();
}

#endif // ENABLE_CPU_TESTING

// ==================== Arithmetic Debug Helper ==============================
inline void printArithmeticDebug(const std::string& label,
                                 uint8_t initialA,
                                 uint8_t operand,
                                 bool carryIn,
                                 uint8_t resultA,
                                 const Flags& flags,
                                 uint8_t expectedA,
                                 bool expectedCY,
                                 std::optional<bool> expectedZ = std::nullopt,
                                 std::optional<bool> expectedS = std::nullopt,
                                 std::optional<bool> expectedP = std::nullopt,
                                 std::optional<bool> expectedAC = std::nullopt)
{
    std::cout << "[DEBUG: " << label << "]\n"
              << "  Initial A      = " << hex(initialA, 2) << "\n"
              << "  Operand        = " << hex(operand, 2) << (carryIn ? " + CY(1)" : "") << "\n"
              << "  Result A       = " << hex(resultA, 2) << "\n"
              << "  Flags          = Z=" << flags.z
              << " S=" << flags.s
              << " P=" << flags.p
              << " CY=" << flags.cy
              << " AC=" << flags.ac << "\n"
              << "  Expected A     = " << hex(expectedA, 2)
              << ", CY = " << std::boolalpha << expectedCY << "\n";

    if (expectedZ)  std::cout << "  Expected Z     = " << expectedZ.value() << "\n";
    if (expectedS)  std::cout << "  Expected S     = " << expectedS.value() << "\n";
    if (expectedP)  std::cout << "  Expected P     = " << expectedP.value() << "\n";
    if (expectedAC) std::cout << "  Expected AC    = " << expectedAC.value() << "\n";

    std::cout << "\n";
}

// ===================== Logic Debug Helper ==================================
inline void printLogicDebug(const std::string& label,
                            uint8_t initialA,
                            uint8_t operand,
                            uint8_t resultA,
                            const Flags& flags,
                            uint8_t expectedA,
                            std::optional<bool> expectedZ = std::nullopt,
                            std::optional<bool> expectedS = std::nullopt,
                            std::optional<bool> expectedP = std::nullopt,
                            std::optional<bool> expectedCY = std::nullopt,
                            std::optional<bool> expectedAC = std::nullopt)
{
    std::cout << "[DEBUG: " << label << "]\n"
              << "  Initial A      = " << hex(initialA, 2) << "\n"
              << "  Operand        = " << hex(operand, 2) << "\n"
              << "  Result A       = " << hex(resultA, 2) << "\n"
              << "  Flags          = Z=" << flags.z
              << " S=" << flags.s
              << " P=" << flags.p
              << " CY=" << flags.cy
              << " AC=" << flags.ac << "\n"
              << "  Expected A     = " << hex(expectedA, 2) << "\n";

    if (expectedZ)  std::cout << "  Expected Z     = " << expectedZ.value() << "\n";
    if (expectedS)  std::cout << "  Expected S     = " << expectedS.value() << "\n";
    if (expectedP)  std::cout << "  Expected P     = " << expectedP.value() << "\n";
    if (expectedCY) std::cout << "  Expected CY    = " << expectedCY.value() << "\n";
    if (expectedAC) std::cout << "  Expected AC    = " << expectedAC.value() << "\n";

    std::cout << "\n";
}

inline CPUState runInstructionWithMemory(const std::vector<uint8_t>& opcode,
                                         uint16_t memAddress,
                                         uint8_t memValue,
                                         const std::function<void(CPUState&, Memory&)>& setupFn)
{
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    mem.WriteByte(memAddress, memValue); // preload memory

    if (setupFn) setupFn(cpu, mem);

    for (size_t i = 0; i < opcode.size(); ++i)
        mem.writeRomBytes(static_cast<uint16_t>(0x0000 + i), opcode[i]);

    emu.emulateCycles(1);
    return emu.getCPUState();
}

#endif // TEST_UTILS_HPP
