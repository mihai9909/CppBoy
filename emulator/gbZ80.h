#pragma once

#include "../types.h"
#include "memory.h"

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

    /*** Registers ***/
	typedef struct Registers {

        // AF
        union {
            struct {
                BYTE f;
                BYTE a;
            };
            WORD af;
        };

        // BC
        union {
            struct {
                BYTE c;
                BYTE b;
            };
            WORD bc;
        };

        // DE
        union {
            struct {
                BYTE e;
                BYTE d;
            };
            WORD de;
        };

        // HL
        union {
            struct {
                BYTE l;
                BYTE h;
            };
            WORD hl;
        };

		WORD sp; // stack pointer
		WORD pc; // program counter
	} Regs;

	Memory* mem;
	Regs regs;
};
