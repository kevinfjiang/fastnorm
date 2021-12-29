IDIR=../src
CC=gcc
ODIR=../bin
CFLAGS=-I$(ODIR)


LDIR =../lib

LIBS=-lm

_DEPS = normal.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = normal.o clock.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_DOBJ = normal.o debug.o
DOBJ = $(patsubst %,$(ODIR)/%,$(_DOBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

time: $(OBJ)
	$(CC) -o $@.exe $^ $(CFLAGS) $(LIBS)

.PHONY: clean

debug: $(DOBJ)
	$(CC) -o $@.exe $^ $(CFLAGS) $(LIBS) -DDEBUG

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f *.exe