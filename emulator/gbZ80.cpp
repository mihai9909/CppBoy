#include "gbZ80.h"

GBZ80::GBZ80(Memory* memory) : mem(memory)
{
	regs = Regs();
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
		case JR_COND_IMM8: jrcondimm8(cond, instr); return;
		case STOP: stop(); return;
		case HALT: halt(); return;
		default: break;
	}

	switch (opCode & (~BITS_34))
	{
	case JR_IMM8: jrimm8(instr); return;
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
	default: break;
	}

	switch (opCode & (~BITS_012)) {
	case ADD_A_R8: addAR8(r8_low);
	case ADC_A_R8: adcAR8(r8_low);
	case SUB_A_R8: subAR8(r8_low);
	case SBC_A_R8: sbcAR8(r8_low);
	case AND_A_R8: andAR8(r8_low);
	case XOR_A_R8: xorAR8(r8_low);
	case OR_A_R8: orAR8(r8_low);
	case CP_A_IMM8: cpAR8(r8_low);
	default: break;
	}

	switch (opCode & (~BITS_345))
	{
	case INC_R8: incR8(r8); return;
	case DEC_R8: decR8(r8); return;
	case LD_R8_IMM8: loadR8imm8(r8, instr); return;
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

void GBZ80::rlca() {
	BYTE msb = regs.a >> 7;
	regs.a = (regs.a << 1) | msb;
}

void GBZ80::rrca() {
	BYTE lsb = regs.a << 7;
	regs.a = (regs.a >> 1) | lsb;
}

void GBZ80::rla() {
	regs.a = regs.a << 1;
}

void GBZ80::rra() {
	regs.a = regs.a >> 1;
}

void GBZ80::daa() {
	// TODO: decimal adjust accumulator
}

void GBZ80::cpl() {
	regs.a = ~regs.a;
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
	regs.a += *getR8(R8);
}

void GBZ80::adcAR8(BYTE R8) {
	addAR8(R8);
	if ((regs.f & CARRY_FLAG) != 0)
		regs.a++; // add carry flag
}

void GBZ80::subAR8(BYTE R8) {
	regs.a -= *getR8(R8);
}

void GBZ80::sbcAR8(BYTE R8) {
	subAR8(R8);
	if ((regs.f & CARRY_FLAG) != 0)
		regs.a--; // subtract carry flag
}

void GBZ80::andAR8(BYTE R8) {
	regs.a = regs.a & *getR8(R8);
}

void GBZ80::xorAR8(BYTE R8) {
	regs.a = regs.a ^ *getR8(R8);
}

void GBZ80::orAR8(BYTE R8) {
	regs.a = regs.a | *getR8(R8);
}

void GBZ80::cpAR8(BYTE R8) {
	BYTE res = regs.a - *getR8(R8);

	setZFlag(res == 0);
	setNFlag(true);
	setHFlag(LOWER_NIBBLE(regs.a) < LOWER_NIBBLE(*getR8(R8)));
	setCFlag(regs.a < *getR8(R8));
}

void GBZ80::addAimm8(std::vector<BYTE> instr) {
	regs.a += instr[0];
}

void GBZ80::adcAimm8(std::vector<BYTE> instr) {
	regs.a += instr[0];
	if ((regs.f & CARRY_FLAG) != 0)
		regs.a++;
}

void GBZ80::subAimm8(std::vector<BYTE> instr) {
	regs.a -= instr[0];
}

void GBZ80::sbcAimm8(std::vector<BYTE> instr) {
	regs.a -= instr[0];
	if ((regs.f & CARRY_FLAG) != 0)
		regs.a--;
}

void GBZ80::andAimm8(std::vector<BYTE> instr) {
	regs.a = regs.a & instr[0];
}

void GBZ80::xorAimm8(std::vector<BYTE> instr) {
	regs.a = regs.a ^ instr[0];
}

void GBZ80::orAimm8(std::vector<BYTE> instr) {
	regs.a = regs.a | instr[0];
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
