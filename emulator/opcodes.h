#pragma once

#define BITS_012 0x07
#define BITS_34 0x18
#define BITS_45 0x30
#define BITS_345 0x38

/*** opcode of instructions, the parameters inside the opcodes have been set to 0 ***/
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

#define RLCA 0x07
#define RRCA 0x0F
#define RLA 0x17
#define RRA 0x1F
#define DAA 0x27
#define CPL 0x2F
#define SCF 0x37
#define CCF 0x3F

#define JR_IMM8 0x18
#define JR_COND_IMM8 0x2

#define STOP 0x10
#define HALT 0x76 // LD [hl], [hl]

#define LD_R8_R8 0x40

// 8-bit arithmetic
#define ADD_A_R8 0x80
#define ADC_A_R8 0x88
#define SUB_A_R8 0x90
#define SBC_A_R8 0x98
#define AND_A_R8 0xA0
#define XOR_A_R8 0xA8
#define OR_A_R8 0xB0
#define CP_A_R8 0xB8

// 8-bit immediate arithmetic
#define ADD_A_IMM8 0xC6
#define ADC_A_IMM8 0xCE
#define SUB_A_IMM8 0xD6
#define SBC_A_IMM8 0xDE
#define AND_A_IMM8 0xE6
#define XOR_A_IMM8 0xEE
#define OR_A_IMM8 0xF6
#define CP_A_IMM8 0xFE

#define RET_COND 0xC0
#define RET 0xC9
#define RETI 0xD9
#define JP_COND_IMM16 0xC2
#define JP_IMM16 0xC3
#define JP_HL 0xE9
#define CALL_COND_IMM16 0xC4
#define CALL_IMM16 0xCD
#define RST_TGT3 0xC7

#define POP_R16STK 0xC1
#define PUSH_R16STK 0xC5

#define PREFIX 0xCB

#define LDH_MEMC_A 0xE2
#define LDH_PIMM8_A 0xE0
#define LD_PIMM16_A 0xEA
#define LDH_A_MEMC 0xF2
#define LDH_A_PIMM8 0xF0
#define LD_A_PIMM16 0xFA

#define ADD_SP_IMM8 0xE8
#define LD_HL_SP_IMM8 0xF8
#define LD_SP_HL 0xF9

#define DI 0xF3
#define EI 0xFB
