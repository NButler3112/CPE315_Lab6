CC = gcc
CFLAGS = -Wall -std=gnu99 -pedantic
MAIN = MIPS
OBJS = mips.o

all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

clean :
	rm $(MAIN) $(OBJS)
