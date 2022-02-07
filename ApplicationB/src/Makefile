IDIR =../inc
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib
BIN_DIR = ../bin

LIBS=-lpthread

_DEPS = SendResult.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = SendResult.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN_DIR)/ApplicationB.bin: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
