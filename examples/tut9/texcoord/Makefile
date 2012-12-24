## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: texgen1

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags` -I../support/
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU -L../support/ -lsupport


LIBS1 = texgen1.o 
texgen1: ${LIBS1}
	$(LD) -o $@ ${LIBS1} ${LFLAGS}



clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f texgen1

