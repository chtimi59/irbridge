CC=gcc
IRCODECPATH=../../ircodec

CFLAGS=-W -Wall -ansi -pedantic -I$(IRCODECPATH)/include
LDFLAGS=-L$(IRCODECPATH)/lib
EXEC=test
OBJS = main.o iokey.o

all: $(EXEC)

$(IRCODECPATH)/lib/libircodec.a :
	@(cd $(IRCODECPATH)/linux && $(MAKE))

test: $(OBJS) $(IRCODECPATH)/lib/libircodec.a
	$(CC) -o  $@  $^ -lircodec $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -f test
	@rm -f *.o