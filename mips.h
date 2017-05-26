#ifndef MIPS
#define MIPS

typedef struct _mb_hdr {
   char signature[4];
   unsigned int size;
   unsigned int entry;
   unsigned int filler1;
   unsigned char filler2[64-16];
} MB_HDR, *MB_HDR_PTR;

void check_number_of_arguments(int argc);
void step_or_run(char * arg, int * flag);
char * get_filename(int argc, char * argv[]);
FILE * open_file(char * filename);
void read_in_instructions(FILE * file);
void print_instructions();
void print_registers();
void fetch();
void decode();
void execute_r(unsigned int opcode);
void execute_j(unsigned int opcode);
unsigned int make_branch_addr(unsigned int immed);
signed int make_sign_immed(unsigned int immed);
unsigned int make_zero_immed(unsigned int immed);
unsigned int sign_extend_byte(unsigned int value);
void execute_i(unsigned int opcode);
void invalid_instruction();
void complete();
void run_instructions();
void step_instructions();
int main (int argc, char * argv[]);

#endif
