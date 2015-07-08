CC=gcc
IRCODECPATH=../../ircodec

CFLAGS=-W -Wall -ansi -pedantic -I$(IRCODECPATH)/include
LDFLAGS=-L$(IRCODECPATH)/lib
EXEC=scale
OBJS = main.o iokey.o

all: $(EXEC)

$(IRCODECPATH)/lib/libircodec.a :
	@(cd $(IRCODECPATH)/projects/linux && $(MAKE))

scale: $(OBJS) $(IRCODECPATH)/lib/libircodec.a
	$(CC) -o  $@  $^ -lircodec $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -f scale
	@rm -f *.o