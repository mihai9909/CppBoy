#include "gbZ80.h"

GBZ80::GBZ80(Memory* memory) : mem(memory)
{
	regs = Regs();
}

WORD GBZ80::getAF() { return regs.af; }

WORD GBZ80::getA() { return HIBYTE(regs.af); }

WORD GBZ80::getF() { return LOBYTE(regs.af); }

