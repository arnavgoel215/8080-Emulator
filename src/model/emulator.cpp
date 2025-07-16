/**********************************************************
 * @file emulator.cpp
 * 
 * @brief Implementation of the 8080 emulator core.
 * 
 * @author: <TODO: ADD AUTHOR>
 * 
 *********************************************************/

/***************** Include files. ***********************/
#include "emulator.hpp"
#include <iostream>
#include <algorithm> // For std::copy

/***************** Macros and defines. ***********************/
constexpr size_t MEMORY_SIZE = 0x10000; // 64KB memory for the 8080

/***************** Namespaces. ***********************/

/***************** Local Classes. ***********************/

/***************** Local Functions. ***********************/

/***************** Global Class Functions. ***********************/

/**
 * @brief Default constructor. Initializes the CPU state and memory.
 */
Emulator::Emulator()
{
    memory.resize(MEMORY_SIZE);
    reset();
}

void Emulator::loadROM(const std::vector<uint8_t>& romData)
{
    // Simple ROM loading starting at address 0x0000
    std::copy(romData.begin(), romData.end(), memory.begin());
}

void Emulator::reset()
{
    state = {}; // Zero-initialize all registers and flags
    state.pc = 0x0000; // Start execution from the beginning of memory
    state.sp = 0x0000;
    std::fill(memory.begin(), memory.end(), 0);
}

void Emulator::emulateCycles(int cycles)
{
    // This is a simplified cycle loop. A real implementation would
    // decrement cycles based on the cost of each instruction.
    // For now, we'll treat it as "number of instructions to execute".
    for (int i = 0; i < cycles; ++i)
    {
        if (state.pc >= memory.size())
        {
            // Prevent execution from running off the end of memory
            break; 
        }
        executeInstruction();
    }
}

void Emulator::executeInstruction()
{
    uint8_t opcode = memory[state.pc];
    // PC is incremented in the opcode function

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
void Emulator::op_LXI_B() { state.pc += 3; /* TODO */ }
// 0x02: STAX B
void Emulator::op_STAX_B() { state.pc += 1; /* TODO */ }
// 0x06: MVI B,d8
void Emulator::op_MVI_B() { state.pc += 2; /* TODO */ }
// 0x0A: LDAX B
void Emulator::op_LDAX_B() { state.pc += 1; /* TODO */ }
// 0x0E: MVI C,d8
void Emulator::op_MVI_C() { state.pc += 2; /* TODO */ }
// 0x11: LXI D,d16
void Emulator::op_LXI_D() { state.pc += 3; /* TODO */ }
// 0x12: STAX D
void Emulator::op_STAX_D() { state.pc += 1; /* TODO */ }
// 0x16: MVI D,d8
void Emulator::op_MVI_D() { state.pc += 2; /* TODO */ }
// 0x1A: LDAX D
void Emulator::op_LDAX_D() { state.pc += 1; /* TODO */ }
// 0x1E: MVI E,d8
void Emulator::op_MVI_E() { state.pc += 2; /* TODO */ }
// 0x21: LXI H,d16
void Emulator::op_LXI_H() { state.pc += 3; /* TODO */ }
// 0x22: SHLD addr
void Emulator::op_SHLD() { state.pc += 3; /* TODO */ }
// 0x26: MVI H,d8
void Emulator::op_MVI_H() { state.pc += 2; /* TODO */ }
// 0x2A: LHLD addr
void Emulator::op_LHLD() { state.pc += 3; /* TODO */ }
// 0x2E: MVI L,d8
void Emulator::op_MVI_L() { state.pc += 2; /* TODO */ }
// 0x31: LXI SP,d16
void Emulator::op_LXI_SP() { state.pc += 3; /* TODO */ }
// 0x32: STA addr
void Emulator::op_STA() { state.pc += 3; /* TODO */ }
// 0x36: MVI M,d8
void Emulator::op_MVI_M() { state.pc += 2; /* TODO */ }
// 0x3A: LDA addr
void Emulator::op_LDA() { state.pc += 3; /* TODO */ }
// 0x3E: MVI A,d8
void Emulator::op_MVI_A() { state.pc += 2; /* TODO */ }
// 0xEB: XCHG
void Emulator::op_XCHG() { state.pc += 1; /* TODO */ }

// Arithmetic Group
// 0x03: INX B
void Emulator::op_INX_B() { state.pc += 1; /* TODO */ }
// 0x04: INR B
void Emulator::op_INR_B() { state.pc += 1; /* TODO */ }
// 0x05: DCR B
void Emulator::op_DCR_B() { state.pc += 1; /* TODO */ }
// 0x09: DAD B
void Emulator::op_DAD_B() { state.pc += 1; /* TODO */ }
// 0x0B: DCX B
void Emulator::op_DCX_B() { state.pc += 1; /* TODO */ }
// 0x0C: INR C
void Emulator::op_INR_C() { state.pc += 1; /* TODO */ }
// 0x0D: DCR C
void Emulator::op_DCR_C() { state.pc += 1; /* TODO */ }
// 0x13: INX D
void Emulator::op_INX_D() { state.pc += 1; /* TODO */ }
// 0x14: INR D
void Emulator::op_INR_D() { state.pc += 1; /* TODO */ }
// 0x15: DCR D
void Emulator::op_DCR_D() { state.pc += 1; /* TODO */ }
// 0x19: DAD D
void Emulator::op_DAD_D() { state.pc += 1; /* TODO */ }
// 0x1B: DCX D
void Emulator::op_DCX_D() { state.pc += 1; /* TODO */ }
// 0x1C: INR E
void Emulator::op_INR_E() { state.pc += 1; /* TODO */ }
// 0x1D: DCR E
void Emulator::op_DCR_E() { state.pc += 1; /* TODO */ }
// 0x23: INX H
void Emulator::op_INX_H() { state.pc += 1; /* TODO */ }
// 0x24: INR H
void Emulator::op_INR_H() { state.pc += 1; /* TODO */ }
// 0x25: DCR H
void Emulator::op_DCR_H() { state.pc += 1; /* TODO */ }
// 0x27: DAA
void Emulator::op_DAA() { state.pc += 1; /* TODO */ }
// 0x29: DAD H
void Emulator::op_DAD_H() { state.pc += 1; /* TODO */ }
// 0x2B: DCX H
void Emulator::op_DCX_H() { state.pc += 1; /* TODO */ }
// 0x2C: INR L
void Emulator::op_INR_L() { state.pc += 1; /* TODO */ }
// 0x34: INR M
void Emulator::op_INR_M() { state.pc += 1; /* TODO */ }
// 0x35: DCR M
void Emulator::op_DCR_M() { state.pc += 1; /* TODO */ }
// 0x39: DAD SP
void Emulator::op_DAD_SP() { state.pc += 1; /* TODO */ }
// 0x3C: INR A
void Emulator::op_INR_A() { state.pc += 1; /* TODO */ }
// 0x3D: DCR A
void Emulator::op_DCR_A() { state.pc += 1; /* TODO */ }

// Logical Group
// 0x07: RLC
void Emulator::op_RLC() { state.pc += 1; /* TODO */ }
// 0x0F: RRC
void Emulator::op_RRC() { state.pc += 1; /* TODO */ }
// 0x1F: RAR
void Emulator::op_RAR() { state.pc += 1; /* TODO */ }
// 0x2F: CMA
void Emulator::op_CMA() { state.pc += 1; /* TODO */ }
// 0x37: STC
void Emulator::op_STC() { state.pc += 1; /* TODO */ }
// 0x3F: CMC
void Emulator::op_CMC() { state.pc += 1; /* TODO */ }

// Branch Group
// 0xC3: JMP addr
void Emulator::op_JMP() { /* TODO */ }
// 0xC9: RET
void Emulator::op_RET() { /* TODO */ }
// 0xCD: CALL addr
void Emulator::op_CALL() { /* TODO */ }
// 0xE9: PCHL
void Emulator::op_PCHL() { state.pc += 1; /* TODO */ }

// Stack, I/O, and Machine Control Group
// 0x00: NOP
void Emulator::op_NOP() { state.pc += 1; }
// 0x76: HLT
void Emulator::op_HLT() { state.pc += 1; /* TODO */ }
// 0xC1: POP B
void Emulator::op_POP_B() { state.pc += 1; /* TODO */ }
// 0xC5: PUSH B
void Emulator::op_PUSH_B() { state.pc += 1; /* TODO */ }
// 0xD1: POP D
void Emulator::op_POP_D() { state.pc += 1; /* TODO */ }
// 0xD5: PUSH D
void Emulator::op_PUSH_D() { state.pc += 1; /* TODO */ }
// 0xE1: POP H
void Emulator::op_POP_H() { state.pc += 1; /* TODO */ }
// 0xE3: XTHL
void Emulator::op_XTHL() { state.pc += 1; /* TODO */ }
// 0xE5: PUSH H
void Emulator::op_PUSH_H() { state.pc += 1; /* TODO */ }
// 0xF1: POP PSW
void Emulator::op_POP_PSW() { state.pc += 1; /* TODO */ }
// 0xF5: PUSH PSW
void Emulator::op_PUSH_PSW() { state.pc += 1; /* TODO */ }
// 0xFB: EI
void Emulator::op_EI() { state.pc += 1; /* TODO */ }

void Emulator::requestInterrupt(uint8_t interrupt_num)
{
    // TODO: Implement interrupt handling
}


CPUState Emulator::getCPUState() const
{
    return state;
}

const uint8_t* Emulator::getFrameBuffer() const
{
    // The framebuffer for Space Invaders starts at 0x2400
    if (memory.size() >= 0x2400 + VRAM_SIZE)
    {
        return &memory[0x2400];
    }
    return nullptr;
}
