#pragma once

#include "../types.h"
#include "memory.h"
#include "opcodes.h"

/*** Flag reg bits ***/
#define ZERO_FLAG (1<<7) // (7th bit of flag register)
#define SUB_FLAG (1<<6) // subtraction flag (6th bit of flag register)
#define HALF_CARRY_FLAG (1<<5) // (5th bit of flag register)
#define CARRY_FLAG (1<<4) // (4th bit of flag register)

/*** MACROS ***/
#define LOWER_NIBBLE(byte) ((byte) & 0x0F) // lower
#define LOWER_BYTE(word) ((word) & 0x00FF)
#define LOWER_12BITS(word) ((word) & 0x0FFF)

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

    bool evaluateCondition(BYTE cond);

    void setZFlag(bool flag);
    void setNFlag(bool flag);
    void setHFlag(bool flag);
    void setCFlag(bool flag);

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

    void rlca();
    void rrca();
    void rla();
    void rra();
    void daa();
    void cpl();
    void scf();
    void ccf();

    void jrimm8(std::vector<BYTE> instr);
    void jrcondimm8(BYTE cond, std::vector<BYTE> instr);

    void retCond(BYTE cond);
    void ret();
    void reti();
    void jpCondimm16(BYTE cond, std::vector<BYTE> instr);
    void jpimm16(std::vector<BYTE> instr);
    void jpHL();
    void callCondimm16(BYTE cond, std::vector<BYTE> instr);
    void callimm16(std::vector<BYTE> instr);
    void rstTgt(BYTE tgt);
    void popR16stk(BYTE r16stk);
    void pushR16stk(BYTE r16stk);

    void ldhMemCA();
    void ldhPimm8A(std::vector<BYTE> instr);
    void ldPimm16A(std::vector<BYTE> instr);
    void ldhAMemC();
    void ldhAPimm8(std::vector<BYTE> instr);
    void ldAPimm16(std::vector<BYTE> instr);

    void stop();

    void loadR8R8(BYTE dstR8, BYTE srcR8);
    void halt();

    void addAR8(BYTE R8);
    void adcAR8(BYTE R8);
    void subAR8(BYTE R8);
    void sbcAR8(BYTE R8);
    void andAR8(BYTE R8);
    void xorAR8(BYTE R8);
    void orAR8(BYTE R8);
    void cpAR8(BYTE R8);

    void addAimm8(std::vector<BYTE> imm8);
    void adcAimm8(std::vector<BYTE> imm8);
    void subAimm8(std::vector<BYTE> imm8);
    void sbcAimm8(std::vector<BYTE> imm8);
    void andAimm8(std::vector<BYTE> imm8);
    void xorAimm8(std::vector<BYTE> imm8);
    void orAimm8(std::vector<BYTE> imm8);
    void cpAimm8(std::vector<BYTE> imm8);

    void addSPimm8(std::vector<BYTE> imm8);
    void ldHLSPimm8(std::vector<BYTE> imm8);
    void ldSPHL();

    void di();
    void ei();

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
