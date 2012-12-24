## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: mipmap

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags` -I../support/
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU -L../support/ -lsupport


LIBS = mipmap.o 
mipmap: ${LIBS}
	$(LD) -o $@ ${LIBS} ${LFLAGS}




clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f mipmap
