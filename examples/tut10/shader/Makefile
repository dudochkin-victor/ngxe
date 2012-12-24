## This is a sorta standard make file that will need minimum modifications 
##  to compile other sourse code.
##


all: shader

## Compiler
CC=g++
## linker
LD=$(CC)
## compile flags
CPPFLAGS = -Wall -g -DLINUX `sdl-config --cflags` -I../support/
## link flags
CGLIBS = -lCg -lCgGL
GLLIBS = -lGL -lGLU
LFLAGS = `sdl-config --libs` $(GLLIBS) -L../support/ -lsupport $(CGLIBS)


LIBS1 = shader.o 
shader: ${LIBS1}
	$(LD) -o $@ ${LIBS1} ${LFLAGS}


clean:
	rm -f *.o
	rm -f *~

cleanall: clean
	rm -f shader
