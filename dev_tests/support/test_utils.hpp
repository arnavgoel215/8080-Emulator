// ============================================================================
// Test Utility Toolkit
// ----------------------------------------------------------------------------
// Target Module : General Testing Utility for all Modules
// Purpose       : Provides shared test helpers for logging, formatting, and
//                 output styling
// Scope         : Inline print/log utilities for test status and hex formatting
//
// Author        : Fredo
// Date          : 7/19/25
// ============================================================================


// ============================= Define Files =================================
#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

// ============================= Include Files ================================
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "../../src/model/emulator.hpp"


// ===================== Global Variables =====================================
// Global Counter for tracking test execution order
inline int testCounter = 1;


// ======================= ANSI Color Definitions =============================
// Use ENABLE_COLOR_OUTPUT to add color to test result output
#ifdef ENABLE_COLOR_OUTPUT
inline const std::string GREEN = "\033[32m";
inline const std::string RED   = "\033[31m";
inline const std::string RESET = "\033[0m";
#else
inline const std::string GREEN = "";
inline const std::string RED   = "";
inline const std::string RESET = "";
#endif


// ====================== CPU Testing Utilities ===============================
#ifdef ENABLE_CPU_TESTING

// ======================== Single Instruction Runner =========================
// Injects instruction at 0x0000, runs 1 cycle, and returns CPU state
inline CPUState runSingleInstruction(const std::vector<uint8_t>& opcode) {
    Emulator emu;
    emu.reset();
    Memory& mem = emu.getMemoryRef();

    for (size_t i = 0; i < opcode.size(); ++i) {
        mem.writeRomBytes(static_cast<uint16_t>(0x0000 + i), opcode[i]);
    }

    emu.emulateCycles(1);
    return emu.getCPUState();
}

#endif // ENABLE_CPU_TESTING

// ========================= Test Counter Reset ==============================
// Resets global testCounter to 1 for clean batch test output
inline void resetTestCounter() {
    testCounter = 1;
}
// ======================== Test Output Formatter =============================
// Prints test result (PASS/FAIL) with test number and optional color
inline void printTestResult(const std::string& tag, const std::string& description, bool passed) {
    std::cout << "[" << tag << " | Test #" << std::dec << testCounter++ << "] " << description << ": "
              << (passed ? (GREEN + "[PASS]" + RESET) : (RED + "[FAIL]" + RESET))
              << "\n";
}


// ============================ Formatting for Hex ============================
// Formats a 16-bit value as uppercase 0x-prefixed hex string
inline std::string hex(uint16_t value, int width = 4) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase
        << std::setw(width) << std::setfill('0') << value;
    return oss.str();
}


// ======================= Binary File Writer ================================
// Writes a byte vector to file as raw binary
inline void createBinaryFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream out(filename, std::ios::binary);
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
    out.close();
}

// ======================== Emulator Factory Helper ==========================
// Creates and returns a fresh Emulator instance with reset state
inline Emulator createTestEmulator() {
    Emulator emu;
    emu.reset();
    return emu;
}
#endif // TEST_UTILS_HPP
