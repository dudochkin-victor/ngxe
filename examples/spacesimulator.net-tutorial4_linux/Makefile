# Makefile by Panteleakis Ioannis
# mail: pioann@csd.auth.gr

MAINSRC = tutorial4.cpp
MAINLIB = -lglut

SRC = $(MAINSRC) texture.cpp 3dsloader.cpp

OBJ = $(SRC:.c=.o)

OPT = -O3
CC = gcc $(OPT) -Wall

# You might have to change GLU to MesaGLU depending on your Mesa version
GLLIBS = $(MAINLIB) -lGL -lGLU
LIBS = $(GLLIBS) -lm

all: tutorial4

.c.o:
	$(CC) -c $<

tutorial4: $(OBJ)
	gcc -o $@ $^ $(LIBS)

clean:
	rm -f tutorial4 *.o
