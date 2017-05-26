#ifndef LAB5_H
#define LAB5_H

unsigned int findInstruc (unsigned int opcode);
unsigned int branchAddr (unsigned int immed);
unsigned int zeroExtImmed (unsigned int immed);
unsigned int signExtImmed (unsigned int immed);

void rType (unsigned int instr, unsigned int opcode);
void iType (unsigned int instr, unsigned int opcode);
void jType (int pc, unsigned int instr, unsigned int opcode);

void mainDecoder(int pc, unsigned int m);

#endif
