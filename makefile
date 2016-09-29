IDIR=include
CC=g++ -fpermissive --std=c++11 --debug
CFLAGS=-I$(IDIR)

SRCDIR=src
ODIR=obj
LDIR=lib

#Libs are included without the lib prefix because -l does that for you
LIBS=-lm -lsfml-system -lsfml-audio -lsfml-window -lsfml-graphics -lsfml-audio -lBox2D 

#H files go here
_DEPS = AI.h audio.h entity.h fault.h globals.h graphics.h main.h physics.h player.h resourcemanager.h scene.h shape.h sprite.h statemachine.h vectors.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#Target object files go here (There basically the c files)
_OBJ = AI.o audio.o entity.o fault.o globals.o graphics.o main.o physics.o player.o resourcemanager.o scene.o shape.o sprite.o statemachine.o vectors.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o : $(SRCDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

#The end program results name
rwars : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

