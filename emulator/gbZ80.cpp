#include "gbZ80.h"

GBZ80::GBZ80(Memory* memory) : mem(memory)
{
	regs = Regs();
	regs.pc = 0x00;
	regs.af = 0x6000;
	regs.sp = 0x00;
}

GBZ80::~GBZ80() { }

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
	BYTE cond = (opCode & BITS_34) >> 3;
	BYTE r8_low = (opCode & BITS_012);

	switch (opCode) {
		case NO_OP: return;
		case LD_PIMM16_SP: loadPimm16SP(instr); return;
		case RLCA: rlca(); return;
		case RRCA: rrca(); return;
		case RLA: rla(); return;
		case RRA: rra(); return;
		case DAA: daa(); return;
		case SCF: scf(); return;
		case CCF: ccf(); return;
		case JR_IMM8: jrimm8(instr); return;
		case STOP: stop(); return;
		case HALT: halt(); return;
		case ADD_A_IMM8: addAimm8(instr); return;
		case ADC_A_IMM8: adcAimm8(instr); return;
		case SUB_A_IMM8: subAimm8(instr); return;
		case SBC_A_IMM8: sbcAimm8(instr); return;
		case AND_A_IMM8: andAimm8(instr); return;
		case XOR_A_IMM8: xorAimm8(instr); return;
		case OR_A_IMM8: orAimm8(instr); return;
		case CP_A_IMM8: cpAimm8(instr); return;
		case RET: ret(); return;
		case RETI: reti(); return;
		case JP_IMM16: jpimm16(instr); return;
		case JP_HL: jpHL(); return;
		case CALL_IMM16: callimm16(instr); return;
		case LDH_MEMC_A: ldhMemCA(); return;
		case LDH_PIMM8_A: ldhPimm8A(instr); return;
		case LD_PIMM16_A: ldPimm16A(instr); return;
		case LDH_A_MEMC: ldhAMemC(); return;
		case LDH_A_PIMM8: ldhAPimm8(instr); return;
		case LD_A_PIMM16: ldAPimm16(instr); return;
		case ADD_SP_IMM8: addSPimm8(instr); return;
		case LD_HL_SP_IMM8: ldHLSPimm8(instr); return;
		case LD_SP_HL: ldSPHL(); return;
		case DI: di(); return;
		case EI: ei(); return;
		default: break;
	}

	switch (opCode & (~BITS_34))
	{
	case JR_COND_IMM8: jrcondimm8(cond, instr); return;
	case RET_COND: retCond(cond); return;
	case JP_COND_IMM16: jpCondimm16(cond, instr); return;
	default: break;
	}

	switch (opCode & (~BITS_45))
	{
	case LD_R16_IMM16: loadR16N16(r16, instr);  return; 
	case LD_PR16MEM_A: loadPR16memA(r16); return;
	case LD_A_PR16MEM: loadAPR16mem(r16); return;
	case INC_R16: incR16(r16); return;
	case DEC_R16: decR16(r16); return;
	case ADD_HL_R16: addHLR16(r16); return;
	case POP_R16STK: popR16stk(r16); return;
	case PUSH_R16STK: pushR16stk(r16); return;
	default: break;
	}

	switch (opCode & (~BITS_012)) {
	case ADD_A_R8: addAR8(r8_low); return;
	case ADC_A_R8: adcAR8(r8_low); return;
	case SUB_A_R8: subAR8(r8_low); return;
	case SBC_A_R8: sbcAR8(r8_low); return;
	case AND_A_R8: andAR8(r8_low); return;
	case XOR_A_R8: xorAR8(r8_low); return;
	case OR_A_R8: orAR8(r8_low); return;
	case CP_A_IMM8: cpAR8(r8_low); return;
	default: break;
	}

	switch (opCode & (~BITS_345))
	{
	case INC_R8: incR8(r8); return;
	case DEC_R8: decR8(r8); return;
	case LD_R8_IMM8: loadR8imm8(r8, instr); return;
	case RST_TGT3: rstTgt(opCode & BITS_345); return;
	default: break;
	}

	if (opCode & ~(BITS_012 | BITS_345))
		loadR8R8(r8, r8_low);
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

bool GBZ80::evaluateCondition(BYTE cond) {
	switch (cond)
	{
	case 0: return (regs.f & ZERO_FLAG) == 0;
	case 1: return (regs.f & ZERO_FLAG) != 0;
	case 2: return (regs.f & CARRY_FLAG) == 0;
	case 3: return (regs.f & CARRY_FLAG) != 0;
	default:
		break;
	}
}

void GBZ80::setZFlag(bool flag) {
	regs.f = flag ? (regs.f | ZERO_FLAG) : (regs.f & (~ZERO_FLAG));
}

void GBZ80::setNFlag(bool flag) {
	regs.f = flag ? (regs.f | SUB_FLAG) : (regs.f & (~SUB_FLAG));
}

void GBZ80::setHFlag(bool flag) {
	regs.f = flag ? (regs.f | HALF_CARRY_FLAG) : (regs.f & (~HALF_CARRY_FLAG));
}

void GBZ80::setCFlag(bool flag) {
	regs.f = flag ? (regs.f | CARRY_FLAG) : (regs.f & (~CARRY_FLAG));
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

	if (reg == 2) { (*reg16mem)++; } // hl+
	if (reg == 3) { (*reg16mem)--; } // hl-
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

	if (reg == 2) { (* reg16mem)++; } // hl+
	if (reg == 3) { (* reg16mem)--; } // hl-
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
	WORD hl = regs.hl;
	WORD prevR16 = *getR16(r16);

	regs.hl += *getR16(r16);

	setNFlag(false);
	setHFlag(LOWER_12BITS(hl) + LOWER_12BITS(prevR16) > 0xfff);
	setCFlag(hl < regs.hl);
}

void GBZ80::incR8(BYTE r8) {
	(*getR8(r8))++;

	setZFlag(*getR8(r8) == 0);
	setNFlag(false);
	setHFlag(LOWER_NIBBLE(*getR8(r8)) == 0x00);
}

void GBZ80::decR8(BYTE r8) {
	(*getR8(r8))--;

	setZFlag(*getR8(r8) == 0);
	setNFlag(true);
	setHFlag(LOWER_NIBBLE(*getR8(r8)) == 0x0F);
}

void GBZ80::loadR8imm8(BYTE r8, std::vector<BYTE> imm8) {
	(*getR8(r8)) = imm8[0];
}

void GBZ80::rlca() {
	BYTE msb = regs.a >> 7;
	regs.a = (regs.a << 1) | msb;
	setCFlag(msb == 0x01);
	setZFlag(false);
	setNFlag(false);
	setHFlag(false);
}

void GBZ80::rrca() {
	BYTE lsb = regs.a << 7;
	setCFlag(regs.a & 0x01);
	regs.a = (regs.a >> 1) | lsb;
	setZFlag(false);
	setNFlag(false);
	setHFlag(false);
}

void GBZ80::rla() {
	BYTE carry = (regs.f & CARRY_FLAG) >> 4;
	BYTE msb = regs.a >> 7;
	setCFlag(msb);
	regs.a = (regs.a << 1) | carry;
	setZFlag(false);
	setNFlag(false);
	setHFlag(false);
}

void GBZ80::rra() {
	BYTE carry = (regs.f & CARRY_FLAG) >> 4;
	BYTE lsb = regs.a & 0x01;
	setCFlag(lsb);
	regs.a = (regs.a >> 1) | (carry << 7);
	setZFlag(false);
	setNFlag(false);
	setHFlag(false);
}

void GBZ80::daa() {
	BYTE a = regs.a;
	if((regs.f & HALF_CARRY_FLAG) != 0 || LOWER_NIBBLE(regs.a) > 0x09) {
		regs.a += 0x06;
	}

	if ((regs.f & CARRY_FLAG) != 0 || HIGHER_NIBBLE(regs.a) > 0x09) {
		regs.a += 0x60;
	}

	setZFlag(regs.a == 0);
	setHFlag(false);
	setCFlag(regs.a < a);
}

void GBZ80::cpl() {
	regs.a = ~regs.a;
	setNFlag(true);
	setHFlag(true);
}

void GBZ80::scf() {
	regs.f = (regs.f | CARRY_FLAG) & ~(SUB_FLAG | HALF_CARRY_FLAG);
}

void GBZ80::ccf() {
	regs.f = (regs.f ^ CARRY_FLAG) & ~(SUB_FLAG | HALF_CARRY_FLAG);
}

void GBZ80::jrimm8(std::vector<BYTE> instr) {
	regs.pc = regs.pc + (SBYTE)instr[0];
}

void GBZ80::jrcondimm8(BYTE cond, std::vector<BYTE> instr) {
	if (evaluateCondition(cond)) {
		regs.pc = regs.pc + (SBYTE)instr[0];
	}
}

void GBZ80::stop() {
	// TODO
}

void GBZ80::halt() {
	// TODO
}

void GBZ80::loadR8R8(BYTE dstR8, BYTE srcR8) {
	*getR8(dstR8) = *getR8(srcR8);
}

void GBZ80::addAR8(BYTE R8) {
	BYTE a = regs.a;
	BYTE prevR8 = *getR8(R8);

	regs.a += *getR8(R8);

	setZFlag(regs.a == 0x00);
	setNFlag(false);
	setHFlag(LOWER_NIBBLE(a) + LOWER_NIBBLE(prevR8) > 0x0F);
	setCFlag(regs.a < a);
}

void GBZ80::adcAR8(BYTE R8) {
	BYTE carry = (regs.f & CARRY_FLAG) >> 4;
	BYTE a = regs.a;
	BYTE prevR8 = *getR8(R8);

	regs.a = regs.a + *getR8(R8) + carry;

	setZFlag(regs.a == 0);
	setNFlag(false);
	setHFlag(LOWER_NIBBLE(a) + LOWER_NIBBLE(prevR8) + carry > 0x0F);
	setCFlag(regs.a < a || (regs.a == a && carry == 0x01));
}

void GBZ80::subAR8(BYTE R8) {
	BYTE a = regs.a;
	BYTE prevR8 = *getR8(R8);

	regs.a -= *getR8(R8);

	setZFlag(regs.a == 0);
	setNFlag(true);
	setHFlag(LOWER_NIBBLE(prevR8) > LOWER_NIBBLE(a));
	setCFlag(regs.a > a);
}

void GBZ80::sbcAR8(BYTE R8) {
	BYTE a = regs.a;
	BYTE prevR8 = *getR8(R8);
	BYTE carry = (regs.f & CARRY_FLAG) >> 4;

	regs.a = regs.a - *getR8(R8) - carry;

	setZFlag(regs.a == 0);
	setNFlag(true);
	setHFlag(LOWER_NIBBLE(prevR8 + carry) > LOWER_NIBBLE(a));
	setCFlag(regs.a > a || (regs.a == a && carry == 0x01));
}

void GBZ80::andAR8(BYTE R8) {
	regs.a = regs.a & *getR8(R8);

	setZFlag(regs.a == 0);
	setNFlag(false);
	setHFlag(true);
	setCFlag(false);
}

void GBZ80::xorAR8(BYTE R8) {
	regs.a = regs.a ^ *getR8(R8);

	setZFlag(regs.a == 0);
	setNFlag(false);
	setHFlag(false);
	setCFlag(false);
}

void GBZ80::orAR8(BYTE R8) {
	regs.a = regs.a | *getR8(R8);

	setZFlag(regs.a == 0);
	setNFlag(false);
	setHFlag(false);
	setCFlag(false);
}

void GBZ80::cpAR8(BYTE R8) {
	BYTE res = regs.a - *getR8(R8);

	setZFlag(res == 0);
	setNFlag(true);
	setHFlag(LOWER_NIBBLE(regs.a) < LOWER_NIBBLE(*getR8(R8)));
	setCFlag(regs.a < *getR8(R8));
}

void GBZ80::addAimm8(std::vector<BYTE> instr) {
	BYTE a = regs.a;

	regs.a += instr[0];

	setZFlag(regs.a == 0x00);
	setNFlag(false);
	setHFlag(LOWER_NIBBLE(a) + LOWER_NIBBLE(instr[0]) > 0x0F);
	setCFlag(regs.a < a);
}

void GBZ80::adcAimm8(std::vector<BYTE> instr) {
	BYTE carry = (regs.f & CARRY_FLAG) >> 4;
	BYTE a = regs.a;

	regs.a = regs.a + instr[0] + carry;

	setZFlag(regs.a == 0);
	setNFlag(false);
	setHFlag(LOWER_NIBBLE(a) + LOWER_NIBBLE(instr[0]) + carry > 0x0F);
	setCFlag(regs.a < a || (regs.a == a && carry == 0x01));
}

void GBZ80::subAimm8(std::vector<BYTE> instr) {
	BYTE a = regs.a;

	regs.a -= instr[0];

	setZFlag(regs.a == 0);
	setNFlag(true);
	setHFlag(LOWER_NIBBLE(instr[0]) > LOWER_NIBBLE(a));
	setCFlag(regs.a > a);
}

void GBZ80::sbcAimm8(std::vector<BYTE> instr) {
	BYTE a = regs.a;
	BYTE carry = (regs.f & CARRY_FLAG) >> 4;

	regs.a = regs.a - instr[0] - carry;

	setZFlag(regs.a == 0);
	setNFlag(true);
	setHFlag(LOWER_NIBBLE(instr[0] + carry) > LOWER_NIBBLE(a));
	setCFlag(regs.a > a);
}

void GBZ80::andAimm8(std::vector<BYTE> instr) {
	regs.a = regs.a & instr[0];

	setZFlag(regs.a == 0);
	setNFlag(false);
	setHFlag(true);
	setCFlag(false);
}

void GBZ80::xorAimm8(std::vector<BYTE> instr) {
	regs.a = regs.a ^ instr[0];

	setZFlag(regs.a == 0);
	setNFlag(false);
	setHFlag(false);
	setCFlag(false);
}

void GBZ80::orAimm8(std::vector<BYTE> instr) {
	regs.a = regs.a | instr[0];

	setZFlag(regs.a == 0);
	setNFlag(false);
	setHFlag(false);
	setCFlag(false);
}

void GBZ80::cpAimm8(std::vector<BYTE> instr) {
	BYTE res = regs.a - instr[0];

	setZFlag(res == 0);
	setNFlag(true);
	setHFlag(LOWER_NIBBLE(regs.a) < LOWER_NIBBLE(instr[0]));
	setCFlag(regs.a < instr[0]);
}

void GBZ80::ret() {
	regs.pc = mem->readByte(regs.sp);
	regs.sp++;
	regs.pc |= (WORD)mem->readByte(regs.sp) << 8;
	regs.sp++;
}

void GBZ80::retCond(BYTE cond) {
	if (!evaluateCondition(cond))
		return;

	ret();
}

void GBZ80::reti() {
	ei();
	ret();
}

void GBZ80::jpCondimm16(BYTE cond, std::vector<BYTE> instr) {
	if (evaluateCondition(cond)) {
		jpimm16(instr);
	}
}

void GBZ80::jpimm16(std::vector<BYTE> instr) {
	regs.pc = ((WORD)instr[1]) << 8 | instr[0];
}

void GBZ80::jpHL() {
	regs.pc = regs.hl;
}


void GBZ80::callCondimm16(BYTE cond, std::vector<BYTE> instr) {
	if (evaluateCondition(cond)) {
		callimm16(instr);
	}
}

void GBZ80::callimm16(std::vector<BYTE> instr) {
	regs.sp--;
	mem->setByte(regs.sp, regs.pc >> 8);
	regs.sp--;
	mem->setByte(regs.sp, (BYTE)regs.pc);
	regs.pc = ((WORD)instr[1]) << 8 | instr[0];
}

void GBZ80::rstTgt(BYTE tgt) {
	regs.sp--;
	mem->setByte(regs.sp, regs.pc >> 8);
	regs.sp--;
	mem->setByte(regs.sp, (BYTE)regs.pc);
	regs.pc = (WORD)tgt;
}

void GBZ80::pushR16stk(BYTE r16stk) {
	WORD reg = *getR16stk(r16stk);
	regs.sp--;
	mem->setByte(regs.sp, (reg) >> 8);
	regs.sp--;
	mem->setByte(regs.sp, (BYTE)reg);
}

void GBZ80::popR16stk(BYTE r16stk) {
	WORD* reg = getR16stk(r16stk);
	*reg = mem->readByte(regs.sp);
	regs.sp++;
	*reg |= (WORD)mem->readByte(regs.sp) << 8;
	regs.sp++;
}

void GBZ80::ldhMemCA() {
	mem->setByte(0xFF00 | regs.c, regs.a);
}

void GBZ80::ldhPimm8A(std::vector<BYTE> instr) {
	mem->setByte(0xFF00 | instr[0], regs.a);
}

void GBZ80::ldPimm16A(std::vector<BYTE> instr) {
	WORD addr = ((WORD)instr[1]) << 8 | instr[0];
	mem->setByte(addr, regs.a);
}

void GBZ80::ldhAMemC() {
	regs.a = mem->readByte(0xFF00 | regs.c);
}

void GBZ80::ldhAPimm8(std::vector<BYTE> instr) {
	regs.a = mem->readByte(0xFF00 | instr[0]);
}

void GBZ80::ldAPimm16(std::vector<BYTE> instr) {
	WORD addr = ((WORD)instr[1]) << 8 | instr[0];
	regs.a = mem->readByte(addr);
}

void GBZ80::addSPimm8(std::vector<BYTE> instr) {
	WORD sp = regs.sp;
	SBYTE simm8 = (SBYTE)instr[0];
	regs.sp += simm8;

	setZFlag(0);
	setNFlag(0);
	setCFlag(LOWER_BYTE(sp) + (WORD)simm8 > 0x00FF);
	setHFlag(LOWER_NIBBLE(sp) + LOWER_NIBBLE((WORD)simm8) > 0x000F);
}

void GBZ80::ldHLSPimm8(std::vector<BYTE> instr) {
	WORD sp = regs.sp;
	SWORD simm8 = (SWORD)(SBYTE)instr[0];
	regs.hl = regs.sp + simm8;

	setZFlag(0);
	setNFlag(0);
	setCFlag(LOWER_BYTE(sp) + (WORD)simm8 > 0x00FF);
	setHFlag(LOWER_NIBBLE(sp) + LOWER_NIBBLE((WORD)simm8) > 0x000F);
}

void GBZ80::ldSPHL() {
	regs.hl = regs.sp;
}

void GBZ80::di() {
	ime = false;
}

void GBZ80::ei() {
	ime = true;
}
