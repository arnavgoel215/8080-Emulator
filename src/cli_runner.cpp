#include "controller/controller.hpp"
#include "model/emulator.hpp"
#include <iostream>
#include <iomanip>
#include <string>

// A simple function to print the current state of the CPU registers.
void print_cpu_state(const CPUState& state) {
    std::cout << std::hex << std::uppercase
              << "PC: " << std::setw(4) << std::setfill('0') << state.pc << "  "
              << "SP: " << std::setw(4) << std::setfill('0') << state.sp << "  "
              << "A: " << std::setw(2) << std::setfill('0') << (int)state.a << "  "
              << "BC: " << std::setw(2) << std::setfill('0') << (int)state.b 
              << std::setw(2) << std::setfill('0') << (int)state.c << "  "
              << "DE: " << std::setw(2) << std::setfill('0') << (int)state.d
              << std::setw(2) << std::setfill('0') << (int)state.e << "  "
              << "HL: " << std::setw(2) << std::setfill('0') << (int)state.h
              << std::setw(2) << std::setfill('0') << (int)state.l << "  "
              << "Flags: "
              << (state.flags.z ? "Z" : ".")
              << (state.flags.s ? "S" : ".")
              << (state.flags.p ? "P" : ".")
              << (state.flags.cy ? "C" : ".")
              << (state.flags.ac ? "A" : ".")
              << std::endl;
}

int main(int argc, char* argv[]) {
    Emulator model;
    // We pass nullptr for the view because we are not using the GUI.
    Controller controller(&model, nullptr);

    std::string rom_path = "rom/";
    if (argc > 1) {
        rom_path = argv[1];
    }

    std::cout << "Attempting to load ROM from: " << rom_path << std::endl;
    controller.onLoadROM(rom_path);

    std::cout << "ROM loaded. Starting CLI debugger." << std::endl;
    std::cout << "Press ENTER to step one instruction. Type 'q' and ENTER to quit." << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;

    // Main execution loop
    while (true) {
        // Print the state *before* executing the next instruction
        CPUState currentState = controller.getCPUStateForDebug();
        print_cpu_state(currentState);

        // Wait for user input
        std::string input;
        std::getline(std::cin, input);
        if (input == "q") {
            break;
        }

        controller.stepSingleInstruction();
    }

    return 0;
}
