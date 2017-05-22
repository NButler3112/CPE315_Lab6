#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mips_asm_header.h"

char* regs[] = 
{
   "$zero\0",
   "$at\0",
   "$v0\0",
   "$v1\0",
   "$a0\0",
   "$a1\0",
   "$a2\0",
   "$a3\0",
   "$t0\0",
   "$t1\0",
   "$t2\0",
   "$t3\0",
   "$t4\0",
   "$t5\0",
   "$t6\0",
   "$t7\0",
   "$s0\0",
   "$s1\0",
   "$s2\0",
   "$s3\0",
   "$s4\0",
   "$s5\0",
   "$s6\0",
   "$s7\0",
   "$t8\0",
   "$t9\0",
   "$k1\0",
   "$k2\0",
   "$gp\0",
   "$sp\0",
   "$fp\0",
   "$ra\0"
};

char* opcode0[] = 
{
   "R Type\0",
   "Invalid\0",
   "Jump\0",
   "Jump and Link\0",
   "Branch On Equal\0",
   "Branch On Not Equal\0",
   "Invalid\0",
   "Invalid\0",
   "Add Immediate\0",
   "Add Immediate Unsigned\0",
   "Set Less Than Immediate\0",
   "Set Less Than Immediate Unsigned\0", 
   "And Immediate\0",
   "Or Immediate\0",
   "Xor Immediate\0",
   "Load Upper Immediate\0"
};

char* opcode2[] = 
{
   "Load Byte\0",
   "Load Halfword\0",
   "Invalid\0",
   "Load Word\0",
   "Load Byte Unsigned\0",
   "Load Halfword Unsigned\0",
   "Invalid\0",
   "Invalid\0",
   "Store Byte\0",
   "Store Halfword\0",
   "Invalid\0",
   "Store Word\0"
};

char* function0[] = 
{
   "Shift Left Logical\0",
   "Invalid\0",
   "Shift Right Logical\0",
   "Shift Right Arithmetic\0",
   "Shift Left Logical Variable\0",
   "Invalid\0",
   "Shift Right Logical Variable\0",
   "Shift Right Arithmetic Variable\0",
   "Jump Register\0",
   "Jump and Link Register\0"
};

char* function2[] = 
{
   "Add\0",
   "Add Unsigned\0",
   "Subtract\0",
   "Subtract Unsigned\0",
   "And\0",
   "Or\0",
   "Xor\0",
   "Nor\0",
   "Invalid\0",
   "Invalid\0",
   "Set Less Than\0",
   "Set Less Than Unsigned\0"
};

static unsigned int findInstruc(unsigned int opcode)
{
   if (opcode == 0x00)
   {
      return 0;   /*Indicates R type instruction*/
   }
   else if (opcode == 0x02 || opcode == 0x03)
   {
      return 2;   /*Indicates J type instruction*/
   }
   else if (opcode == 0x04 || opcode == 0x05 || opcode == 0x08\
      || opcode == 0x09 || opcode == 0x0A || opcode == 0x0B || opcode == 0x0C\
      || opcode == 0x0D || opcode == 0x0E || opcode == 0x0F || opcode == 0x20\
      || opcode == 0x21 || opcode == 0x23 || opcode == 0x24 || opcode == 0x25\
      || opcode == 0x28 || opcode == 0x29 || opcode == 0x2B)
   {
      return 1;   /*Indicates I type instruction*/
   }
   else
   {
      return -1;  /*Indicates invalid type instruction*/
   }
}

static void rType(unsigned int instr, unsigned int opcode)
{
   unsigned int rs, rt, rd, shamt, funct;
   rs = instr & 0x03E00000;
   rs >>= 21;
   rt = instr & 0x001F0000;
   rt >>= 16;
   rd = instr & 0x0000F800;
   rd >>= 11;
   shamt = instr & 0x000007A;
   shamt >>= 6;
   funct = instr & 0x0000003F;

   printf("   Register Instruction: ");
   if (funct < 0x1F)
   {
      printf("%s (0x00/0x%02X)\n", function0[funct], funct);
   }
   else
   {
      printf("%s (0x00/0x%02X)\n", function2[funct - 0x20], funct);
   }
   printf("   Rs: %s (0x%02X), Rt: %s (0x%02X), Rd: %s (0x%02X), Shamt: %d\n",\
      regs[rs], rs, regs[rt], rt, regs[rd], rd, shamt);

   if (funct == 0x00 || funct == 0x02 || funct == 0x03)
   {
      printf("%s is shifted %d bits and stored in %s\n", regs[rt], shamt, regs[rd]);
   }
   else if (funct == 0x04 || funct == 0x06 || funct == 0x07)
   {
      printf("%s is shifted by bits indicated in register %s and stored in %s\n",\
         regs[rt], regs[rs], regs[rd]);
   }
}

static unsigned int branchAddr(unsigned int immed)
{
   unsigned int brAdd = 0x00000000, immed15 = immed & 0x8000,\
      immedS = immed << 2;

   if (immed15 == 1)
   {
      brAdd |= 0xFFFC0000;
   }
   brAdd |= immedS;
   return brAdd;
}

static unsigned int zeroExtImmed(unsigned int immed)
{
   int ret = 0x00000000 | immed;
   return ret;
}

static unsigned int signExtImmed(unsigned int immed)
{
   int ret = 0x00000000 | immed, immed15 = immed & 0x8000;
   
   if (immed15 == 1)
   {
      ret |= 0xFFFF0000;
   }
   return ret;
}

static void iType(unsigned int instr, unsigned int opcode)
{
   char *a = "Immediate:\0", *b = "Branch Address:\0", *s = "Sign Extended Immediate:\0",\
      *z = "Zero Extended Immediate:\0";
   unsigned int rs, rt, immed;
   rs = instr & 0x03E00000;
   rs >>= 21;
   rt = instr & 0x001F0000;
   rt >>= 16;
   immed = instr & 0x0000FFFF;

   if (opcode == 0x04 || opcode == 0x05)
   {
      immed = branchAddr(immed);
      a = b;
   }
   else if (opcode == 0x08 || opcode == 0x09 || opcode == 0x0A\
      || opcode == 0x0B || opcode == 0x20 || opcode == 0x21 || opcode == 0x23\
      || opcode == 0x24 || opcode == 0x28 || opcode == 0x29 || opcode == 0x2B)
   {
      immed = signExtImmed(immed);
      a = s;
   }
   else if (opcode == 0x0C || opcode == 0x0D || opcode == 0x0E\
      || opcode == 0x25)
   {
      immed = zeroExtImmed(immed);
      a = z;
   }

   printf("   Immediate Instruction: ");
   if (opcode < 0x1F)
   {
      printf("%s (0x%02X)\n", opcode0[opcode], opcode);
   }
   else
   {
      printf("%s (0x%02X)\n", opcode2[opcode - 0x20], opcode);
   }

   printf("   Rs: %s (0x%02X), Rt: %s (0x%02X), %s 0x%08X\n", regs[rs], rs,\
      regs[rt], rt, a, immed);

}

static void jType(int pc, unsigned int instr, unsigned int opcode)
{
   unsigned int tempPC, tempWord, word = 0x00000000;
   tempPC = pc & 0xF0000000;
   word |= tempPC;
   tempWord = instr & 0x03FFFFFF;
   tempWord <<= 2;
   word |= tempWord;

   printf("   Jump Instruction: %s\n", opcode0[opcode]);
   printf("   Jump Address: 0x%08X\n", word);
}

void mainDecoder(int i, unsigned int m)
{
   unsigned int opcode, sOpcode, mask = 0xFC000000;
   opcode = m & mask;
   opcode >>= 26;
   sOpcode = findInstruc(opcode);
   if (sOpcode == -1)
   {
      printf("Invalid Instruction: %08X\n", m);
   }
   else if (sOpcode == 0)
   {
      rType(m, opcode);
   }
   else if (sOpcode == 1)
   {
      iType(m, opcode);
   }
   else if (sOpcode == 2)
   {
      jType(i, m, opcode);
   }
   printf("\n");
}
