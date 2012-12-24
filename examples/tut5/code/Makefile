## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: solarsystem

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags`
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU

SUPPORTLIBS = video.o event.o error.o lists.o texture.o light.o material.o


LIBS = solar.o skybox.o camera.o planet.o ${SUPPORTLIBS}
solarsystem: ${LIBS}
	$(LD) -o $@ ${LIBS} ${LFLAGS}



clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f solarsystem
