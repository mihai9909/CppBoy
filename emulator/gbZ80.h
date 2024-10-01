#pragma once

#include "../types.h"
#include "memory.h"

/*** MACROS ***/
// concatenate two 8-bit registers into a 16-bit value
#define CONCAT_REGS(high, low) ((((WORD)high) << 8) | (low))
#define LOBYTE(word) (BYTE)word // get lower byte from word
#define HIBYTE(word) (BYTE)(word >> 8) // get higher byte from word

/*** Flag reg bits ***/
#define ZERO_FLAG 1<<7 // (7th bit of flag register)
#define SUB_FLAG 1<<6 // subtraction flag (6th bit of flag register)
#define HALF_CARRY_FLAG 1<<5 // (5th bit of flag register)
#define CARRY_FLAG 1<<4 // (4th bit of flag register)

class GBZ80 {
public:
	GBZ80(Memory* mem);
	~GBZ80();
	WORD getAF();
	WORD getA();
	WORD getF();

private:
	typedef struct Registers {
		WORD af;
		WORD bc;
		WORD de;
		WORD hl;
		WORD sp; // stack pointer
		WORD pc; // program counter
	} Regs;

	Memory* mem;
	Regs regs;
};
