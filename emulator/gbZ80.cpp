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
