# Makefile for a demo written by Nate Robins
# Author: Marius Nita <marius@pdx.edu>

CFLAGS = -I. -I/usr/X11R6/include
LDFLAGS = -L/usr/X11R6 -lGL -lGLU -lglut -lm

all: maiden

maiden: maiden.o

clean:
	rm -rf maiden *.o
