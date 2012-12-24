## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##




## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags`
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU

SUPPORTLIBS = video.o event.o error.o lists.o texture.o light.o material.o

all: libsupport.a

libsupport.a: ${SUPPORTLIBS}
	$(LD) -o $@ ${SUPPORTLIBS} -shared ${LDFLAGS}


clean:
	rm -f *.o
	rm -f *~
cleanall: clean
	rm -f libsupport.a

