## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: keys view

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags`
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU

SUPPORTLIBS = video.o event.o


keys: keys.o
	$(LD) -o $@ $< ${LFLAGS}


VIEWLIBS = view.o ${SUPPORTLIBS}
view: ${VIEWLIBS}
	$(LD) -o $@ ${VIEWLIBS} ${LFLAGS}

clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f keys
	rm -f view
