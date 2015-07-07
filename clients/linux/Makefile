CC=gcc
IRCODECPATH=../../ircodec

CFLAGS=-W -Wall -ansi -pedantic -I$(IRCODECPATH)/include
LDFLAGS=-L$(IRCODECPATH)/lib
EXEC=client
OBJS = main.o client.o

all: $(EXEC)

$(IRCODECPATH)/lib/libircodec.a :
	@(cd $(IRCODECPATH)/linux && $(MAKE))
	
client: $(OBJS) $(IRCODECPATH)/lib/libircodec.a
	$(CC) -o  $@  $^ -lircodec $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean:
	@rm -f *.o
	@rm -f client	