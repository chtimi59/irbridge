ROOTPATH=../..
SRCPATH=$(ROOTPATH)/src
LIBPATH=$(ROOTPATH)/lib
INCPATH=$(ROOTPATH)/include

AR=ar
CC=gcc
CFLAGS=-I$(INCPATH) -W -Wall -ansi -pedantic -std=gnu99
LDFLAGS=-L$(LIBPATH)

EXTRACFLAG=
ifneq (,$(findstring utest,$(MAKECMDGOALS)))
    EXTRACFLAG=-DUNITARY_TEST
endif



OBJS = bitops.o ir_nec.o ir_unkn0.o

all: $(LIBPATH)/libircodec.a

$(LIBPATH)/libircodec.a: $(OBJS)
	@mkdir -p $(LIBPATH)
	$(AR) rcs $@ $^ 

$(LIBPATH)/libircodecd.a: $(OBJS)
	@mkdir -p $(LIBPATH)
	$(AR) rcs $@ $^ 
	
%.o: $(SRCPATH)/%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(EXTRACFLAG)

utest: $(LIBPATH)/libircodecd.a $(OBJS) main.o
	$(CC) $(OBJS) main.o $(LDFLAGS) -lircodecd -o $@ 
	
clean:
	@rm -f *.o
	@rm -f utest	
