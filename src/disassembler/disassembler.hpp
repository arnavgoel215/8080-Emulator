/**********************************************************
 * @file disassembler.hpp
 * 
 * @brief Public interface for the 8080 disassembler utility.
 * 
 * @author 
 * 
 *********************************************************/
#ifndef DISASSEMBLER_HPP_
#define DISASSEMBLER_HPP_

/***************** Include files. ***********************/
#include <cstdint>
#include <vector>
#include <string>

/***************** Global Classes. ***********************/

/**
 * @brief A utility class for disassembling 8080 binary code.
 */
class Disassembler
{
public:
    /**
     * @brief Disassembles a single instruction at a given memory location.
     * @param codeBuffer Pointer to the start of the code buffer.
     * @param pc The offset (program counter) into the buffer.
     * @return A formatted string like "0x1000: MOV A, B".
     */
    static std::string disassembleInstruction(const uint8_t* codeBuffer, uint16_t pc);

    /**
     * @brief Disassembles an entire ROM buffer.
     * @param romData A vector of bytes representing the ROM.
     * @return A vector of strings, with each string being one disassembled instruction.
     */
    static std::vector<std::string> disassembleROM(const std::vector<uint8_t>& romData);
};

#endif /* DISASSEMBLER_HPP_ */