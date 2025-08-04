/**********************************************************
 * @file emulator.hpp
 * 
 * @brief Public interface for the 8080 emulation model.
 *        This class serves as the main facade for the Controller
 *        to interact with the CPU, memory, and I/O subsystems.
 * 
 * @author Jese/Arnav (CPU Core), Fredo (Memory/IO), Sergio (Inputs)
 * 
 *********************************************************/
#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

/***************** Include files. ***********************/
#include "memory.hpp"
#include <cstdint>
#include <vector>
#include <string>

/***************** Macros and defines. ***********************/
// The size of the Space Invaders Video RAM in bytes. (256x224 pixels / 8 bits per byte)
// constexpr size_t VRAM_SIZE = 7168;

/***************** Global Classes. ***********************/

/**
 * @brief An enumeration of all possible game inputs for Space Invaders.
 * Used by the Controller to report key presses to the model.
 */
enum class GameInput
{
    Coin,
    P1_Start,
    P1_Shoot,
    P1_Left,
    P1_Right,
    // P2 inputs can be added here if needed
};

/**
 * @brief Union used to easily write and read single
 * bits in the input port 1.
 */
typedef union
{
    struct
    {
        uint8_t coin:1;            // Coin insertion on 1.
        uint8_t p2_start:1;        // Player 2 Start button
        uint8_t p1_start:1;        // Player 1 Start button
        uint8_t bit_3_reserved:1;  // Reserved, always set to 1.
        uint8_t p1_shoot:1;        // Player 1 shoot button.
        uint8_t p1_left:1;         // Player 1 Left Joystick input.
        uint8_t p1_right:1;        // Player 1 Right Joystic input.
        uint8_t bit_7_reserved;    // Reserved.
    };
    uint8_t byte;
} port1_t;

/**
 * @brief Union used to easily write and read single
 * bits in the input port 1.
 * 
 * TODO: Controller interface not implemented. Everything
 * is defaulted to 0.
 */
typedef union
{
    struct
    {
        uint8_t lives_dipswitch_0:1;
        uint8_t lives_dipswitch_1:1;
        uint8_t tilt_button:1;
        uint8_t bonus_life_dipswitch:1;
        uint8_t p2_shoot:1;
        uint8_t p2_left:1;
        uint8_t p2_right:1;
        uint8_t coin_demo_dipswitch:1;
    };
    uint8_t byte;
} port2_t;

/**
 * @brief Input port enumeration.
 */
enum class InPortNum : uint8_t
{
    INP0 = 0,    // Test inputs (Unused).
    INP1 = 1,    // P1 Inputs, and Coin deposit.
    INP2 = 2,    // P2 Inputs, and dipswitches.
    SHFT_IN = 3, // Shift register result.
};

/**
 * @brief Output port enumeration.
 */
enum class OutPortNum : uint8_t
{
    SHFTAMNT = 2,   // Shift amount, how many bits the shift register uses.
    SOUND1 = 3,     // Sound chip 1. (TODO: Not implemented.)
    SHFT_DATA = 4,  // Shift data, the actual data being shifted.
    SOUND2 = 5,     // Sound chip 2 (TODO: Not implemented.)
    WATCHDOG = 6,   // Watchdog controller (Unused by emulation).
};

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

    // I/O ports for Space Invaders hardware
    // Port 0 is unused.
    port1_t port_in_1 = {};
    port2_t port_in_2 = {};
    // Port 3 is tied to the shift register.

    uint16_t shift_register = 0;
    uint8_t shift_offset = 0;
};

/**
 * @brief A plain data structure to hold the state of the sound-related hardware ports.
 */
struct SoundState
{
    uint8_t port3 = 0; // Sound port 1
    uint8_t port5 = 0; // Sound port 2
    bool hasChanged = false; // Flag to indicate a new sound event
};

/**
 * @brief The main class for the 8080 emulation model.
 */
class Emulator
{
// --- DEBUG MODE --- 
// Expose the Memory private class ONLY while testing and DEBUGGING
#ifdef ENABLE_CPU_TESTING
public:
    // --- Lifecycle and Control ---


    /**
     *    @brief Enables debug mode for CPU's Memory.
     *    Allow access to private memory class.
     *    will not be accessible during normal run time.
     */
    Memory& getMemoryRef(){
        return memory;
    }

    /**
     * @brief Grants mutable access to CPU state for testing.
     *        Only available in DEBUG/UNIT TEST builds.
     */
    CPUState& getCPUStateRef() {
        return state;
}
#endif // --- END DEBUG ---

public:
    /**
     * @brief Default constructor. Initializes the CPU state.
     */
    Emulator();

    /**
     * @brief Loads a ROM file's contents into the emulator's memory.
     * @param romFilePath The path to the ROM file.
     * 
     * @returns true if path is valid, and ROM was successfully read.
     */
    bool loadROM(const std::string& romFilePath);

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

    /**
     * @brief Sets the state of a game input bit.
     * @param input The game input to change.
     * @param isPressed True if the button is pressed, false if released.
     */
    void setInputState(GameInput input, bool isPressed);

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

    /**
     * @brief Gets the current state of the sound hardware ports.
     * @return A SoundState struct containing the port values.
     */
    SoundState getSoundState();

    /**
     * @brief Defines the registers for the MOV instruction
     */
    enum RegisterCode : uint8_t {
        REG_B = 0,
        REG_C = 1,
        REG_D = 2,
        REG_E = 3,
        REG_H = 4,
        REG_L = 5,
        REG_M = 6,
        REG_A = 7
    };

private:
    // --- Internal CPU State ---
    
    /**
     * @brief The current state of the CPU registers and flags.
     */
    CPUState state;

    /**
     * @brief The current state of the sound hardware.
     */
    SoundState m_soundState;

    /**
     * @brief The 64KB memory space of the 8080.
     * 64KB RAM - Includes ROM, WAM, VRAM and Debugging support.
     */
    Memory memory;

    
    // --- Helper Functions ---

    /**
     * @brief Fetches, decodes, and executes a single instruction from memory.
     */
    void executeInstruction();

        /**
     * @brief Sets flags based on result of executing instruction
     */
    void setFlags(uint8_t result);

    /**
     * @brief Returns the value of the 16 bit register pair for registers H and L
     */
    uint16_t hl() const;

    /**
     * @brief Handles input into the system
     */
    uint8_t io_read(InPortNum port);

    /**
     * @brief Handles output from the system
     */
    void io_write(OutPortNum port, uint8_t val);

    /**
     * @brief Gets value stored in register
     */
    uint8_t get_reg(uint8_t code);

    /**
     * @brief Sets value in register
     */
    void set_reg(uint8_t code, uint8_t val);

    // --- Opcode Functions ---

    // Data Transfer Group
    /** @brief 0x01: Load Immediate into register pair B and C */
    void op_LXI_B(); 
    /** @brief 0x02: Store accumulator indirect */
    void op_STAX_B();
    /** @brief 0x06: Move immediate to register B */
    void op_MVI_B();
    /** @brief 0x0A: Load accumulator indirect */
    void op_LDAX_B();
    /** @brief 0x0E: Move immediate to register C */
    void op_MVI_C();
    /** @brief 0x11: Load immediate into register pair D and E */
    void op_LXI_D();
    /** @brief 0x12: Store accumulator indirect */
    void op_STAX_D();
    /** @brief 0x16: Move immediate to register D */
    void op_MVI_D();
    /** @brief 0x1A: Load accumulator indirect */
    void op_LDAX_D();
    /** @brief 0x1E: Move immediate to register E */
    void op_MVI_E();
    /** @brief 0x21: Load immediate into register pair H and L */
    void op_LXI_H();
    /** @brief 0x22: Store H and L direct */
    void op_SHLD();
    /** @brief 0x26: Move immediate to register H */
    void op_MVI_H();
    /** @brief 0x2A: Load H and L direct */
    void op_LHLD();
    /** @brief 0x2E: Move immediate to register L */
    void op_MVI_L();
    /** @brief 0x31: Load immediate into stack pointer */
    void op_LXI_SP();
    /** @brief 0x32: Store accumulator direct */
    void op_STA();
    /** @brief 0x36: Move immediate to memory */
    void op_MVI_M();
    /** @brief 0x3A: Load accumulator direct */
    void op_LDA();
    /** @brief 0x3E: Move immediate to accumulator */
    void op_MVI_A();
    /** @brief 0xEB: Exchange H and L with D and E */
    void op_XCHG();

    // Arithmetic Group
    /** @brief 0x03: Increment register pair B and C */
    void op_INX_B();
    /** @brief 0x04: Increment register B */
    void op_INR_B();
    /** @brief 0x05: Decrement register B */
    void op_DCR_B();
    /** @brief 0x09: Add register pair B and C to H and L */
    void op_DAD_B();
    /** @brief 0x0B: Decrement register pair B and C */
    void op_DCX_B();
    /** @brief 0x0C: Increment register C */
    void op_INR_C();
    /** @brief 0x0D: Decrement register C */
    void op_DCR_C();
    /** @brief 0x13: Increment register pair D and E */
    void op_INX_D();
    /** @brief 0x14: Increment register D */
    void op_INR_D();
    /** @brief 0x15: Decrement register D */
    void op_DCR_D();
    /** @brief 0x19: Add register pair D and E to H and L */
    void op_DAD_D();
    /** @brief 0x1B: Decrement register pair D and E */
    void op_DCX_D();
    /** @brief 0x1C: Increment register E */
    void op_INR_E();
    /** @brief 0x1D: Decrement register E */
    void op_DCR_E();
    /** @brief 0x23: Increment register pair H and L */
    void op_INX_H();
    /** @brief 0x24: Increment register H */
    void op_INR_H();
    /** @brief 0x25: Decrement register H */
    void op_DCR_H();
    /** @brief 0x27: Decimal adjust accumulator */
    void op_DAA();
    /** @brief 0x29: Add register pair H and L to H and L */
    void op_DAD_H();
    /** @brief 0x2B: Decrement register pair H and L */
    void op_DCX_H();
    /** @brief 0x2C: Increment register L */
    void op_INR_L();
    /** @brief 0x2D: Decrement register L */
    void op_DCR_L();
    /** @brief 0x34: Increment memory */
    void op_INR_M();
    /** @brief 0x35: Decrement memory */
    void op_DCR_M();
    /** @brief 0x39: Add stack pointer to H and L */
    void op_DAD_SP();
    /** @brief 0x3C: Increment accumulator */
    void op_INR_A();
    /** @brief 0x3D: Decrement accumulator */
    void op_DCR_A();
    /** @brief Executes the ADD instruction */
    void op_ADD(uint8_t val);
     /** @brief Executes the ADC instruction */
    void op_ADC(uint8_t val);
    /** @brief Executes the SUB instruction */
    void op_SUB(uint8_t val);
    /** @brief Executes the SBB instruction */
    void op_SBB(uint8_t val);

    // Logical Group
    /** @brief 0x07: Rotate accumulator left */
    void op_RLC();
    /** @brief 0x0F: Rotate accumulator right */
    void op_RRC();
    /** @brief 0x1F: Rotate accumulator right through carry */
    void op_RAR();
    /** @brief 0x2F: Complement accumulator */
    void op_CMA();
    /** @brief 0x37: Set carry */
    void op_STC();
    /** @brief 0x3F: Complement carry */
    void op_CMC();
    /** @brief Executes the ANA instruction (AND accumulator) */
    void op_ANA(uint8_t val);
    /** @brief Executes the XRA Instruction (XOR accumulator) */
    void op_XRA(uint8_t val);
    /** @brief Executes the ORA instruction (OR accumulator) */
    void op_ORA(uint8_t val);
    /** @brief Executes the CMP instruction (compare accumulator) */
    void op_CMP(uint8_t val);

    // Branch Group
    /** @brief 0xC3: Jump */
    void op_JMP();
    /** @brief 0xC9: Return */
    void op_RET();
    /** @brief 0xCD: Call */
    void op_CALL();
    /** @brief 0xE9: Load program counter from H and L */
    void op_PCHL();
    /** @brief Executes return conditional instructions */
    void op_RET_cond(bool condition);
    /** @brief Executes jump conditional instructions */
    void op_JMP_cond(bool condition);
    /** @brief Executes call conditional instructions */
    void op_CALL_cond(bool condition);

    // Stack, I/O, and Machine Control Group
    /** @brief 0x00: No operation */
    void op_NOP();
    /** @brief 0x76: Halt */
    void op_HLT();
    /** @brief 0xC1: Pop register pair B and C off stack */
    void op_POP_B();
    /** @brief 0xC5: Push register pair B and C onto stack */
    void op_PUSH_B();
    /** @brief 0xD1: Pop register pair D and E off stack */
    void op_POP_D();
    /** @brief 0xD5: Push register pair D and E onto stack */
    void op_PUSH_D();
    /** @brief 0xE1: Pop register pair H and L off stack */
    void op_POP_H();
    /** @brief 0xE3: Exchange top of stack with H and L */
    void op_XTHL();
    /** @brief 0xE5: Push register pair H and L onto stack */
    void op_PUSH_H();
    /** @brief 0xF1: Pop processor status word off stack */
    void op_POP_PSW();
    /** @brief 0xF5: Push processor status word onto stack */
    void op_PUSH_PSW();
    /** @brief 0xFB: Enable interrupts */
    void op_EI();
    /** @brief Handles the input to the system */
    void op_IN();
    /** @brief Handles output from the system */
    void op_OUT();
    /** @brief Executes MOV instruction */
    void op_MOV(uint8_t opcode);
    /** @brief Handles RST 0-7 Opcodes */
    void op_RST(int index);
    /** @brief  Handles Opcode 0xF9 */
    void op_SPHL();  // SP ← HL
    /** @brief Handles Opcode 0xC6 (ADI) */
    void op_ADI(uint8_t immediateVal); // ADI d8
    /** @brief Handles Opcode 0xCE (ACI) */
    void op_ACI(uint8_t immediateVal); // ACI d8
    /** @brief Hanldes Opcode 0xD6 */
    void op_SUI(uint8_t val); // SUI d8
    /** @brief Handles Opcode 0xDE */
    void op_SBI(uint8_t val); // SBI d8



};

#endif /* EMULATOR_HPP_ */