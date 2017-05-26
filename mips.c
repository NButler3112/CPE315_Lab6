#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "mips.h"
#include "readline.h"

unsigned int mem[1024];
MB_HDR mb_hdr;
unsigned int mem_pointer = 0;

unsigned int pc = 0;
unsigned int instruction = 0;
unsigned int registers[] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0
};

unsigned int instr_count = 0;
unsigned int clock_count = 0;
unsigned int memref_count = 0;

void check_number_of_arguments(int argc) {
   
   if (argc != 3) {
      fprintf(stderr, "Please provide the correct number of arguments.\n");
      fprintf(stderr, "\t./MIPS testcase [-s|-r]\n");
      exit(EXIT_FAILURE);
   }

   return;
}

void step_or_run(char * arg, int * flag) {

   if (strcmp(arg, "-s") == 0) {
      *flag = 1;
   } else if (strcmp(arg, "-r") == 0) {
      *flag = 2;
   } else {
      fprintf(stderr, "Please provide the correct command line flag.\n");
      fprintf(stderr, "\t./MIPS testcase [-s|-r]\n");
      exit(EXIT_FAILURE);
   }
   
   return;

}

char * get_filename(int argc, char * argv[]) {
   struct stat buffer;
   char * filename = argv[1];

   if (stat(filename, &buffer) == -1) {
      fprintf(stderr, "File named '%s' does not exist. Pass in an existing file.\n", filename);
      exit(EXIT_FAILURE);
   }

   return filename;
}

FILE * open_file(char * filename) {
   FILE * file;

   file = fopen(filename, "rb");

   if (file == NULL) {
      fprintf(stderr, "Couldn't open '%s'.  Quitting.\n", filename);
      exit(EXIT_FAILURE);
   }

   printf("Opened file named '%s'. Let's check it's .mb\n", filename);

   fread((void *) &mb_hdr, sizeof(mb_hdr), 1, file);
   
   if (strcmp(mb_hdr.signature, "~MB") != 0) {
      fprintf(stderr, "'%s' is not a mips_asm binary file.  Quitting.\n", filename);
      fclose(file);
      exit(EXIT_FAILURE);
   }
   
   printf("Correct file type. Program size = %d bytes.\n\n", mb_hdr.size);

   return file;
}

void read_in_instructions(FILE * file) {

   int n;

   while (mem_pointer < sizeof(mem)) {
      n = fread((void *) &mem[mem_pointer/4], 4, 1, file);

      if (n)
         mem_pointer += 4;
      else
         break;
   }
      
   return;
}

void print_instructions() {

   int i;
   
   printf("Instructions in memory:\n");

   for (i = 0; i < mem_pointer; i += 4) {
      printf("  %08x : %08x\n", i, mem[i/4]);
   }

   printf("\n");

   return;
}

void print_registers() {

   int i;
   
   printf("\nRegisters:\n");

   for (i = 0; i < 32; i++){
      printf("  $%2d : 0x%08x\n", i, registers[i]);
   }

   return;
}

void fetch() {

   instruction = mem[pc/4];
   pc += 4;
   
   clock_count += 1;
   return;
}

void decode() {
   unsigned int opcode = (instruction & 0xFC000000) >> 26;
   clock_count += 1;
   
   switch (opcode) {
      case 0x00:
         execute_r(opcode);
         break;
      case 0x02:
      case 0x03:
         execute_j(opcode);
         break;
      case 0x04:
      case 0x05:
      case 0x08:
      case 0x09:
      case 0x0A:
      case 0x0B:
      case 0x0C:
      case 0x0D:
      case 0x0E:
      case 0x0F:
      case 0x20:
      case 0x21:
      case 0x23:
      case 0x24:
      case 0x25:
      case 0x28:
      case 0x29:
      case 0x2B:
         execute_i(opcode);
         break;
      default:
         /*invalid_instruction();*/
         break;
   }
   
   return;
}

void execute_r(unsigned int opcode) {

   unsigned int rs = (instruction & 0x03E00000) >> 21;
   unsigned int rt = (instruction & 0x001F0000) >> 16;
   unsigned int rd = (instruction & 0x0000F800) >> 11;
   unsigned int shamt = (instruction & 0x000007C0) >> 6;
   unsigned int funct = instruction & 0x0000003F;
   
   clock_count += 1;
   instr_count += 1;

   switch (funct) {
      case 0x00:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = (unsigned int) registers[rt] << shamt;
         
         break;
      case 0x02:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = (unsigned int) registers[rt] >> shamt;

         break;
      case 0x03:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = (signed int) registers[rt] >> shamt;

         break;
      case 0x04:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = (unsigned int) registers[rt] << (unsigned int) registers[rs];
         
         break;
      case 0x06:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = (unsigned int) registers[rt] >> (unsigned int) registers[rs];

         break;
      case 0x07:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = (signed int) registers[rt] >> (unsigned int) registers[rs];

         break;
      case 0x08:
         clock_count += 1;

         pc = (unsigned int) registers[rs];

         break;
      case 0x09:
         clock_count += 1;

         registers[31] = (unsigned int) pc;
         pc = (unsigned int) registers[rs];

         break;
      case 0x20:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = (signed int) registers[rs] + (signed int) registers[rt];
         
         break;
      case 0x21:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = (unsigned int) registers[rs] + (unsigned int) registers[rt];

         break;
      case 0x22:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = (signed int) registers[rs] - (signed int) registers[rt];

         break;
      case 0x23:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = (unsigned int) registers[rs] - (unsigned int) registers[rt];

         break;
      case 0x24:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = registers[rs] & registers[rt];

         break;
      case 0x25:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = registers[rs] | registers[rt];

         break;
      case 0x26:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = registers[rs] ^ registers[rt];

         break;
      case 0x27:
         clock_count += 1;

         if (rd != 0)
            registers[rd] = ~(registers[rs] | registers[rt]);

         break;
      case 0x2A:
         clock_count += 1;

         if (rd != 0) {
            if ((signed int) registers[rs] < (signed int) registers[rt])
               registers[rd] = 1;
            else
               registers[rd] = 0;
         }

         break;
      case 0x2B:
         clock_count += 1;

         if (rd != 0) {
            if ((unsigned int) registers[rs] < (unsigned int) registers[rt])
               registers[rd] = 1;
            else
               registers[rd] = 0;
         }

         break;
      default:
         
         if (instruction == 0x0000000C) {
            complete();
            exit(EXIT_SUCCESS);
         }
         
         break;
   }

   return;
}

void execute_j(unsigned int opcode) {

   unsigned int word_index = (instruction & 0x03FFFFFF) << 2;
   unsigned int jump_addr = (pc & 0xF0000000) | word_index;

   clock_count += 1;
   instr_count += 1;

   switch (opcode) {
      case 0x02:
         clock_count += 1;
         pc = jump_addr;
         break;
      case 0x03:
         clock_count += 1;
         registers[31] = pc;
         pc = jump_addr;
         break;
      default:
         invalid_instruction();
         break;
   }

   return;
}

unsigned int make_branch_addr(unsigned int immed) {

   unsigned int branch_addr;
   
   if ( (immed & 0x00008000) == 0x00008000) {
      branch_addr = 0xFFFC0000 | (immed << 2);
   } else {
      branch_addr = 0x0000FFFF & (immed << 2);
   }

   return branch_addr;
}

signed int make_sign_immed(unsigned int immed) {

   signed int sign_immed;

   if ( (immed & 0x00008000) == 0x00008000) {
      sign_immed = 0xFFFF0000 | immed;
   } else {
      sign_immed = 0x0000FFFF & immed;
   }

   return sign_immed;
}

unsigned int make_zero_immed(unsigned int immed) {
   
   unsigned int zero_immed;

   zero_immed = 0x03FFFFFF & immed;

   return zero_immed;
}

unsigned int sign_extend_byte(unsigned int value) {
   
   unsigned int sign_extend_byte;

   if ( (value & 0x00000080) == 0x00000080)
      sign_extend_byte = 0xFFFFFF00 | value;
   else
      sign_extend_byte = 0x000000FF & value;

   return sign_extend_byte;
}

unsigned int sign_extend_halfword(unsigned int value) {
   
   unsigned int sign_extend_halfword;

   if ( (value & 0x00008000) == 0x00008000)
      sign_extend_halfword = 0xFFFF0000 | value;
   else
      sign_extend_halfword = 0x0000FFFF & value;

   return sign_extend_halfword;
}

void execute_i(unsigned int opcode) {
   
   unsigned int rs = (instruction & 0x03E00000) >> 21;
   unsigned int rt = (instruction & 0x001F0000) >> 16;
   unsigned int immed = instruction & 0x0000FFFF;

   unsigned int branch_addr;
   unsigned int zero_immed;
   signed int sign_immed;

   clock_count += 1;
   instr_count += 1;

   switch (opcode) {
      case 0x04:
         clock_count += 1;
         branch_addr = make_branch_addr(immed);
         
         if (registers[rs] == registers[rt])
            pc += branch_addr;
         
         break;
      case 0x05:
         clock_count += 1;
         branch_addr = make_branch_addr(immed);

         if (registers[rs] != registers[rt])
            pc += branch_addr;
      
         break;
      case 0x08:
         clock_count += 1;
         sign_immed = make_sign_immed(immed);
         
         if (rt != 0)
            registers[rt] = registers[rs] + sign_immed;

         break;
      case 0x09:
         clock_count += 1;
         zero_immed = make_zero_immed(immed);

         if (rt != 0)
            registers[rt] = registers[rs] + zero_immed;

         break;
      case 0x0A:
         clock_count += 1;
         sign_immed = make_sign_immed(immed);

         if (rt != 0) {
            if (registers[rs] < sign_immed)
               registers[rt] = 1;
            else
               registers[rt] = 0;
         }

         break;
      case 0x0B:
         clock_count += 1;
         zero_immed = make_zero_immed(immed);

         if (rt != 0) {
            if (registers[rs] < zero_immed)
               registers[rt] = 1;
            else
               registers[rt] = 0;
         }

         break;
      case 0x0C:
         clock_count += 1;
         zero_immed = make_zero_immed(immed);

         if (rt != 0)
            registers[rt] = registers[rs] & zero_immed;

         break;
      case 0x0D:
         clock_count += 1;
         zero_immed = make_zero_immed(immed);

         if (rt != 0)
            registers[rt] = registers[rs] | zero_immed;

         break;
      case 0x0E:
         clock_count += 1;
         zero_immed = make_zero_immed(immed);

         if (rt != 0)
            registers[rt] = registers[rs] ^ zero_immed;

         break;
      case 0x0F:
         clock_count += 1;

         if (rt != 0)
            registers[rt] = registers[rs] << 16;
      
         break;
      case 0x20:
         clock_count += 2;
         memref_count += 1;
         sign_immed = make_sign_immed(immed);
         
         if (rt != 0)
            registers[rt] = sign_extend_byte(mem[registers[rs] + sign_immed] & 0x000000FF);
       
         break;
      case 0x21:
         clock_count += 2;
         memref_count += 1;
         sign_immed = make_sign_immed(immed);

         if (rt != 0)
            registers[rt] = sign_extend_halfword(mem[registers[rs] + sign_immed] & 0x0000FFFF);

         break;
      case 0x23:
         clock_count += 2;
         memref_count += 1;
         sign_immed = make_sign_immed(immed);

         if (rt != 0)
            registers[rt] = mem[registers[rs] + sign_immed];

         break;
      case 0x24:
         clock_count += 2;
         memref_count += 1;
         zero_immed = make_zero_immed(immed);

         if (rt != 0)
            registers[rt] = 0x000000FF & sign_extend_byte(mem[registers[rs] + zero_immed] & 0x000000FF);

         break;
      case 0x25:
         clock_count += 2;
         memref_count += 1;
         zero_immed = make_zero_immed(immed);

         if (rt != 0)
            registers[rt] = 0x0000FFFF & sign_extend_byte(mem[registers[rs] + zero_immed] & 0x0000FFFF);
      
         break;
      case 0x28:
         clock_count += 2;
         memref_count += 1;
         sign_immed = make_sign_immed(immed);

         mem[registers[rs] + sign_immed] = registers[rt] & 0x000000FF;
      
         break;
      case 0x29:
         clock_count += 2;
         memref_count += 1;
         sign_immed = make_sign_immed(immed);

         mem[registers[rs] + sign_immed] = registers[rt] & 0x0000FFFF;
      
         break;
      case 0x2B:
         clock_count += 2;
         memref_count += 1;
         sign_immed = make_sign_immed(immed);

         mem[registers[rs] + sign_immed] = registers[rt];
   
         break;
      default:
         invalid_instruction();
         break;
   }

   return;
}

void invalid_instruction() {

   fprintf(stderr, "Instruction 0x%08x at 0x%08x is invalid.  Quitting.\n", instruction, pc);
   exit(EXIT_FAILURE);

   return;
}

void complete() {

   print_registers();

   printf("\nInstructions Run:  %d\n", instr_count);
   printf("Clock Cycles:      %d\n", clock_count);
   printf("Memory References: %d\n", memref_count);

   printf("\n\tCPI: %f\n\n", (float) clock_count / (float) instr_count);

   return;
}

void run_instructions() {
   
   printf("Running instructions.\n\n");

   while (pc != mem_pointer) {
      fetch();
      decode();
   }
   
   complete();

   return;
}

void step_instructions() {

   printf("Stepping through instructions.\n");

   char * line;

   while (pc != mem_pointer) {

      printf("\nPress ENTER to step.\n");

      line = readline(stdin);
      while (strcmp(line, "\n") != 0) {
         line = readline(stdin);
      }

      fetch();
      decode();

      print_registers();
   }

   complete();

   return;
}

int main (int argc, char * argv[]) {
   
   FILE * file;
   char * filename;
   int flag = 0;

   check_number_of_arguments(argc);
   step_or_run(argv[2], &flag);
   filename = get_filename(argc, argv);
   file = open_file(filename);

   read_in_instructions(file);   
   fclose(file);

   print_instructions();
   
   if (flag == 1) {
      step_instructions();
   } else if (flag == 2) {
      run_instructions();
   }

   return EXIT_SUCCESS;
}











