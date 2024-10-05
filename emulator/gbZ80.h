#pragma once

#include "../types.h"
#include "memory.h"
#include "opcodes.h"

/*** Flag reg bits ***/
#define ZERO_FLAG 1<<7 // (7th bit of flag register)
#define SUB_FLAG 1<<6 // subtraction flag (6th bit of flag register)
#define HALF_CARRY_FLAG 1<<5 // (5th bit of flag register)
#define CARRY_FLAG 1<<4 // (4th bit of flag register)

class GBZ80 {
public:
	GBZ80(Memory* mem);
	~GBZ80();
    BYTE fetchOpcode();
    std::vector<BYTE> fetchInstruction(BYTE opCode);
    void executeInstruction(BYTE opCode, std::vector<BYTE> instr);
    void execute();

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

    BYTE* getR8(BYTE index);
    WORD* getR16(BYTE index);
    WORD* getR16mem(BYTE index);
    WORD* getR16stk(BYTE index);

    // BLOCK 0
    void loadR16N16(BYTE reg, std::vector<BYTE> instr);
    void loadPR16memA(BYTE reg);
    void loadAPR16mem(BYTE reg);
    void loadPimm16SP(std::vector<BYTE> imm16);
    void incR16(BYTE r16);
    void decR16(BYTE r16);
    void addHLR16(BYTE r16);
    void incR8(BYTE r8);
    void decR8(BYTE r8);
    void loadR8imm8(BYTE r8, std::vector<BYTE> imm8);

    // number of bytes per instruction excluding opcode
    std::vector<int> instLens = { 0, 2, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 1, 0, // 0x00
                                  1, 2, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, // 0x10
                                  1, 2, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, // 0x20
                                  1, 2, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, // 0x30
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x40
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x50
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x60
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x70
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x80
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x90
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xA0
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xB0
                                  0, 0, 2, 2, 2, 0, 1, 0, 0, 0, 2, 0, 2, 2, 1, 0, // 0xC0
                                  0, 0, 2, 0, 2, 0, 1, 0, 0, 0, 2, 0, 2, 0, 1, 0, // 0xD0
                                  1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 2, 0, 0, 0, 1, 0, // 0xE0
                                  1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 2, 0, 0, 0, 1, 0 }; // 0xF0

	Memory* mem;
	Regs regs;
};
