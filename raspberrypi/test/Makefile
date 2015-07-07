CC=gcc
IRCODECPATH=../../ircodec

CFLAGS=-W -Wall -ansi -pedantic -I$(IRCODECPATH)/include
LDFLAGS=-L$(IRCODECPATH)/lib -lircodec
EXEC=test
OBJS = main.o iokey.o

all: $(EXEC)

test: $(OBJS)
	$(CC) -o  $@  $^  $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -f test
	@rm -f *.o