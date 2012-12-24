## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: coll1

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags` -I../support/
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU -L../support/ -lsupport


LIBS1 = coll1.o 
coll1: ${LIBS1}
	$(LD) -o $@ ${LIBS1} ${LFLAGS}


clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f coll1
