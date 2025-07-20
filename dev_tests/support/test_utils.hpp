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



// ========================= Define Files ================================
#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

// ====================== Include Files ===================================
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

// ===================== Global Variable ==================================
// Global Counter for # of times you ran a test
inline int testCounter = 1;

// ===================== ANSI Color Definitions ============================
// Use ENABLE_COLOR_OUTPUT to add color to test results 
#ifdef ENABLE_COLOR_OUTPUT
inline const std::string GREEN = "\033[32m";
inline const std::string RED   = "\033[31m";
inline const std::string RESET = "\033[0m";
#else
inline const std::string GREEN = "";
inline const std::string RED   = "";
inline const std::string RESET = "";
#endif

// =============================== Printing ================================================
// Formats and prints test result status
inline void printTestResult(const std::string& tag, const std::string& description, bool passed) {
    std::cout << "[" << tag << " Test #" << testCounter++ << "] " << description << ": "
              << (passed ? (GREEN + "[PASS]" + RESET) : (RED + "[FAIL]" + RESET))
              << "\n";
}

// ============================ Formatting for Hex =========================================
// Formats a 16-bit value as uppercase 0x-prefixed hex string
inline std::string hex(uint16_t value, int width = 4) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase
        << std::setw(width) << std::setfill('0') << value;
    return oss.str();
}

#endif // TEST_UTILS_HPP
