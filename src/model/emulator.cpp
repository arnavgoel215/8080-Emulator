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

        // TODO: Add other opcodes here

        default:
            std::cerr << "Error: Unimplemented opcode " 
                      << std::hex << static_cast<int>(opcode) 
                      << " at address " << std::hex << (state.pc - 1) 
                      << std::endl;
            // In a real scenario, we might want to halt or handle this error differently.
            break;
    }
}

void Emulator::add(uint8_t val)
{
    uint8_t *a = &state.a;
    bool *cy = &state.flags.cy;
    bool *ac = &state.flags.ac;

    uint16_t result = *a + val;
    *cy = result > 0xFF;
    *ac = ((*a & 0x0F) + (val & 0x0F) + (*cy ? 1 : 0)) > 0x0F;
    *a = result & 0xFF;

    setFlags(*a);
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
    state.flags.z = (result == 0);
    state.flags.s = (result & 0x80);
    state.flags.p = __builtin_parity(result);
}
