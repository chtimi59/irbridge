CC=gcc
IRCODECPATH=../../ircodec

CFLAGS=-W -Wall -ansi -D_GNU_SOURCE -pedantic -I$(IRCODECPATH)/include
LDFLAGS=-L$(IRCODECPATH)/lib
EXEC=irbrgd
OBJS = main.o server.o led.o starfrit.o

all: $(EXEC)

$(IRCODECPATH)/lib/libircodec.a :
	@(cd $(IRCODECPATH)/projects/linux && $(MAKE))
	
irbrgd: $(OBJS) $(IRCODECPATH)/lib/libircodec.a
	$(CC) -o  $@  $^  $(LDFLAGS)
	
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -f *.o
	@rm -f irbrgd	