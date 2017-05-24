#include <stdio.h>
#include <stdlib.h>

/* Generic Functions/Components */
int mux21(int i0, int i1, int sel)
{
   if (sel == 0)
   {
      return i0;
   }
   else if (sel == 1)
   {
      return i1;
   }
   else
   {
      fprintf(stderr, "Incorrect 2x1 Mux Selector\n");
      exit(EXIT_FAILURE);
   }
}

int mux41(int i0, int i1, int i2, int i3, int sel)
{
   if (sel == 0)
   {
      return i0;
   }
   else if (sel == 1)
   {
      return i1;
   }
   else if (sel == 2)
   {
      return i2;
   }
   else if (sel == 3)
   {
      return i3;
   }
   else
   {
      fprintf(stderr, "Incorrect 4x1 Mux Selector\n");
      exit(EXIT_FAILURE);
   }
}

int reg(int input, int write, int *reg)
{
   if (write == 1)
   {
      *reg = input;
   }
   else if (write != 0)
   {
      fprintf(stderr, "Invalid Write Signal\n");
      exit(EXIT_FAILURE);
   }
   return *reg;
}

/* Specific Functions/Components */
int memory(int addr, int writeData, int MemWrite, int MemRead, int *memory[32])
{
   if (addr > 32)
   {
      fprintf(stderr, "Invalid Memory Address\n");
      exit(EXIT_FAILURE);
   }
   if (MemWrite == 0 && MemRead == 1)
   {
      return (*memory)[addr];
   }
   else if (MemWrite == 1 && MemRead == 0)
   {
      (*memory)[addr] = writeData;
      return (*memory)[addr];
   }
   else
   {
      fprintf(stderr, "Invalid MemWrite/MemRead\n");
      exit(EXIT_FAILURE);
   }
   return (*memory)[addr];
}

void instrucReg(int memData, int IRWrite, int *opcode,\
   int *rs, int *rt, int *rd, int *shamt, int *immed, int *funct)
{
   if (IRWrite == 1)
   {
      *opcode = memData & 0xFC000000;
      *opcode >>= 26;
      *rs = memData & 0x03E00000;
      *rs >>= 21;
      *rt = memData & 0x001F0000;
      *rt >>= 16;
      *rd = memData & 0x0000F800;
      *rd >>= 11;
      *shamt = memData & 0x000007C0;
      *shamt >>= 6;
      *funct = memData & 0x0000003F;
   }
   else if (IRWrite  != 0)
   {
      fprintf(stderr, "Invalid IRWrite\n");
      exit(EXIT_FAILURE);
   }
}

void registers(int readR1, int readR2, int writeR,\
   int writeD, int RegWrite, int *readD1, int *readD2,\
   int *registers[32])
{
   if (writeR > 32 || readR1 > 32 || readR2 > 32)
   {
      fprintf(stderr, "Invalid Write/Read Register 1/Read Register 2\n");
      exit(EXIT_FAILURE);
   }
   if (RegWrite == 1)
   {
      (*registers)[writeR] = writeD;
   }
   else if (RegWrite != 0)
   {
      fprintf(stderr, "Invalid RegWrite\n");
      exit(EXIT_FAILURE);
   }
   *readD1 = (*registers)[readR1];
   *readD2 = (*registers)[readR2];
}

int ALU(int a, int b, int ALUOp, int *zero)
{
   int ret;
   *zero = 0;
   if (ALUOp == 0) /*add*/
      ret = a + b;
   else if (ALUOp == 1) /*sub*/
      ret = a - b;
   else if (ALUOp == 2) /*and*/
      ret = a & b;
   else if (ALUOp == 3) /*or*/
      ret = a | b;
   else if (ALUOp == 4) /*xor*/
      ret = a ^ b;
   else if (ALUOp == 5) /*nor*/
      ret = ~(a | b);
   else if (ALUOp == 6) /*srl*/
      ret = (unsigned)a >> b;
   else if (ALUOp == 7) /*sra*/
      ret = a >> b;
   else if (ALUOp == 8) /*sll*/
      ret = a << b;
   else if (ALUOp == 9) /*slt*/
   {
      ret = a - b;
      if (ret < 0)
         ret = 1;
      else
         ret = 0;
   }
   else if (ALUOp == 10) /*sltu*/
   {
      ret = (unsigned)a - (unsigned)b;
      if (ret < 0)
         ret = 1;
      else
         ret = 0;
   }
   else if (ALUOp == 11) /*lui*/
   {
      b <<= 16;
      ret = a & 0xFFFF0000;
      ret = a | b;
   }
   else
   {
      fprintf(stderr, "Invalid ALUOp\n");
      exit(EXIT_FAILURE);
   }
   if (ret == 0)
   {
      *zero = 1;
   }
   return ret;
}

int signExtend(int immed)
{
   int ret = 0x00000000 | immed, immed15 = immed & 0x8000;

   if (immed15 == 1)
      ret |= 0xFFFF0000;
   return ret;
}

int shiftLeft2(int seImmed)
{
   return seImmed << 2;
}
