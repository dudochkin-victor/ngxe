LDFLAGS=-L/usr/X11R6/lib
CFLAGS=-I/usr/X11R6/include
GLLIBS = -lglut -lGL -lGLU 
OBJS = MysticFalls.o
EXTLIBPATHS = 

all: MysticFalls

MysticFalls: ${OBJS}
	g++ ${GLLIBS} ${EXTLIBPATHS} ${LDFLAGS} ${CFLAGS} -o MysticFalls ${OBJS}

clean:
	rm -f *.o
	rm -f MysticFalls
