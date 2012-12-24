## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: lists1 lists2 error1

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags`
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU

SUPPORTLIBS = video.o event.o


LIBS1 = lists1.o ${SUPPORTLIBS}
lists1: ${LIBS1}
	$(LD) -o $@ ${LIBS1} ${LFLAGS}


LIBS2 = lists2.o ${SUPPORTLIBS}
lists2: ${LIBS2}
	$(LD) -o $@ ${LIBS2} ${LFLAGS}

LIBSE = error1.o error.o ${SUPPORTLIBS}
error1: ${LIBSE}
	$(LD) -o $@ ${LIBSE} ${LFLAGS}


clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f error1
	rm -f lists1
	rm -f lists2
