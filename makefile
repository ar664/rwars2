IDIR=include
CC=gcc
CFLAGS=-I$(IDIR)

SRCDIR=src
ODIR=obj
LDIR=lib

#Libs are included without the lib prefix because -l does that for you
LIBS=-lm 

#H files go here
_DEPS = entity.h main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#Target object files go here (There basically the c files)
_OBJ = entity.h main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o : $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#The end program results name
program_name : $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

