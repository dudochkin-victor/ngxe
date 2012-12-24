## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: shot

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags` -I../support/
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU -L../support/ -lsupport


LIBS1 = shot.o 
shot: ${LIBS1}
	$(LD) -o $@ ${LIBS1} ${LFLAGS}


clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f shot
	rm -f screenshot.bmp
