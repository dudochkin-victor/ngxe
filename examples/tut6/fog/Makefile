## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: fog1 fog2

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags` -I../support/
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU -L../support/ -lsupport


LIBS1 = fog1.o 
fog1: ${LIBS1}
	$(LD) -o $@ ${LIBS1} ${LFLAGS}

LIBS2 = fog2.o 
fog2: ${LIBS2}
	$(LD) -o $@ ${LIBS2} ${LFLAGS}


clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f fog1
	rm -f fog2

