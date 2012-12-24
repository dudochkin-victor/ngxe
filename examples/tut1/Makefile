## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: bare


## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags`
## link flags
LFLAGS = `sdl-config --libs` -lGL -lGLU

#specify objects used
OBJS = bare.o

bare: ${OBJS}
	$(LD) -o $@ ${OBJS} ${LFLAGS}


clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f bare
