objects = main.o menu.o theme.o font.o
LIBS = -lSDL2_ttf -lpthread

build: game

game: $(objects)
	gcc $(objects) -o game `sdl2-config --libs` $(LIBS)

main.o: main.cpp
	gcc -c `sdl2-config --cflags` main.cpp

menu.o: menu.cpp
	gcc -c `sdl2-config --cflags` menu.cpp

theme.o: theme.cpp
	gcc -c `sdl2-config --cflags` theme.cpp

font.o: font.cpp
	gcc -c `sdl2-config --cflags` font.cpp

clean:
	rm -f $(objects)

all: clean build
