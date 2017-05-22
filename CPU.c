#include <stdio.h>
#include <stdlib.h>

/* Generic Functions/Components */
unsigned mux21(unsigned i0, unsigned i1, unsigned sel)
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

unsigned mux41(unsigned i0, unsigned i1, unsigned i2, unsigned i3, unsigned sel)
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

unsigned reg(unsigned input, unsigned write, unsigned *reg)
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
unsigned memory(unsigned addr, unsigned writeData, unsigned MemWrite, unsigned MemRead, unsigned *memory[32])
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

void instrucReg(unsigned memData, unsigned IRWrite, unsigned *opcode,\
   unsigned *rs, unsigned *rt, unsigned *rd, unsigned *immed, unsigned *funct)
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
      *funct = memData & 0x0000003F;
   }
   else if (IRWrite  != 0)
   {
      fprintf(stderr, "Invalid IRWrite\n");
      exit(EXIT_FAILURE);
   }
}

void registers(unsigned readR1, unsigned readR2, unsigned writeR,\
   unsigned writeD, unsigned RegWrite, unsigned *readD1, unsigned *readD2,\
   unsigned *registers[32])
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

unsigned ALU(unsigned a, unsigned b, unsigned ALUOp, unsigned *zero)
{
   unsigned ret;
   if (ALUOp == 0)
      ret = a + b;
   else if (ALUOp == 1)
      ret = a - b;
   else if (ALUOp == 2)
      ret = a & b;
   else if (ALUOp == 3)
      ret = a | b;
   else if (ALUOp == 4)
      ret = a ^ b;
   else if (ALUOp == 5)
      ret = ~(a | b);
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
