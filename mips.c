#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "mips.h"

unsigned int mem[1024];
MB_HDR mb_hdr;

void check_number_of_arguments(int argc) {
   
   if (argc < 3) {
      fprintf(stderr, "Please provide the correct number of arguments.\n");
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

void read_in_instructions(FILE * file, int * mem_pointer) {

   int n;

   while (*mem_pointer < sizeof(mem)) {
      n = fread((void *) &mem[*mem_pointer/4], 4, 1, file);

      if (n)
         *mem_pointer += 4;
      else
         break;
   }
      
   return;
}

void print_instructions(int * mem_pointer) {

   int i;

   for (i = 0; i < *mem_pointer; i += 4) {
      printf("%08x : %08x\n", i, mem[i/4]);
   }

   return;
}

int main (int argc, char * argv[]) {
   
   FILE * file;
   char * filename;
   int mem_pointer = 0;

   check_number_of_arguments(argc);
   filename = get_filename(argc, argv);
   file = open_file(filename);

   read_in_instructions(file, &mem_pointer);   
   fclose(file);

   print_instructions(&mem_pointer);

   return EXIT_SUCCESS;
}











