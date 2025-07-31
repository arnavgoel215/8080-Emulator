/**********************************************************
 * @file common_frame_cfg.h
 * 
 * @brief Common constants for Frame Data.
 * 
 * @author: Sergio Chavarria
 * 
 *********************************************************/

#ifndef COMMON_FRAME_CFG_H
#define COMMON_FRAME_CFG_H

/***************** Include files. ***********************/

// Standard includes.
#include <array>
#include <cstdint>

/***************** Macros, constants, and defines. ***********************/

static constexpr size_t FRAME_HEIGHT = 256;
static constexpr size_t FRAME_WIDTH = 224;
static constexpr size_t PIXELS_PER_BYTE = 8;
static constexpr size_t FRAME_BUFFER_LEN = (FRAME_HEIGHT * FRAME_WIDTH) / PIXELS_PER_BYTE;
static constexpr size_t FRAME_BUFFER_MID_SCREEN = FRAME_BUFFER_LEN / 2;

using frame_buffer_t = std::array<uint8_t, FRAME_BUFFER_LEN>;

/***************** Namespaces. ***********************/

/***************** Local Classes. ***********************/

/***************** Local Functions. ***********************/

/***************** Global Class Functions. ***********************/

#endif // COMMON_FRAME_CFG_H
