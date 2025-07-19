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
    state.pc++;

    switch (opcode)
    {
        case 0x00: // NOP (No Operation)
            // Does nothing but consume a cycle and advance the PC.
            // PC is already advanced, so we do nothing.
            break;
        case 0x80: op_ADD(state.b); break;  // ADD B
        case 0x81: op_ADD(state.c); break;  // ADD C
        case 0x82: op_ADD(state.d); break;  // ADD D
        case 0x83: op_ADD(state.e); break;  // ADD E
        case 0x84: op_ADD(state.h); break;  // ADD H
        case 0x85: op_ADD(state.l); break;  // ADD L
        case 0x86: op_ADD(memory[hl()]); break;  // ADD M
        case 0x87: op_ADD(state.a); break;  // ADD A
        case 0x88: op_ADC(state.b); break;  // ADC B
        case 0x89: op_ADC(state.c); break;  // ADC C
        case 0x8A: op_ADC(state.d); break;  // ADC D
        case 0x8B: op_ADC(state.e); break;  // ADC E
        case 0x8C: op_ADC(state.h); break;  // ADC H
        case 0x8D: op_ADC(state.l); break;  // ADC L
        case 0x8E: op_ADC(memory[hl()]); break;  // ADC M
        case 0x8F: op_ADC(state.a); break;  // ADC A
        case 0x90: op_SUB(state.b); break;  // SUB B
        case 0x91: op_SUB(state.c); break;  // SUB C
        case 0x92: op_SUB(state.d); break;  // SUB D
        case 0x93: op_SUB(state.e); break;  // SUB E
        case 0x94: op_SUB(state.h); break;  // SUB H
        case 0x95: op_SUB(state.h); break;  // SUB L
        case 0x96: op_SUB(memory[hl()]); break;  // SUB M
        case 0x97: op_SUB(state.a); break;  // SUB A
        case 0x98: op_SBB(state.b); break;  // SBB B
        case 0x99: op_SBB(state.c); break;  // SBB C
        case 0x9A: op_SBB(state.d); break;  // SBB D
        case 0x9B: op_SBB(state.e); break;  // SBB E
        case 0x9C: op_SBB(state.h); break;  // SBB H
        case 0x9D: op_SBB(state.l); break;  // SBB L
        case 0x9E: op_SBB(memory[hl()]); break;  // SBB M
        case 0x9F: op_SBB(state.a); break;  // SBB A
        case 0xA0: op_ANA(state.b); break;  // ANA B
        case 0xA1: op_ANA(state.c); break;  // ANA C
        case 0xA2: op_ANA(state.d); break;  // ANA D
        case 0xA3: op_ANA(state.e); break;  // ANA E
        case 0xA4: op_ANA(state.h); break;  // ANA H
        case 0xA5: op_ANA(state.l); break;  // ANA L
        case 0xA6: op_ANA(memory[hl()]); break;  // ANA M
        case 0xA7: op_ANA(state.a); break;  // ANA A
        case 0xA8: op_XRA(state.b); break;  // XRA B
        case 0xA9: op_XRA(state.c); break;  // XRA C
        case 0xAA: op_XRA(state.d); break;  // XRA D
        case 0xAB: op_XRA(state.e); break;  // XRA E
        case 0xAC: op_XRA(state.h); break;  // XRA H
        case 0xAD: op_XRA(state.l); break;  // XRA L
        case 0xAE: op_XRA(memory[hl()]); break;  // XRA M
        case 0xAF: op_XRA(state.a); break;  // XRA A
        case 0xB0: op_ORA(state.b); break;  // ORA B
        case 0xB1: op_ORA(state.c); break;  // ORA C
        case 0xB2: op_ORA(state.d); break;  // ORA D
        case 0xB3: op_ORA(state.e); break;  // ORA E
        case 0xB4: op_ORA(state.h); break;  // ORA H
        case 0xB5: op_ORA(state.l); break;  // ORA L
        case 0xB6: op_ORA(memory[hl()]); break;  // ORA M
        case 0xB7: op_ORA(state.a); break;  // ORA A

        default:
            std::cerr << "Error: Unimplemented opcode " 
                      << std::hex << static_cast<int>(opcode) 
                      << " at address " << std::hex << (state.pc - 1) 
                      << std::endl;
            // In a real scenario, we might want to halt or handle this error differently.
            break;
    }
}

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

//  98 to 9F SBB
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