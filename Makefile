CC = gcc
CFLAGS = -Wall -std=gnu99 -pedantic -g
MAIN = MIPS
OBJS = mips.o readline.o

all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

mips.o : mips.c mips.h
	$(CC) $(CFLAGS) -c mips.c

readline.o : readline.c readline.h
	$(CC) $(CFLAGS) -c readline.c

clean :
	rm $(MAIN) $(OBJS)
