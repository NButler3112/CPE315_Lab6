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
char * get_filename(int argc, char * argv[]);
FILE * open_file(char * filename);
void read_in_instructions(FILE * file, int * mem_pointer);
void print_instructions(int * mem_pointer);
int main (int argc, char * argv[]);

#endif
