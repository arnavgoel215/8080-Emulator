/**********************************************************
 * @file emulator.hpp
 * 
 * @brief Public interface for the 8080 emulation model.
 *        This class serves as the main facade for the Controller
 *        to interact with the CPU, memory, and I/O subsystems.
 * 
 * @author Arnav (CPU Core), Fredo (Memory/IO)
 * 
 *********************************************************/
#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

/***************** Include files. ***********************/
#include <cstdint>
#include <vector>
#include <string>

/***************** Macros and defines. ***********************/
// The size of the Space Invaders Video RAM in bytes. (256x224 pixels / 8 bits per byte)
constexpr size_t VRAM_SIZE = 7168;

/***************** Global Classes. ***********************/

/**
 * @brief A plain data structure to hold a snapshot of the CPU's state for debugging.
 */
struct CPUState
{
    // General-purpose registers
    uint8_t a, b, c, d, e, h, l;
    
    // Special-purpose registers
    uint16_t sp; // Stack Pointer
    uint16_t pc; // Program Counter

    // Condition flags
    struct
    {
        bool z, s, p, cy, ac; // Zero, Sign, Parity, Carry, Aux Carry
    } flags;

    bool interrupts_enabled;
};

/**
 * @brief The main class for the 8080 emulation model.
 */
class Emulator
{
public:
    // --- Lifecycle and Control ---

    /**
     * @brief Default constructor. Initializes the CPU state.
     */
    Emulator();

    /**
     * @brief Loads a ROM file's contents into the emulator's memory.
     * @param romData A vector of bytes containing the ROM file.
     */
    void loadROM(const std::vector<uint8_t>& romData);

    /**
     * @brief Resets the emulator to its initial power-on state.
     */
    void reset();

    /**
     * @brief Executes CPU instructions for a given number of clock cycles.
     * @param cycles The number of 2MHz clock cycles to emulate.
     */
    void emulateCycles(int cycles);

    /**
     * @brief Requests a hardware interrupt (RST instruction).
     * @param interrupt_num The interrupt number (1 or 2 for Space Invaders).
     */
    void requestInterrupt(uint8_t interrupt_num);

    // --- Data Output to Controller ---

    /**
     * @brief Gets a snapshot of the current CPU registers and flags for debugging.
     * @return A CPUState struct containing the current state.
     */
    CPUState getCPUState() const;

    /**
     * @brief Provides direct, read-only access to the video RAM for rendering.
     * @return A constant pointer to the start of the 7KB video RAM buffer.
     */
    const uint8_t* getFrameBuffer() const;

private:
    // --- Internal CPU State ---
    
    /**
     * @brief The current state of the CPU registers and flags.
     */
    CPUState state;

    /**
     * @brief The 64KB memory space of the 8080.
     */
    std::vector<uint8_t> memory;

    // --- Helper Functions ---

    /**
     * @brief Fetches, decodes, and executes a single instruction from memory.
     */
    void executeInstruction();

    /**
     * @brief Executes the ADD instruction
     */
    void op_ADD(uint8_t val);

    /**
     * @brief Sets flags based on result of executing instruction
     */
    void setFlags(uint8_t result);

    /**
     * @brief Returns the value of the 16 bit register pair for registers H and L
     */
    uint16_t hl() const;

    /**
     * @brief Executes the ADC instruction
     */
    void op_ADC(uint8_t val);
};

#endif /* EMULATOR_HPP_ */