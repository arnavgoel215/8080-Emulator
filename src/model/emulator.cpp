/**********************************************************
 * @file emulator.cpp
 * 
 * @brief Implementation of the 8080 emulator core.
 * 
 * @author: Jesse, Arnav
 * 
 *********************************************************/

/***************** Include files. ***********************/
#include "emulator.hpp"
#include <iostream>
#include <algorithm> // For std::copy
#include "memory.hpp"
#include "romloader.hpp"

/***************** Macros and defines. ***********************/


/***************** Namespaces. ***********************/

/***************** Local Classes. ***********************/

/***************** Local Functions. ***********************/

/***************** Global Class Functions. ***********************/

/**
 * @brief Default constructor. Initializes the CPU state and memory.
 */
Emulator::Emulator()
{
    reset();
}

void Emulator::reset()
{
    state = {}; // Zero-initialize all registers and flags
    state.pc = 0x0000; // Start execution from the beginning of memory
    state.sp = 0x0000;
    memory.Clear();
    LoadSpaceInvadersROM(memory);
}

void Emulator::emulateCycles(int cycles)
{
    // This is a simplified cycle loop. A real implementation would
    // decrement cycles based on the cost of each instruction.
    // For now, we'll treat it as "number of instructions to execute".
    for (int i = 0; i < cycles; ++i)
    {
        if (state.pc >= 0xFFFF)
        {
            // Prevent execution from running off the end of memory
            break; 
        }
        executeInstruction();
    }
}

void Emulator::executeInstruction()
{
     uint8_t opcode = memory.ReadByte(state.pc);
    // PC is incremented in the opcode function

    // Checks for MOV opcodes.
    if ((opcode & 0xC0) == 0x40 && opcode != 0x76) // 0x76 is the opcode for HLT and not a MOV opcode
    {
        RegisterCode dst = (RegisterCode)((opcode >> 3) & 0x07);  // Determines which register to set
        RegisterCode src = (RegisterCode)(opcode & 0x07);  // Determines which register to get value from
        set_reg(dst, get_reg(src));
        return;
    }

    // Switch statement for rest of the opcodes
    switch (opcode)
    {
        case 0x00: op_NOP(); break;
        case 0x01: op_LXI_B(); break;
        case 0x02: op_STAX_B(); break;
        case 0x03: op_INX_B(); break;
        case 0x04: op_INR_B(); break;
        case 0x05: op_DCR_B(); break;
        case 0x06: op_MVI_B(); break;
        case 0x07: op_RLC(); break;
        case 0x09: op_DAD_B(); break;
        case 0x0a: op_LDAX_B(); break;
        case 0x0b: op_DCX_B(); break;
        case 0x0c: op_INR_C(); break;
        case 0x0d: op_DCR_C(); break;
        case 0x0e: op_MVI_C(); break;
        case 0x0f: op_RRC(); break;
        case 0x11: op_LXI_D(); break;
        case 0x12: op_STAX_D(); break;
        case 0x13: op_INX_D(); break;
        case 0x14: op_INR_D(); break;
        case 0x15: op_DCR_D(); break;
        case 0x16: op_MVI_D(); break;
        case 0x19: op_DAD_D(); break;
        case 0x1a: op_LDAX_D(); break;
        case 0x1b: op_DCX_D(); break;
        case 0x1c: op_INR_E(); break;
        case 0x1d: op_DCR_E(); break;
        case 0x1e: op_MVI_E(); break;
        case 0x1f: op_RAR(); break;
        case 0x21: op_LXI_H(); break;
        case 0x22: op_SHLD(); break;
        case 0x23: op_INX_H(); break;
        case 0x24: op_INR_H(); break;
        case 0x25: op_DCR_H(); break;
        case 0x26: op_MVI_H(); break;
        case 0x27: op_DAA(); break;
        case 0x29: op_DAD_H(); break;
        case 0x2a: op_LHLD(); break;
        case 0x2b: op_DCX_H(); break;
        case 0x2c: op_INR_L(); break;
        case 0x2e: op_MVI_L(); break;
        case 0x2f: op_CMA(); break;
        case 0x31: op_LXI_SP(); break;
        case 0x32: op_STA(); break;
        case 0x34: op_INR_M(); break;
        case 0x35: op_DCR_M(); break;
        case 0x36: op_MVI_M(); break;
        case 0x37: op_STC(); break;
        case 0x39: op_DAD_SP(); break;
        case 0x3a: op_LDA(); break;
        case 0x3c: op_INR_A(); break;
        case 0x3d: op_DCR_A(); break;
        case 0x3e: op_MVI_A(); break;
        case 0x3f: op_CMC(); break;
        case 0x76: op_HLT(); break;
        case 0xc1: op_POP_B(); break;
        case 0xc3: op_JMP(); break;
        case 0xc5: op_PUSH_B(); break;
        case 0xc9: op_RET(); break;
        case 0xcd: op_CALL(); break;
        case 0xd1: op_POP_D(); break;
        case 0xd5: op_PUSH_D(); break;
        case 0xe1: op_POP_H(); break;
        case 0xe3: op_XTHL(); break;
        case 0xe5: op_PUSH_H(); break;
        case 0xe9: op_PCHL(); break;
        case 0xeb: op_XCHG(); break;
        case 0xf1: op_POP_PSW(); break;
        case 0xf5: op_PUSH_PSW(); break;
        case 0xfb: op_EI(); break;
        case 0x80: op_ADD(state.b); break;  // ADD B
        case 0x81: op_ADD(state.c); break;  // ADD C
        case 0x82: op_ADD(state.d); break;  // ADD D
        case 0x83: op_ADD(state.e); break;  // ADD E
        case 0x84: op_ADD(state.h); break;  // ADD H
        case 0x85: op_ADD(state.l); break;  // ADD L
        case 0x86: op_ADD(memory.ReadByte(hl())); break;   // ADD M
        case 0x87: op_ADD(state.a); break;  // ADD A
        case 0x88: op_ADC(state.b); break;  // ADC B
        case 0x89: op_ADC(state.c); break;  // ADC C
        case 0x8A: op_ADC(state.d); break;  // ADC D
        case 0x8B: op_ADC(state.e); break;  // ADC E
        case 0x8C: op_ADC(state.h); break;  // ADC H
        case 0x8D: op_ADC(state.l); break;  // ADC L
        case 0x8E: op_ADC(memory.ReadByte(hl())); break;   // ADC M
        case 0x8F: op_ADC(state.a); break;  // ADC A
        case 0x90: op_SUB(state.b); break;  // SUB B
        case 0x91: op_SUB(state.c); break;  // SUB C
        case 0x92: op_SUB(state.d); break;  // SUB D
        case 0x93: op_SUB(state.e); break;  // SUB E
        case 0x94: op_SUB(state.h); break;  // SUB H
        case 0x95: op_SUB(state.l); break;  // SUB L
        case 0x96: op_SUB(memory.ReadByte(hl())); break;  // SUB M
        case 0x97: op_SUB(state.a); break;  // SUB A
        case 0x98: op_SBB(state.b); break;  // SBB B
        case 0x99: op_SBB(state.c); break;  // SBB C
        case 0x9A: op_SBB(state.d); break;  // SBB D
        case 0x9B: op_SBB(state.e); break;  // SBB E
        case 0x9C: op_SBB(state.h); break;  // SBB H
        case 0x9D: op_SBB(state.l); break;  // SBB L
        case 0x9E: op_SBB(memory.ReadByte(hl())); break;  // SBB M
        case 0x9F: op_SBB(state.a); break;  // SBB A
        case 0xA0: op_ANA(state.b); break;  // ANA B
        case 0xA1: op_ANA(state.c); break;  // ANA C
        case 0xA2: op_ANA(state.d); break;  // ANA D
        case 0xA3: op_ANA(state.e); break;  // ANA E
        case 0xA4: op_ANA(state.h); break;  // ANA H
        case 0xA5: op_ANA(state.l); break;  // ANA L
        case 0xA6: op_ANA(memory.ReadByte(hl())); break;  // ANA M
        case 0xA7: op_ANA(state.a); break;  // ANA A
        case 0xA8: op_XRA(state.b); break;  // XRA B
        case 0xA9: op_XRA(state.c); break;  // XRA C
        case 0xAA: op_XRA(state.d); break;  // XRA D
        case 0xAB: op_XRA(state.e); break;  // XRA E
        case 0xAC: op_XRA(state.h); break;  // XRA H
        case 0xAD: op_XRA(state.l); break;  // XRA L
        case 0xAE: op_XRA(memory.ReadByte(hl())); break; // XRA M
        case 0xAF: op_XRA(state.a); break;  // XRA A
        case 0xB0: op_ORA(state.b); break;  // ORA B
        case 0xB1: op_ORA(state.c); break;  // ORA C
        case 0xB2: op_ORA(state.d); break;  // ORA D
        case 0xB3: op_ORA(state.e); break;  // ORA E
        case 0xB4: op_ORA(state.h); break;  // ORA H
        case 0xB5: op_ORA(state.l); break;  // ORA L
        case 0xB6: op_ORA(memory.ReadByte(hl())); break;  // ORA M
        case 0xB7: op_ORA(state.a); break;  // ORA A
        case 0xB8: op_CMP(state.b); break;  // CMP B
        case 0xB9: op_CMP(state.c); break;  // CMP C
        case 0xBA: op_CMP(state.d); break;  // CMP D
        case 0xBB: op_CMP(state.e); break;  // CMP E
        case 0xBC: op_CMP(state.h); break;  // CMP H
        case 0xBD: op_CMP(state.l); break;  // CMP L
        case 0xBE: op_CMP(memory.ReadByte(hl())); break;  // CMP M
        case 0xBF: op_CMP(state.a); break;  // CMP A
        case 0xC0: op_RET_cond(!state.flags.z); break;  // RNZ
        case 0xC2: op_JMP_cond(!state.flags.z); break;  // JNZ addr
        case 0xC4: op_CALL_cond(!state.flags.z); break;  // CNZ addr
        case 0xC6: op_ADD(memory.ReadByte(state.pc++)); break;  // ADI d8
        case 0xC8: op_RET_cond(state.flags.z); break;  // RZ
        case 0xCA: op_JMP_cond(state.flags.z); break;  // JZ addr
        case 0xCC: op_CALL_cond(state.flags.z); break; // CZ addr
        case 0xCE: op_ADD(memory.ReadByte(state.pc++)); break;  // ACI d8
        case 0xD0: op_RET_cond(!state.flags.cy); break;  // RNC
        case 0xD2: op_JMP_cond(!state.flags.cy); break;  // JNC addr
        case 0xD4: op_CALL_cond(!state.flags.cy); break;  // CNC addr
        case 0xD6: op_SUB(memory.ReadByte(state.pc++)); break;  // SUI d8
        case 0xD8: op_RET_cond(state.flags.cy); break;  // RC
        case 0xDA: op_RET_cond(state.flags.cy); break;  // JC addr
        case 0xDC: op_CALL_cond(state.flags.cy); break;  // CC addr
        case 0xDE: op_SBB(memory.ReadByte(state.pc++)); break;  // SBI d8
        case 0xE0: op_RET_cond(!state.flags.p); break;  // RPO
        case 0xE2: op_JMP_cond(!state.flags.p); break;  // JPO addr
        case 0xE4: op_CALL_cond(!state.flags.p); break;  // CPO addr
        case 0xE6: op_ANA(memory.ReadByte(state.pc++)); break; // ANI d8
        case 0xE8: op_RET_cond(state.flags.p); break;  // RPE
        case 0xEA: op_JMP_cond(state.flags.p); break;  // JPE addr
        case 0xEC: op_CALL_cond(state.flags.p); break;  // CPE addr
        case 0xEE: op_XRA(memory.ReadByte(state.pc++)); break; // XRI d8
        case 0xF0: op_RET_cond(!state.flags.s); break;  // RP
        case 0xF2: op_JMP_cond(!state.flags.s); break;  // JP addr
        case 0xF4: op_CALL_cond(!state.flags.s); break;  // CP addr
        case 0xF6: op_ORA(memory.ReadByte(state.pc++)); break; // ORI d8
        case 0xC7: op_RST(0); break;  // RST 0 | Call address 0x00
        case 0xCF: op_RST(1); break;  // RST 1 | Call address 0x08
        case 0xD7: op_RST(2); break;  // RST 2 | Call address 0x10
        case 0xDF: op_RST(3); break;  // RST 3 | Call address 0x18
        case 0xE7: op_RST(4); break;  // RST 4 | Call address 0x20
        case 0xEF: op_RST(5); break;  // RST 5 | Call address 0x28
        case 0xF7: op_RST(6); break;  // RST 6 | Call address 0x30
        case 0xFF: op_RST(7); break;  // RST 7 | Call address 0x38
        case 0xF9: op_SPHL(); break;  // SPHL: SP ← HL
        case 0xF8: op_RET_cond(state.flags.s); break;  // RM
        case 0xFA: op_JMP_cond(state.flags.s); break;  // JM addr
        case 0xFC: op_CALL_cond(state.flags.s); break;  // CM addr
        case 0xFE: op_CMP(memory.ReadByte(state.pc++)); break;  // CPI d8
        case 0xD3: op_OUT(); break;  // OUT d8
        case 0xDB: op_IN(); break;  // IN d8

        default:
            std::cerr << "Error: Unimplemented opcode " 
                      << std::hex << static_cast<int>(opcode) 
                      << " at address " << std::hex << (state.pc) 
                      << std::endl;
            state.pc++; // Advance past the unknown opcode
            break;
    }
}

// --- Opcode Implementations ---

// Data Transfer Group
// 0x01: LXI B,d16
void Emulator::op_LXI_B()
{ 
    state.c = memory.ReadByte(state.pc + 1);
    state.b = memory.ReadByte(state.pc + 2);
    state.pc += 3;
}
// 0x02: STAX B
void Emulator::op_STAX_B()
{
    uint16_t addr = (state.b << 8) | state.c;
    memory.WriteByte(addr, state.a);
    state.pc += 1;
}
// 0x06: MVI B,d8
void Emulator::op_MVI_B()
{
    state.b = memory.ReadByte(state.pc + 1);
    state.pc += 2;
}
// 0x0A: LDAX B
void Emulator::op_LDAX_B()
{
    uint16_t addr = (state.b << 8) | state.c;
    state.a = memory.ReadByte(addr);
    state.pc += 1;
}
// 0x0E: MVI C,d8
void Emulator::op_MVI_C()
{
    state.c = memory.ReadByte(state.pc + 1);
    state.pc += 2;
}
// 0x11: LXI D,d16
void Emulator::op_LXI_D()
{
    state.e = memory.ReadByte(state.pc + 1); 
    state.d = memory.ReadByte(state.pc + 2); 
    state.pc += 3;
}
// 0x12: STAX D
void Emulator::op_STAX_D()
{
    uint16_t addr = (state.d << 8) | state.e;
    memory.WriteByte(addr, state.a); 
    state.pc += 1;
}
// 0x16: MVI D,d8
void Emulator::op_MVI_D()
{
    state.d = memory.ReadByte(state.pc + 1);
    state.pc += 2;
}
// 0x1A: LDAX D
void Emulator::op_LDAX_D()
{
    uint16_t addr = (state.d << 8) | state.e;
    state.a = memory.ReadByte(addr); 
    state.pc += 1;
}
// 0x1E: MVI E,d8
void Emulator::op_MVI_E()
{
    state.e = memory.ReadByte(state.pc + 1);
    state.pc += 2;
}
// 0x21: LXI H,d16
void Emulator::op_LXI_H()
{
    state.l = memory.ReadByte(state.pc + 1); 
    state.h = memory.ReadByte(state.pc + 2);
    state.pc += 3;
}
// 0x22: SHLD addr
void Emulator::op_SHLD()
{
    uint16_t addr = (memory.ReadByte(state.pc + 2) << 8) | memory.ReadByte(state.pc + 1); 
    memory.WriteByte(addr, state.l);           
    memory.WriteByte(addr + 1, state.h);
    state.pc += 3;
}
// 0x26: MVI H,d8
void Emulator::op_MVI_H()
{
    state.h = memory.ReadByte(state.pc + 1);
    state.pc += 2;
}
// 0x2A: LHLD addr
void Emulator::op_LHLD()
{
    uint16_t addr = (memory.ReadByte(state.pc + 2) << 8) | memory.ReadByte(state.pc + 1); 
    state.l = memory.ReadByte(addr);              
    state.h = memory.ReadByte(addr + 1); 
    state.pc += 3;
}
// 0x2E: MVI L,d8
void Emulator::op_MVI_L()
{
    state.l = memory.ReadByte(state.pc + 1);
    state.pc += 2;
}
// 0x31: LXI SP,d16
void Emulator::op_LXI_SP()
{
    state.sp = (memory.ReadByte(state.pc + 2) << 8) | memory.ReadByte(state.pc + 1);
    state.pc += 3;
}
// 0x32: STA addr
void Emulator::op_STA()
{
    uint16_t addr = (memory.ReadByte(state.pc + 2) << 8) | memory.ReadByte(state.pc + 1); 
    memory.WriteByte(addr, state.a);
    state.pc += 3;
}
// 0x36: MVI M,d8
void Emulator::op_MVI_M()
{
    uint16_t addr = (state.h << 8) | state.l;
    memory.WriteByte(addr, memory.ReadByte(state.pc + 1)); 
    state.pc += 2;
}
// 0x3A: LDA addr
void Emulator::op_LDA()
{
    uint16_t addr = (memory.ReadByte(state.pc + 2) << 8) | memory.ReadByte(state.pc + 1); 
    state.a = memory.ReadByte(addr); 
    state.pc += 3;
}
// 0x3E: MVI A,d8
void Emulator::op_MVI_A()
{
    state.a = memory.ReadByte(state.pc + 1);
    state.pc += 2;
}
// 0xEB: XCHG
void Emulator::op_XCHG()
{
    std::swap(state.h, state.d);
    std::swap(state.l, state.e);
    state.pc += 1;
}

// Arithmetic Group
// 0x03: INX B
void Emulator::op_INX_B()
{
    uint16_t bc = (state.b << 8) | state.c;
    bc++;
    state.b = (bc >> 8) & 0xFF;
    state.c = bc & 0xFF;
    state.pc += 1;
}
// 0x04: INR B
void Emulator::op_INR_B()
{
    uint8_t result = state.b + 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.b & 0x0F) + 1) > 0x0F;
    state.b = result;
    state.pc += 1;
}
// 0x05: DCR B
void Emulator::op_DCR_B()
{
    uint8_t result = state.b - 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.b & 0x0F) - 1) < 0;
    state.b = result;
    state.pc += 1;
}
// 0x09: DAD B
void Emulator::op_DAD_B()
{
    uint16_t hl = (state.h << 8) | state.l;
    uint16_t bc = (state.b << 8) | state.c;
    uint32_t result = hl + bc;
    state.flags.cy = (result > 0xFFFF);
    hl = result & 0xFFFF;
    state.h = (hl >> 8) & 0xFF;
    state.l = hl & 0xFF;
    state.pc += 1;
}
// 0x0B: DCX B
void Emulator::op_DCX_B()
{
    uint16_t bc = (state.b << 8) | state.c;
    bc--;
    state.b = (bc >> 8) & 0xFF;
    state.c = bc & 0xFF;
    state.pc += 1;
}
// 0x0C: INR C
void Emulator::op_INR_C()
{
    uint8_t result = state.c + 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.c & 0x0F) + 1) > 0x0F;
    state.c = result;
    state.pc += 1;
}
// 0x0D: DCR C
void Emulator::op_DCR_C()
{
    uint8_t result = state.c - 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.c & 0x0F) - 1) < 0;
    state.c = result;
    state.pc += 1;
}
// 0x13: INX D
void Emulator::op_INX_D()
{
    uint16_t de = (state.d << 8) | state.e;
    de++;
    state.d = (de >> 8) & 0xFF;
    state.e = de & 0xFF;
    state.pc += 1;
}
// 0x14: INR D
void Emulator::op_INR_D()
{
    uint8_t result = state.d + 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.d & 0x0F) + 1) > 0x0F;
    state.d = result;
    state.pc += 1;
}
// 0x15: DCR D
void Emulator::op_DCR_D()
{
    uint8_t result = state.d - 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.d & 0x0F) - 1) < 0;
    state.d = result;
    state.pc += 1;
}
// 0x19: DAD D
void Emulator::op_DAD_D()
{
    uint16_t hl = (state.h << 8) | state.l;
    uint16_t de = (state.d << 8) | state.e;
    uint32_t result = hl + de;
    state.flags.cy = (result > 0xFFFF);
    hl = result & 0xFFFF;
    state.h = (hl >> 8) & 0xFF;
    state.l = hl & 0xFF;
    state.pc += 1;
}
// 0x1B: DCX D
void Emulator::op_DCX_D()
{
    uint16_t de = (state.d << 8) | state.e;
    de--;
    state.d = (de >> 8) & 0xFF;
    state.e = de & 0xFF;
    state.pc += 1;
}
// 0x1C: INR E
void Emulator::op_INR_E()
{
    uint8_t result = state.e + 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.e & 0x0F) + 1) > 0x0F;
    state.e = result;
    state.pc += 1;
}
// 0x1D: DCR E
void Emulator::op_DCR_E()
{
    uint8_t result = state.e - 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.e & 0x0F) - 1) < 0;
    state.e = result;
    state.pc += 1;
}
// 0x23: INX H
void Emulator::op_INX_H()
{
    uint16_t hl = (state.h << 8) | state.l;
    hl++;
    state.h = (hl >> 8) & 0xFF;
    state.l = hl & 0xFF;
    state.pc += 1;
}
// 0x24: INR H
void Emulator::op_INR_H()
{
    uint8_t result = state.h + 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.h & 0x0F) + 1) > 0x0F;
    state.h = result;
    state.pc += 1;
}
// 0x25: DCR H
void Emulator::op_DCR_H()
{
    uint8_t result = state.h - 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.h & 0x0F) - 1) < 0;
    state.h = result;
    state.pc += 1;
}
// 0x27: DAA
void Emulator::op_DAA()
{
    uint8_t lsb = state.a & 0x0F;
    uint8_t msb = state.a >> 4;
    uint8_t correction = 0;

    if (state.flags.ac || lsb > 9)
    {
        correction += 0x06;
    }

    if (state.flags.cy || msb > 9 || (msb >= 9 && lsb > 9))
    {
        correction += 0x60;
        state.flags.cy = true;
    }

    uint16_t result = state.a + correction;
    state.a = result & 0xFF;
    state.flags.z = (state.a == 0);
    state.flags.s = ((state.a & 0x80) != 0);
    state.flags.p = __builtin_parity(state.a);
    state.flags.ac = ((state.a & 0x0F) < (lsb & 0x0F));
    state.pc += 1;
}
// 0x29: DAD H
void Emulator::op_DAD_H()
{
    uint16_t hl = (state.h << 8) | state.l;
    uint32_t result = hl + hl;
    state.flags.cy = (result > 0xFFFF);
    hl = result & 0xFFFF;
    state.h = (hl >> 8) & 0xFF;
    state.l = hl & 0xFF;
    state.pc += 1;
}
// 0x2B: DCX H
void Emulator::op_DCX_H()
{
    uint16_t hl = (state.h << 8) | state.l;
    hl--;
    state.h = (hl >> 8) & 0xFF;
    state.l = hl & 0xFF;
    state.pc += 1;
}
// 0x2C: INR L
void Emulator::op_INR_L()
{
    uint8_t result = state.l + 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.l & 0x0F) + 1) > 0x0F;
    state.l = result;
    state.pc += 1;
}
// 0x34: INR M
void Emulator::op_INR_M()
{
    uint16_t addr = (state.h << 8) | state.l;
    uint8_t original = memory.ReadByte(addr); 
    uint8_t result = original + 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((original & 0x0F) + 1) > 0x0F;
    memory.WriteByte(addr, result); 
    state.pc += 1;
}
// 0x35: DCR M
void Emulator::op_DCR_M()
{
    uint16_t addr = (state.h << 8) | state.l;
    uint8_t original = memory.ReadByte(addr);
    uint8_t result = original - 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((original & 0x0F) - 1) < 0;
    memory.WriteByte (addr, result);
    state.pc += 1;
}
// 0x39: DAD SP
void Emulator::op_DAD_SP()
{
    uint16_t hl = (state.h << 8) | state.l;
    uint32_t result = hl + state.sp;
    state.flags.cy = (result > 0xFFFF);
    hl = result & 0xFFFF;
    state.h = (hl >> 8) & 0xFF;
    state.l = hl & 0xFF;
    state.pc += 1;
}
// 0x3C: INR A
void Emulator::op_INR_A()
{
    uint8_t result = state.a + 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.a & 0x0F) + 1) > 0x0F;
    state.a = result;
    state.pc += 1;
}
// 0x3D: DCR A
void Emulator::op_DCR_A()
{
    uint8_t result = state.a - 1;
    state.flags.z = (result == 0);
    state.flags.s = ((result & 0x80) != 0);
    state.flags.p = __builtin_parity(result);
    state.flags.ac = ((state.a & 0x0F) - 1) < 0;
    state.a = result;
    state.pc += 1;
}
// 80 to 87: ADD
void Emulator::op_ADD(uint8_t val)
{
    // Pointers to register A and to flags CY and AC
    uint8_t *a = &state.a;  // Register A
    bool *cy = &state.flags.cy;  // Carry flag
    bool *ac = &state.flags.ac;  // Auxiliary carry flag

    uint16_t result = *a + val;
    *cy = result > 0xFF;
    *ac = ((*a & 0x0F) + (val & 0x0F) + (*cy ? 1 : 0)) > 0x0F;
    *a = result & 0xFF;  // Ensures that new value fits within 8-bits

    setFlags(*a);
}
// 88 to 8F: ADC
void Emulator::op_ADC(uint8_t val)
{
    // Pointers to register A and to flags CY and AC
    uint8_t *a = &state.a;  // Register A
    bool *cy = &state.flags.cy;  // Carry flag
    bool *ac = &state.flags.ac;  // Auxiliary flag

    uint16_t result = *a + val + (*cy ? 1 : 0);
    *cy = result > 0xFF;
    *ac = ((*a & 0x0F) + (val & 0x0F) + (*cy ? 1 : 0)) > 0x0F;
    *a = result & 0xFF;  // Ensures that new value fits within 8-bits

    setFlags(*a);
}
// 90 to 97 SUB
void Emulator::op_SUB(uint8_t val) 
{
    // Pointers to register A and to flags CY and AC
    uint8_t *a = &state.a;  // Register A
    bool *cy = &state.flags.cy;  // Carry flag
    bool *ac = &state.flags.ac;  // Auxiliary carry flag

    uint16_t result = *a - val;
    *cy = (*a < val);
    *ac = ((*a & 0x0F) < (val & 0x0F));
    *a = result & 0xFF;  // Ensures that new value fits within 8-bits

    setFlags(*a);
}
// 98 to 9F SBB
void Emulator::op_SBB(uint8_t val)
{
    // Pointers to register A and to flags CY and AC
    uint8_t *a = &state.a;  // Register A
    bool *cy = &state.flags.cy;  // Carry flag
    bool *ac = &state.flags.ac;  // Auxiliary carry flag

    uint16_t borrow = *cy ? 1 : 0;  // Sets the borrow value based on status of the carry flag
    uint16_t result = *a - val - borrow;

    *cy = (*a < (val + borrow));
    *ac = ((*a & 0x0F) < ((val & 0x0F) + borrow));

    *a = result & 0xFF;  // Ensures that new value fits within 8-bits
    setFlags(*a);
}
// Logical Group
// 0x07: RLC
void Emulator::op_RLC()
{
    state.flags.cy = (state.a & 0x80) != 0;
    state.a = (state.a << 1) | (state.a >> 7);
    state.pc += 1;
}
// 0x0F: RRC
void Emulator::op_RRC()
{
    state.flags.cy = (state.a & 0x01) != 0;
    state.a = (state.a >> 1) | (state.a << 7);
    state.pc += 1;
}
// 0x1F: RAR
void Emulator::op_RAR()
{
    bool old_cy = state.flags.cy;
    state.flags.cy = (state.a & 0x01) != 0;
    state.a = (state.a >> 1) | (old_cy ? 0x80 : 0x00);
    state.pc += 1;
}
// 0x2F: CMA
void Emulator::op_CMA()
{
    state.a = ~state.a;
    state.pc += 1;
}
// 0x37: STC
void Emulator::op_STC()
{
    state.flags.cy = true;
    state.pc += 1;
}
// 0x3F: CMC
void Emulator::op_CMC()
{
    state.flags.cy = !state.flags.cy;
    state.pc += 1;
}
// A0 to A7 ANA
void Emulator::op_ANA(uint8_t val)
{
    state.a = state.a & val;  // AND operation
    state.flags.cy = 0;
    state.flags.ac = 1;  // AC flag is always set to 1 during ANA instruction
    setFlags(state.a);
}
// A8 to AF XRA
void Emulator::op_XRA(uint8_t val)
{
    state.a = state.a ^ val;  // XOR operation
    state.flags.cy = 0;
    state.flags.ac = 0;
    setFlags(state.a);
}
// B0 to B7 ORA
void Emulator::op_ORA(uint8_t val)
{
    state.a = state.a | val;  // OR operation
    state.flags.cy = 0;
    state.flags.ac = 0;
    setFlags(state.a);
}
// B8 to B12 CMP
void Emulator::op_CMP(uint8_t val)
{
    uint8_t *a = &state.a;
    bool *cy = &state.flags.cy;
    bool *ac = &state.flags.ac;

    uint16_t result = *a - val;
    *cy = (*a < val);
    *ac = ((*a & 0x0F) < (val & 0x0F));

    setFlags(result & 0xFF);
}

// Branch Group
// 0xC3: JMP addr
void Emulator::op_JMP()
{
    state.pc = (memory.ReadByte(state.sp + 1) << 8) | memory.ReadByte(state.sp);
}
// 0xC9: RET
void Emulator::op_RET()
{
    state.pc = (memory.ReadByte(state.sp + 1) << 8) | memory.ReadByte(state.sp);
    state.sp += 2;
}
// 0xCD: CALL addr
void Emulator::op_CALL()
{
    uint16_t ret_addr = state.pc + 3;
    memory.WriteByte(state.sp - 1, (ret_addr >> 8) & 0xFF);
    memory.WriteByte(state.sp - 2, ret_addr & 0xFF); 
    state.sp -= 2;
    state.pc = (memory.ReadByte(state.pc + 2) << 8) | memory.ReadByte(state.pc + 1);
}
// 0xE9: PCHL
void Emulator::op_PCHL()
{
    state.pc = (state.h << 8) | state.l;
}
// Handles return conditionals
void Emulator::op_RET_cond(bool condition)
{
    if (condition) 
        op_RET();
}
// Handles jump conditionals
void Emulator::op_JMP_cond(bool condition)
{
     uint16_t addr = memory.ReadByte(state.pc) | (memory.ReadByte(state.pc + 1) << 8);
    if (condition)
        state.pc = addr;
    else
        state.pc += 2;
}
// Handles call conditionals
void Emulator::op_CALL_cond(bool condition)
{
    uint16_t addr = memory.ReadByte(state.pc) | (memory.ReadByte(state.pc + 1) << 8); // replaced
    if (condition)
    {
        state.pc += 2;
        memory.WriteByte(--state.sp, (state.pc >> 8) & 0xFF);
        memory.WriteByte(--state.sp, state.pc & 0xFF);  
        state.pc = addr;
    }
    else
    {
        state.pc += 2;
    }
}

// Stack, I/O, and Machine Control Group
// 0x00: NOP
void Emulator::op_NOP() { state.pc += 1; }
// 0x76: HLT
void Emulator::op_HLT()
{
    // For this emulator, HLT is a NOP
    state.pc += 1;
}
// 0xC1: POP B
void Emulator::op_POP_B()
{
    state.c = memory.ReadByte(state.sp);       
    state.b = memory.ReadByte(state.sp + 1); 
    state.sp += 2;
    state.pc += 1;
}
// 0xC5: PUSH B
void Emulator::op_PUSH_B()
{
    memory.WriteByte(state.sp - 1, state.b); 
    memory.WriteByte(state.sp - 2, state.c); 
    state.sp -= 2;
    state.pc += 1;
}
// 0xD1: POP D
void Emulator::op_POP_D()
{
    state.e = memory.ReadByte(state.sp);         
    state.d = memory.ReadByte(state.sp + 1);
    state.sp += 2;
    state.pc += 1;
}
// 0xD5: PUSH D
void Emulator::op_PUSH_D()
{
    memory.WriteByte(state.sp - 1, state.d);  
    memory.WriteByte(state.sp - 2, state.e);  
    state.sp -= 2;
    state.pc += 1;
}
// 0xE1: POP H
void Emulator::op_POP_H()
{
    state.l = memory.ReadByte(state.sp);         
    state.h = memory.ReadByte(state.sp + 1); 
    state.sp += 2;
    state.pc += 1;
}

// =========================== Opcode: XTHL ================================
// Opcode      : 0xE3
// Mnemonic    : XTHL
// 
// Exchange the contents of register pair HL with the word at
// the top of the stack (i.e., memory at address SP and SP+1).
// L <-> [SP] | H <-> [SP+1]
// This is a direct, in-place swap with no flags affected.
void Emulator::op_XTHL() {
    // Save stack contents before overwriting
    uint8_t temp_l = memory.ReadByte(state.sp);       
    uint8_t temp_h = memory.ReadByte(state.sp + 1);   

    // Write HL into stack
    memory.WriteByte(state.sp,     state.l);          
    memory.WriteByte(state.sp + 1, state.h);

    // Load original stack contents into HL
    state.l = temp_l;
    state.h = temp_h;

    // Advance PC
    state.pc += 1;
}


// ========================== Opcode: PUSH H ================================
// Opcode      : 0xE5
// Mnemonic    : PUSH H
// Push the contents of register pair HL onto the stack.
// Stack grows downward, so SP is decremented by 2 before writing.
// L is written to SP, H to SP+1. Program counter is advanced.
//
// Stack Effect: SP = SP - 2 || [SP]   = L || [SP+1] = H

void Emulator::op_PUSH_H() {
    // Decrement SP before writing
    state.sp -= 2;

    // Write HL into stack (low at SP, high at SP+1)
    memory.WriteByte(state.sp,     state.l);  
    memory.WriteByte(state.sp + 1, state.h);

    // Advance program counter
    state.pc += 1;
}


// 0xF1: POP PSW
void Emulator::op_POP_PSW()
{
    uint8_t psw = memory.ReadByte(state.sp);
    state.flags.cy = (psw & 0x01) != 0;
    state.flags.p = (psw & 0x04) != 0;
    state.flags.ac = (psw & 0x10) != 0;
    state.flags.z = (psw & 0x40) != 0;
    state.flags.s = (psw & 0x80) != 0;
    state.a = memory.ReadByte(state.sp + 1); 
    state.sp += 2;
    state.pc += 1;
}
// 0xF5: PUSH PSW
void Emulator::op_PUSH_PSW()
{
    memory.WriteByte(state.sp - 1, state.a);            
    uint8_t psw = (state.flags.s << 7) | (state.flags.z << 6) |
                  (state.flags.ac << 4) | (state.flags.p << 2) |
                  (state.flags.cy << 0) | 0x02;
    memory.WriteByte(state.sp - 2, psw); 
    state.sp -= 2;
    state.pc += 1;
}
// 0xFB: EI
void Emulator::op_EI()
{
    state.interrupts_enabled = true;
    state.pc += 1;
}
// 0xDB: IN d8
void Emulator::op_IN()
{
    uint8_t port = memory.ReadByte(state.pc++);
    io_read(port);
}
// 0xD3: OUT d8 
void Emulator::op_OUT()
{
    uint8_t port = memory.ReadByte(state.pc++); 
    state.a = io_write(port, state.a);
}

// ====================== Opcode: Stack ==========================

// ====================== Opcode: RST =============================
// (0)0xC7 = 0x0 | (1)0xCF = 0x08 | (2)0xD7 = 0x10 | (3)0xDF = 0x18 
// (4)0xE7 = 0x20 | (5)0xEF = 0x28 | (6)0x F7 = 0x30 | (7)0xFF = 0x38 
// Simulates a restart instruction by pushing the return address 
// (PC + 1) onto the stack and jumping to a hardcoded address n * 8
// n - The restart number (0 to 7), corresponding to address 0x00 to 0x38
// SP is decremented by 2, and the return address is stored on stack.
// Return address is the instruction following the RST call
void Emulator::op_RST(int n) {
    uint16_t returnAddr = state.pc + 1;  
    state.sp -= 1;
    memory.WriteByte(state.sp, (returnAddr >> 8) & 0xFF);
    state.sp -= 1;
    memory.WriteByte(state.sp, returnAddr & 0xFF);
    state.pc = n * 8;
}

// ====================== Opcode: SPHL (0xF9) ==========================
// Simulates the SPHL instruction (Opcode 0xF9)
// SPHL copies the contents of the HL register pair into the Stack Pointer (SP).
// No flags are affected. HL remains unchanged.
// This is typically used to transfer a calculated memory address into SP
// for stack operations or function calls.
// 
// SP ← HL
void Emulator::op_SPHL() {
    state.sp = (static_cast<uint16_t>(state.h) << 8) | state.l;
}





void Emulator::requestInterrupt(uint8_t interrupt_num)
{
    if (state.interrupts_enabled)
    {
        // Push PC onto stack
        memory.WriteByte(state.sp - 1, (state.pc >> 8) & 0xFF); 
        memory.WriteByte(state.sp - 2, state.pc & 0xFF);        

        state.sp -= 2;

        // Jump to interrupt service routine
        state.pc = 8 * interrupt_num;

        // Disable interrupts
        state.interrupts_enabled = false;
    }
}


CPUState Emulator::getCPUState() const
{
    return state;
}

const uint8_t* Emulator::getFrameBuffer() const
{
    // The framebuffer for Space Invaders starts at 0x2400
    return memory.GetVRAMPointer();
}

void Emulator::setFlags(uint8_t result)
{
    // Flags Z, S and P get set based on final result of operation
    state.flags.z = (result == 0);
    state.flags.s = (result & 0x80);
    state.flags.p = __builtin_parity(result);
}

uint16_t Emulator::hl() const
{
    return (state.h << 8) | state.l;
}

uint8_t Emulator::io_read(uint8_t port)
{
    switch (port)
    {
        case 0:  // IN0: coin and start buttons
            return state.port_in_0;
        case 1:  // IN1: player 1 controls
            return state.port_in_1;
        case 2:  // IN2: player 2 controls
            return state.port_in_2;
        case 3:  // IN3: shift register result
            return (state.shift_register >> (8 - state.shift_offset)) & 0xFF;
        default:
            return 0;
    }
}

uint8_t Emulator::io_write(uint8_t port, uint8_t val)
{
    switch (port)
    {
        case 2:  // OUT2: shift register offset
            state.shift_offset = val & 0x07;  // only lower 3 bits used
            break;
        case 4:  // OUT4: shift register data
            state.shift_register = (state.shift_register >> 8) | (val << 8);
            break;
        case 3:  // OUT3: sound control (not fully implemented here)
            std::cout << "Sound control (OUT 3) write: " << std::hex << (int)val << "\n";
            break;
        case 5:  // OUT5: sound control 2
            std::cout << "Sound control (OUT 5) write: " << std::hex << (int)val << "\n";
            break;
        default:
            std::cout << "Unknown OUT port " << std::hex << (int)port << ": " << (int)val << "\n";
            break;
    }
    return 0;
}

uint8_t Emulator::get_reg(uint8_t code)
{
    switch (code) 
    {
        case REG_B: return state.b;
        case REG_C: return state.c;
        case REG_D: return state.d;
        case REG_E: return state.e;
        case REG_H: return state.h;
        case REG_L: return state.l;
        case REG_M: return memory.ReadByte(hl());
        case REG_A: return state.a;
        default: return 0;
    }
}

void Emulator::set_reg(uint8_t code, uint8_t val)
{
    switch (code)
    {
        case REG_B: state.b = val; break;
        case REG_C: state.c = val; break;
        case REG_D: state.d = val; break;
        case REG_E: state.e = val; break;
        case REG_H: state.h = val; break;
        case REG_L: state.l = val; break;
        case REG_M: memory.WriteByte(hl(), val);
        case REG_A: state.a = val; break;
    }
}