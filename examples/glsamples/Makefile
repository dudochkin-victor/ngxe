# Makefile for Nate Robins' OpenGL samples at http://www.xmission.com/~nate/sgi.html
# Author: Marius Nita <marius@pdx.edu>

CFLAGS = -I. -I/usr/X11R6/include
LDFLAGS = -L/usr/X11R6/lib -lGL -lGLU -lglut -lm
MAKE = make
CC = gcc

all: nii qix starfield strip texture voronoi multiview null rotate
	$(MAKE) -C maiden
	$(MAKE) -C smooth

nii: nii.o

qix: qix.o

starfield: starfield.o

strip: strip.o

texture: texture.o

voronoi: voronoi.o

multiview: multiview.o

null: null.o

rotate: rotate.o

clean:
	rm -rf *.o nii qix starfield strip texture voronoi multiview null rotate
	$(MAKE) -C maiden clean
	$(MAKE) -C smooth clean
