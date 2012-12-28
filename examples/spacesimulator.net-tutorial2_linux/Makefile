MAINSRC = tutorial2.cpp
MAINLIB = -lglut

SRC = $(MAINSRC)

OBJ = $(SRC:.c=.o)

OPT = -O3
CC = gcc $(OPT) -Wall

# You might have to change GLU to MesaGLU depending on your Mesa version
GLLIBS = $(MAINLIB) -lGL -lGLU
LIBS = $(GLLIBS) -lm

all: tutorial2

.c.o:
	$(CC) -c $<

tutorial2: $(OBJ)
	gcc -o $@ $^ $(LIBS)

clean:
	rm -f tutorial2 *.o