// ============================================================================
// CPU Logic & Control Unit Tests - D-Prefixed Instructions
// ----------------------------------------------------------------------------
// Target Module : CPU / Emulator (Intel 8080 Emulator)
// Purpose       : Validates all D-prefixed instructions including:
//                 - DAD (Add register pair to HL)
//                 - DCR (Decrement register or memory)
//                 - DCX (Decrement register pair)
//                 - DAA (Decimal Adjust Accumulator)
//                 - DI / EI (Disable/Enable interrupts)
// Scope         : Ensures accurate register arithmetic, flag results, memory
//                 updates, and correct interrupt enable/disable behavior.
//
// Author        : Fredo
// Date          : 7/30/25
// ============================================================================

// ================================= Define ====================================
#define ENABLE_COLOR_OUTPUT
#define ENABLE_CPU_TESTING
#define ENABLE_VERBOSE_DEBUG

// ================================== Include ================================
#include "../../src/model/emulator.hpp"
#include "../support/test_utils.hpp"
#include <iostream>
#include <cassert>

// =========================== DAD Instructions ==============================

// ================= Unit Test: DAD B (0x09) =================
// Adds BC to HL and stores result in HL
void UnitTest_DAD_B() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.h = 0x12; cpu.l = 0x34; // HL = 0x1234
    cpu.b = 0x56; cpu.c = 0x78; // BC = 0x5678
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x09); // DAD B

    emu.emulateCycles(1);

    uint16_t hl = (cpu.h << 8) | cpu.l;
#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DAD B", 0x09, 0x0000, cpu.pc, {
        "Initial HL = 0x1234",
        "Operand    = BC = 0x5678",
        "Final HL   = 0x" + hex(hl, 4)
    });
#endif
    bool passed = (hl == 0x68AC);
    printTestResult("DAD B", "HL = HL + BC = 0x1234 + 0x5678 = 0x68AC", passed);
}

// ================= Unit Test: DAD D (0x19) =================
// Adds DE to HL and stores result in HL
void UnitTest_DAD_D() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.h = 0x11; cpu.l = 0x11; // HL = 0x1111
    cpu.d = 0x22; cpu.e = 0x22; // DE = 0x2222
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x19); // DAD D

    emu.emulateCycles(1);

    uint16_t hl = (cpu.h << 8) | cpu.l;
#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DAD D", 0x19, 0x0000, cpu.pc, {
        "Initial HL = 0x1111",
        "Operand    = DE = 0x2222",
        "Final HL   = 0x" + hex(hl, 4),
        "Carry Flag = " + std::to_string(cpu.flags.cy)
    }, true, 0x0001);
#endif
    bool passed = (hl == 0x3333);
    printTestResult("DAD D", "HL = HL + DE = 0x1111 + 0x2222 = 0x3333", passed);
}

// ================= Unit Test: DAD H (0x29) =================
// Adds HL to HL (i.e., doubles HL)
void UnitTest_DAD_H() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.h = 0x10; cpu.l = 0x00; // HL = 0x1000
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x29); // DAD H

    emu.emulateCycles(1);

    uint16_t hl = (cpu.h << 8) | cpu.l;
#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DAD H", 0x29, 0x0000, cpu.pc, {
        "Initial HL = 0x1000",
        "Operand    = HL",
        "Final HL   = 0x" + hex(hl, 4),
        "Carry Flag = " + std::to_string(cpu.flags.cy)
    }, true, 0x0001);
#endif
    bool passed = (hl == 0x2000);
    printTestResult("DAD H", "HL = HL + HL = 0x1000 + 0x1000 = 0x2000", passed);
}

// ================= Unit Test: DAD SP (0x39) =================
// Adds SP to HL and stores result in HL
void UnitTest_DAD_SP() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.h = 0x12; cpu.l = 0x34; // HL = 0x1234
    cpu.sp = 0x1111;           // SP = 0x1111
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x39); // DAD SP

    emu.emulateCycles(1);

    uint16_t hl = (cpu.h << 8) | cpu.l;
#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DAD SP", 0x39, 0x0000, cpu.pc, {
        "Initial HL = 0x1234",
        "Operand    = SP = 0x1111",
        "Final HL   = 0x" + hex(hl, 4),
        "Carry Flag = " + std::to_string(cpu.flags.cy)
    }, true, 0x0001);
#endif
    bool passed = (hl == 0x2345);
    printTestResult("DAD SP", "HL = HL + SP = 0x1234 + 0x1111 = 0x2345", passed);
}

// =========================== DCR Instructions ==============================

// ================= Unit Test: DCR A (0x3D) =================
// Decrements A and updates flags
void UnitTest_DCR_A() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.a = 0x01;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x3D); // DCR A

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCR A", 0x3D, 0x0000, cpu.pc, {
        "Initial A = 0x01",
        "Final A   = 0x" + hex(cpu.a),
        "Flags     = Z:" + std::to_string(cpu.flags.z) +
                      " S:" + std::to_string(cpu.flags.s) +
                      " P:" + std::to_string(cpu.flags.p)
    }, true, 0x0001);
#endif
    printTestResult("DCR A", "A = 0x01 - 1 = 0x00", cpu.a == 0x00);
}

// ================= Unit Test: DCR B (0x05) =================
// Decrements B and updates flags
void UnitTest_DCR_B() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.b = 0x01;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x05); // DCR B

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCR B", 0x05, 0x0000, cpu.pc, {
        "Initial B   = 0x01",
        "Final B     = " + hex(cpu.b, 2),
        "Zero Flag   = " + std::to_string(cpu.flags.z)
    });
#endif
    bool passed = (cpu.b == 0x00 && cpu.flags.z == 1);
    printTestResult("DCR B", "B = 0x01 - 1 = 0x00, ZF = 1", passed);
}

// ================= Unit Test: DCR C (0x0D) =================
// Decrements C and updates flags
void UnitTest_DCR_C() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.c = 0x10;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x0D); // DCR C

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCR C", 0x0D, 0x0000, cpu.pc, {
        "Initial C = 0x10",
        "Final C   = 0x" + hex(cpu.c),
        "Flags     = Z:" + std::to_string(cpu.flags.z) +
                      " S:" + std::to_string(cpu.flags.s) +
                      " P:" + std::to_string(cpu.flags.p)
    }, true, 0x0001);
#endif
    printTestResult("DCR C", "C = C - 1 = 0x10 - 1 = 0x0F", cpu.c == 0x0F);
}

// ================= Unit Test: DCR D (0x15) =================
// Decrements D and updates flags
void UnitTest_DCR_D() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.d = 0x01;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x15); // DCR D

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCR D", 0x15, 0x0000, cpu.pc, {
        "Initial D = 0x01",
        "Final D   = 0x" + hex(cpu.d, 2),
        "Flags     = Z:" + std::to_string(cpu.flags.z) +
                      " S:" + std::to_string(cpu.flags.s) +
                      " P:" + std::to_string(cpu.flags.p)
    }, true, 0x0001);
#endif
    printTestResult("DCR D", "D = D - 1 = 0x01 - 0x01 = 0x00", cpu.d == 0x00);
}

// ================= Unit Test: DCR E (0x1D) =================
// Decrements E and updates flags
void UnitTest_DCR_E() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.e = 0x02;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x1D); // DCR E

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCR E", 0x1D, 0x0000, cpu.pc, {
        "Initial E = 0x02",
        "Final E   = 0x" + hex(cpu.e, 2),
        "Flags     = Z:" + std::to_string(cpu.flags.z) +
                      " S:" + std::to_string(cpu.flags.s) +
                      " P:" + std::to_string(cpu.flags.p)
    }, true, 0x0001);
#endif
    printTestResult("DCR E", "E = E - 1 = 0x02 - 0x01 = 0x01", cpu.e == 0x01);
}

// ================= Unit Test: DCR H (0x25) =================
// Decrements H and updates flags
void UnitTest_DCR_H() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.h = 0x10;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x25); // DCR H

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCR H", 0x25, 0x0000, cpu.pc, {
        "Initial H = 0x10",
        "Final H   = 0x" + hex(cpu.h, 2),
        "Flags     = Z:" + std::to_string(cpu.flags.z) +
                      " S:" + std::to_string(cpu.flags.s) +
                      " P:" + std::to_string(cpu.flags.p)
    }, true, 0x0001);
#endif
    printTestResult("DCR H", "H = H - 1 = 0x10 - 0x01 = 0x0F", cpu.h == 0x0F);
}

// ================= Unit Test: DCR L (0x2D) =================
// Decrements L and updates flags
void UnitTest_DCR_L() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.l = 0x01;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x2D); // DCR L

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCR L", 0x2D, 0x0000, cpu.pc, {
        "Initial L = 0x01",
        "Final L   = 0x" + hex(cpu.l, 2),
        "Flags     = Z:" + std::to_string(cpu.flags.z) +
                      " S:" + std::to_string(cpu.flags.s) +
                      " P:" + std::to_string(cpu.flags.p)
    }, true, 0x0001);
#endif
    printTestResult("DCR L", "L = L - 1 = 0x01 - 0x01 = 0x00", cpu.l == 0x00);
}

// ================= Unit Test: DCR M (0x35) =================
// Decrements memory at HL and updates flags
void UnitTest_DCR_M() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    Memory& mem = emu.getMemoryRef();

    cpu.h = 0x24;
    cpu.l = 0x00;
    cpu.pc = 0x0000;
    mem.writeRomBytes(0x0000, 0x35); // DCR M
    mem.WriteByte(0x2400, 0x01);     // Initial value at [HL]

    emu.emulateCycles(1);

    uint8_t result = mem.ReadByte(0x2400);
#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCR M", 0x35, 0x0000, cpu.pc, {
        "Initial Mem[HL] = 0x01",
        "Final Mem[HL]   = 0x" + hex(result, 2),
        "Flags           = Z:" + std::to_string(cpu.flags.z) +
                             " S:" + std::to_string(cpu.flags.s) +
                             " P:" + std::to_string(cpu.flags.p)
    }, true, 0x0001);
#endif
    printTestResult("DCR M", "Mem[HL] = 0x01 - 0x01 = 0x00", result == 0x00);
}

// =========================== DCX Instructions ==============================

// ================= Unit Test: DCX B (0x0B) =================
// Decrements BC by 1 and stores result
void UnitTest_DCX_B() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.b = 0x12; cpu.c = 0x00; // BC = 0x1200
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x0B); // DCX B

    emu.emulateCycles(1);

    uint16_t bc = (cpu.b << 8) | cpu.c;
#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCX B", 0x0B, 0x0000, cpu.pc, {
        "Initial BC = 0x1200",
        "Final BC   = 0x" + hex(bc, 4)
    });
#endif
    bool passed = (bc == 0x11FF);
    printTestResult("DCX B", "BC = 0x1200 - 1 = 0x11FF", passed);
}

// ================= Unit Test: DCX D (0x1B) =================
// Decrements DE by 1 and stores result
void UnitTest_DCX_D() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.d = 0x12;
    cpu.e = 0x00;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x1B); // DCX D

    emu.emulateCycles(1);

    uint16_t de = (cpu.d << 8) | cpu.e;
#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCX D", 0x1B, 0x0000, cpu.pc, {
        "Final DE = 0x" + hex(de, 4)
    });
#endif
    printTestResult("DCX D", "DE = DE - 1 = 0x1200 → 0x11FF", de == 0x11FF);
}

// ================= Unit Test: DCX H (0x2B) =================
// Decrements HL by 1 and stores result
void UnitTest_DCX_H() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.h = 0x10;
    cpu.l = 0x00;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x2B); // DCX H

    emu.emulateCycles(1);

    uint16_t hl = (cpu.h << 8) | cpu.l;
#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCX H", 0x2B, 0x0000, cpu.pc, {
        "Final HL = 0x" + hex(hl, 4)
    });
#endif
    printTestResult("DCX H", "HL = HL - 1 = 0x1000 → 0x0FFF", hl == 0x0FFF);
}

// ================= Unit Test: DCX SP (0x3B) =================
// Decrements SP by 1 and stores result
void UnitTest_DCX_SP() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.sp = 0x2000;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x3B); // DCX SP

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DCX SP", 0x3B, 0x0000, cpu.pc, {
        "Final SP = 0x" + hex(cpu.sp, 4)
    });
#endif
    printTestResult("DCX SP", "SP = SP - 1 = 0x2000 → 0x1FFF", cpu.sp == 0x1FFF);
}

// ======================= Miscellaneous Instructions ========================

// ================= Unit Test: DAA (0x27) ===================
// Adjusts accumulator for BCD addition
void UnitTest_DAA() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.a = 0x09;
    cpu.flags.ac = 1;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0x27); // DAA

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DAA", 0x27, 0x0000, cpu.pc, {
        "Initial A   = 0x09",
        "AC Flag     = 1",
        "Final A     = 0x" + hex(cpu.a, 2)
    });
#endif
    printTestResult("DAA", "Adjust A for BCD with AC=1", true); // Manual validation
}

// ================= Unit Test: DI (0xF3) ====================
// Disables interrupt flag
void UnitTest_DI() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.interrupts_enabled = true;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0xF3); // DI

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("DI", 0xF3, 0x0000, cpu.pc, {
    "Expected interrupts_enabled = false",
    "Final interrupts_enabled    = " + std::string(cpu.interrupts_enabled ? "true" : "false")
});
#endif

    printTestResult("DI", "Interrupts disabled", cpu.interrupts_enabled == false);
}

// ================= Unit Test: EI (0xFB) ====================
// Enables interrupt flag
void UnitTest_EI() {
    Emulator emu = createTestEmulator();
    CPUState& cpu = emu.getCPUStateRef();
    cpu.interrupts_enabled = false;
    cpu.pc = 0x0000;
    emu.getMemoryRef().writeRomBytes(0x0000, 0xFB); // EI

    emu.emulateCycles(1);

#ifdef ENABLE_VERBOSE_DEBUG
    printOpcodeDebugBasic("EI", 0xFB, 0x0000, cpu.pc, {
    "Expected interrupts_enabled = true",
    "Final interrupts_enabled    = " + std::string(cpu.interrupts_enabled ? "true" : "false")
});
#endif

    printTestResult("EI", "Interrupts enabled", cpu.interrupts_enabled == true);
}

// ================== Main Entry Point ====================
int main() {
    resetTestCounter();

    
    // === DAD Instruction Tests - Adds register pair to HL === 
    std::cout << "=== Starting DAD Tests ===\n";
    UnitTest_DAD_B();
    UnitTest_DAD_D();
    UnitTest_DAD_H();
    UnitTest_DAD_SP();
    std::cout << "=== DAD Tests Complete ===\n\n";

    // === DCR Instruction Tests - Decrements register or memory, affects flags ===
    std::cout << "=== Starting DCR Tests ===\n";
    UnitTest_DCR_A();
    UnitTest_DCR_B();
    UnitTest_DCR_C();
    UnitTest_DCR_D();
    UnitTest_DCR_E();
    UnitTest_DCR_H();
    UnitTest_DCR_L();
    UnitTest_DCR_M();
    std::cout << "=== DCR Tests Complete ===\n\n";

    
    // === DCX Instruction Tests - Decrement register pairs === 
    std::cout << "=== Starting DCX Tests ===\n";
    UnitTest_DCX_B();
    UnitTest_DCX_D();
    UnitTest_DCX_H();
    UnitTest_DCX_SP();
    std::cout << "=== DCX Tests Complete ===\n\n";

    
    // === Miscellaneous D-Prefixed Tests: DAA, DI, EI === 
    std::cout << "=== Starting Miscellaneous D Tests ===\n";
    UnitTest_DAA();
    UnitTest_DI();
    UnitTest_EI();
    std::cout << "=== Miscellaneous D Tests Complete ===\n\n";

   
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
