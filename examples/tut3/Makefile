## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##



all: light1 texture1

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags`
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU

SUPPORTLIBS = video.o event.o


LIBS = light1.o ${SUPPORTLIBS}
light1: ${LIBS}
	$(LD) -o $@ ${LIBS} ${LFLAGS}

TLIBS = texture1.o ${SUPPORTLIBS}
texture1: ${TLIBS}
	$(LD) -o $@ ${TLIBS} ${LFLAGS}


clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f light1
	rm -f texture1

