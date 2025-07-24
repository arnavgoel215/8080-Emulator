/**********************************************************
 * @file emulator_main.cpp
 * 
 * @brief Example file for developing and testing the emulator module.
 * 
 * @author: <TODO: ADD AUTHOR>
 * 
 *********************************************************/

/***************** Include files. ***********************/
#include <iostream>
#include <vector>
#include <iomanip> // For std::hex and std::setw
#include <fstream> // For std::ofstream
#include "emulator.hpp"

/***************** Macros and defines. ***********************/

/***************** Namespaces. ***********************/

/***************** Local Classes. ***********************/

/***************** Local Functions. ***********************/

// Helper function to print the CPU state in a formatted way
void printCPUState(const CPUState& state)
{
    std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0') << state.pc
              << " | SP: 0x" << std::hex << std::setw(4) << std::setfill('0') << state.sp
              << " | A: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)state.a
              << " | B: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)state.b
              << " | C: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)state.c
              << " | D: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)state.d
              << " | E: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)state.e
              << " | H: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)state.h
              << " | L: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)state.l
              << " | Flags: Z:" << state.flags.z << " S:" << state.flags.s << " P:" << state.flags.p
              << " CY:" << state.flags.cy << " AC:" << state.flags.ac << std::endl;
}


int main (int argc, char * argv[])
{
    Emulator emulator;
    
    // Create a simple program with 5 NOP instructions and write it to a temp file
    const char* test_rom_path = "build/nop_test.rom";
    std::vector<uint8_t> rom = {0x00, 0x00, 0x00, 0x00, 0x00};
    std::ofstream rom_file(test_rom_path, std::ios::out | std::ios::binary);
    rom_file.write(reinterpret_cast<const char*>(rom.data()), rom.size());
    rom_file.close();

    // Load the test ROM
    emulator.loadROM(test_rom_path);

    std::cout << "--- 8080 NOP Instruction Test ---" << std::endl;
    
    std::cout << "Initial State:" << std::endl;
    printCPUState(emulator.getCPUState());

    // Emulate 3 instructions
    std::cout << "\nEmulating 3 cycles (3 NOP instructions)..." << std::endl;
    emulator.emulateCycles(3);

    std::cout << "\nState after 3 cycles:" << std::endl;
    printCPUState(emulator.getCPUState());

    // Emulate 2 more instructions
    std::cout << "\nEmulating 2 more cycles (2 NOP instructions)..." << std::endl;
    emulator.emulateCycles(2);

    std::cout << "\nState after 5 cycles:" << std::endl;
    printCPUState(emulator.getCPUState());

    std::cout << "\nTest complete. PC should be 5." << std::endl;

    return 0;
}

/***************** Global Class Functions. ***********************/
