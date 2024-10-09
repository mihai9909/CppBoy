#include "gbZ80.h"

GBZ80::GBZ80(Memory* memory) : mem(memory)
{
	regs = Regs();
}

BYTE GBZ80::fetchOpcode() { return mem->readByte(regs.pc++); }

std::vector<BYTE> GBZ80::fetchInstruction(BYTE opCode) {
	int instructionLength = instLens[opCode];
	std::vector<BYTE> instruction = mem->readByteSequence(regs.pc, instructionLength);
	regs.pc += instructionLength;
	return instruction;
}

void GBZ80::executeInstruction(BYTE opCode, std::vector<BYTE> instr) {
	BYTE r16 = (opCode & BITS_45) >> 4;
	BYTE r8 = (opCode & BITS_345) >> 3;

	switch (opCode) {
		case NO_OP: return;
		case LD_PIMM16_SP: loadPimm16SP(instr); return;
	}

	switch (opCode & (~BITS_45))
	{
	case LD_R16_IMM16: loadR16N16(r16, instr);  return; 
	case LD_PR16MEM_A: loadPR16memA(r16); return;
	case LD_A_PR16MEM: loadAPR16mem(r16); return;
	case INC_R16: incR16(r16); return;
	case DEC_R16: decR16(r16); return;
	case ADD_HL_R16: addHLR16(r16); return;
	default: 
		break;
	}


	switch (opCode & (~BITS_345))
	{
	case INC_R8: incR8(r8); return;
	case DEC_R8: decR8(r8); return;
	case LD_R8_IMM8: loadR8imm8(r8, instr); return;
	default:
		break;
	}
}

void GBZ80::execute() {
	BYTE opCode = fetchOpcode();
	std::vector<BYTE> instruction = fetchInstruction(opCode);
	executeInstruction(opCode, instruction);
}


BYTE* GBZ80::getR8(BYTE index) {
	switch (index)
	{
	case 0: return &regs.b; break;
	case 1: return &regs.c; break;
	case 2: return &regs.d; break;
	case 3: return &regs.e; break;
	case 4: return &regs.h; break;
	case 5: return &regs.l; break;
	case 6: return mem->readPByte(regs.hl); break; // TODO
	case 7: return &regs.a; break;
	default: break;  //TODO: display error
	}
}

WORD* GBZ80::getR16(BYTE index) {
	switch (index)
	{
	case 0: return &regs.bc; break;
	case 1: return &regs.de; break;
	case 2: return &regs.hl; break;
	case 3: return &regs.sp; break;
	default: break;  //TODO: display error
	}
}

WORD* GBZ80::getR16stk(BYTE index) {
	switch (index)
	{
	case 0: return &regs.bc; break;
	case 1: return &regs.de; break;
	case 2: return &regs.hl; break;
	case 3: return &regs.af; break;
	default: break; //TODO: display error
	}
}

void GBZ80::loadR16N16(BYTE reg, std::vector<BYTE> instr) {
	WORD* r16 = getR16(reg);
	WORD imm16 = (instr[1] << 8) | instr[0];
	*r16 = imm16;
}

void GBZ80::loadPR16memA(BYTE reg) {
	WORD* reg16mem;
	switch (reg)
	{
	case 0: reg16mem = &regs.bc; break;
	case 1: reg16mem = &regs.de; break;
	case 2: reg16mem = &regs.hl; break;
	case 3: reg16mem = &regs.hl; break;
	default: break;
	}

	mem->setByte(*reg16mem, regs.a);

	if (reg == 2) { *reg16mem++; } // hl+
	if (reg == 3) { *reg16mem--; } // hl-
}

// same as above but swapped dest and src
void GBZ80::loadAPR16mem(BYTE reg) {
	WORD* reg16mem;
	switch (reg)
	{
	case 0: reg16mem = &regs.bc; break;
	case 1: reg16mem = &regs.de; break;
	case 2: reg16mem = &regs.hl; break;
	case 3: reg16mem = &regs.hl; break;
	default: break;
	}

	regs.a = mem->readByte(*reg16mem);

	if (reg == 2) { *reg16mem++; } // hl+
	if (reg == 3) { *reg16mem--; } // hl-
}

void GBZ80::loadPimm16SP(std::vector<BYTE> instr) {
	WORD imm16 = (instr[1] << 8) | instr[0];
	mem->setByte(imm16, regs.sp);
}

void GBZ80::incR16(BYTE r16) {
	(*getR16(r16))++;
}

void GBZ80::decR16(BYTE r16) {
	(*getR16(r16))--;
}

void GBZ80::addHLR16(BYTE r16) {
	regs.hl += *getR16(r16);
}


void GBZ80::incR8(BYTE r8) {
	(*getR8(r8))++;
}

void GBZ80::decR8(BYTE r8) {
	(*getR8(r8))--;
}

void GBZ80::loadR8imm8(BYTE r8, std::vector<BYTE> imm8) {
	(*getR8(r8)) = imm8[0];
}
