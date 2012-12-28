CC			= g++
OBJ			= tutorial2.o init.o

TARGET		= tutorial2

$(TARGET):	$(OBJ)
	$(CC)  -o $(TARGET) $(OBJ) `sdl-config --libs` -lGL -lGLU -lm

tutorial2.o:		tutorial2.cpp 
	$(CC) -c tutorial2.cpp `sdl-config --cflags`

init.o:		init.cpp 
	$(CC) -c init.cpp `sdl-config --cflags`

clean:
	rm -f $(TARGET) $(OBJ)
