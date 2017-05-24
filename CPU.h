#ifndef CPU_H
#define CPU_H

int mux21(int i0, int i1, int sel);
int mux41(int i0, int i1, int i2, int i3, int sel);
int reg(int input, int write, int *reg);
int memory(int addr, int writeData, int MemWrite, int MemRead, int *memory[32]);
void registers(int readR1, int readR2, int writeR,\
   int writeD, int RegWrite, int *readD1, int *readD2,\
   int *registers[32]);
int ALU(int a, int b, int ALUOp, int *zero);
int signExtend(int immed);
int shiftLeft2(int seImmed);

#endif
