# Makefile for a demo written by Nate Robins
# Author: Marius Nita <marius@pdx.edu>

CFLAGS = -I. -I/usr/X11R6/include
LDFLAGS = -L/usr/X11R6 -lGL -lGLU -lglut -lm
CC = gcc

all: smooth

smooth: glm.o gltb.o

clean:
	rm -rf *.o smooth
