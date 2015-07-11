CC=gcc
IRCODECPATH=../../ircodec

CFLAGS=-W -Wall -ansi -pedantic -I$(IRCODECPATH)/include
LDFLAGS=-L$(IRCODECPATH)/lib
EXEC=server
OBJS = main.o server.o

all: $(EXEC)

$(IRCODECPATH)/lib/libircodec.a :
	@(cd $(IRCODECPATH)/projects/linux && $(MAKE))
	
server: $(OBJS)
	$(CC) -o  $@  $^  $(LDFLAGS)
	
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -f *.o
	@rm -f server	