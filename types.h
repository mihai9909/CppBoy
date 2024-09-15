#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <cstdint>  // For fixed-width integer types

typedef std::uint8_t  BYTE;   // 8-bit unsigned integer
typedef std::uint16_t WORD;   // 16-bit unsigned integer
typedef std::uint32_t DWORD;  // 32-bit unsigned integer
typedef std::uint64_t QWORD;  // 64-bit unsigned integer

typedef std::int8_t   SBYTE;  // 8-bit signed integer
typedef std::int16_t  SWORD;  // 16-bit signed integer
typedef std::int32_t  SDWORD; // 32-bit signed integer
typedef std::int64_t  SQWORD; // 64-bit signed integer

#endif // TYPES_H