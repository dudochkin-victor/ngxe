LDFLAGS=-L/usr/X11R6/lib
CFLAGS=-I/usr/X11R6/include
GLLIBS = -lglut -lGL -lGLU 
OBJS = tornado.o
EXTLIBPATHS = 

all: Tornado

Tornado: ${OBJS}
	g++ ${GLLIBS} ${EXTLIBPATHS} ${LDFLAGS} ${CFLAGS} -o Tornado ${OBJS}

clean:
	rm -f *.o
	rm -f Tornado
