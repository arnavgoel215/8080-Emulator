// romloader.hpp

#ifndef ROMLOADER_HPP
#define ROMLOADER_HPP

#include <string>
#include "memory.hpp"

// =============== Space Invaders ROM Loader ==================

// Loads the 4-part Space Invaders ROM set into memory:
//   - invaders.h → 0x0000
//   - invaders.g → 0x0800
//   - invaders.f → 0x1000
//   - invaders.e → 0x1800

bool LoadSpaceInvadersROM(Memory& memory, const std::string& overrideDirectory = "");

#endif // ROMLOADER_HPP
