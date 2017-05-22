CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = load_testcase
OBJS = Lab5.o load_testcase.o

all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

Lab5.o : Lab5.c Lab5.h
	$(CC) $(CFLAGS) -c Lab5.c

load_testcase.o : load_testcase.c mips_asm_header.h
	$(CC) $(CFLAGS) -c load_testcase.c

clean:
	rm *.o $(MAIN)
