#pragma once

#define BITS_45 0x30
#define BITS_345 0x38


/*** 0x00 ***/
#define NO_OP 0x00

#define LD_R16_IMM16 0x01
#define LD_PR16MEM_A 0x02
#define LD_A_PR16MEM 0x0A
#define LD_PIMM16_SP 0x08

#define INC_R16 0x03
#define DEC_R16 0x0B
#define ADD_HL_R16 0x09

#define INC_R8 0x04
#define DEC_R8 0x05

#define LD_R8_IMM8 0x06

/*** 0x10 ***/
