# Non working make file, going to need to figure this out and make it better!
IDIR =. 
CC=gcc
CFLAGS=-I$(IDIR)


ODIR=.
LDIR =../lib

LIBS=-lm

_DEPS = normal.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

CFLAGS :=

_OBJ = normal.o clock.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
	$(CC) -c -o  $@ $< $(CFLAGS) 

test: $(OBJ)
	$(CC) -o $@ $^ 

.PHONY: clean

run:
	./test

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm test