## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: ttf1

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags` -I../support/ `freetype-config --cflags`
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU -L../support/ -lsupport `freetype-config --libs`


LIBS1 = ttf1.o 
ttf1: ${LIBS1}
	$(LD) -o $@ ${LIBS1} ${LFLAGS}




clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f ttf1
