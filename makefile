IDIR=include
CC=gcc
CFLAGS=-I$(IDIR)

SRCDIR=src
ODIR=obj
LDIR=lib

#Libs are included without the lib prefix because -l does that for you
LIBS=-lm -lSFML 

#H files go here
_DEPS = sprite.h entity.h main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#Target object files go here (There basically the c files)
_OBJ = sprite.o entity.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o : $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#The end program results name
rwars : $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

