IDIR=include
CC=g++ -std=c++11 --debug
CFLAGS=-I$(IDIR)

SRCDIR=src
ODIR=obj
LDIR=lib

#Libs are included without the lib prefix because -l does that for you
LIBS=-lm -lsfml-system -lsfml-audio -lsfml-window -lsfml-graphics -lsfml-audio 

#H files go here
_DEPS = audio.h physics.h player.h sprite.h globals.h graphics.h vectors.h entity.h main.h game.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#Target object files go here (There basically the c files)
_OBJ = audio.o physics.o player.o sprite.o globals.o graphics.o vectors.o entity.o main.o game.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o : $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

#The end program results name
rwars : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

