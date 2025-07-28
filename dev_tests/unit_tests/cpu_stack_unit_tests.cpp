// ============================================================================
// CPU Stack Unit Tests
// ----------------------------------------------------------------------------
// Target Module : CPU / Emulator (Intel 8080 Emulator)
// Purpose       : Verifies stack operations: PUSH, POP, XTHL, SPHL
// Scope         : Ensures correct memory writes/reads, SP updates, and register
//                 integrity during stack manipulation.
//
// Author        : Fredo
// Date          : 7/26/25
// ============================================================================

// ====================== Defines ========================================
#define ENABLE_COLOR_OUTPUT
#define ENABLE_CPU_TESTING
#define ENABLE_VERBOSE_DEBUG

// =======================  Includes ======================================
#include "../../src/model/emulator.hpp"
#include "../support/test_utils.hpp"
#include <iostream>
#include <cassert>
#include <iomanip>
#include <bitset>
#include <iostream>

// ========================= Helper Functions ==============================
// Write bytes into ROM starting at a given address (for test setup)
void writeRomInstructionSequence(Memory& mem, uint16_t startAddr, const std::vector<uint8_t>& bytes) {
    for (size_t i = 0; i < bytes.size(); ++i) {
        mem.writeRomBytes(startAddr + static_cast<uint16_t>(i), bytes[i]);
    }
}

// Write bytes into RAM starting at a given address (for test setup)
void writeRamInstructionSequence(Memory& mem, uint16_t startAddr, const std::vector<uint8_t>& bytes) {
    for (size_t i = 0; i < bytes.size(); ++i) {
        mem.WriteByte(startAddr + static_cast<uint16_t>(i), bytes[i]);
    }
}

// Print actual vs. expected flags using a bitmask (for debug comparison)
void printFlagsDebug(uint8_t actualFlags, uint8_t expectedFlags, uint8_t mask) {
    uint8_t maskedActual = actualFlags & mask;
    uint8_t maskedExpected = expectedFlags & mask;
    uint8_t delta = maskedActual ^ maskedExpected;

    std::cout << "=== FLAGS COMPARISON DEBUG ===" << std::endl;
    std::cout << "MASK           : 0x" << std::hex << (int)mask << " (" << std::bitset<8>(mask) << ")" << std::endl;
    std::cout << "Actual         : 0x" << std::hex << (int)actualFlags << " (" << std::bitset<8>(actualFlags) << ")" << std::endl;
    std::cout << "Expected       : 0x" << std::hex << (int)expectedFlags << " (" << std::bitset<8>(expectedFlags) << ")" << std::endl;
    std::cout << "Masked Actual  : 0x" << std::hex << (int)maskedActual << " (" << std::bitset<8>(maskedActual) << ")" << std::endl;
    std::cout << "Masked Expected: 0x" << std::hex << (int)maskedExpected << " (" << std::bitset<8>(maskedExpected) << ")" << std::endl;
    std::cout << "Delta          : 0x" << std::hex << (int)delta << " (" << std::bitset<8>(delta) << ")" << std::endl;

    if (delta == 0) {
        std::cout << "Flags match!" << std::endl;
    } else {
        std::cout << "Flags mismatch! Bits differ: " << std::bitset<8>(delta) << std::endl;
    }
    std::cout << "================================\n" << std::endl;
}


// ========================= Basic Stack PUSH Instructions =================
// For general-purpose register pairs: B, D, H

// ====================== Unit Test: PUSH B ===============================
// PUSH B: Pushes register pair BC to stack, decrements SP by 2
void UnitTest_PUSH_B() {
    uint8_t b = 0x12, c = 0x34;
    uint16_t sp = 0x4000;

    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    cpu.b = b;
    cpu.c = c;
    cpu.sp = sp;

    mem.writeRomBytes(0x0000, 0xC5); // PUSH B
    emu.emulateCycles(1);

    StackInspector stack(mem, cpu.sp);
    bool pass = stack.validatePair(b, c) && cpu.sp == 0x3FFE;

#ifdef ENABLE_VERBOSE_DEBUG
    printStackDebug("PUSH B", 0x00C5, sp, cpu.sp, std::make_pair(b, c), stack.readBytePair());
#endif
    printTestResult("PUSH B", "BC pushed to stack", pass);
}

// ====================== Unit Test: PUSH D ===============================
// PUSH D: Pushes register pair DE to stack, decrements SP by 2
void UnitTest_PUSH_D() {
    uint8_t d = 0x56, e = 0x78;
    uint16_t sp = 0x3000;

    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    cpu.d = d;
    cpu.e = e;
    cpu.sp = sp;

    mem.writeRomBytes(0x0000, 0xD5); // PUSH D
    emu.emulateCycles(1);

    StackInspector stack(mem, cpu.sp);
    bool pass = stack.validatePair(d, e) && cpu.sp == 0x2FFE;

#ifdef ENABLE_VERBOSE_DEBUG
    printStackDebug("PUSH D", 0x00D5, sp, cpu.sp, std::make_pair(d, e), stack.readBytePair());
#endif
    printTestResult("PUSH D", "DE pushed to stack", pass);
}

// ====================== Unit Test: PUSH H ===============================
// PUSH H: Pushes register pair HL to stack, decrements SP by 2
void UnitTest_PUSH_H() {
    uint8_t h = 0xAB, l = 0xCD;
    uint16_t sp = 0x4000;

    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    cpu.h = h;
    cpu.l = l;
    cpu.sp = sp;

    mem.writeRomBytes(0x0000, 0xE5); // PUSH H
    emu.emulateCycles(1);

    StackInspector stack(mem, cpu.sp);
    bool pass = stack.validatePair(h, l) && cpu.sp == 0x3FFE;

#ifdef ENABLE_VERBOSE_DEBUG
    printStackDebug("PUSH H", 0x00E5, sp, cpu.sp, std::make_pair(h, l), stack.readBytePair());
#endif
    printTestResult("PUSH H", "HL pushed to stack", pass);
}


// ============ Special Stack Operations (PSW, Flags) ============

// ====================== Unit Test: PUSH PSW ===============================
// PUSH PSW: Pushes Accumulator and Flags to stack, decrements SP by 2
void UnitTest_PUSH_PSW() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    // Setup CPU state
    cpu.a = 0xAB;
    cpu.sp = 0x4000;
    cpu.flags = { .s = 1, .z = 0, .ac = 1, .p = 1, .cy = 0 };

    // Assemble PUSH PSW instruction
    mem.writeRomBytes(0x0000, 0xF5); // PUSH PSW
    emu.emulateCycles(1);

    //  Stack check
    StackInspector stack(mem, cpu.sp);

    // Pack flags manually (bitwise)
    uint8_t expectedFlags =
        (cpu.flags.s  << 7) |
        (cpu.flags.z  << 6) |
        (1            << 1) |  
        (cpu.flags.ac << 4) |
        (cpu.flags.p  << 2) |
        (cpu.flags.cy << 0);

    // Validate outcome
    bool pass = (stack.getHighByte() == cpu.a &&
                 stack.getLowByte()  == expectedFlags &&
                 cpu.sp == 0x3FFE);

#ifdef ENABLE_VERBOSE_DEBUG
    printStackDebug("PUSH PSW", 0x00F5, 0x4000, cpu.sp,
                    std::make_pair(cpu.a, expectedFlags),
                    stack.readBytePair());
#endif

    printTestResult("PUSH PSW", "A+Flags pushed to stack", pass);
}

// ====================== Unit Test: POP PSW ==============================
// POP PSW: Restores Accumulator and Flags from the stack
void UnitTest_POP_PSW() {
    const uint8_t a_val = 0xF1;                  // Accumulator value
    const uint8_t flags_val = 0b10111000;        // Packed flags byte
    const uint16_t sp_start = 0x5000;
    const uint8_t opcode = 0xF1;                 // POP PSW
    const uint16_t romAddr = 0x0000;

    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    // Setup stack before POP
    cpu.sp = sp_start;
    mem.WriteByte(sp_start, flags_val);          // Low byte = Flags
    mem.WriteByte(sp_start + 1, a_val);          // High byte = Accumulator

    // Write POP PSW instruction to ROM
    mem.writeRomBytes(romAddr, opcode);
    emu.emulateCycles(1);

    // Manually repack flags after POP
    uint8_t actualFlags =
        (cpu.flags.s  << 7) |
        (cpu.flags.z  << 6) |
        (cpu.flags.ac << 4) |
        (cpu.flags.p  << 2) |
        (cpu.flags.cy << 0);

    const uint8_t FLAGS_MASK = 0xD5;
    const bool flagsMatch = ((actualFlags & FLAGS_MASK) == (flags_val & FLAGS_MASK));
    const bool pass = (cpu.a == a_val) &&
                  flagsMatch &&
                  (cpu.sp == sp_start + 2);

 #ifdef ENABLE_VERBOSE_DEBUG
    printPopDebug("POP PSW", opcode, sp_start, cpu.sp,
                  std::make_pair(a_val, flags_val),
                  std::make_pair(cpu.a, actualFlags));
    printFlagsDebug(actualFlags, 0xB8, 0xD5);
#endif

    printTestResult("POP PSW", "Restore A and Flags from stack", pass);
    std::cout << "Flags.z = " << (int)cpu.flags.z << "\n";

}

// ====================== Basic Stack POP Instructions ===================

// ====================== Unit Test: POP B ===============================
// POP B: Restores register pair BC from stack and increments SP by 2
void UnitTest_POP_B() {
    uint8_t lo = 0x78, hi = 0x56;
    uint16_t sp = 0x4000;

    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();

    mem.WriteByte(sp, lo);
    mem.WriteByte(sp + 1, hi);
    cpu.sp = sp;

    mem.writeRomBytes(0x0000, 0xC1); // POP B
    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printPopDebug("POP B", 0x00C1, sp, cpu.sp, std::make_pair(hi, lo), std::make_pair(cpu.b, cpu.c));
#endif

    bool pass = (cpu.b == hi && cpu.c == lo && cpu.sp == 0x4002);
    printTestResult("POP B", "Stack to BC", pass);
}

// ====================== Unit Test: POP D ===============================
// POP D: Restores register pair DE from stack and increments SP by 2
void UnitTest_POP_D() {
    uint8_t lo = 0x11, hi = 0x22;
    uint16_t sp = 0x3000;

    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();

    mem.WriteByte(sp, lo);
    mem.WriteByte(sp + 1, hi);
    cpu.sp = sp;

    mem.writeRomBytes(0x0000, 0xD1); // POP D
    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printPopDebug("POP D", 0x00D1, sp, cpu.sp, std::make_pair(hi, lo), std::make_pair(cpu.d, cpu.e));
#endif

    bool pass = (cpu.d == hi && cpu.e == lo && cpu.sp == 0x3002);
    printTestResult("POP D", "Stack to DE", pass);
}

// ====================== Unit Test: POP H ===============================
// POP H: Restores register pair HL from stack and increments SP by 2
void UnitTest_POP_H() {
    uint8_t lo = 0xDE, hi = 0xAD;
    uint16_t sp = 0x4000;

    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();

    mem.WriteByte(sp, lo);
    mem.WriteByte(sp + 1, hi);
    cpu.sp = sp;

    mem.writeRomBytes(0x0000, 0xE1); 
    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printPopDebug("POP H", 0x00E1, sp, cpu.sp, std::make_pair(hi, lo), std::make_pair(cpu.h, cpu.l));
#endif

    bool pass = (cpu.h == hi && cpu.l == lo && cpu.sp == 0x4002);
    printTestResult("POP H", "Stack to HL", pass);
}

// ============ Stack Transfer and Exchange Instructions ============

// ====================== Unit Test: SPHL ===============================
// SPHL: Sets SP to current value of HL register pair
void UnitTest_SPHL() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();

    cpu.h = 0x12;
    cpu.l = 0x34;
    cpu.sp = 0x0000;

    uint16_t originalSP = cpu.sp;
    Memory& mem = emu.getMemoryRef();
    mem.writeRomBytes(0x0000, 0xF9); 
    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printSPHLDebug(0x00F9, originalSP, cpu.sp);
#endif

    bool pass = (cpu.sp == 0x1234);
    printTestResult("SPHL", "SP = HL", pass);
}

// ====================== Unit Test: XTHL ===============================
// XTHL: Exchanges HL with contents at address pointed by SP
void UnitTest_XTHL() {
    uint16_t sp = 0x3000;
    uint8_t stackLo = 0x22, stackHi = 0x11;
    uint8_t hlLo = 0x44, hlHi = 0x33;

    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    mem.WriteByte(sp, stackLo);
    mem.WriteByte(sp + 1, stackHi);
    cpu.sp = sp;
    cpu.l = hlLo;
    cpu.h = hlHi;

    mem.writeRomBytes(0x0000, 0xE3); 
    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printXTHLDebug(0x00E3, sp,
                  std::make_pair(hlHi, hlLo),
                  std::make_pair(stackHi, stackLo),
                  std::make_pair(cpu.h, cpu.l),
                  std::make_pair(mem.ReadByte(sp + 1), mem.ReadByte(sp)));
#endif

    bool memMatch = (mem.ReadByte(sp) == hlLo && mem.ReadByte(sp + 1) == hlHi);
    bool hlMatch = (cpu.l == stackLo && cpu.h == stackHi);

    printTestResult("XTHL", "Exchange HL <-> Stack[SP]", memMatch && hlMatch);
}

// ===================== Stack Boundary Test ===============================
// Verifies edge case behavior when SP reaches lowest memory boundary
void UnitTest_StackBoundary_PUSH_B() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();
 

    cpu.b = 0x12;
    cpu.c = 0x34;
    cpu.sp = 0x2002;
    cpu.pc = 0x2000;

    mem.WriteByte (0x2000, 0xC5 ); // PUSH B
    emu.emulateCycles(1);

    StackInspector stack(mem, cpu.sp);
    bool pass = stack.validatePair(0x12, 0x34) && cpu.sp == 0x2000;

#ifdef ENABLE_VERBOSE_DEBUG
    printStackDebug("Stack Boundary PUSH B", 0x00C5, 0x0002, cpu.sp, std::make_pair(0x12, 0x34), stack.readBytePair());
#endif
    printTestResult("Stack Boundary", "PUSH B near memory limit", pass);
}

// ====================== Unit Test: Basic PUSH/POP ========================
void UnitTest_Basic_PUSH_POP() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    cpu.b = 0x11;
    cpu.c = 0x22;
    cpu.d = 0x33;
    cpu.e = 0x44;
    cpu.sp = 0x4004;
    cpu.pc = 0x0000;

    writeRomInstructionSequence(mem, 0x0000, { 0xC5, 0xD5, 0xD1, 0xC1 });
    emu.emulateCycles(4);

    const bool dRestored = (cpu.d == 0x33 && cpu.e == 0x44);
    const bool bRestored = (cpu.b == 0x11 && cpu.c == 0x22);
    const bool finalSP   = (cpu.sp == 0x4004);

#ifdef ENABLE_VERBOSE_DEBUG
    std::cout << "[DEBUG: Basic PUSH/POP]\n";
    std::cout << "  SP Final   = 0x" << std::hex << cpu.sp << "\n";
    std::cout << "  BC Restored= 0x" << std::hex << (int)cpu.b << (int)cpu.c << "\n";
    std::cout << "  DE Restored= 0x" << std::hex << (int)cpu.d << (int)cpu.e << "\n";
#endif

    printTestResult("Basic Stack", "PUSH B, D then POP D, B", dRestored && bRestored && finalSP);
}

void UnitTest_Nested_PUSH_POP() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    // Initial CPU register setup
    cpu.b = 0x11; cpu.c = 0x22;
    cpu.d = 0x33; cpu.e = 0x44;
    cpu.h = 0x55; cpu.l = 0x66;
    cpu.sp = 0x4000;

    //  PUSH B, D, H — then POP H, D, B (reverse order)
    const std::vector<uint8_t> opcodes = {
        0xC5, // PUSH B
        0xD5, // PUSH D
        0xE5, // PUSH H
        0xE1, // POP H
        0xD1, // POP D
        0xC1  // POP B
    };

    writeRomInstructionSequence(mem, 0x0000, opcodes);
    emu.emulateCycles(opcodes.size());

    //  Validate: All registers restored correctly and SP reset
    bool pass = (cpu.b == 0x11 && cpu.c == 0x22 &&
                 cpu.d == 0x33 && cpu.e == 0x44 &&
                 cpu.h == 0x55 && cpu.l == 0x66 &&
                 cpu.sp == 0x4000);

#ifdef ENABLE_VERBOSE_DEBUG
    std::cout << "[DEBUG: Nested PUSH/POP Test]\n";
    std::cout << "  Final SP      = 0x" << std::hex << cpu.sp << "\n";
    std::cout << "  B, C Register = 0x" << std::hex << (int)cpu.b << ", 0x" << (int)cpu.c << "\n";
    std::cout << "  D, E Register = 0x" << std::hex << (int)cpu.d << ", 0x" << (int)cpu.e << "\n";
#endif

    printTestResult("Nested Stack", "PUSH/POP B,D,H sequence", pass);
}

// ===================== CALL/RET Stack Operation ==========================
// CALL pushes return address to stack, RET pops it into PC
void UnitTest_CALL_RET() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    cpu.sp = 0x3000;
    cpu.pc = 0x0000;

    // CALL 0x1234 (CD 34 12)
    writeRomInstructionSequence(mem, 0x0000, { 0xCD, 0x34, 0x12 }); // CALL
    writeRomInstructionSequence(mem, 0x1234, { 0xC9 });             // RET at target
    emu.emulateCycles(2); // CALL and RET

    StackInspector inspector(mem, 0x2FFE);  // After CALL, SP = 0x2FFE

    bool pcRestored   = (cpu.pc == 0x0003); // Should return here after RET
    bool spRestored   = (cpu.sp == 0x3000); // Should match original
    bool returnAddrOk = inspector.readBytePair() == std::make_pair(0x00, 0x03);

#ifdef ENABLE_VERBOSE_DEBUG
    printStackDebug("CALL/RET", 0x00CD, 0x3000, cpu.sp, std::make_pair(0x00, 0x03), inspector.readBytePair());
#endif

    printTestResult("CALL/RET", "CALL subroutine and return", pcRestored && spRestored && returnAddrOk);
}


// ===================== RST Instruction Stack Behavior ====================


// ====================== Unit Test: RST 5 =======================
// RST pushes current PC and jumps to vector address
void UnitTest_RST5() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem   = emu.getMemoryRef();

    cpu.sp = 0x3000;
    cpu.pc = 0x0000;

    writeRomInstructionSequence(mem, 0x0000, { 0xEF }); // RST 5 (0x28)
    emu.emulateCycles(1);

    StackInspector inspector(mem, 0x2FFE);

    bool pcJumped     = (cpu.pc == 0x0028);
    bool spAdjusted   = (cpu.sp == 0x2FFE);
    bool returnStored = inspector.validatePair(0x00, 0x01);

#ifdef ENABLE_VERBOSE_DEBUG
    printStackDebug("RST 5", 0x00EF, 0x3000, cpu.sp, std::make_pair(0x00, 0x01), inspector.readBytePair());
    std::cout << "  Actual PC      = 0x" << std::hex << cpu.pc << "\n";
    std::cout << "  Expected PC    = 0x0028\n";
    std::cout << "  Inspector Pair = 0x" << std::hex << (int)inspector.getHighByte()
              << ", 0x" << (int)inspector.getLowByte() << "\n";
#endif

    printTestResult("RST Instruction", "RST 5 pushes PC and jumps to 0x0028", pcJumped && spAdjusted && returnStored);
}



// ====================== Main Entry Point ===============================
int main() {

    // == Basic Stack PUSH == 
    UnitTest_PUSH_B();
    UnitTest_PUSH_D();
    UnitTest_PUSH_H();

    // == Special Stack Operations ==
    UnitTest_PUSH_PSW();
    UnitTest_POP_PSW();

    // == Basic Stack POP == 
    UnitTest_POP_B();
    UnitTest_POP_D();
    UnitTest_POP_H();

    // == Stack Transfer ==
    UnitTest_SPHL();
    UnitTest_XTHL();

    // == Edge Cases and Composite ==
    UnitTest_StackBoundary_PUSH_B();
    UnitTest_Basic_PUSH_POP();
    UnitTest_Nested_PUSH_POP();
    UnitTest_CALL_RET();
    UnitTest_RST5();
    
    

    std::cout << "\n=== Unit Tests Complete: " << testCounter - 1 << " Total ===\n";
    return 0;
}
